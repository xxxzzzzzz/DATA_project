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
	{ 0x9cde24cd, "platform_device_register" },
	{ 0xfa2a45e, "__memzero" },
	{ 0x67c2fa54, "__copy_to_user" },
	{ 0xfbc74f64, "__copy_from_user" },
	{ 0x5838f6c9, "rtc_valid_tm" },
	{ 0xae233aeb, "dev_set_drvdata" },
	{ 0x43a0192b, "rtc_device_register" },
	{ 0xea147363, "printk" },
	{ 0x3030ea7f, "platform_driver_unregister" },
	{ 0xc327eadd, "platform_device_unregister" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "62399342F1A6D9D69C1E6DB");
