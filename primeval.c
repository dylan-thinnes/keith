#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "primes.h"

// Simple sieve for prime numbers, bounded at 15485863 ^ 2
int is_prime (int n) {
  if (n <= 1) return 0;
  int bound = floor(sqrt((double) n));
  int ii = 0;
  while (primes[ii] <= bound) {
    if (n % primes[ii] == 0) return 0;
    ii++;
  }
  return 1;
}

int len = 1;
int tallies[10] = {0,1,0,0,0,0,0,0,0,0};
int prime_total = 0;
int permutation_lengths[20];

long int hardcoded[21] =
      { 1
      , 1
      , 2
      , 6
      , 24
      , 120
      , 720
      , 5040
      , 40320
      , 362880
      , 3628800
      , 39916800
      , 479001600
      , 6227020800
      , 87178291200
      , 1307674368000
      , 20922789888000
      , 355687428096000
      , 6402373705728000
      , 121645100408832000
      , 2432902008176640000
      };

long int factorial (int n) {
  return hardcoded[n];
}

int estimates_made = 0;
long int estimate_permutations (long int dedupe_count, int digits_so_far, int start_idx) {
  long int total = 0;
  if (start_idx == 10) {
    total = factorial(digits_so_far) / dedupe_count;
    //printf("Running total: %ld\n", total);
    estimates_made += 1;
    return total;
  }
  total += estimate_permutations(dedupe_count, digits_so_far, start_idx + 1);
  for (int ii = 1; ii <= tallies[start_idx]; ii++) {
    dedupe_count *= ii; // incrementally calculate factorial
    digits_so_far += 1;
    total += estimate_permutations(dedupe_count, digits_so_far, start_idx + 1);
  }
  return total;
}

int permute (int length, int is_first, int total, int power, int called_with_zero, int running_total) {
  int tried = 0;
  int anything_emitted = 0;

  int tally_decrement = 1;
  if (is_first) tally_decrement = 2;

  for (int ii = 9; ii >= 0; ii -= tally_decrement) {
    if (is_first && ii == 5) continue;
    if (tallies[ii]) {
      if (anything_emitted == 0 && ii == 0) break;
      anything_emitted = 1;
      tallies[ii]--; //emit
      total += power * ii;
      tried += permute(length + 1, 0, total, power * 10, 0 == ii, running_total + ii);
      tallies[ii]++; //deemit
      total -= power * ii;
    }
  }

  if (!called_with_zero && running_total % 3 != 0) {
    tried += 1;
    permutation_lengths[length]++;
    if (length > 0 && is_prime(total)) {
      prime_total++;
    }
  }

  return tried;
}

int get_prime_total () {
  prime_total = 0;
  for (int ii = 0; ii < 20; ii++) permutation_lengths[ii] = 0;
  int tried = permute(0, 1, 0, 1, 1, 0);
  for (int ii = 0; ii < 20; ii++) printf("%d %d\n", ii, permutation_lengths[ii]);
  return tried;
}

void next_tally () {
  if (tallies[9] == len) { // a
    len += 1;
    tallies[0] = tallies[9];
    tallies[1] = 1;
    tallies[9] = 0;
    return;
  } else if (tallies[0] == 0 && tallies[9] + 1 == len) { // b
    // We know that tallies[1:8] contains a single value (i.e. is onehot)
    for (int ii = 8; ii >= 1; ii--) {
      if (tallies[ii]) {
        tallies[ii]--;
        tallies[0] = tallies[9];
        tallies[9] = 0;
        tallies[ii+1]++;
        break;
      }
    }
  } else if (tallies[0] > 0 && tallies[0] == len) { // e
    printf("Invalid case found with all zeroes!\n");
    exit(1);
  } else if (tallies[0] > 0 && tallies[9] > 0 && tallies[0] + tallies[9] == len) { // f
    tallies[0]--;
    tallies[9]++;
  } else if (tallies[0] > 0 && tallies[0] + tallies[9] + 1 == len) { // d
    tallies[0]--;
    // We know that tallies[1:8] contains a single value (i.e. is onehot)
    for (int ii = 8; ii >= 1; ii--) {
      if (tallies[ii]) {
        tallies[ii] = tallies[9] + 2;
        break;
      }
    }
    tallies[9] = 0;
  } else { // c
    for (int ii = 8; ii >= 1; ii--) {
      if (tallies[ii]) {
        tallies[ii]--;
        int nines = tallies[9];
        tallies[9] = 0;
        tallies[ii+1] += 1 + nines;
        break;
      }
    }
  }
}

void print_seq () {
  printf(" %d:", len);
  for (int ii = 0; ii < 10; ii++) {
    printf(" %d", tallies[ii]);
  }

  printf(" : ");

  int first_nonzero = 0;
  for (int ii = 1; ii < 10; ii++) {
    if (tallies[ii] > 0) {
      printf("%d", ii);
      first_nonzero = ii;
      tallies[ii]--;
      break;
    }
  }
  for (int ii = 0; ii < tallies[0]; ii++) {
    printf("0");
  }
  for (int ii = 1; ii < 10; ii++) {
    for (int jj = 0; jj < tallies[ii]; jj++) {
      printf("%d", ii);
    }
  }
  if (first_nonzero > 0) {
    tallies[first_nonzero]++;
  }
}

int main () {
  int best_so_far = -1;
  //int nth_tally = 0;
  int nth_tally = 0;
  tallies[0] = 1;
  tallies[1] = 2;
  tallies[2] = 1;
  tallies[3] = 1;
  tallies[4] = 1;
  tallies[5] = 1;
  tallies[6] = 1;
  tallies[7] = 1;
  tallies[8] = 0;
  tallies[9] = 1;
  len = 10;

  int skips_made = 0;
  int total_tried = 0;
  int total_skippable = 0;
  int ii = 100;
  while (ii--) {
    estimates_made = 0;
    long int estimated_perms = estimate_permutations(1, 0, 0);
    printf("permutation estimate: %ld %d\n", estimated_perms, estimates_made);

    if (estimated_perms >= best_so_far) {
      fprintf(stdout, "Getting prime total...\n");
      int tried = get_prime_total();
      total_tried += tried;
      if (tried < best_so_far) total_skippable += tried;
      fprintf(stdout, "Tried %d, got new prime total: %d\n", tried, prime_total);
      if (prime_total > best_so_far) {
        best_so_far = prime_total;
        printf("Improvement found!\n");
        print_seq();
        printf(": %d : %d\n", nth_tally, prime_total);
      } else {
        print_seq();
        printf("\n");
      }
    } else {
      skips_made++;
      printf("Skipping, permutation estimate too low.");
    }
    //break;

    fprintf(stdout, "Sourcing new tally...\n");
    next_tally();
    fprintf(stdout, "Sourced new tally.\n");
    nth_tally++;
  }

  printf("Skips made: %d\n", skips_made);
  printf("Total tried: %d\n", total_tried);
  printf("Total skippable: %d\n", total_skippable);
}
