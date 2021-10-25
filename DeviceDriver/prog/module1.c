#include<linux/module.h>
#include<linux/version.h>
#include<linux/kernel.h>
int init_mod(void)
{
	printk(KERN_ALERT "Module1 started...\n");
	return 0;
}
void end_mod(void)
{
	printk(KERN_ALERT "Module1 ended...\n");
}
module_init(init_mod);
module_exit(end_mod);
//MODULE_LICENSE (" GPL ");

