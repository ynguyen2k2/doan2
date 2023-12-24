/////////////////////////////////////////////////////////////////////////
////                          graphics.c                             ////
////                                                                 ////
////   This file contains functions to draw lines, rectangles, bars, ////
////   circles and text to a display. A function which draws a       ////
////   single pixel must be defined before calling the functions in  ////
////   this file. Call it glcd_pixel(x, y, color) where x is the     ////
////   horizontal coordinate, y is the vertical coordinate, and      ////
////   color is 1 bit to turn the pixel on or off.                   ////
////                                                                 ////
////   * Note: (0, 0) is treated as the upper left corner            ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////
////                                                                 ////
////  glcd_line(x1, y1, x2, y2, color)                               ////
////     * Draws a line from the first point to the second point     ////
////       with the given color                                      ////
////       - color can be ON or OFF                                  ////
////                                                                 ////
////  glcd_rect(x1, y1, x2, y2, fill, color)                         ////
////     * Draws a rectangle with one corner at point (x1,y1) and    ////
////       the other corner at point (x2,y2)                         ////
////       - fill can be YES or NO                                   ////
////       - color can be ON or OFF                                  ////
////                                                                 ////
////  glcd_bar(x1, y1, x2, y2, width, color)                         ////
////     * Draws a bar (wide line) from the first point to the       ////
////       second point                                              ////
////       - width is the number of pixels wide                      ////
////       - color is ON or OFF                                      ////
////                                                                 ////
////  glcd_circle(x, y, radius, fill, color)                         ////
////     * Draws a circle with center at (x,y)                       ////
////       - fill can be YES or NO                                   ////
////       - color can be ON or OFF                                  ////
////                                                                 ////
////  glcd_text57(x, y, textptr, size, color)                        ////
////     * Write the null terminated text pointed to by textptr with ////
////       the upper left coordinate of the first character at (x,y) ////
////       Characters are 5 pixels wide and 7 pixels tall            ////
////       - size is an integer that scales the size of the text     ////
////       - color is ON or OFF                                      ////
////     * Note - This function wraps characters to the next line    ////
////              use #define GLCD_WIDTH to specify a display width  ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996, 2010 Custom Computer Services        ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
/////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////
#ifndef GRAPHICS_DRAWING_FUNCTIONS
#define GRAPHICS_DRAWING_FUNCTIONS
/////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////
#ifndef ON
#define ON  1
#endif

#ifndef OFF
#define OFF 0
#endif

#ifndef YES
#define YES 1
#endif

#ifndef NO
#define NO  0
#endif
/////////////////////////////////////////////////////////////////////////
#include "string.h"

#include "font.h"

/////////////////////////////////////////////////////////////////////////
// Purpose:       Draw a line on a graphic LCD using Bresenham's
//                line drawing algorithm
// Inputs:        (x1, y1) - the start coordinate
//                (x2, y2) - the end coordinate
//                color - ON or OFF
// Dependencies:  glcd_pixel()
/////////////////////////////////////////////////////////////////////////

void glcd_line(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2,  unsigned char r, unsigned char g, unsigned char b)
{
   unsigned short        dy, dx;
   signed char  addx=1, addy=1;
   signed short P, diff;

   unsigned short i=0;
	 if(x2>=x1)dx= x2 - x1;
	 else dx = x1 - x2;
	 if(y2>=y1)dy= y2 - y1;
	 else dy = y1 - y2;


   if(x1 > x2)
      addx = -1;
   if(y1 > y2)
      addy = -1;

   if(dx >= dy)
   {
      dy *= 2;
      P = dy - dx;
      diff = P - dx;

      for(; i<=dx; ++i)
      {
				 
//         glcd_pixel(x1, y1, color);
				matrix_pixel_xy(x1,y1,r,g,b);

         if(P < 0)
         {
            P  += dy;
            x1 += addx;
         }
         else
         {
            P  += diff;
            x1 += addx;
            y1 += addy;
         }
      }
   }
   else
   {
      dx *= 2;
      P = dx - dy;
      diff = P - dy;

      for(; i<=dy; ++i)
      {
//         glcd_pixel(x1, y1, color);
				matrix_pixel_xy(x1,y1,r,g,b);

         if(P < 0)
         {
            P  += dx;
            y1 += addy;
         }
         else
         {
            P  += diff;
            x1 += addx;
            y1 += addy;
         }
      }
   }
}


