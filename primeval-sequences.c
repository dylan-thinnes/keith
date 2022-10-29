#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int len = 1;
int seq[10] = {0,1,0,0,0,0,0,0,0,0};

void print_seq () {
  printf(" %d:", len);
  for (int ii = 0; ii < 10; ii++) {
    printf(" %d", seq[ii]);
  }

  printf(" : ");

  int first_nonzero = 0;
  for (int ii = 1; ii < 10; ii++) {
    if (seq[ii] > 0) {
      printf("%d", ii);
      first_nonzero = ii;
      seq[ii]--;
      break;
    }
  }
  for (int ii = 0; ii < seq[0]; ii++) {
    printf("0");
  }
  for (int ii = 1; ii < 10; ii++) {
    for (int jj = 0; jj < seq[ii]; jj++) {
      printf("%d", ii);
    }
  }
  if (first_nonzero > 0) {
    seq[first_nonzero]++;
  }
  printf("\n");
}

void next () {
  if (seq[9] == len) { // a
    //printf("a ");
    len += 1;
    seq[0] = seq[9];
    seq[1] = 1;
    seq[9] = 0;
    return;
  } else if (seq[0] == 0 && seq[9] + 1 == len) { // b
    //printf("b ");
    // We know that seq[1:8] contains a single value (i.e. is onehot)
    for (int ii = 8; ii >= 1; ii--) {
      if (seq[ii]) {
        seq[ii]--;
        seq[0] = seq[9];
        seq[9] = 0;
        seq[ii+1]++;
        break;
      }
    }
  } else if (seq[0] > 0 && seq[0] == len) { // e
    //printf("e ");
    printf("AHHHHH!\n");
    exit(1);
  } else if (seq[0] > 0 && seq[9] > 0 && seq[0] + seq[9] == len) { // f
    //printf("f ");
    seq[0]--;
    seq[9]++;
  } else if (seq[0] > 0 && seq[0] + seq[9] + 1 == len) { // d
    //printf("d ");
    seq[0]--;
    // We know that seq[1:8] contains a single value (i.e. is onehot)
    for (int ii = 8; ii >= 1; ii--) {
      if (seq[ii]) {
        //printf("%d %d %d\n", ii, seq[ii], seq[9]);
        seq[ii] = seq[9] + 2;
        //printf("%d %d %d\n", ii, seq[ii], seq[9]);
        break;
      }
    }
    seq[9] = 0;
  } else { // c
    //printf("c ");
    for (int ii = 8; ii >= 1; ii--) {
      if (seq[ii]) {
        seq[ii]--;
        int nines = seq[9];
        seq[9] = 0;
        seq[ii+1] += 1 + nines;
        break;
      }
    }
  }
}

int main () {
  for (int ii = 0; ii < 1000; ii++) {
    printf("L%d ", ii);
    print_seq();
    next();
  }
}

/*
int map[20] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int map_max = 0;

void next_tally () {
  // (b) 0{n}[1-9]+[0-8]9{m} -> 0{n}[1-9]+[1-9]9{m}
  // (c) 0{n}[1-9]9{m} -> 0{n-1}[1-9]{m+1}
  // (d) 9{n} -> 10{n}
  // (e) 0{n}[0-9] -> 0{n-1}[0-9]{2}

  // Check (d)
  if (map_len == 1 && map[0] == 9) {
    map_len = 2;
    map[0] = 0;
    map[1] = map[1];
    map[2] = 1;
    map[3] = 1;
  }

  int zeroes = map[0] == 0 ? map[1] : 0;
  int nines = map[(map_len - 1) << 1] == 9 ? map[(map_len - 1) << 1 | 1] : 0;

  // Check (d)
  if (map_len == nines) {
    map_len = map_len + 1;
    map[0] = 0;
    map[1] = map[(map_len - 1) << 1];
    map[2] = 1;
    map[3] = 1;
  }

  // Check (e)
  if (map_len - 1 == zeroes) {
    map[0] = 0;
    map[1] = zeroes - 1;
    map[2] = map[2];
    map[3] = 1;
  }
}
*/

