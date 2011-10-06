/**
 * @fileoverview Interface for the porcupine opcode fuzzer kernel module.
 */

/**
 * The number of porcupine threads to be spawned by the kernel module.
 */
#define NUM_THREADS 2


/**
 * The initialization function for a single porcupine thread.
 * @param seed A pointer to the pseudorandom seed to be used for this run.
 * @return An exit code - The function should not be expected to ever actually
 *     return.
 */
int porcupine_run(void* seed);