/////////////////////////////////////////////////////////////////////////
// Purpose:       Draw a rectangle on a graphic LCD
// Inputs:        (x1, y1) - the start coordinate
//                (x2, y2) - the end coordinate
//                fill  - YES or NO
//                color - ON or OFF
// Dependencies:  glcd_pixel(), glcd_line()
/////////////////////////////////////////////////////////////////////////

void glcd_rect(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, char fill,  unsigned char r, unsigned char g, unsigned char b)
{
   if(fill)
   {  
      unsigned short i, xmin, xmax, ymin, ymax;
      if(x1 < x2)                            //  Find x min and max
      {
         xmin = x1;
         xmax = x2;
      }
      else
      {
         xmin = x2;
         xmax = x1;
      }

      if(y1 < y2)                            // Find the y min and max
      {
         ymin = y1;
         ymax = y2;
      }
      else
      {
         ymin = y2;
         ymax = y1;
      }

      for(; xmin <= xmax; ++xmin)
      {
         for(i=ymin; i<=ymax; ++i)
         {
         //   glcd_pixel(xmin, i, color);
					 matrix_pixel_xy(xmin,y1,r,g,b);
         }
      }
   }
   else
   {
      glcd_line(x1, y1, x2, y1, r,g,b);      // Draw the 4 sides
      glcd_line(x1, y2, x2, y2, r,g,b);
      glcd_line(x1, y1, x1, y2, r,g,b);
      glcd_line(x2, y1, x2, y2, r,g,b);
   }
}

/////////////////////////////////////////////////////////////////////////
// Purpose:       Draw a bar (wide line) on a graphic LCD
// Inputs:        (x1, y1) - the start coordinate
//                (x2, y2) - the end coordinate
//                width  - The number of pixels wide
//                color - ON or OFF
/////////////////////////////////////////////////////////////////////////
void glcd_bar(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned char width,  unsigned char r, unsigned char g, unsigned char b)
{
   unsigned char         half_width;
   signed short dy, dx;
   signed char  addx=1, addy=1, j;
   signed short P, diff, c1, c2;
   unsigned short i=0;
   if(x2>=x1)dx= x2 - x1;
	 else dx = x1 - x2;
	 if(y2>=y1)dy= y2 - y1;
	 else dy = y1 - y2;

   half_width = width/2;
   c1 = -(dx*x1 + dy*y1);
   c2 = -(dx*x2 + dy*y2);

   if(x1 > x2)
   {
      signed short temp;
      temp = c1;
      c1 = c2;
      c2 = temp;
      addx = -1;
   }
   if(y1 > y2)
   {
      signed short temp;
      temp = c1;
      c1 = c2;
      c2 = temp;
      addy = -1;
   }

   if(dx >= dy)
   {
      P = 2*dy - dx;
      diff = P - dx;

      for(i=0; i<=dx; ++i)
      {
         for(j=-half_width; j<half_width+width%2; ++j)
         {
//               glcd_pixel(x1, y1+j, color);
					 matrix_pixel_xy(x1,y1+j,r,g,b);
         }
         if(P < 0)
         {
            P  += 2*dy;
            x1 += addx;
         }
         else
         {
            P  += diff;
            x1 += addx;
            y1 += addy;
         }
      }
   }
   else
   {
      P = 2*dx - dy;
      diff = P - dy;

      for(i=0; i<=dy; ++i)
      {
         if(P < 0)
         {
            P  += 2*dx;
            y1 += addy;
         }
         else
         {
            P  += diff;
            x1 += addx;
            y1 += addy;
         }
         for(j=-half_width; j<half_width+width%2; ++j)
         {
              // glcd_pixel(x1+j, y1, color);
							 matrix_pixel_xy(x1+j,y1,r,g,b);
         }
      }
   }
}


