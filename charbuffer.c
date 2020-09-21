
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "charbuffer.h"

CharBuffer buffer_alloc(uint16_t width,uint16_t height) {
    CharBuffer ret;
    ret.width=width;
    ret.height=height;
    ret.data=calloc(1,width*height*sizeof(char));
    return ret;
}

void buffer_clear(CharBuffer *buffer,char character) {  
    memset(buffer->data, character, buffer->width*buffer->height*sizeof(char));
}
bool buffer_was_initalized(CharBuffer *buffer) {
    return buffer->data;
}
void buffer_resize(CharBuffer *buffer,uint16_t width,uint16_t height) {
    buffer->width=width;
    buffer->height=height;
    buffer->data=realloc(buffer->data,width*height*sizeof(char));
}
char buffer_read(CharBuffer *buffer,uint16_t x,uint16_t y) {
    if(x>=buffer->width || y>=buffer->height) return 0;
    return buffer->data[y*buffer->width+x];

}
void buffer_write(CharBuffer *buffer,uint16_t x,uint16_t y,char c) {
    if(x>=buffer->width || y>=buffer->height) return;
    buffer->data[y*buffer->width+x]=c;
}
void buffer_write_text(CharBuffer *buffer,uint16_t x, uint16_t y,char *text) {
    int initial_position= y*buffer->width +x;
    size_t str_size=strlen(text);
    
    // check memory pointer limit
    if(initial_position+str_size<buffer->width*buffer->height) {
         memcpy(buffer->data+initial_position, text, str_size*sizeof(char));
    }
};
void buffer_draw_line(CharBuffer *buffer,uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,char character) {

  uint16_t dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
  uint16_t dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
  uint16_t err = (dx>dy ? dx : -dy)/2, e2;

  for(;;){
    buffer_write(buffer,x0,y0,character);
    if (x0==x1 && y0==y1) break;
    e2 = err;
    if (e2 >-dx) { err -= dy; x0 += sx; }
    if (e2 < dy) { err += dx; y0 += sy; }
  }
}

