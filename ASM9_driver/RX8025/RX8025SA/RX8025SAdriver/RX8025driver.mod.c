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
	{ 0xe147dc8e, "kobject_put" },
	{ 0x43a0192b, "rtc_device_register" },
	{ 0xfbc74f64, "__copy_from_user" },
	{ 0x67c2fa54, "__copy_to_user" },
	{ 0xae233aeb, "dev_set_drvdata" },
	{ 0xd77f7134, "rtc_device_unregister" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0x7a37f716, "set_pin_mux" },
	{ 0x3c2c5af5, "sprintf" },
	{ 0x7d7c2925, "sysfs_remove_group" },
	{ 0xe2d5255a, "strcmp" },
	{ 0x14570de9, "kobject_create_and_add" },
	{ 0x5edd0762, "bin2bcd" },
	{ 0xfa2a45e, "__memzero" },
	{ 0x5f754e5a, "memset" },
	{ 0xea147363, "printk" },
	{ 0x1ff8457, "sysfs_create_group" },
	{ 0xb5e1ecd7, "platform_driver_register" },
	{ 0x685bfc12, "read_GPIO" },
	{ 0xfec3c2f2, "bcd2bin" },
	{ 0x5838f6c9, "rtc_valid_tm" },
	{ 0x9d669763, "memcpy" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x3030ea7f, "platform_driver_unregister" },
	{ 0xe914e41e, "strcpy" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "1CDA9ABD6E05D9F2FACF6A6");
