#include <linux/delay.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include "porcupine.h"
#include "vmx.c"

/* Module Information */
MODULE_DESCRIPTION("The porcupine opcode fuzzer.  A module for detecting hardware bugs.");
MODULE_AUTHOR("Raymond Cheng <ryscheng@cs.washington.edu>, Will Scott <wrs@cs.washington.edu>");
MODULE_LICENSE("GPL");

static struct task_struct* thread;

/**
 * Initialize the porcupine kernel module, and spawn appropriate threads.
 */
static int __init porcupine_init(void)
{
  printk("Porcupine Installed.  Running.\n");

  thread = kthread_run(porcupine_run, NULL, PROCESS_IDENTIFIER);

  return 0;
}


/**
 * Remove the porcupine kernel module.
 */
static void __exit porcupine_exit(void)
{
  kthread_stop(thread);
  
  printk("Porcupine Removed.\n");
  return;
}

int porcupine_run(void* param) {
  set_current_state(TASK_INTERRUPTIBLE);
  while(!kthread_should_stop()) {
    msleep_interruptible(1000);
    hardware_status();
    //hardware_enable(NULL);
    //hardware_disable(NULL);
    set_current_state(TASK_INTERRUPTIBLE);
    printk(".");
  }
  return 0;
}

module_init(porcupine_init);
module_exit(porcupine_exit);

