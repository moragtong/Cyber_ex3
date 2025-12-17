#docker run -dit --name attacker-server attacker-server
docker start attacker-server
docker exec -it attacker-server /bin/bash
