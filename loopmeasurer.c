#include <stdlib.h>
#include <stdio.h>

#ifdef __MACH__
#include <mach/mach_time.h>
#define time_t uint64_t
#define gettime(a) a = mach_absolute_time();
#define elapsed(a,b) (b - a)
#else
#include <time.h>
#define time_t struct timespec
#define gettime(a) clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &a);
#define elapsed(a,b) (b.tv_nsec - a.tv_nsec)
#endif

int main(int argc, char** argv) {
  int i, sum = 0, max, cond;
  time_t        start, end;
  cond = atoi(argv[1]);
  max = atoi(argv[2]);
  int* a = malloc(max * sizeof(int));
  int* b = malloc(max * sizeof(int));

  if (a == NULL || b == NULL) {
    exit(1);
  }

  for (i = 0; i < max; i++) {
    if (random() % cond == 0) {
      sum = !sum;
    }
    a[i] = sum;
    b[i] = random() & 1;
  }

  sum = 0;

  gettime(start);
  for (i = 0; i < max; i++) {
    // Indicate the branch should be compiled statically.
    if (__builtin_expect(a[i], 0)) {
      sum += b[i];
    } else {
      sum -= b[i];
    }
  }
  gettime(end);
  free(a);
  free(b);
  printf("elapsed: %llu\n", elapsed(start, end));
  printf("random walk: %d\n", sum);
}
