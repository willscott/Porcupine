#include <linux/module.h>
#include <linux/config.h>
#include <linux/init.h>


static int __init mymodule_init(void)
{
  printk ("Porcupine Installed\n");
  return 0;
}


static void __exit mymodule_exit(void)
{
  printk ("Porcupine Destoryed.\n");
  return;
}

module_init(mymodule_init);
module_exit(mymodule_exit);

MODULE_LICENSE("GPL");
