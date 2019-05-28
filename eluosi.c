/*************************************************************************
	> File Name: eluosi.c
	> Author:gpfly 
	> Mail:g13087572923@163,com 
	> Created Time: 2016年06月15日 星期三 16时59分41秒
 ************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#define _T      1000000
#define HANG    30
#define LIE     30
static struct termios stored_settings;
void set_keypress(void);          
void reset_keypress(void);       
void biankuang( void );
void show( int h, int l,int co  );
int  line   (int xs, int ys, int xz, int yz,int co);
void * fun_next(void * aoe);
void * fun_key(void * eof);
int makeL  (int h, int l, int guide,int co);
int makefL (int h, int l, int guide,int co);
int makel  (int h, int l, int guide,int co);
int makek  (int h, int l, int guide,int co);
int makez  (int h, int l, int guide,int co);
int makefz (int h, int l, int guide,int co);
int makeo  (int h, int l, int guide,int co);
void disply (int h, int l, int hl, int ll);
int operat(char key);
void option(int a,int x, int y, int mk,int g);  
int zxtogezi(int zx);
int estimate(void);
int gezi[HANG+1][LIE+1]={0};
int a_lie = 10  ,a_hang = 3 ,a_guide=1;
int rand_o,rand_n;
int check(void);
int move_gezi(int h);
int lost =0;
struct _ZAOXING_{
    int a[2];
    int b[2];
    int c[2];
    int d[2];
    int color;
}zx_n,zx_o;
int main(int argc,char * argv[]){
    set_keypress();
    pthread_t tid1 = 0 ;
    pthread_t tid2 = 0 ;
    pthread_create(&tid1,NULL,fun_next,NULL);
    pthread_detach(tid1);
    pthread_create(&tid2,NULL,fun_key,NULL);
    pthread_detach(tid2);
    biankuang();
    while(1)
    {
        sleep(1);
        if(lost == 1)  break;
    }
    pthread_cancel(tid1);
    pthread_cancel(tid2);
    reset_keypress();
    return 0;
}
void * fun_key(void * eof){
    char key ;
    int l,h,gui;
    while(1)
    {
        key = getchar();
        l = a_lie ; h = a_hang ; gui = a_guide;
        if(operat(key))  break;
        option(rand_o,h,l,0,gui);
        option(rand_o,a_hang,a_lie,1,a_guide);
        zxtogezi(0);
        if(estimate())
        {
            a_lie = l; a_hang = h; a_guide = gui;
            option(rand_o,h,l,1,gui);
            zxtogezi(1);
            continue;
            }
        zxtogezi(1);
        disply(2,2,HANG-1,LIE*2/3-1);
    }
    lost =1;
    pthread_exit(0);
}
void * fun_next(void * aoe){
    int h , l , gui;
    srand((int)time(0));
    rand_n = rand()%7;
    while(1)
    {
#if 1
        rand_o = rand_n;
        rand_n = rand()%7;
        a_hang = 3;
        a_lie  = 11;
        a_guide= 1;
        option(rand_o,9,25,0,1);
        option(rand_n,9,25,1,1);
        zxtogezi(0);
        zxtogezi(1);
        disply(6,22,18,29);
#endif
        while(1)
        {
            h   = a_hang;
            l   = a_lie;
            gui = a_guide;
            a_hang++;
            option(rand_o,h,l,0,gui);
            zxtogezi(0);
            option(rand_o,a_hang,a_lie,1,a_guide);
            if(estimate())
            {
                if(h==3) 
                {
                    printf("\033[15;17H\033[35m\033[47mYou are lost!\n \033[0m");
                    lost = 1;
                    pthread_exit(0);
                }
                option(rand_o,h,l,1,gui);
                zxtogezi(1);
                check();
                break;
            }
            zxtogezi(1);
            disply(2,2,HANG-1,LIE*2/3-1);
            //disply(1,1,HANG,LIE);
            usleep(_T);
        }
    }
}
int zxtogezi(int zx){
    if(zx == 0 ){
        gezi[zx_o.a[0]][zx_o.a[1]] = zx_o.color;   
        gezi[zx_o.b[0]][zx_o.b[1]] = zx_o.color;   
        gezi[zx_o.c[0]][zx_o.c[1]] = zx_o.color;   
        gezi[zx_o.d[0]][zx_o.d[1]] = zx_o.color;   
    }
    if(zx == 1 ){
        gezi[zx_n.a[0]][zx_n.a[1]] = zx_n.color;   
        gezi[zx_n.b[0]][zx_n.b[1]] = zx_n.color;   
        gezi[zx_n.c[0]][zx_n.c[1]] = zx_n.color;   
        gezi[zx_n.d[0]][zx_n.d[1]] = zx_n.color;   
    }
}
void disply (int h, int l, int hl, int ll){
    int i , j;
    for(i=h;i<=hl;i++)
    {
        for(j=l;j<=ll;j++)
        {
            show(i,j,gezi[i][j]);
        }
    }
}
int operat(char key){
    switch(key)
    {
        case 'w':{
                     a_guide++;
                     if(a_guide>4)
                         a_guide=1;
                 } break;
        case 'a':a_lie--;break;
        case 's':a_hang++;break;
        case 'd':a_lie++;break;
        case 'q':return 1;
        defaule:break;
    }
    return 0;
}
void option(int a,int h, int l, int mk,int g){
    if(mk==0)
    {
        switch(a)
        {
            case 0:makeL(h,l,g,0);break;
            case 1:makefL(h,l,g,0);break;
            case 2:makel(h,l,g,0);break;
            case 3:makek(h,l,g,0);break;
            case 4:makez(h,l,g,0);break;
            case 5:makefz(h,l,g,0);break;
            case 6:makeo(h,l,g,0);break;
        }
    }
    if(mk == 1)
    {
        switch(a)
        {
            case 0:makeL(h,l,g,40);break;
            case 1:makefL(h,l,g,41);break;
            case 2:makel(h,l,g,42);break;
            case 3:makek(h,l,g,43);break;
            case 4:makez(h,l,g,44);break;
            case 5:makefz(h,l,g,45);break;
            case 6:makeo(h,l,g,46);break;
        }
    }
}
void show( int h, int l , int co){
    l = 2 * l - 1;
    printf("\033[%d;%dH\033[%dm  \033[0m",h,l,co);
    fflush(stdout);
}
void biankuang( void ){
    int i,j;
    for(i=1;i<=LIE;i++)
    {
        for(j=1;j<=HANG;j++)
        {
            gezi[i][j] = 0;
        }
    }
    line(1,1,1,HANG,45);
    line(1,1,LIE,1,45);
    line(LIE,1,LIE,HANG,45);
    line(1,HANG,LIE,HANG,45);
    line(LIE*2/3,1,LIE*2/3,HANG,45);
    line(LIE*2/3,HANG*2/3-2,LIE,HANG*2/3-2,45);
    disply(1,1,HANG,LIE);
    printf("\033[%d;%dHWecome!\033[0m",HANG*2/3,(LIE*2/3+3)*2);
    printf("\033[%d;%dHPress <w> turn!\033[0m",HANG*2/3+2,  (LIE*2/3+1)*2);
    printf("\033[%d;%dHPress <a> left!\033[0m",HANG*2/3+3,  (LIE*2/3+1)*2);
    printf("\033[%d;%dHPress <s> down!\033[0m",HANG*2/3+4,  (LIE*2/3+1)*2);
    printf("\033[%d;%dHPress <d> right!\033[0m",HANG*2/3+5, (LIE*2/3+1)*2);
    printf("\033[%d;%dHPress <q> quit!\033[0m",HANG*2/3+6,  (LIE*2/3+1)*2);
    printf("\033[%d;%dHNEXT\033[0m",5,(LIE*2/3+4)*2);
    fflush(stdout);
}
int line(int xs, int ys, int xz, int yz, int co){
    int x,y;
    int xe,ye;
    int f=0;
    x = xs;  y = ys;
    xe = xz - xs; ye = yz - ys;
    gezi[y][x]=co;
    while(xe>0&&ye>=0)
    {
        if(x==xz&&y==yz) return 0;
        if(f>=0)
        {
            x++;
            gezi[y][x]=co;
            f = f - ye ;
        }
        if(x==xz&&y==yz) return 0;
        if(f<0)
        {
            y++;
            gezi[y][x]=co;
            f = f + xe;
        }
    }
    while(xe<=0&&ye>0)
    {
        if(x==xz&&y==yz) return 0;
        if(f>=0)
        {
            y++;
            gezi[y][x]=co;
            f = f + xe ;
        }
        if(x==xz&&y==yz) return 0;
        if(f<0)
        {
            x--;
            gezi[y][x]=co;
            f = f + ye;
        }
    }
    while(xe<0&&ye<=0)
    {
        if(x==xz&&y==yz) return 0;
        if(f>=0)
        {
            x--;
            gezi[y][x]=co;
            f = f + ye ;
        }
        if(x==xz&&y==yz) return 0;
        if(f<0)
        {
            y--;
            gezi[y][x]=co;
            f = f - xe;
        }
    }
    while(xe>=0&&ye<0)
    {
        if(x==xz&&y==yz) return 0;
        if(f>=0)
        {
            y--;
            gezi[y][x]=co;
            f = f - xe ;
        }
        if(x==xz&&y==yz) return 0;
        if(f<0)
        {
            x++;
            gezi[y][x]=co;
            f = f - ye;
        }
    }
}
int makeL(int h, int l, int guide,int co){
    int a[2] = { 0 };
    int b[2] = { 0 };
    int c[2] = { 0 };
    int d[2] = { 0 };
    b[0] = h; b[1] = l ;
    if(guide == 1 )
    {
        a[0] = h-1;      a[1] = l;
        c[0] = h+1;      c[1] = l;
        d[0] = h+1;      d[1] = l+1;
    }
    if(guide == 2 )
    {
        a[0] = h;       a[1] = l-1;
        c[0] = h+1;     c[1] = l-1;
        d[0] = h;       d[1] = l+1;
    }
    if(guide == 3 )
    {
        a[0] = h-1;      a[1] = l-1;
        c[0] = h+1;      c[1] = l;
        d[0] = h-1;      d[1] = l;
    }
    if(guide == 4 )
    {
        a[0] = h;   a[1] = l-1;
        c[0] = h;   c[1] = l+1;
        d[0] = h-1;   d[1] = l+1;
    }
    if(co==0)
    {
        zx_o.color = co;
        zx_o.a[0] = a[0];
        zx_o.a[1] = a[1];
        zx_o.b[0] = b[0];
        zx_o.b[1] = b[1];
        zx_o.c[0] = c[0];
        zx_o.c[1] = c[1];
        zx_o.d[0] = d[0];
        zx_o.d[1] = d[1];
    }
    else
    {
        zx_n.color = co;
        zx_n.a[0] = a[0];
        zx_n.a[1] = a[1];
        zx_n.b[0] = b[0];
        zx_n.b[1] = b[1];
        zx_n.c[0] = c[0];
        zx_n.c[1] = c[1];
        zx_n.d[0] = d[0];
        zx_n.d[1] = d[1];
    }
}
int makefL(int h, int l, int guide,int co){
    int a[2] = { 0 };
    int b[2] = { 0 };
    int c[2] = { 0 };
    int d[2] = { 0 };
    b[0] = h; b[1] = l ;
    if(guide == 1 )
    {
        a[0] = h+1;      a[1] = l-1;
        c[0] = h+1;      c[1] = l;
        d[0] = h-1;      d[1] = l;
    }
    if(guide == 2 )
    {
        a[0] = h-1;      a[1] = l-1;
        c[0] = h;        c[1] = l-1;
        d[0] = h;        d[1] = l+1;
    }
    if(guide == 3 )
    {
        a[0] = h-1;       a[1] = l;
        c[0] = h+1;       c[1] = l;
        d[0] = h-1;       d[1] = l+1;
    }
    if(guide == 4 )
    {
        a[0] = h;       a[1] = l-1;
        c[0] = h + 1;   c[1] = l+1;
        d[0] = h;       d[1] = l + 1;
    }
    if(co==0)
    {
        zx_o.color = co;
        zx_o.a[0] = a[0];
        zx_o.a[1] = a[1];
        zx_o.b[0] = b[0];
        zx_o.b[1] = b[1];
        zx_o.c[0] = c[0];
        zx_o.c[1] = c[1];
        zx_o.d[0] = d[0];
        zx_o.d[1] = d[1];
    }
    else
    {
        zx_n.color = co;
        zx_n.a[0] = a[0];
        zx_n.a[1] = a[1];
        zx_n.b[0] = b[0];
        zx_n.b[1] = b[1];
        zx_n.c[0] = c[0];
        zx_n.c[1] = c[1];
        zx_n.d[0] = d[0];
        zx_n.d[1] = d[1];
    }
}
int makel(int h, int l, int guide,int co){
    int a[2] = { 0 };
    int b[2] = { 0 };
    int c[2] = { 0 };
    int d[2] = { 0 };
    if(guide>2) guide = guide - 2 ;
    b[0] = h; b[1] = l ;
    if(guide == 1 )
    {
        a[0] = h-1;       a[1] = l;
        c[0] = h+2;       c[1] = l;
        d[0] = h+1;       d[1] = l;
    }
    if(guide == 2 )
    {
        a[0] = h;   a[1] = l-1;
        c[0] = h;   c[1] = l+1;
        d[0] = h;   d[1] = l+2;
    }
    if(co==0)
    {
        zx_o.color = co;
        zx_o.a[0] = a[0];
        zx_o.a[1] = a[1];
        zx_o.b[0] = b[0];
        zx_o.b[1] = b[1];
        zx_o.c[0] = c[0];
        zx_o.c[1] = c[1];
        zx_o.d[0] = d[0];
        zx_o.d[1] = d[1];
    }
    else
    {
        zx_n.color = co;
        zx_n.a[0] = a[0];
        zx_n.a[1] = a[1];
        zx_n.b[0] = b[0];
        zx_n.b[1] = b[1];
        zx_n.c[0] = c[0];
        zx_n.c[1] = c[1];
        zx_n.d[0] = d[0];
        zx_n.d[1] = d[1];
    }
}
int makek(int h, int l, int guide,int co){
    int a[2] = { 0 };
    int b[2] = { 0 };
    int c[2] = { 0 };
    int d[2] = { 0 };
    b[0] = h; b[1] = l ;
    if(guide == 1 )
    {
        a[0] = h+1;      a[1] = l-1;
        c[0] = h+1;      c[1] = l;
        d[0] = h+1;      d[1] = l+1;
    }
    if(guide == 2 )
    {
        a[0] = h-1;        a[1] = l;
        c[0] = h+1;        c[1] = l;
        d[0] = h;          d[1] = l+1;
    }
    if(guide == 3 )
    {
        a[0] = h;            a[1] = l-1;
        c[0] = h+1;          c[1] = l;
        d[0] = h;            d[1] = l+1;
    }
    if(guide == 4 )
    {
        a[0] = h;           a[1] = l-1;
        c[0] = h+1;         c[1] = l;
        d[0] = h-1;         d[1] = l;
    }
    if(co==0)
    {
        zx_o.color = co;
        zx_o.a[0] = a[0];
        zx_o.a[1] = a[1];
        zx_o.b[0] = b[0];
        zx_o.b[1] = b[1];
        zx_o.c[0] = c[0];
        zx_o.c[1] = c[1];
        zx_o.d[0] = d[0];
        zx_o.d[1] = d[1];
    }
    else
    {
        zx_n.color = co;
        zx_n.a[0] = a[0];
        zx_n.a[1] = a[1];
        zx_n.b[0] = b[0];
        zx_n.b[1] = b[1];
        zx_n.c[0] = c[0];
        zx_n.c[1] = c[1];
        zx_n.d[0] = d[0];
        zx_n.d[1] = d[1];
    }
}
int makez(int h, int l, int guide,int co){
    int a[2] = { 0 };
    int b[2] = { 0 };
    int c[2] = { 0 };
    int d[2] = { 0 };
    if(guide>2) guide = guide - 2 ;
    b[0] = h; b[1] = l ;
    if(guide == 1 )
    {
        a[0] = h;       a[1] = l-1;
        c[0] = h+1;     c[1] = l-1;
        d[0] = h-1;     d[1] = l;
    }
    if(guide == 2 )
    {
        a[0] = h;       a[1] = l-1;
        c[0] = h+1;     c[1] = l;
        d[0] = h+1;     d[1] = l+1;
    }
    if(co==0)
    {
        zx_o.color = co;
        zx_o.a[0] = a[0];
        zx_o.a[1] = a[1];
        zx_o.b[0] = b[0];
        zx_o.b[1] = b[1];
        zx_o.c[0] = c[0];
        zx_o.c[1] = c[1];
        zx_o.d[0] = d[0];
        zx_o.d[1] = d[1];
    }
    else
    {
        zx_n.color = co;
        zx_n.a[0] = a[0];
        zx_n.a[1] = a[1];
        zx_n.b[0] = b[0];
        zx_n.b[1] = b[1];
        zx_n.c[0] = c[0];
        zx_n.c[1] = c[1];
        zx_n.d[0] = d[0];
        zx_n.d[1] = d[1];
    }
}
int makefz(int h, int l, int guide,int co){
    int a[2] = { 0 };
    int b[2] = { 0 };
    int c[2] = { 0 };
    int d[2] = { 0 };
    if(guide>2) guide = guide - 2 ;
    b[0] = h; b[1] = l ;
    if(guide == 1 )
    {
        a[0] = h-1;     a[1] = l;
        c[0] = h+1;     c[1] = l+1;
        d[0] = h;       d[1] = l+1;
    }
    if(guide == 2 )
    {
        a[0] = h+1;     a[1] = l-1;
        c[0] = h+1;     c[1] = l;
        d[0] = h;       d[1] = l+1;
    }
    if(co==0)
    {
        zx_o.color = co;
        zx_o.a[0] = a[0];
        zx_o.a[1] = a[1];
        zx_o.b[0] = b[0];
        zx_o.b[1] = b[1];
        zx_o.c[0] = c[0];
        zx_o.c[1] = c[1];
        zx_o.d[0] = d[0];
        zx_o.d[1] = d[1];
    }
    else
    {
        zx_n.color = co;
        zx_n.a[0] = a[0];
        zx_n.a[1] = a[1];
        zx_n.b[0] = b[0];
        zx_n.b[1] = b[1];
        zx_n.c[0] = c[0];
        zx_n.c[1] = c[1];
        zx_n.d[0] = d[0];
        zx_n.d[1] = d[1];
    }
}
int makeo(int h, int l, int guide,int co){
    int a[2] = { 0 };
    int b[2] = { 0 };
    int c[2] = { 0 };
    int d[2] = { 0 };
    a[0] = h;        a[1] = l-1;
    b[0] = h;        b[1] = l ;
    c[0] = h+1;      c[1] = l-1;
    d[0] = h+1;      d[1] = l;
    if(co==0)
    {
        zx_o.color = co;
        zx_o.a[0] = a[0];
        zx_o.a[1] = a[1];
        zx_o.b[0] = b[0];
        zx_o.b[1] = b[1];
        zx_o.c[0] = c[0];
        zx_o.c[1] = c[1];
        zx_o.d[0] = d[0];
        zx_o.d[1] = d[1];
    }
    else
    {
        zx_n.color = co;
        zx_n.a[0] = a[0];
        zx_n.a[1] = a[1];
        zx_n.b[0] = b[0];
        zx_n.b[1] = b[1];
        zx_n.c[0] = c[0];
        zx_n.c[1] = c[1];
        zx_n.d[0] = d[0];
        zx_n.d[1] = d[1];
    }
}
 void set_keypress(void)   
 {
         struct termios new_settings;
         tcgetattr(0,&stored_settings);
         new_settings = stored_settings;
         new_settings.c_lflag &= (~ICANON);
         new_settings.c_lflag &= (~ECHO);
         new_settings.c_cc[VTIME] = 0;
         new_settings.c_cc[VMIN] = 1;
         tcsetattr(0,TCSANOW,&new_settings);
         return;
 }
   void reset_keypress(void)      
   {
           tcsetattr(0,TCSANOW,&stored_settings);
           return;
   }
int estimate(void){
    if(gezi[zx_n.a[0]][zx_n.a[1]]>0) return 1;
    if(gezi[zx_n.b[0]][zx_n.b[1]]>0) return 1;
    if(gezi[zx_n.c[0]][zx_n.c[1]]>0) return 1;
    if(gezi[zx_n.d[0]][zx_n.d[1]]>0) return 1;
    return 0;
}
int check(void){
    int h,l;
    for(h = 29; h >= 2; h--)
    {
        for(l = 2; l < 20; l++)
        {
            if(gezi[h][l] == 0)  break;
            if(l == 19 ) 
            {
                move_gezi(h);
                h++;
            }
        }
    }
    return 0;
}
int move_gezi(int h){
    int l;
    for(l = 2 ; l < 20 ; l++)
    {
        gezi[h][l] = 0;
    }
    for(;h > 2; h--)
    {
        for(l = 2 ; l < 20 ;l++)
        {
            gezi[h][l] = gezi[h-1][l];
        }
    }
    return 0;
}
