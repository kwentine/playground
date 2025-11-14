#!/bin/env bash
instance="${1:-0}"
image=mariadb:11.1.3-jammy
conf_dir="$(pwd)/mariadb/conf.d"
initdb_dir="$(pwd)/mariadb/initdb.d"
name="mariadb"
network="mariadb-net"
database="colors"
user="painter"

my_print_defaults() {
  podman run --rm --name mariadb-matomo-tmp -v "${confdir}:/etc/mysql/conf.d:Z" "${image}" my_print_defaults --mysqld
}

podman_args=(
  --network mariadb-matomo
  --env MARIADB_ROOT_PASSWORD=root
  --env MARIADB_USER=matomo
  --env MARIADB_PASSWORD=matomo
  --env MARIADB_DATABASE=matomo
  # Simulate Pod that does not start as root
  --userns=keep-id:uid=999,gid=999
  --mount "type=volume,src=mariadb-data${instance+-${instance}},destination=/var/lib/mysql,Z"
  --mount "type=volume,src=mariadb-backup,destination=/var/mariadb/backup,Z"
  --mount "type=bind,src=${conf_dir},destination=/etc/mysql/conf.d,Z"
#  --mount "type=bind,src=${initdb_dir},destination=/docker-entrypoint-initdb.d,Z"
)


interactive=0
health_check=0
mariadb=1
restore=0
if ((interactive)); then
  podman_args+=(
    -it
    --rm
    --entrypoint /bin/bash
  )
elif ((health_check)); then
  podman_args+=(
    --rm
    --entrypoint healthcheck.sh
    --env MYSQL_HOST=mariadb-matomo
    --env MYSQL_PWD=matomo
  )
  maria_args=(
    --connect
  )
elif ((mariadb)); then
  podman_args+=(
    -it
    --rm
    --env MYSQL_HOST=mariadb-matomo
    --env MYSQL_PWD=matomo
  )
  maria_args=(
    mariadb
    -u matomo
    -D matomo
  )
elif ((restore)); then
  podman_args+=(
    --rm
    --name mariadb-matomo-restore
    --env MYSQL_PWD=root
  )
  maria_args=(
    mariadb-backup
    -u root
    --copy-back
    --target-dir=/var/mariadb/backup
  )
else
  podman_args+=(
    --name mariadb-matomo"${instance+-${instance}}"
    -d
  )
fi

declare -a cmd
dry_run=0
if ((dry_run)); then
  cmd=(echo)
fi

cmd+=(podman run)

"${cmd[@]}" "${podman_args[@]}" "${image}" "${maria_args[@]}"
