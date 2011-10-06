#include <linux/module.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include "porcupine.h"



/**
 * Initialize the porcupine kernel module, and spawn appropriate threads.
 */
static int __init porcupine_init(void)
{
  size_t i;
  int seed = 0;

  printk ("Porcupine Installed\n");

  for (i = 0; i < NUM_THREADS; i++) {
    kthread_run(porcupine_run, &seed, "porcupine");
  }
  return 0;
}


/**
 * Remove the porcupine kernel module.
 */
static void __exit porcupine_exit(void)
{
  printk ("Porcupine Removed.\n");
  return;
}


int porcupine_run(void* seed) {
  printk ("Porcupine thread created.");
  return 0;
}

module_init(porcupine_init);
module_exit(porcupine_exit);

MODULE_LICENSE("GPL");

