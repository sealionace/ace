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
	{ 0x59caa4c3, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x8d86fd79, __VMLINUX_SYMBOL_STR(platform_device_unregister) },
	{ 0x8a662e1b, __VMLINUX_SYMBOL_STR(sysfs_remove_group) },
	{ 0x8c29ae20, __VMLINUX_SYMBOL_STR(input_unregister_device) },
	{ 0xc05b1b8e, __VMLINUX_SYMBOL_STR(input_register_device) },
	{ 0x61999eb6, __VMLINUX_SYMBOL_STR(input_set_abs_params) },
	{ 0x5d570e8e, __VMLINUX_SYMBOL_STR(input_allocate_device) },
	{ 0xb2711b8f, __VMLINUX_SYMBOL_STR(sysfs_create_group) },
	{ 0x3781c1f9, __VMLINUX_SYMBOL_STR(platform_device_register_full) },
	{ 0x6232b4e3, __VMLINUX_SYMBOL_STR(input_event) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x20c55ae0, __VMLINUX_SYMBOL_STR(sscanf) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "D7C3438EB62318DBAE1299D");
