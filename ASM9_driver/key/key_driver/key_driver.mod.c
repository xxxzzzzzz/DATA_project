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
	{ 0x228a4ced, "input_event" },
	{ 0x685bfc12, "read_GPIO" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0x7d11c268, "jiffies" },
	{ 0x2dd77c66, "mod_timer" },
	{ 0x70e43fd0, "io_irq_clr" },
	{ 0x78cd5d8e, "io_irq_mask" },
	{ 0x859c6dc7, "request_threaded_irq" },
	{ 0xa7f44650, "add_timer" },
	{ 0xf229e057, "init_timer_key" },
	{ 0xdf323e15, "input_register_device" },
	{ 0xea147363, "printk" },
	{ 0xf8e1bbd7, "input_allocate_device" },
	{ 0xa8b38389, "io_irq_enable_externIO" },
	{ 0x7a37f716, "set_pin_mux" },
	{ 0x9644e055, "input_free_device" },
	{ 0xb4d0cadd, "input_unregister_device" },
	{ 0xb6867ec3, "del_timer" },
	{ 0xf20dabd8, "free_irq" },
	{ 0x9ddbd56e, "io_irq_disable" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x3030ea7f, "platform_driver_unregister" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "DAE0C5C6CD43110BD1B8B2E");
