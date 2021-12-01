// ========== Real time Julia fractal on the ice-v-dual
// adapted from sylefeb silice dual core demo (made single-core)
// mul trick from: http://cowlark.com/2018-05-26-bogomandel/index.html
// see also the great notes on Mandlebrot by reenigne:
// https://github.com/reenigne/reenigne/blob/master/8088/mandel/mandel.txt

#include "demos.h"
#include "lite_oled.h"

#include <stdio.h>
#include <libbase/uart.h>
#include <libbase/console.h>

// ==== fixed point setup
#define FP     6
#define IP     (FP+3)
#define CUTOFF (4<<FP)

#ifdef __riscv

unsigned short sq[] = {
 0,0,0,0,0,0,0,0,1,1,1,1,2,2,3,3,4,4,5,5,6,6,7,8,9,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,25,26,27,28,30,31,33,34,36,37,39,40,42,43,45,47,49,50,
 52,54,56,58,60,62,64,66,68,70,72,74,76,78,81,83,85,87,90,92,95,97,100,102,105,107,110,112,115,118,121,123,126,129,132,135,138,141,144,147,150,153,156,
 159,162,165,169,172,175,178,182,185,189,192,196,199,203,206,210,213,217,221,225,228,232,236,240,244,248,252,256,260,264,268,272,276,280,284,289,293,297,
 301,306,310,315,319,324,328,333,337,342,346,351,356,361,365,370,375,380,385,390,395,400,405,410,415,420,425,430,435,441,446,451,456,462,467,473,478,484,
 489,495,500,506,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,
 511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,
 511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,
 511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,
 511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,
 511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,
 511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,
 511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,
 511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,
 511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,
 511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,
 511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,
 511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,
 511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,
 511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,
 511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,
 511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,
 511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,506,500,495,489,484,478,473,467,462,456,451,446,441,435,430,425,420,415,
 410,405,400,395,390,385,380,375,370,365,361,356,351,346,342,337,333,328,324,319,315,310,306,301,297,293,289,284,280,276,272,268,264,260,256,252,248,244,
 240,236,232,228,225,221,217,213,210,206,203,199,196,192,189,185,182,178,175,172,169,165,162,159,156,153,150,147,144,141,138,135,132,129,126,123,121,118,
 115,112,110,107,105,102,100,97,95,92,90,87,85,83,81,78,76,74,72,70,68,66,64,62,60,58,56,54,52,50,49,47,45,43,42,40,39,37,36,34,33,31,30,28,27,26,25,23,
 22,21,20,19,18,17,16,15,14,13,12,11,10,9,9,8,7,6,6,5,5,4,4,3,3,2,2,1,1,1,1,0,0,0,0,0,0,0,0};

void oled_julia(void) {
   
#define MASK   ((1<<(IP+1))-1)
#define CLAMP  ((1<<(IP-1))-1)
#define NEG    ((1<<(IP+1))  )

#define XCmin  -90
#define XCmax  (XCmin+40)
#define YCmin  10
#define YCmax  (YCmin+40)

  int x_c = (XCmin+XCmax)>>1; int x_c_i = 1;
  int y_c = (YCmin+YCmax)>>1; int y_c_i = 3;

  oled_init();
  puts("Press any key to exit");
   
  while (1) {
    oled_write_window(0,0,OLED_WIDTH-1,OLED_HEIGHT-1);
    int j_f = -OLED_HEIGHT/2;
    int pix = 0;
    for (int j = 0 ; j < OLED_HEIGHT ; ++j) {
      int i_f = -OLED_WIDTH/2;
      for (int i = 0 ; i < OLED_WIDTH ; ++i) {
        int x_f  = i_f;
        int y_f  = j_f;
        int clr=0; int clr8=32;
        for ( ; clr<24 ; clr++,clr8+=8) {
          int a_f = x_f & MASK;
          int b_f = y_f & MASK;
          int u_f = sq[a_f];
          int v_f = sq[b_f];
          int c_f = (x_f+y_f) & MASK;
          int s_f = sq[c_f];
          int w_f = s_f - u_f - v_f;
          x_f     = u_f - v_f + x_c; // + i_f; // use for Mandlebrot
          y_f     = w_f       + y_c; // + j_f; // use for Mandlebrot
          if (u_f + v_f > CUTOFF) {
            break;
          }
        }
	oled_byte(OLED_SPI_DAT,(uint8_t)clr8);
	oled_byte(OLED_SPI_DAT,(uint8_t)clr);	 
        pix += 1;
        i_f += 1;
      }
      j_f += 1;
    }

    x_c += x_c_i;
    if (x_c < XCmin || x_c > XCmax) { x_c_i = - x_c_i; }
    y_c += y_c_i;
    if (y_c < YCmin || y_c > YCmax) { y_c_i = - y_c_i; }
     
    if (readchar_nonblock()) {
      getchar();
      break;
    }
  }
  oled_off();
}


#else

// =================== this gets used when compiling with gcc one desktop =====
// =================== recomputes the dual_fractal_table.h file

// to regenerate the squares table do (from projects/ice-v):
//    cd src ; gcc dual_fractal.c ; ./a.exe ; cd ..
// this outputs "dual_fractal_table.h"

#include <stdio.h>

// generates precomputation table
void main()
{
  unsigned short sq[1<<(IP+1)];

  for (int s = 0; s < (1<<(IP+1)) ; ++s) {
    int q = s;
    if (s >= (1<<IP)) {
      // negative part, we make it so after 'and-ing' the
      // negative value with (1<<(IP+1))-1 we get the correct square
      q = (1<<(IP+1))-1-s;
    }
    sq[s]  = (q*q)>>FP;
    if (sq[s] >= (1<<IP)) {
      sq[s] = (1<<IP)-1;
    }
  }

  FILE *f = fopen("dual_fractal_table.h","w");
  fprintf(f,"unsigned short sq[] = {");
  // for (int s = 0; s < (1<<(IP-1)) ; ++s) {
  for (int s = 0; s < (1<<(IP+1)) ; ++s) {
    if (s>0) fprintf(f,",");
    fprintf(f,"%d",sq[s]);
  }
  fprintf(f,"};\n");
  fclose(f);
}

#endif