/////////////////////////////////////////////////////////////////////////
// Purpose:       Draw a circle on a graphic LCD
// Inputs:        (x,y) - the center of the circle
//                radius - the radius of the circle
//                fill - YES or NO
//                color - ON or OFF
/////////////////////////////////////////////////////////////////////////
void glcd_circle(unsigned short x, unsigned short y, unsigned short radius, char fill,  unsigned char r, unsigned char g, unsigned char bl)
{
   
   signed short a, b, P;
   a = 0;
   b = radius;
   P = 1 - radius;

   do
   {
      if(fill)
      {
         glcd_line(x-a, y+b, x+a, y+b, r,g,bl);
         glcd_line(x-a, y-b, x+a, y-b, r,g,bl);
         glcd_line(x-b, y+a, x+b, y+a, r,g,bl);
         glcd_line(x-b, y-a, x+b, y-a, r,g,bl);
      }
      else
      {
//         glcd_pixel(a+x, b+y, color);
//         glcd_pixel(b+x, a+y, color);
//         glcd_pixel(x-a, b+y, color);
//         glcd_pixel(x-b, a+y, color);
//         glcd_pixel(b+x, y-a, color);
//         glcd_pixel(a+x, y-b, color);
//         glcd_pixel(x-a, y-b, color);
//         glcd_pixel(x-b, y-a, color);
				
				 matrix_pixel_xy(a+x, b+y, r,g,bl);
         matrix_pixel_xy(b+x, a+y, r,g,bl);
         matrix_pixel_xy(x-a, b+y, r,g,bl);
         matrix_pixel_xy(x-b, a+y, r,g,bl);
         matrix_pixel_xy(b+x, y-a, r,g,bl);
         matrix_pixel_xy(a+x, y-b, r,g,bl);
         matrix_pixel_xy(x-a, y-b, r,g,bl);
         matrix_pixel_xy(x-b, y-a, r,g,bl);
      }

      if(P < 0)
         P += 3 + 2 * a++;
      else
         P += 5 + 2 * (a++ - b--);
    } while(a <= b);
}


