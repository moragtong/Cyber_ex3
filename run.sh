gnome-terminal \
  --tab --title="MariaDB" --command="docker start mariadb-server" \
  --tab --title="Web Server" --command="docker start vapp"\
  --tab --title="Attacker Client" --command="env_files/dockers/attacker-client/run.sh" \
  --tab --title="Attacker Server" --command="env_files/dockers/attacker-server/run.sh"
