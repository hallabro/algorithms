#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "quick-find.h"

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Usage: quick-find <size>\n");
    return 1;
  }

  size_t s = atoi(argv[1]);
  uint8_t id[s];

  // initialize with default values, each item being its own component
  for (size_t i = 0; i < s; i++) {
    id[i] = i;
  }

  char cmd[25];
  while(fgets(cmd, sizeof(cmd), stdin)) {
    if(strcmp(cmd, "connect\n") == 0) {
      int8_t p, q;
      char input[10];
      fgets(input, sizeof(input), stdin);
      sscanf(input, "%hhd %hhd", &p, &q);
      qf_union(id, s, p, q);
      printf("connected %d to %d\n", p, q);
    } else if (strcmp(cmd, "check\n") == 0) {
      printf("%d\n", qf_find(id, 1, 2));
    } else if (strcmp(cmd, "print\n") == 0) {
      for (size_t i = 0; i < s; i++) {
        printf("index %lu => %d\n", i, id[i]);
      }
    }
  }

  return 0;
}

bool qf_find(uint8_t *id, uint8_t p, uint8_t q) {
  return id[p] == id[q];
}

void qf_union(uint8_t *id, size_t size, uint8_t p, uint8_t q) {
  // pid and qid are necessary because id[p] and id[q] may change
  // during union
  uint8_t pid = id[p];
  uint8_t qid = id[q];

  for (size_t i = 0; i < size; i++) {
    if (id[i] == pid) {
      id[i] = qid;
    }
  }
}
