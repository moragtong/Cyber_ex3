docker run -d \
  --name mariadb-server \
  -p 3306:3306 \
  -v /home/michael/Documents/Cyber/Desktop/_data:/var/lib/mysql \
  -e MARIADB_USER="u67607"\
  -e MARIADB_PASSWORD="courseUser" \
  -e MARIADB_ROOT_PASSWORD="c0urs3Cyb3r!" \
  -e GOSU_VERSION="1.17" \
  -e LANG="C.UTF-8" \
  -e MARIADB_VERSION="1:11.6.2+maria~ubu2404" \
  mariadb:11.6.2
#docker start mariadb-server
