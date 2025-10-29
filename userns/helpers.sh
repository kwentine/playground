
run() {
  podman run --rm --name sleeper alpine:latest sleep "${1:-3600}"
}

getpid() {
  : "Get container host PID"
  podman inspect --format '{{ .State.Pid }}' "${1}"
}

getppid() {
  : "Get container host PPID"
}

getns() {
  : "Get container user namespace,
  or root user namespace if no argument is given"
}
