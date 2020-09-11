#ifndef CHBUFFER_H_INCLUDED
#define CHBUFFER_H_INCLUDED
#include "stdlib.h"

char *buffer=NULL;
int buffer_width=0;
int buffer_height=0;

extern void buffer_create(int width,int height) {
    buffer_width=width;
    buffer_height=height;
    buffer=calloc(width*height,sizeof(char));
}
extern int buffer_getWidth() {
    return buffer_width;
}
extern void buffer_clear(char c) {
    memset(buffer, c, buffer_width*buffer_height*sizeof(char));
}
extern int buffer_getHeight() {
    return buffer_height;
}
extern int buffer_iscreated() {
    return buffer!=NULL;
}
extern void buffer_resize(int width,int height) {
    buffer_width=width;
    buffer_height=height;
    buffer=realloc(buffer,width*height*sizeof(char));
}
extern char buffer_read(int x,int y) {
    if(x>=buffer_width || y>=buffer_height) return 0;
    return *(buffer+ y*buffer_width +x);
}

extern void buffer_write(int x,int y,char c) {
    if(x>=buffer_width || y>=buffer_height) return;
    *(buffer+ y*buffer_width +x)=c;
}
extern void buffer_write_text(int x, int y,char *text) {
    int posicao_inicial= y*buffer_width +x;
    memcpy(buffer+posicao_inicial, text, strlen(text)*sizeof(char));
}
extern void buffer_draw_line(int x0, int y0, int x1, int y1,char c) {

  int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
  int err = (dx>dy ? dx : -dy)/2, e2;

  for(;;){
    buffer_write(x0,y0,c);
    if (x0==x1 && y0==y1) break;
    e2 = err;
    if (e2 >-dx) { err -= dy; x0 += sx; }
    if (e2 < dy) { err += dx; y0 += sy; }
  }
}

#endif // CHBUFFER_H_INCLUDED
