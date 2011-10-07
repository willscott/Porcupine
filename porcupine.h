/**
 * @fileoverview Interface for the porcupine opcode fuzzer kernel module.
 */



/**
 * The identifier for the worker threads spawned by porcupine.
 */
#define PROCESS_IDENTIFIER "porcupine"

/**
 * The number of porcupine threads to be spawned by the kernel module.
 */
#define NUM_THREADS 2


/**
 * The seed to use when generating instructions.  Successive threads created
 * are run with adjacent seeds (eg. with seed n, the first thread will seed with
 * n, the next with n+1, and so on.)
 * This value is loaded with the module.  Use
 * insmod porcupine.ko SEED=n to set it.
 */
static int SEED = 0;


/**
 * Pointer to a list of the per-thread seeds to use.  This memory is allocated
 * in porcupine_init, and freed in porcupine_exit.
 */
int *seeds;

/**
 * The initialization function for a single porcupine thread.
 * @param seed A pointer to the pseudorandom seed to be used for this run.
 * @return An exit code - The function should not be expected to ever actually
 *     return.
 */
int porcupine_run(void* seed);

