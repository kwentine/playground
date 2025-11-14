net=mariadb-matomo
volumes=(
  mariadb-data
  matomo-data
)

if ! podman network exists "${net}"; then
  podman network create "${net}"
  echo "Created network: ${net}"
fi

for vol in "${volumes}"; do
  if ! podman volume exists "${vol}"; then
    podman volume create "${vol}"
    echo "Created volume: ${vol}"
  fi
done
