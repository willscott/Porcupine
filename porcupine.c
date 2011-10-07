#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include "porcupine.h"


module_param(SEED, int, 0);
MODULE_PARM_DESC(SEED, "The seed to use in generating instructions.");

/**
 * Initialize the porcupine kernel module, and spawn appropriate threads.
 */
static int __init porcupine_init(void)
{
  size_t i;
  int seed = SEED;

  printk("Porcupine Installed.  Running seed %d.\n", seed);

  for (i = 0; i < NUM_THREADS; i++) {
    kthread_run(porcupine_run, &SEED, PROCESS_IDENTIFIER);
  }
  return 0;
}


/**
 * Remove the porcupine kernel module.
 */
static void __exit porcupine_exit(void)
{
  printk("Porcupine Removed.\n");
  return;
}


int porcupine_run(void* param) {
  int seed = *(int *)param;
  printk("Porcupine thread created with seed %d.\n", seed);
  return 0;
}

module_init(porcupine_init);
module_exit(porcupine_exit);

/* module information */
MODULE_DESCRIPTION("The porcupine opcode fuzzer.  A module for detecting hardware bugs.");
MODULE_AUTHOR("Raymond Cheng <ryscheng@cs.washington.edu>, Will Scott <wrs@cs.washington.edu>");
MODULE_LICENSE("GPL");

