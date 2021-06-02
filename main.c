#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <stdbool.h>
#include <time.h>
#include "screen.c"

#define PLAY 0
#define GAMEOVER 1
int mode=PLAY;
int score=0;
int best_score=0;
int update_screen=0;

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

typedef struct SnakeTail SnakeTail;
typedef struct Snake Snake;
struct Snake {
    float x, y;
    int direction;
    SnakeTail *r;
};
struct SnakeTail {
    Vec2 position;
    SnakeTail *r;
};
Snake snake={0};
Vec2 food1={0};
Vec2 food2={0};

void recreate_food(Vec2 *food){
    food->y=(rand() % (buffer->height-3))+1;
    food->x=(rand() % (buffer->width-3))+1;
}

void freeSnakeTail() {
    if(snake.r) {
        SnakeTail *r=snake.r;
        while(r!=NULL) {
            SnakeTail *proximosnaketail=r->r;
            free(r);
            r=proximosnaketail;
        }
    }
}
void init_snake() {
    snake.direction=0;
    snake.x=buffer->width/2.0f;
    snake.y=buffer->height/2.0f;
    snake.r=NULL;
    freeSnakeTail();
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
    buffer_write(buffer,(int) snake.x,(int) snake.y,'O');
    drawSnakeTail(snake.r);
}
void drawfood(Vec2 food) {
    buffer_write(buffer,food.x,food.y,'x');
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
    SnakeTail *r=calloc(1,sizeof(SnakeTail));
        r->r=NULL;

    SnakeTail *lastSnakeTail=snake.r;
    if(lastSnakeTail) {
        while(lastSnakeTail->r) {lastSnakeTail=lastSnakeTail->r;}
        lastSnakeTail->r=r;
    } else snake.r=r;

    r->position.x=lastSnakeTail->position.x;
    r->position.y=lastSnakeTail->position.y;
    ////*/
}

bool snake_collide_withself(Snake *pos, SnakeTail *r) {
    if(!r) return false;
    if((int)(pos->x)==(int)(r->position.x) && (int)(pos->y)==(int)(r->position.y)) {
        return true;
    } else {
        return snake_collide_withself(pos,r->r);
    }
}
bool snake_eat_food(Snake snake,Vec2 food) {
    return ((int) snake.x==food.x) &&
           ((int)snake.y)==food.y;
} 
void updateSnake() {
    int width=buffer->width-2;
    int height=buffer->height-2;
    int key=screen_getKey();

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
        snake.r->position.x=snake.x;
        snake.r->position.y=snake.y;
        if(snake.r->r) { updateSnakeTail(snake.r);}
     }
    if(snake_eat_food(snake,food1)) {
        recreate_food(&food1);
        score++;
        createSnakeTail();
        beep();
    }
    if(snake_eat_food(snake,food2)) {
        recreate_food(&food2);
        score++;
        createSnakeTail();
        beep();
    }
    if(snake.direction==KEY_RIGHT) {
        snake.x++;
        if(snake.x>width) snake.x=1;
    }
    if(snake.direction==KEY_LEFT) {
        snake.x--;
        if(snake.x<1) snake.x=width-1;
    }
    if(snake.direction==KEY_DOWN) {
        snake.y+=0.7;
        if(snake.y>height) snake.y=1;
    }
    if(snake.direction==KEY_UP) {
        snake.y-=0.7;
        if(snake.y<1) snake.y=height-1;
    }
   if(snake_collide_withself(&snake,snake.r)) {
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
    screen_init();
    init_snake();
    recreate_food(&food1);
    recreate_food(&food2);
    while(screen_getKey()!='x') {
        usleep(1000*100);
        if(mode==PLAY) {
            updateSnake();
        } else {
            if(key=='\n' || key==' ') {
                beep();
                score=0;
                    mode=PLAY;
            }
        }
        if(true) {
            update_screen=false;
            if(mode==PLAY) {
                drawSnake();
                drawfood(food1);
                drawfood(food2);
            } else {
                buffer_write_text(buffer,(buffer->width/2)-5,buffer->height/2,"GAME OVER!!");
                buffer_write_text(buffer,(buffer->width/2)-9,(buffer->height/2)+1,str_score);
            }
            screen_update();
        }
    }
    screen_end();
    return 0;
}
