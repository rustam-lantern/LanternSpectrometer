#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("usb:v04D8p0011d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v04D8p0000d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v04D8p0101d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v04D8p0100d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v04D8p0022d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v04D8p0023d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v04D8p0030d*dc*dsc*dp*ic*isc*ip*in*");

MODULE_INFO(srcversion, "30CCAE88C0A67469E7747C4");
