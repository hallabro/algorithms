#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

void read_pair(int8_t *p, int8_t *q);
uint8_t qu_root(uint8_t *id, uint8_t i);
bool qu_connected(uint8_t *id, uint8_t p, uint8_t q);
void qu_union(uint8_t *id, uint8_t p, uint8_t q);
