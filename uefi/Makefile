OVMF_DIR := /usr/share/edk2/ovmf
TEMPLATES_DIR := /var/vms/templates

all: vars code

vars: OVMF_VARS.fd
code: OVMF_CODE.fd

OVMF_VARS.fd:
	cp $(OVMF_DIR)/$@ .

OVMF_CODE.fd:
	cp $(OVMF_DIR)/$@ .

bios: OVMF_CODE.fd
	ln -sT $^ $@.bin

hda:
	mkdir -p hda

arch.qcow2:
	cp /var/vms/templates/arch/Arch-Linux-x86_64-basic.qcow2 $@

rhel.qcow2:
	cp /var/vms/templates/rhel-10.0-x86_64-kvm.qcow2 $@

clean-vms:
	rm -f arch.qcow2
