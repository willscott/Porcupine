/**
 * Mac:
 * Compile with gcc -ggdb -m32 -fno-pic funcmeasurer.c
 * Linux:
 * gcc -m32 -fno-pic -lrt funcmeasurer.c
 * Sadly, 64 bit compilation doesn't seem to respect no-pic, and I haven't been
 * able to avoid step() getting compiled with relative references to the global
 * arrays.  (e.g. -0x4ci($rip), which doesn't work once step gets relocated.)
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#ifdef __MACH__
#include <mach/mach_time.h>
#define time_t uint64_t
#define gettime(a) a = mach_absolute_time();
#define elapsed(a,b) printf("%llu", b - a);
#else
#include <time.h>
#define time_t struct timespec
#define gettime(a) clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &a);
#define elapsed(a,b) printf("%lu", b.tv_nsec - a.tv_nsec);
#endif

static int (**pointers)(int, int);
static int* conditions;
static int* taken;
static int* missed;

int step(int i, int j) {
  if (__builtin_expect(conditions[i], 0)) {
    j += taken[i];
    return pointers[taken[i]](i + 1, j);
  } else {
    j += missed[i];
    return pointers[missed[i]](i + 1, j);
  }
}
void step_end(void) {}

int finish(int i, int j) {
  return j;
}

int main(int argc, char** argv) {
  int i, sum = 0, slots, max, predictability;
  size_t steplen = (void*)&step_end - (void*)&step;
  time_t start, end;
  predictability = atoi(argv[1]);
  slots = atoi(argv[2]);
  max = atoi(argv[3]);

  conditions = malloc(max * sizeof(int));
  taken = malloc(max * sizeof(int));
  missed = malloc(max * sizeof(int));
  pointers = malloc((slots + 1) * sizeof(void (*)(int)));

  if (conditions == NULL || taken == NULL || missed == NULL || pointers == NULL) {
    exit(1);
  }

  for (i = 0; i < slots; i++) {
    if (posix_memalign(&pointers[i], sysconf(_SC_PAGESIZE), steplen)) {
      exit(1);
    }
    mprotect(pointers[i], steplen, PROT_READ | PROT_WRITE | PROT_EXEC);
    memcpy(pointers[i], &step, steplen);
  }

  sum = 0;
  for (i = 0; i < max; i++) {
    if (random() % predictability == 0) {
      sum = !sum;
    }
    conditions[i] = sum;
    taken[i] = (i + 1) % slots;
    missed[i] = (max - i) % slots;
  }
  taken[max - 1] = slots;
  missed[max - 1] = slots;
  pointers[slots] = finish;

  sum = 0;

  gettime(start);
  sum = pointers[0](0, 0);
  gettime(end);

  free(taken);
  free(missed);
  for (i = 0; i < slots; i++) {
    free(pointers[i]);
  }
  free(pointers);

  elapsed(start, end);
  printf("iterations: %d\n", sum);
  sum = 0;
  for (i = 0; i < max; i++) {
    if (conditions[i]) {
      sum += 1;
    }
  }
  free(conditions);
  printf("taken: %d\n", sum);
  return 0;
}
