#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

int WIN_HEIGHT;
int WIN_WIDTH;
int START_X;
int START_Y;
int CLOCK_SPEED_US = 80000;
const int MAX_SIZE = 20;
char position[25];

typedef enum {
  UP,
  DOWN,
  LEFT,
  RIGHT,
} DIRECTION ;

typedef struct {
  char symbol;
  int x;
  int y;
} Part;

typedef struct {
  int size;
  DIRECTION direction;
  int speed;
  Part part[MAX_SIZE];
} Snake;

Snake move_snake(Snake snake) {
  int dx = 0;
  int dy = 0;
  switch (snake.direction) {
    case LEFT:
      if (snake.part[0].x-1 > START_X) {
        dx = -1;
      }
      break;
    case RIGHT:
      if (snake.part[0].x+1 < START_X+WIN_WIDTH-1) {
        dx = 1;
      }
      break;
    case UP:
      if (snake.part[0].y-1 > START_Y) {
        dy = -1;
      }
      break;
    case DOWN:
      if (snake.part[0].y+1 < START_Y+WIN_HEIGHT-1) {
        dy = +1;
      }
      break;
  }
  int px = snake.part[0].x;
  int py = snake.part[0].y;
  mvaddch(snake.part[0].y, snake.part[0].x, ' ');
  snake.part[0].x += dx;
  snake.part[0].y += dy;
  mvaddch(snake.part[0].y, snake.part[0].x, snake.part[0].symbol);
  for (int i = 1; i < snake.size; i++) {
    mvaddch(snake.part[i].y, snake.part[i].x, ' ');
    int tx = px;
    int ty = py;
    px = snake.part[i].x;
    py = snake.part[i].y;
    snake.part[i].x = tx;
    snake.part[i].y = ty;
    mvaddch(snake.part[i].y, snake.part[i].x, snake.part[i].symbol);
  }
  return snake;
}

int main (void) {
  // Setup
  // - Init and draw
  //   - Window
  ESCDELAY = 0;
  WINDOW *win;
  initscr();
  noecho();
  cbreak();
  nodelay(stdscr, TRUE);
  curs_set(0);
  keypad(stdscr, TRUE);
  WIN_HEIGHT = 24;
  WIN_WIDTH = 41;
  START_X = (COLS - WIN_WIDTH) / 2;
  START_Y = (LINES - WIN_HEIGHT) / 2;
  refresh();
  win = newwin(WIN_HEIGHT, WIN_WIDTH, START_Y, START_X);
  box(win, 0, 0);
  wrefresh(win);
  //   - Snake
  Snake snake = {
    .size = MAX_SIZE-1,
    .direction = RIGHT,
    .part[0].symbol = '@',
    .part[0].x = COLS/2,
    .part[0].y = LINES/2,
  };
  for (int i = 1; i < snake.size; i++) {
    snake.part[i].symbol = '0';
    snake.part[i].x = COLS/2-i;
    snake.part[i].y = LINES/2;
  }
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
        snake.direction = LEFT;
        break;
      case KEY_RIGHT:
        snake.direction = RIGHT;
        break;
      case KEY_UP:
        snake.direction = UP;
        break;
      case KEY_DOWN:
        snake.direction = DOWN;
        break;
      case '<':
        CLOCK_SPEED_US += 10000;
        break;
      case '>':
        if (CLOCK_SPEED_US - 10000 >= 0) {
          CLOCK_SPEED_US -= 10000;
        }
        break;
    }
    snake = move_snake(snake);
    // Need to decouple game loop from clock speed
    usleep(CLOCK_SPEED_US);
    sprintf(position, "Head: %d,%d", snake.part[0].x, snake.part[0].y);
    mvaddstr(LINES-1, 0, position);

    wrefresh(win);
  }

  endwin();
  return 0;
}
