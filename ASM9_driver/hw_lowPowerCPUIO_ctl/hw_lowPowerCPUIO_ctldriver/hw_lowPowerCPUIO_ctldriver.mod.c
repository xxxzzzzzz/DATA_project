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
	{ 0xb5e1ecd7, "platform_driver_register" },
	{ 0x7a37f716, "set_pin_mux" },
	{ 0xe147dc8e, "kobject_put" },
	{ 0x1ff8457, "sysfs_create_group" },
	{ 0x14570de9, "kobject_create_and_add" },
	{ 0x3c2c5af5, "sprintf" },
	{ 0xe914e41e, "strcpy" },
	{ 0xba781a1, "write_GPIO" },
	{ 0xe2d5255a, "strcmp" },
	{ 0xea147363, "printk" },
	{ 0x61b7c26c, "clear_GPIO" },
	{ 0x4694c794, "kobject_del" },
	{ 0x7d7c2925, "sysfs_remove_group" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x3030ea7f, "platform_driver_unregister" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "4AB9EF29C29F4348E907276");
