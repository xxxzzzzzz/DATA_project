#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xc3043f12, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xbf7c6ab1, __VMLINUX_SYMBOL_STR(i2c_del_driver) },
	{ 0x2f364ccf, __VMLINUX_SYMBOL_STR(i2c_register_driver) },
	{ 0x67c2fa54, __VMLINUX_SYMBOL_STR(__copy_to_user) },
	{ 0xa0b061f1, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0xd8b518f3, __VMLINUX_SYMBOL_STR(i2c_master_recv) },
	{ 0xf6c545b5, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0xc8eba6e9, __VMLINUX_SYMBOL_STR(i2c_master_send) },
	{ 0xd8e484f0, __VMLINUX_SYMBOL_STR(register_chrdev_region) },
	{ 0x6b65ec4a, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0x21d6c5e4, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0x97e1157e, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0x90412057, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0x29537c9e, __VMLINUX_SYMBOL_STR(alloc_chrdev_region) },
	{ 0xefd6cf06, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr0) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0x463c58ef, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0x95799440, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0xdad963e1, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0x2e5810c6, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr1) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("of:N*T*Cbh1750*");

MODULE_INFO(srcversion, "D6F6313BE4FB33264E31A74");
