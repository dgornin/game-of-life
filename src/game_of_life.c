// Copyright 2022 @student.21-school.ru

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Game settingis
#define RIGHT_BORDER 80
#define DOWN_BORDER 25
#define UP_BORDER 0
#define LEFT_BORDER 0

void draw(int (*feld)[RIGHT_BORDER], int tic);
void input(int (*feld)[RIGHT_BORDER]);
int x_rotate(int x);
int y_rotate(int y);
int check(int (*feld)[RIGHT_BORDER], int x, int y, int item);
int new_live(int (*feld)[RIGHT_BORDER], int (*feld_c)[RIGHT_BORDER], int x,
             int y);
int dead(int (*feld)[RIGHT_BORDER], int (*feld_c)[RIGHT_BORDER], int x, int y);
void copy(int (*feld)[RIGHT_BORDER], int (*feld_c)[RIGHT_BORDER]);
int is_empty(int (*feld)[RIGHT_BORDER]);

int main() {
  int tic = 100;
  int game = 1;
  int feld[DOWN_BORDER][RIGHT_BORDER];
  int feld_c[DOWN_BORDER][RIGHT_BORDER];
  for (int i = 0; i < DOWN_BORDER; i++) {
    for (int j = 0; j < RIGHT_BORDER; j++) {
      feld[i][j] = 0;
    }
  }
  input(feld);
  initscr();
  cbreak();
  noecho();
  scrollok(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  if (!freopen("/dev/tty", "r", stdin)) {
    perror("/dev/tty");
    game = 0;
  }
  curs_set(0);
  clear();
  while (game) {
    draw(feld, tic);
    char inp = getch();
    usleep(tic * 1000);
    copy(feld_c, feld);
    int state = 0;
    for (int i = 0; i < DOWN_BORDER; i++) {
      for (int j = 0; j < RIGHT_BORDER; j++) {
        state += new_live(feld, feld_c, i, j);
        state += dead(feld, feld_c, i, j);
      }
    }
    if (state == 0 || is_empty(feld)) {
      game = 0;
    }
    if (inp == 'Q' || inp == 'q') {
      game = 0;
    }
    if (inp == 'S' || inp == 's') {
      if (tic - 10 >= 0) {
        tic -= 10;
      }
    }
    if (inp == 'A' || inp == 'a') {
      tic += 10;
    }
    copy(feld, feld_c);
    refresh();
  }
  draw(feld, tic);
  endwin();
  printf("End of evolution");
  return 0;
}

void draw(int (*feld)[RIGHT_BORDER], int tic) {
  clear();
  for (int i = UP_BORDER; i < DOWN_BORDER; ++i) {
    for (int j = LEFT_BORDER; j < RIGHT_BORDER; ++j) {
      if (feld[i][j]) {
        printw("#");
      } else {
        printw(".");
      }
    }
    printw("\n");
  }
  printw("\n");
  printw("press Q/q to exit the game, A/a and S/s to change speed\n");
  printw("Speed: %d", tic);
}

void input(int (*feld)[RIGHT_BORDER]) {
  char t;
  for (int i = UP_BORDER; i < DOWN_BORDER; ++i) {
    for (int j = LEFT_BORDER; j < RIGHT_BORDER; ++j) {
      t = getchar();
      if (t == '0') {
        feld[i][j] = 0;
      }
      if (t == '1') {
        feld[i][j] = 1;
      }
    }
    getchar();
  }
}

int new_live(int (*feld)[RIGHT_BORDER], int (*feld_c)[RIGHT_BORDER], int x,
             int y) {
  int f = 0;
  if (feld[x][y] == 0) {
    if (check(feld, x, y, 1) == 3) {
      feld_c[x][y] = 1;
      f = 1;
    }
  }
  return f;
}

int dead(int (*feld)[RIGHT_BORDER], int (*feld_c)[RIGHT_BORDER], int x, int y) {
  int f = 0;
  if (feld[x][y] == 1) {
    int c = check(feld, x, y, 1);
    if (c < 2 || c > 3) {
      feld_c[x][y] = 0;
      f = 1;
    }
  }
  return f;
}

int check(int (*feld)[RIGHT_BORDER], int x, int y, int item) {
  int c = 0;
  if (feld[x_rotate(x - 1) % DOWN_BORDER][y_rotate(y - 1) % RIGHT_BORDER] ==
      item) {
    c += 1;
  }
  if (feld[x_rotate(x - 1) % DOWN_BORDER][y] == item) {
    c += 1;
  }
  if (feld[x_rotate(x - 1) % DOWN_BORDER][(y + 1) % RIGHT_BORDER] == item) {
    c += 1;
  }
  if (feld[x][(y + 1) % RIGHT_BORDER] == item) {
    c += 1;
  }
  if (feld[(x + 1) % DOWN_BORDER][(y + 1) % RIGHT_BORDER] == item) {
    c += 1;
  }
  if (feld[(x + 1) % DOWN_BORDER][y] == item) {
    c += 1;
  }
  if (feld[(x + 1) % DOWN_BORDER][y_rotate(y - 1) % RIGHT_BORDER] == item) {
    c += 1;
  }
  if (feld[x][y_rotate(y - 1) % RIGHT_BORDER] == item) {
    c += 1;
  }
  return c;
}

int x_rotate(int x) {
  int ans;
  if (x < 0) {
    ans = DOWN_BORDER + x;
  } else {
    ans = x;
  }
  return ans;
}

int y_rotate(int y) {
  int ans;
  if (y < 0) {
    ans = RIGHT_BORDER + y;
  } else {
    ans = y;
  }
  return ans;
}

void copy(int (*feld)[RIGHT_BORDER], int (*feld_c)[RIGHT_BORDER]) {
  for (int i = 0; i < DOWN_BORDER; i++) {
    for (int j = 0; j < RIGHT_BORDER; j++) {
      feld[i][j] = feld_c[i][j];
    }
  }
}

int is_empty(int (*feld)[RIGHT_BORDER]) {
  int f = 1;
  for (int i = 0; i < DOWN_BORDER; i++) {
    for (int j = 0; j < RIGHT_BORDER; j++) {
      if (feld[i][j] == 1) {
        f = 0;
      }
    }
  }
  return f;
}
