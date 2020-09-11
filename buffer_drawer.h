#ifndef CH_LIBRARY_INCLUDED
#define CH_LIBRARY_INCLUDED

#include "chbuffer.h"
#include "ncurses.h"
#include "stdlib.h"
int key;

extern void chlib_init() {
   initscr();
    keypad(stdscr, TRUE);
    noecho();
    raw();
    nodelay(stdscr, TRUE);
    buffer_create(getmaxx(stdscr),getmaxy(stdscr));

}
extern int chlib_getKey() {
    return key;
}
void draw_border() {
buffer_draw_line(0,0,buffer_width-1,0,'+');
buffer_draw_line(0,0,0,buffer_height-1,'+');
buffer_draw_line(0,buffer_height-1,buffer_width-1,buffer_height-1,'+');
buffer_draw_line(buffer_width-1,0,buffer_width-1,buffer_height-1,'+');

}
extern void chlib_update() {

        int new_width=getmaxx(stdscr);
        int new_height=getmaxy(stdscr);
        if(new_width!=buffer_getWidth() || new_height!=buffer_getHeight()) {
            buffer_resize(new_width,new_height);
        }
       move(0,0);
        for(int y=0;y<buffer_getHeight();y++) {
            for(int x=0;x<buffer_getWidth();x++) {
                int ch=buffer_read(x,y);
                if(ch==0) {printw(" ");}
                else { printw("%c",ch);}
            }
        }
        refresh();

        buffer_clear(' ');
        draw_border();
        key=getch();
        usleep(1000*70);

}
extern void chlib_end() {
    endwin();
}
#endif // CH_LIBRARY_INCLUDED
