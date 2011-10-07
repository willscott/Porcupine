/**
 * @fileoverview Interface for the porcupine opcode fuzzer kernel module.
 */

/**
 * The identifier for the worker threads spawned by porcupine.
 */
#define PROCESS_IDENTIFIER "porcupine"

/**
 * Pointer to a list of the per-thread seeds to use.  This memory is allocated
 * in porcupine_init, and freed in porcupine_exit.
 */
int *threadids;

/**
 * The initialization function for a single porcupine thread.
 * @param seed A pointer to the pseudorandom seed to be used for this run.
 * @return An exit code - The function should not be expected to ever actually
 *     return.
 */
int porcupine_run(void* threadid);

