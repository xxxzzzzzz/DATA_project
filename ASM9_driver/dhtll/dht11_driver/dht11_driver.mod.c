#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
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
	{ 0x4035d6c5, "module_layout" },
	{ 0x37237927, "cdev_del" },
	{ 0x5683fe8d, "cdev_init" },
	{ 0x67c2fa54, "__copy_to_user" },
	{ 0xd8e484f0, "register_chrdev_region" },
	{ 0x61b7c26c, "clear_GPIO" },
	{ 0xdd0d7608, "device_destroy" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0x7a37f716, "set_pin_mux" },
	{ 0xea147363, "printk" },
	{ 0xc4154c40, "device_create" },
	{ 0xb5e1ecd7, "platform_driver_register" },
	{ 0x685bfc12, "read_GPIO" },
	{ 0x12037388, "cdev_add" },
	{ 0xba781a1, "write_GPIO" },
	{ 0x8ae71dc6, "class_destroy" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x3030ea7f, "platform_driver_unregister" },
	{ 0x14f8f52a, "__class_create" },
	{ 0x29537c9e, "alloc_chrdev_region" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "50C4503D786485CF49D1402");
