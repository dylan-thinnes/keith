#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

uint64_t digits[100] = {};

void keith (uint64_t n) {
  // Initialize buffer
  uint64_t x = n;
  int digits_length = 1 + floor(log10((double) x));
  uint64_t window_total = 0;
  for (int ii = digits_length - 1; ii >= 0; ii--) {
    digits[ii] = x % 10;
    window_total += digits[ii];
    x /= 10;
  }

  // Sum over digits
  int j = digits_length;
  digits[j] = window_total;
  while (digits[j] < n) {
    window_total -= digits[j - digits_length];
    window_total += digits[j];
    j++;
    digits[j] = window_total;
  }

#ifdef DEBUG
  for (int ii = 0; ii <= j; ii++) {
    printf("%lu\n", digits[ii]);
  }
#endif

  if (digits[j] == n) printf("%lu\n", n);
}

int main (int argc, char** argv) {
  if (argc > 1) {
    for (int ii = 1; ii < argc; ii++) {
      uint64_t n;
      int success = sscanf(argv[ii], "%lu", &n);
      if (success != 1) {
        fprintf(stderr, "Argument %d, '%s', is not a number.", ii, argv[ii]);
        continue;
      }

      keith(n);
    }
  } else {
    uint64_t n = 10;
    while (1) keith(n++);
  }
}
