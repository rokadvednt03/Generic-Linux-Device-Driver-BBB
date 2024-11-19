#include<linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("VEDANT A. ROKAD");
MODULE_DESCRIPTION("BBB_HelloWorld Module");
MODULE_INFO(board,"BBB");

static int __init init_function(void){
	pr_info("Hello World\n");
	pr_info("I'm looking for you Linux\n");
	return 0 ;
}

static void __exit exit_function(void){
	pr_info("GOOD BYE from Linux Kernel !!!\n");	
}

module_init(init_function);
module_exit(exit_function);

