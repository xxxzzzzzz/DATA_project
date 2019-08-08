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
	{ 0x70e43fd0, "io_irq_clr" },
	{ 0xaec564d2, "get_io_irq_status" },
	{ 0x859c6dc7, "request_threaded_irq" },
	{ 0x419fa454, "io_irq_enable_edge" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0xf20dabd8, "free_irq" },
	{ 0x9ddbd56e, "io_irq_disable" },
	{ 0xea147363, "printk" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "4C42867F4D4D075D191A42F");
