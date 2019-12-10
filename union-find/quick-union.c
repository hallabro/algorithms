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

  // initialize with default values, each item being its own component
  for (size_t i = 0; i < s; i++) {
    id[i] = i;
  }

  char cmd[25];
  int8_t p, q;
  while(fgets(cmd, sizeof(cmd), stdin)) {
    if(strcmp(cmd, "connect\n") == 0) {
      read_pair(&p, &q);
      qu_union(id, p, q);
      printf("connected %d to %d\n", p, q);
    } else if (strcmp(cmd, "check\n") == 0) {
      read_pair(&p, &q);
      printf("%d\n", qu_connected(id, p, q));
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
    i = id[i];
  }

  return i;
}

bool qu_connected(uint8_t *id, uint8_t p, uint8_t q) {
  return id[qu_root(id, p)] == id[qu_root(id, q)];
}

void qu_union(uint8_t *id, uint8_t p, uint8_t q) {
  id[qu_root(id, p)] = id[qu_root(id, q)];
}
