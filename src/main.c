#include <ncurses.h>

typedef struct {
  char symbol;
  int x;
  int y;
} Part;
typedef struct {
  int size;
  int speed;
  Part part[20];
} Snake;

int main (void) {
  // Setup
  // - Init and draw
  //   - Window
  ESCDELAY = 0;
  WINDOW *win;
  initscr();
  noecho();
  cbreak();
  curs_set(0);
  keypad(stdscr, TRUE);
  int WIN_HEIGHT = 24;
  int WIN_WIDTH = 40;
  int START_X = (COLS - WIN_WIDTH) / 2;
  int START_Y = (LINES - WIN_HEIGHT) / 2;
  refresh();
  win = newwin(WIN_HEIGHT, WIN_WIDTH, START_Y, START_X);
  box(win, 0, 0);
  wrefresh(win);
  //   - Snake
  Snake snake = {
    .size = 0,
    .part[0].symbol = '@',
    .part[0].x = COLS/2,
    .part[0].y = LINES/2,
  };
  mvaddch(snake.part[0].y, snake.part[0].x, snake.part[0].symbol);
  //   - Dots
  // Game loop
  // - Move snake
  // - Check collisions
  //   - Boundary
  //   - Body
  //   - Food
  // - Draw objects
  int ch;
  while ((ch = getch()) != 'q') {
    switch (ch) {
      case KEY_LEFT:
        if (snake.part[0].x-1 > START_X) {
          mvaddch(snake.part[0].y, snake.part[0].x--, ' ');
          mvaddch(snake.part[0].y, snake.part[0].x, snake.part[0].symbol);
        }
        break;
      case KEY_RIGHT:
        if (snake.part[0].x+1 < START_X+WIN_WIDTH-1) {
          mvaddch(snake.part[0].y, snake.part[0].x++, ' ');
          mvaddch(snake.part[0].y, snake.part[0].x, snake.part[0].symbol);
        }
        break;
      case KEY_UP:
        if (snake.part[0].y-1 > START_Y) {
          mvaddch(snake.part[0].y--, snake.part[0].x, ' ');
          mvaddch(snake.part[0].y, snake.part[0].x, snake.part[0].symbol);
        }
        break;
      case KEY_DOWN:
        if (snake.part[0].y+1 < START_Y+WIN_HEIGHT-1) {
          mvaddch(snake.part[0].y++, snake.part[0].x, ' ');
          mvaddch(snake.part[0].y, snake.part[0].x, snake.part[0].symbol);
        }
        break;
    }
    wrefresh(win);
  }

  endwin();
  return 0;
}
