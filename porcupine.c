#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/vmalloc.h>
#include "porcupine.h"

/* Module Information */
MODULE_DESCRIPTION("The porcupine opcode fuzzer.  A module for detecting hardware bugs.");
MODULE_AUTHOR("Raymond Cheng <ryscheng@cs.washington.edu>, Will Scott <wrs@cs.washington.edu>");
MODULE_LICENSE("GPL");
//module_param(SEED, int, 0);

/**
 * Initialize the porcupine kernel module, and spawn appropriate threads.
 */
static int __init porcupine_init(void)
{
  size_t i;
  threadids = (int*)vmalloc(sizeof(int) * NUM_THREADS);
  printk("Porcupine Installed.  Running.\n");

  for (i = 0; i < NUM_THREADS; i++) {
    threadids[i] = i;
    kthread_run(porcupine_run, &threadids[i], PROCESS_IDENTIFIER);
  }

  return 0;
}


/**
 * Remove the porcupine kernel module.
 */
static void __exit porcupine_exit(void)
{
  vfree(threadids);
  printk("Porcupine Removed.\n");
  return;
}

module_init(porcupine_init);
module_exit(porcupine_exit);

/**
 * Thread run()
 */

