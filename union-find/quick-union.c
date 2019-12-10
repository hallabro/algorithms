#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "quick-union.h"

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Usage: quick-union <size>\n");
    return 1;
  }

  size_t s = atoi(argv[1]);
  uint8_t id[s];

  uint8_t id_size[s]; // keep track of the tree sizes. this will allow
                      // us doing weighted quick union by always
                      // merging the smaller tree into the bigger one
                      // and not vice versa

  // initialize with default values, each item being its own component
  for (size_t i = 0; i < s; i++) {
    id[i] = i;
    id_size[i] = 0;
  }

  char cmd[25];
  int8_t p, q;
  while(fgets(cmd, sizeof(cmd), stdin)) {
    if(strcmp(cmd, "connect\n") == 0) {
      read_pair(&p, &q);
      qu_union(id, id_size, p, q);
      printf("connected %d to %d\n", p, q);
    } else if (strcmp(cmd, "check\n") == 0) {
      read_pair(&p, &q);
      printf("%d\n", qu_find(id, p, q));
    } else if (strcmp(cmd, "print\n") == 0) {
      for (size_t i = 0; i < s; i++) {
        printf("index %lu => %d\n", i, id[i]);
      }
    }
  }

  return 0;
}

void read_pair(int8_t *p, int8_t *q) {
  char input[10];

  fgets(input, sizeof(input), stdin);
  sscanf(input, "%hhd %hhd", p, q);
}

uint8_t qu_root(uint8_t *id, uint8_t i) {
  while(id[i] != i) {
    id[i] = id[id[i]]; // one pass path compression by letting every
                       // other node point to it's grandparent
    i = id[i];
  }

  return i;
}

bool qu_find(uint8_t *id, uint8_t p, uint8_t q) {
  return id[qu_root(id, p)] == id[qu_root(id, q)];
}

void qu_union(uint8_t *id, uint8_t *size, uint8_t p, uint8_t q) {
  int i = qu_root(id, p);
  int j = qu_root(id, q);

  if (size[i] > size[j]) {
    id[j] = i;
    size[i] += size[j];
  } else {
    id[i] = j;
    size[j] += size[i];
  }
}
