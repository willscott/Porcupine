#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/vmalloc.h>
#include "porcupine.h"



/**
 * Initialize the porcupine kernel module, and spawn appropriate threads.
 */
static int __init porcupine_init(void)
{
  size_t i;
  seeds = (int*)vmalloc(sizeof(int) * NUM_THREADS);

  printk("Porcupine Installed.  Running seed %d.\n", SEED);

  for (i = 0; i < NUM_THREADS; i++) {
    seeds[i] = SEED + i;
    kthread_run(porcupine_run, &seeds[i], PROCESS_IDENTIFIER);
  }
  return 0;
}


/**
 * Remove the porcupine kernel module.
 */
static void __exit porcupine_exit(void)
{
  vfree(seeds);
  printk("Porcupine Removed.\n");
  return;
}


int porcupine_run(void* param) {
  int seed = *(int *)param;
  printk("Porcupine thread %d created with seed %d.\n", current->pid, seed);

  do_exit(0);
  return 0;
}

module_init(porcupine_init);
module_exit(porcupine_exit);

/* module information */
MODULE_DESCRIPTION("The porcupine opcode fuzzer.  A module for detecting hardware bugs.");
MODULE_AUTHOR("Raymond Cheng <ryscheng@cs.washington.edu>, Will Scott <wrs@cs.washington.edu>");
MODULE_LICENSE("GPL");
module_param(SEED, int, 0);


