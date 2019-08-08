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
	{ 0xf7802486, "__aeabi_uidivmod" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0x7a37f716, "set_pin_mux" },
	{ 0x5edd0762, "bin2bcd" },
	{ 0xe707d823, "__aeabi_uidiv" },
	{ 0x5f754e5a, "memset" },
	{ 0xea147363, "printk" },
	{ 0xc327eadd, "platform_device_unregister" },
	{ 0xb5e1ecd7, "platform_driver_register" },
	{ 0x685bfc12, "read_GPIO" },
	{ 0xfec3c2f2, "bcd2bin" },
	{ 0x9cde24cd, "platform_device_register" },
	{ 0x9d669763, "memcpy" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x3030ea7f, "platform_driver_unregister" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "5E9C87C84E6C759E6267371");
