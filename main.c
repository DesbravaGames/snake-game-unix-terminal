#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include "buffer_drawer.c"

#define PLAY 0
#define GAMEOVER 1
int mode=PLAY;
int score=0;
int best_score=0;


void game_save() {
    FILE *fptr;
    fptr = fopen("best_score.dat", "w");
    if(fptr) {
      fprintf(fptr,"%d",best_score);
    }
     fclose(fptr);

}
void game_load() {
 FILE *fptr;
 fptr = fopen("best_score.dat", "r");
    if(fptr) {
        fscanf(fptr,"%d",&best_score);
        fclose(fptr);
    }
}
char str_score[30];

typedef struct {
    int x,y;
} Vec2;

void init_vec2(Vec2 *v) {
    v->x=0;v->y=0;
}
typedef struct SnakeTail SnakeTail;
typedef struct Snake Snake;
Snake snake;
Vec2 food1;
Vec2 food2;

void restart_food1(){
        food1.y=(rand() % (buffer->height-3))+1;
        food1.x=(rand() % (buffer->width-3))+1;
}
void restart_food2(){
        food2.y=(rand() % (buffer->height-3)+1);
        food2.x=(rand() % (buffer->width-3))+1;
}
struct Snake {
    Vec2 position;
    int direction;
    SnakeTail *r;
};


struct SnakeTail {
    Vec2 position;
    SnakeTail *r;
};
void liberarSnakeTail() {
    if(snake.r) {
        SnakeTail *r=snake.r;
        while(r!=NULL) {
            SnakeTail *proximosnaketail=r->r;
            free(r);
            r=proximosnaketail;
        }
    }
}
void init_snaketail(SnakeTail *snaketail) {
    init_vec2(&snaketail->position);
    snaketail->r=NULL;
}
void init_snake() {
    snake.direction=0;
    init_vec2(&snake.position);
    snake.position.x=buffer->width/2;
    snake.position.y=buffer->height/2;
    snake.r=NULL;
    liberarSnakeTail();
}
void drawSnakeTail(SnakeTail *r) {
 if(r) {
        if(r->r) {
        buffer_write(buffer,r->position.x,r->position.y,'o');
        drawSnakeTail(r->r);
        } else {
            buffer_write(buffer,r->position.x,r->position.y,'o');
        }
    }
}
void drawSnake() {
    buffer_write(buffer,snake.position.x,snake.position.y,'O');
    drawSnakeTail(snake.r);
}
void drawfood1() {
    buffer_write(buffer,food1.x,food1.y,'x');
}
void drawfood2() {
    buffer_write(buffer,food2.x,food2.y,'x');
}

void updateSnakeTail(SnakeTail *r) {
   if(r->r) {
       updateSnakeTail(r->r);
      r->r->position.x=r->position.x;
      r->r->position.y=r->position.y;
    }
}
void createSnakeTail() {
 // RABO NOVO
    SnakeTail *r=malloc(sizeof(SnakeTail));
        r->r=NULL;
        r->position.x=snake.position.x;
        r->position.y=snake.position.y;

    SnakeTail *lastSnakeTail=snake.r;
    if(lastSnakeTail) {
        while(lastSnakeTail->r) {lastSnakeTail=lastSnakeTail->r;}
        lastSnakeTail->r=r;
    } else snake.r=r;
    ////*/
}

int colide_com_snaketail(Vec2 *pos, SnakeTail *r) {
    if(r) {
        if(pos->x==r->position.x && pos->y==r->position.y) {
            return true;
        } else {
            return colide_com_snaketail(pos,r->r);
         }
    }
    return 0;
}
void updateSnake() {
    int width=buffer->width-2;
    int height=buffer->height-2;
    int key=chlib_getKey();

    if(key) {
        if((key==KEY_UP && snake.direction!=KEY_DOWN ) ||
            (key==KEY_DOWN && snake.direction!=KEY_UP ) ||
            (key==KEY_LEFT && snake.direction!=KEY_RIGHT) ||
            (key==KEY_RIGHT && snake.direction!=KEY_LEFT)
            ) {
                snake.direction=key;
            }
    }

    if(snake.r){
        snake.r->position.x=snake.position.x;
        snake.r->position.y=snake.position.y;
        if(snake.r->r) { updateSnakeTail(snake.r);}
     }
    if(snake.position.x==food1.x &&
        snake.position.y==food1.y) {///*
            restart_food1();
                score++;
                createSnakeTail();
                beep();
            }
        if(snake.position.x==food2.x &&
        snake.position.y==food2.y) {///*
            restart_food2();
                score++;
                createSnakeTail();
                beep();
            }
    if(snake.direction==KEY_RIGHT) {
        snake.position.x++;
        if(snake.position.x>width) snake.position.x=1;
    }
    if(snake.direction==KEY_LEFT) {
        snake.position.x--;
        if(snake.position.x<1) snake.position.x=width-1;
    }
    if(snake.direction==KEY_DOWN) {
        snake.position.y++;
        if(snake.position.y>height) snake.position.y=1;
    }
    if(snake.direction==KEY_UP) {
        snake.position.y--;
        if(snake.position.y<1) snake.position.y=height-1;
    }
   if(colide_com_snaketail(&snake.position,snake.r)) {
    mode=GAMEOVER;
    beep();
    init_snake();
    if(score>best_score) best_score=score;
    if(score==best_score){
     sprintf(str_score, "score: %d NEW Record!", score);
        game_save();
    }   else sprintf(str_score, "score: %d Best Score: %d", score,best_score);

   }
}
/* main.c */
int main(int argc, char *argv[]) {
    srand(time(NULL));
    game_load(); 
    chlib_init();
    init_snake();
    restart_food1();
    restart_food2();
    while(chlib_getKey()!='x') {
        if(mode==PLAY) {
            updateSnake();
            drawSnake();
            drawfood1();
            drawfood2();
        } else {
            buffer_write_text(buffer,(buffer->width/2)-5,buffer->height/2,"GAME OVER!!");
              buffer_write_text(buffer,(buffer->width/2)-9,(buffer->height/2)+1,str_score);
            if(key=='\n' || key==' ') {
            beep();
                score=0;
                    mode=PLAY;
            }
        }
        chlib_update();
    }
    chlib_end();
    return 0;
}
