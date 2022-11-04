#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
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
int global_idx = 0;
int global_tally[10] = {0,1,0,0,0,0,0,0,0,0};
pthread_mutex_t global_tally_mutex;

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

int permute (int tally[10], int* prime_total, int length, int is_first, int total, int power, int called_with_zero, int running_total) {
  int tried = 0;
  int anything_emitted = 0;

  int tally_decrement = 1;
  //if (is_first) tally_decrement = 2;

  for (int ii = 9; ii >= 0; ii -= tally_decrement) {
    //if (is_first && ii == 5) continue;
    if (tally[ii]) {
      if (anything_emitted == 0 && ii == 0) break;
      anything_emitted = 1;
      tally[ii]--; //emit
      total += power * ii;
      tried += permute(tally, prime_total, length + 1, 0, total, power * 10, 0 == ii, running_total + ii);
      tally[ii]++; //deemit
      total -= power * ii;
    }
  }

  if (!called_with_zero /*&& running_total % 3 != 0*/) {
    tried += 1;
    int prime = is_prime(total);
    if (length > 0 && prime) {
      *prime_total += 1;
    }
    //printf("try %d %d\n", total, prime);
  }

  return tried;
}

void save_global_tally_to (int tally[10]) {
  for (int ii = 0; ii < 10; ii++) {
    tally[ii] = global_tally[ii];
  }
}

void next_tally_global () {
  global_idx++;
  if (global_tally[9] == len) { // a
    len += 1;
    global_tally[0] = global_tally[9];
    global_tally[1] = 1;
    global_tally[9] = 0;
    return;
  } else if (global_tally[0] == 0 && global_tally[9] + 1 == len) { // b
    // We know that global_tally[1:8] contains a single value (i.e. is onehot)
    for (int ii = 8; ii >= 1; ii--) {
      if (global_tally[ii]) {
        global_tally[ii]--;
        global_tally[0] = global_tally[9];
        global_tally[9] = 0;
        global_tally[ii+1]++;
        break;
      }
    }
  } else if (global_tally[0] > 0 && global_tally[0] == len) { // e
    printf("Invalid case found with all zeroes!\n");
    exit(1);
  } else if (global_tally[0] > 0 && global_tally[9] > 0 && global_tally[0] + global_tally[9] == len) { // f
    global_tally[0]--;
    global_tally[9]++;
  } else if (global_tally[0] > 0 && global_tally[0] + global_tally[9] + 1 == len) { // d
    global_tally[0]--;
    // We know that global_tally[1:8] contains a single value (i.e. is onehot)
    for (int ii = 8; ii >= 1; ii--) {
      if (global_tally[ii]) {
        global_tally[ii] = global_tally[9] + 2;
        break;
      }
    }
    global_tally[9] = 0;
  } else { // c
    for (int ii = 8; ii >= 1; ii--) {
      if (global_tally[ii]) {
        global_tally[ii]--;
        int nines = global_tally[9];
        global_tally[9] = 0;
        global_tally[ii+1] += 1 + nines;
        break;
      }
    }
  }
}

void print_tally (int tally[10]) {
  int first_nonzero = 0;
  for (int ii = 1; ii < 10; ii++) {
    if (tally[ii] > 0) {
      printf("%d", ii);
      first_nonzero = ii;
      tally[ii]--;
      break;
    }
  }
  for (int ii = 0; ii < tally[0]; ii++) {
    printf("0");
  }
  for (int ii = 1; ii < 10; ii++) {
    for (int jj = 0; jj < tally[ii]; jj++) {
      printf("%d", ii);
    }
  }
  if (first_nonzero > 0) {
    tally[first_nonzero]++;
  }
}

void* start_primevals_thread (void* inp) {
  int local_tally[10] = {0,0,0,0,0,0,0,0,0,0};
  int local_idx = 0;

  while (1) {
    // Get next tally
    pthread_mutex_lock(&global_tally_mutex);
    save_global_tally_to(local_tally);
    local_idx = global_idx;
    next_tally_global();
    pthread_mutex_unlock(&global_tally_mutex);

    int prime_total = 0;
    //printf("Testing ");
    //print_tally(local_tally);
    //printf("\n");
    int tried = permute(local_tally, &prime_total, 0, 1, 0, 1, 1, 0);
    //printf("Report ");
    pthread_mutex_lock(&global_tally_mutex);
    print_tally(local_tally);
    printf(" %d %d %d\n", local_idx, tried, prime_total);
    fflush(stdout);
    pthread_mutex_unlock(&global_tally_mutex);
  }

  return NULL;
}

#define NUM_THREADS 8

int main () {
  global_idx = 0;
  global_tally[0] = 1;
  global_tally[1] = 2;
  global_tally[2] = 1;
  global_tally[3] = 1;
  global_tally[4] = 1;
  global_tally[5] = 1;
  global_tally[6] = 1;
  global_tally[7] = 1;
  global_tally[8] = 0;
  global_tally[9] = 1;
  len = 10;

  pthread_t threads[NUM_THREADS];
  for (int ii = 0; ii < NUM_THREADS; ii++) {
    pthread_create(&threads[ii], NULL, start_primevals_thread, NULL);
  }

  for (int ii = 0; ii < NUM_THREADS; ii++) {
    pthread_join(threads[ii], NULL);
  }
}
