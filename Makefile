obj-$(CONFIG_SECURITY_ZTLINUX) := ztlinux.o
ztlinux-y := hooks.o sidtab.o policy.o ztlinuxfs.o avc.o
ztlinuxfs-obj := sidtab.o policy.o