/*
  Copyright (C) 2015 Felipe Izzo
  Copyright (C) 1996-1997 Id Software, Inc.

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

  See the GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include "quakedef.h"
#include "d_local.h"

#include "ctr.h"

extern
viddef_t	vid;				// global video state

int	basewidth, baseheight;

byte	*vid_buffer;
short	*zbuffer;
//byte	surfcache[512*1024];
float separation_distance;

uint16_t	d_8to16table[256];
uint32_t surfcache[1024*64];

void	VID_SetPalette (unsigned char *palette)
{
  int i;
  uint8_t *pal = palette;
  uint16_t *table = d_8to16table;
  unsigned r, g, b;
  for(i=0; i<256; i++){
    r = pal[0];
    g = pal[1];
    b = pal[2];
    table[0] = ((r>>3)<<11)+((g>>2)<<5)+(b>>3);
    table++;
    pal += 3;
  }
}

void	VID_ShiftPalette (unsigned char *palette)
{
  VID_SetPalette(palette);
}

void	VID_Init (unsigned char *palette)
{
  baseheight = 240;
  basewidth = 320;

  vid_buffer = malloc(sizeof(byte) * basewidth * baseheight); //Left buffer
  zbuffer = malloc(sizeof(short) * basewidth * baseheight);

  vid.maxwarpwidth = vid.width = vid.conwidth = basewidth;
  vid.maxwarpheight = vid.height = vid.conheight = baseheight;
  vid.aspect = 1.0;
  vid.numpages = 1;
  vid.colormap = host_colormap;
  vid.fullbright = 256 - LittleLong (*((int *)vid.colormap + 2048));
  vid.buffer = vid.conbuffer = vid_buffer;
  vid.rowbytes = vid.conrowbytes = basewidth;
  VID_SetPalette(palette);
  d_pzbuffer = zbuffer;
  D_InitCaches (surfcache, sizeof(surfcache));
}

void	VID_SetBuffer (int buffer)
{
  vid.conbuffer = vid.buffer = vid_buffer;
}

void	VID_Shutdown (void)
{
  free(vid_buffer);
  free(zbuffer);
}

extern float g_fps[32];
extern int g_fpsp;

void	VID_Update (vrect_t *rects){
  static double oldtime;
  double time;
  /* int x,y; */
  /* char buff[12000]=""; */
  /* char tmp[128]; */
  /* int r,g,b; */
  //  printf("\033[1;1H");
  //  printf("screen=%d->%dx%d\n",rects->x,rects->height,rects->width);
  double Sys_FloatTime (void);

  time = Sys_FloatTime();
  g_fps[g_fpsp++&0x1F] = 1.f/(time - oldtime);
  lcd_draw_pic_palette(0,vid_buffer,d_8to16table);
  oldtime = time;
  /* for(y = rects->x; y < rects->height; y+=2){ */
  /*   buff[0]=0; */
  /*   for(x = rects->x; x < rects->width; x+=2){ */
  /*     //    table[0] = ((r>>3)<<11)+((g>>2)<<5)+(b>>3); */
  /*     r = d_8to16table[vid_buffer[0][y*basewidth + x]]>>11; */
  /*     g = (d_8to16table[vid_buffer[0][y*basewidth + x]]>>5)&0x3F; */
  /*     b = d_8to16table[vid_buffer[0][y*basewidth + x]]&0x1F; */
  /*     sprintf(tmp,"\033[48;2;%d;%d;%dm  ",r*8,g*4,b*8);strcat(buff,tmp); */
  /*     //      left_fb[((x*240) + (239 -y))] = d_8to16table[vid_buffer[0][y*basewidth + x]]; */
  /*   } */
  /*   puts(buff); */
  /* } */
}

/*
  ================
  D_BeginDirectRect
  ================
*/
void D_BeginDirectRect (int x, int y, byte *pbitmap, int width, int height)
{
}


/*
  ================
  D_EndDirectRect
  ================
*/
void D_EndDirectRect (int x, int y, int width, int height)
{
}
