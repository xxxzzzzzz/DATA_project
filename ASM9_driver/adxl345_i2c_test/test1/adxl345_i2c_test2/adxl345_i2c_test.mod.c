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
	{ 0xabfe3b9e, "i2c_put_adapter" },
	{ 0x3b50e1ad, "i2c_new_device" },
	{ 0xbe576300, "i2c_get_adapter" },
	{ 0xd7f26261, "i2c_register_driver" },
	{ 0xe693f497, "i2c_smbus_read_byte_data" },
	{ 0xea147363, "printk" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x94d09f9e, "i2c_del_driver" },
	{ 0x7abf738f, "i2c_unregister_device" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "09EE5BAC0D5A46F3575D47B");
