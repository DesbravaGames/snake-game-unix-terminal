#ifndef CHBUFFER_H_INCLUDED
#define CHBUFFER_H_INCLUDED

#include "stdint.h"
#include "stdbool.h"

typedef struct {
    uint16_t width,height;
    char *data;
} CharBuffer;

CharBuffer buffer_alloc(uint16_t width,uint16_t height);

void buffer_clear(CharBuffer *buffer,char character);
bool buffer_was_initalized(CharBuffer *buffer);
void buffer_resize(CharBuffer *buffer,uint16_t width,uint16_t height);
char buffer_read(CharBuffer *buffer,uint16_t x,uint16_t y);

void buffer_write(CharBuffer *buffer,uint16_t x,uint16_t y,char c);
void buffer_write_text(CharBuffer *buffer,uint16_t x, uint16_t y,char text[]);
void buffer_draw_line(CharBuffer *buffer,uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,char character);

#endif // CHBUFFER_H_INCLUDED
