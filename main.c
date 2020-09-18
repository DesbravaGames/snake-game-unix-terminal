#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include "buffer_drawer.c"

#define PLAY 0
#define GAMEOVER 1
int modo=PLAY;
int pontos=0;
int recorde=0;


void salvar() {
    FILE *fptr;
    fptr = fopen("recorde.dat", "w");
    if(fptr) {
      fprintf(fptr,"%d",recorde);
    }
     fclose(fptr);

}
void carregar() {
 FILE *fptr;
 fptr = fopen("recorde.dat", "r");
    if(fptr) {
        fscanf(fptr,"%d",&recorde);
        fclose(fptr);
    }
}
char str_pontos[30];

typedef struct {
    int x,y;
} Vec2;

void init_vec2(Vec2 *v) {
    v->x=0;v->y=0;
}
typedef struct Rabo Rabo;
typedef struct Cobrinha Cobrinha;
Cobrinha cobrinha;
Vec2 comida1;
Vec2 comida2;

void reposicionar_comida1(){
        comida1.y=(rand() % (buffer.height-3))+1;
        comida1.x=(rand() % (buffer.width-3))+1;
}
void reposicionar_comida2(){
        comida2.y=(rand() % (buffer.height-3)+1);
        comida2.x=(rand() % (buffer.width-3))+1;
}
struct Cobrinha {
    Vec2 position;
    int direction;
    Rabo *r;
};


struct Rabo {
    Vec2 position;
    Rabo *r;
};
void liberarRabo() {
    if(cobrinha.r) {
        Rabo *r=cobrinha.r;
        while(r!=NULL) {
            Rabo *proximorabo=r->r;
            free(r);
            r=proximorabo;
        }
    }
}
void init_rabo(Rabo *rabo) {
    init_vec2(&rabo->position);
    rabo->r=NULL;
}
void init_cobrinha() {
    cobrinha.direction=0;
    init_vec2(&cobrinha.position);
    cobrinha.position.x=buffer.width/2;
    cobrinha.position.y=buffer.height/2;
    cobrinha.r=NULL;
    liberarRabo();
}
void desenharRabo(Rabo *r) {
 if(r) {
        if(r->r) {
        buffer_write(&buffer,r->position.x,r->position.y,'o');
        desenharRabo(r->r);
        } else {
            buffer_write(&buffer,r->position.x,r->position.y,'o');
        }
    }
}
void desenharCobrinha() {
   buffer_write(&buffer,cobrinha.position.x,cobrinha.position.y,'O');
    desenharRabo(cobrinha.r);
}
void desenharComida1() {
    buffer_write(&buffer,comida1.x,comida1.y,'x');
}
void desenharComida2() {
    buffer_write(&buffer,comida2.x,comida2.y,'x');
}

void atualizarRabo(Rabo *r) {
   if(r->r) {
       atualizarRabo(r->r);
      r->r->position.x=r->position.x;
      r->r->position.y=r->position.y;
    }
}
void criarRabo() {
 // RABO NOVO
            Rabo *r=malloc(sizeof(Rabo));
                r->r=NULL;
                r->position.x=cobrinha.position.x;
                r->position.y=cobrinha.position.y;

               Rabo *ultimoRabo=cobrinha.r;
                if(ultimoRabo) {
                    while(ultimoRabo->r) {ultimoRabo=ultimoRabo->r;}
                    ultimoRabo->r=r;
                } else cobrinha.r=r;
              ////*/
}

int colide_com_rabo(Vec2 *pos, Rabo *r) {
    if(r) {
        if(pos->x==r->position.x && pos->y==r->position.y) {
            return true;
        } else {
            return colide_com_rabo(pos,r->r);
         }
    }
    return 0;
}
void atualizarCobra() {
    int width=buffer.width-2;
    int height=buffer.height-2;
    int key=chlib_getKey();

    if(key) {
        if((key==KEY_UP && cobrinha.direction!=KEY_DOWN ) ||
            (key==KEY_DOWN && cobrinha.direction!=KEY_UP ) ||
            (key==KEY_LEFT && cobrinha.direction!=KEY_RIGHT) ||
            (key==KEY_RIGHT && cobrinha.direction!=KEY_LEFT)
            ) {
                cobrinha.direction=key;
            }
    }

    if(cobrinha.r){
        cobrinha.r->position.x=cobrinha.position.x;
        cobrinha.r->position.y=cobrinha.position.y;
        if(cobrinha.r->r) { atualizarRabo(cobrinha.r);}
     }
    if(cobrinha.position.x==comida1.x &&
        cobrinha.position.y==comida1.y) {///*
            reposicionar_comida1();
                pontos++;
                criarRabo();
                beep();
            }
        if(cobrinha.position.x==comida2.x &&
        cobrinha.position.y==comida2.y) {///*
            reposicionar_comida2();
                pontos++;
                criarRabo();
                beep();
            }
    if(cobrinha.direction==KEY_RIGHT) {
        cobrinha.position.x++;
        if(cobrinha.position.x>width) cobrinha.position.x=1;
    }
    if(cobrinha.direction==KEY_LEFT) {
        cobrinha.position.x--;
        if(cobrinha.position.x<1) cobrinha.position.x=width-1;
    }
    if(cobrinha.direction==KEY_DOWN) {
        cobrinha.position.y++;
        if(cobrinha.position.y>height) cobrinha.position.y=1;
    }
    if(cobrinha.direction==KEY_UP) {
        cobrinha.position.y--;
        if(cobrinha.position.y<1) cobrinha.position.y=height-1;
    }
   if(colide_com_rabo(&cobrinha.position,cobrinha.r)) {
    modo=GAMEOVER;
    beep();
    init_cobrinha();
    if(pontos>recorde) recorde=pontos;
    if(pontos==recorde){
     sprintf(str_pontos, "Pontos: %d NOVO RECORDE!", pontos);
        salvar();
    }   else sprintf(str_pontos, "Pontos: %d Recorde: %d", pontos,recorde);

   }
}

int main() {
    srand(time(NULL));
    carregar(); 
    chlib_init();
    init_cobrinha();
    reposicionar_comida1();
    reposicionar_comida2();
    while(chlib_getKey()!='x') {
        if(modo==PLAY) {

            atualizarCobra();
            desenharCobrinha();
            desenharComida1();
            desenharComida2();
        } else {
            buffer_write_text(&buffer,(buffer.width/2)-5,buffer.height/2,"GAME OVER!!");
              buffer_write_text(&buffer,(buffer.width/2)-9,(buffer.height/2)+1,str_pontos);
            if(key=='\n' || key==' ') {
            beep();
                pontos=0;
                    modo=PLAY;
            }
        }
        chlib_update();
    }
    chlib_end();
    return 0;
}
