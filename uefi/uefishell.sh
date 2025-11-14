# Run a UEFI shell
# See: https://github.com/tianocore/tianocore.github.io/wiki/How-to-run-OVMF
qemu-system-x86_64 -bios OVMF_CODE.fd \
                   -net none \
                   -drive file=fat:rw:disk-contents,format=raw
