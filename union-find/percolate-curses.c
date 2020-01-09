#include <stdlib.h>
#include <curses.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <limits.h>

#define QU_VIRTUAL_TOP_REL_IDX    2
#define QU_VIRTUAL_BOTTOM_REL_IDX 1

uint16_t qu_root(uint16_t *qu, uint16_t i) {
  while(qu[i] != i) {
    qu[i] = qu[qu[i]]; // one pass path compression by letting every
                       // other node point to it's grandparent
    i = qu[i];
  }

  return i;
}

bool qu_find(uint16_t *qu, uint16_t p, uint16_t q) {
  return qu[qu_root(qu, p)] == qu[qu_root(qu, q)];
}

void qu_union(uint16_t *qu, uint16_t *size, uint16_t p, uint16_t q) {
  int i = qu_root(qu, p);
  int j = qu_root(qu, q);

  if (size[i] > size[j]) {
    qu[j] = i;
    size[i] += size[j];
  } else {
    qu[i] = j;
    size[j] += size[i];
  }
}

double next_bool(double probability) {
  return random() <  probability * ((double)RAND_MAX + 1.0);
}

uint16_t flatten_grid_pos(int x, int y, int width) {
  return y * width + x;
}

void qu_init(uint16_t* qu, uint16_t* qu_size, uint16_t size) {
  for (size_t i = 0; i < size; i++) {
    qu[i] = i;
    qu_size[i] = 0;
  }

  qu[size + QU_VIRTUAL_TOP_REL_IDX] = size + QU_VIRTUAL_TOP_REL_IDX;
  qu[size + QU_VIRTUAL_BOTTOM_REL_IDX] = size + QU_VIRTUAL_BOTTOM_REL_IDX;
  qu_size[size + QU_VIRTUAL_TOP_REL_IDX] = size + QU_VIRTUAL_TOP_REL_IDX;
  qu_size[size + QU_VIRTUAL_BOTTOM_REL_IDX] = size + QU_VIRTUAL_BOTTOM_REL_IDX;
}

int main(void) {
  srandom(time(0));

  initscr();
  noecho();
  start_color();
  curs_set(0);

  int height, width;
  getmaxyx(stdscr, height, width);

  uint16_t size = height * width;
  bool grid[width][height];

  // probability that a cell is blocking
  double probability = 0.3f;
  uint16_t qu[size + QU_VIRTUAL_TOP_REL_IDX];
  uint16_t qu_size[size + QU_VIRTUAL_TOP_REL_IDX];

  qu_init(qu, qu_size, size);

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      if (y == 0) {
        qu_union(qu, qu_size, x, size + 2);
      } else if (y == height - 1) {
        qu_union(qu, qu_size, flatten_grid_pos(x, y, width), size + 1);
      }

      grid[x][y] = next_bool(probability);
    }
  }

  for (int y = 0; y <= height - 1; y++) {
    for (int x = 0; x <= width - 1; x++) {
      if (y > 0 && !grid[x][y-1] && !grid[x][y]) {
        qu_union(qu, qu_size, flatten_grid_pos(x, y, width), flatten_grid_pos(x, y-1, width));
      }

      if (x > 0 && !grid[x-1][y] && !grid[x][y]) {
        qu_union(qu, qu_size, flatten_grid_pos(x, y, width), flatten_grid_pos(x-1, y, width));
      }
    }
  }

  newwin(height, width, 0, 0);
  init_pair(1, COLOR_BLACK, COLOR_WHITE);
  attron(COLOR_PAIR(1));

  for (int y = 0; y <= height - 1; y++) {
    for (int x = 0; x <= width - 1; x++) {
      move(y, x);
      if (!grid[x][y]) {
        printw(" ");
      }
    }
  }

  move(10, 10);

  if (qu_find(qu, size + 2, size + 1)) {
    printw("percolates");
  } else {
    printw("does not percolate :(");
  }
  refresh();

  getch();
  endwin();

  return 0;
}