/////////////////////////////////////////////////////////////////////////
// Purpose:       Write text on a graphic LCD
// Inputs:        (x,y) - The upper left coordinate of the first letter
//                textptr - A pointer to an array of text to display
//                size - The size of the text: 1 = 5x7, 2 = 10x14, ...
//                color - ON or OFF
/////////////////////////////////////////////////////////////////////////
void glcd_text57(unsigned short x, unsigned short y, char* textptr, unsigned char size,  unsigned char r, unsigned char g, unsigned char b)
{
   unsigned char j, k, l, m;                       // Loop counters
   unsigned char pixelData[5];                     // Stores character data

   for(; *textptr != '\0'; ++textptr, ++x)// Loop through the passed string
   {
      if(*textptr < 'S') // Checks if the letter is in the first font array
         memcpy(pixelData, FONT[*textptr - ' '], 5);
      else if(*textptr <= '~') // Check if the letter is in the second font array
         memcpy(pixelData, FONT2[*textptr - 'S'], 5);
      else
         memcpy(pixelData, FONT[0], 5);   // Default to space

      // Handles newline and carriage returns
      switch(*textptr)
      {
         case '\n':
            y += 7*size + 1;
            continue;
         case '\r':
            x = 0;
            continue;
      }

      if(x+5*size >= 64)          // Performs character wrapping
      {
         x = 0;                           // Set x at far left position
         y += 7*size + 1;                 // Set y at next position down
      }
      for(j=0; j<5; ++j, x+=size)         // Loop through character byte data
      {
         for(k=0; k < 7; ++k)             // Loop through the vertical pixels
         {
            if(pixelData[j]&(0x01<<k)) // Check if the pixel should be set
            {
               for(l=0; l < size; ++l)    // These two loops change the
               {                          // character's size
                  for(m=0; m < size; ++m)
                  {
                     //glcd_pixel(x+m, y+k*size+l, color); // Draws the pixel
											matrix_pixel_xy(x+m,y+k*size+l,r,g,b);
                  }
               }
            }
         }
      }
   }
}
void glcd_text45(unsigned short x, unsigned short y, char* textptr, unsigned char size, unsigned char r, unsigned char g, unsigned char b)
{
    unsigned char j, k, l, m; // Loop counters
    unsigned char pixelData[5]; // Stores character data

    for (; *textptr != '\0'; ++textptr, x += 4 * size) // Loop through the passed string
    {
        if (*textptr >= '0' && *textptr <= '9') // Check if the character is a digit
        {
            memcpy(pixelData, FONT4x5[*textptr - '0'], 5); // lay 0-9 trong mang font4x5
        }
        else if (*textptr == '-') // kt ky tu -
        {
            memcpy(pixelData, FONT4x5[10], 5); // xài -
        }
        else
        {
            continue; // Skip non-digit and non "-" characters
        }

        // Handles newline and carriage returns
        switch (*textptr)
        {
        case '\n':
            y += 5 * size + 1;
            x = 0;
            continue;
        case '\r':
            x = 0;
            continue;
        }

        if (x + 4 * size >= 64) // Performs character wrapping
        {
            x = 0; // Set x at far left position
            y += 5 * size + 1; // Set y at next position down
        }

        for (j = 0; j < 5; ++j) // Loop through character byte data
        {
            for (k = 0; k < 4; ++k) // Loop through the vertical pixels
            {
                if (pixelData[j] & (0x01 << (4 - k))) // Check if the pixel should be set
                {
                    for (l = 0; l < size; ++l) // These two loops change the character's size
                    {
                        for (m = 0; m < size; ++m)
                        {
                            matrix_pixel_xy(x + k * size + l, y + j * size + m, r, g, b); // Draws the pixel
                        }
                    }
                }
            }
        }
    }
}

void GrTriangleDrawFill(signed short x1,signed short y1, signed short x2, signed short y2, signed short x3, signed short y3, unsigned char r, unsigned char g, unsigned char b)
{
	   signed short x[3],y[3],n,yt1,yt2,yt,dx1,dx2,dx3,dy1,dy2,dy3; 
		 signed long sh1,sh2,sh3;
		 x[0]=x1; x[1]=x2; x[2]=x3;y[0]=y1; y[1]=y2; y[2]=y3;
		 if(x[0]>x[1]){yt=x[0];x[0]=x[1];x[1]=yt;yt=y[0];y[0]=y[1];y[1]=yt;}
		 if(x[0]>x[2]){yt=x[0];x[0]=x[2];x[2]=yt;yt=y[0];y[0]=y[2];y[2]=yt;}
		 if(x[1]>x[2]){yt=x[1];x[1]=x[2];x[2]=yt;yt=y[1];y[1]=y[2];y[2]=yt;}
     dx1= x[2]-x[0]; dx2= x[1]-x[0];dx3=x[2]-x[1];
		 dy1= y[2]-y[0]; dy2= y[1]-y[0];dy3=y[2]-y[1];
		 sh1=x[2]*y[0] - x[0]*y[2]; sh2=x[1]*y[0] - x[0]*y[1]; sh3=x[2]*y[1] - x[1]*y[2];
		 
     for(n=x[0] ; n<x[1]; n++)
		 {
			  yt1= (dy1*n + sh1)/dx1;
				yt2= (dy2*n + sh2)/dx2;
			  if(yt2<yt1){yt= yt1; yt1=yt2; yt2=yt;}
				glcd_line(n, yt1,n,yt2,r,g,b);
		 }
		 for(n=x[1] ; n<x[2]; n++)
		 {
			  yt1= (dy1*n + sh1)/dx1;
				yt2= (dy3*n + sh3)/dx3;
			  if(yt2<yt1){yt= yt1; yt1=yt2; yt2=yt;}
				glcd_line(n, yt1,n,yt2,r,g,b);
		 }	 
}







#endif
