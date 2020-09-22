#ifdef __unix__
#define _BSD_SOURCE
# include <unistd.h>
#elif defined _WIN32
# include <windows.h>
#define sleep(x) Sleep(1000 * (x))
#endif

#include "charbuffer.h"
#include "ncurses.h"
#include "stdlib.h"

CharBuffer *buffer;
CharBuffer buffer_str;
WINDOW *stdscr;

int key;

void chlib_init() {
    stdscr=initscr();
    buffer_str=buffer_alloc(getmaxx(stdscr),getmaxy(stdscr));
    buffer=&buffer_str;
    keypad(stdscr, TRUE);
    noecho();
    raw();
    nodelay(stdscr, TRUE);
}
int chlib_getKey() {
    return key;
}
void draw_border() {
    buffer_draw_line(buffer,0,0,buffer->width-1,0,'+');
    buffer_draw_line(buffer,0,0,0,buffer->height-1,'P');
    buffer_draw_line(buffer,0,buffer->height-1,buffer->width-1,buffer->height-1,'+');
    buffer_draw_line(buffer,buffer->width-1,0,buffer->width-1,buffer->height-1,'P');
}
void chlib_update() {
        int new_width=getmaxx(stdscr);
        int new_height=getmaxy(stdscr);
        if(new_width!=buffer->width || new_height!=buffer->height) {
            buffer_resize(buffer,new_width,new_height);
        }
        draw_border();
       move(0,0);
        for(int y=0;y<buffer->height;y++) {
            for(int x=0;x<buffer->width;x++) {
                  addch(buffer_read(buffer,x,y));
            }
        }
        refresh();

        buffer_clear(buffer,' ');
        key=getch();
        usleep(1000*70);

}
void chlib_end() {
    endwin();
}
