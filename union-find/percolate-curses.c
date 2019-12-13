#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

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

double next_bool(double probability) {
  return random() <  probability * ((double)RAND_MAX + 1.0);
}

uint16_t flatten_grid_pos(int x, int y, int width) {
  return y * width + x;
}

int main(void) {
  initscr();
  noecho();
  start_color();
  curs_set(0);

  int height, width;
  getmaxyx(stdscr, height, width);

  uint16_t size = height * width;
  bool grid[width][height];
  double probability = 0.5;
  uint16_t qu[size];
  uint16_t qu_size[size];

  for (size_t i = 0; i < size; i++) {
    qu[i] = i;
    qu_size[i] = 0;
  }

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      grid[x][y] = next_bool(probability);
      if (!grid[x][y] || y == 0) { // skip if cell is blocked or we're
                                   // on the first row
        continue;
      }

      if (grid[x][y-1]) {
        qu_union(
          qu,
          qu_size,
          flatten_grid_pos(x, y, width),
          flatten_grid_pos(x, y-1, width)
        );
      }
    }
  }

  WINDOW *window = newwin(height, width, 0, 0);

  init_pair(1, COLOR_BLACK, COLOR_WHITE);

  move(10, 10);
  attron(COLOR_PAIR(1));

  for (int y = 1; y < height - 1; y++) {
    for (int x = 1; x < width - 1; x++) {
      move(y, x);
      if (grid[x-1][y-1]) {
        printw(" ");
      }
    }
  }

  refresh();

  int ch = getch();
  endwin();

  return 0;
}
