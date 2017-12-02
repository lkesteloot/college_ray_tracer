
 /*                                                                                       
 /* RAY TRACER                                                                            
 /* Copyright (c) 1990 by Lawrence Kesteloot and Fredrik Fatemi                           
 /*                                                                                       
 /* Compile on an IRIS computer with:                                                     
 /*      cc ray.c -o ray -Zg -Zf
 /*                                                                                       
 /* To generate an image in the background, type:                                         
 /*      ray name.world &                                                                 
 /*                                                                                       
                                                                                          
                                                                                          
 finclude "comptype.h" /*   This file will tell us what computer this is   */             
                         /* being compiled on. If DOGRAPH is defined,      */             
                         /* then all of the graphics commands will be      */ 
                         /* compiled too. If it is not defined, then       */             
                         /* the graphics commands will be commented out    *1 
                         /* and the program can run on a VAX or something  or/            
                                                                                          
 finclude "stdio.h"                                                                       
 finclude "nath.h"
 fifdef DOGRAPH                                                                           
 *include "gl.h"                                                                          
 finclude "device.h"                                                                      
 finclude "savescreen.h"                                                                  
 fendif
                                                                                          
 fifdef DOGRAPH
 "define IRIS4D     /* If this is defined, this is begin compiled on a 4D Iris */         
 fendif                                                                                   
                                                                                          
 "define PI 3.1415926                                                                     
 'define INITSTEP 16    /* The starting resolution */                                     
 'define ENDSTEP 1      /* The final resolution     */ 
 'define GETMAXX 1023   /* Last pixel horizontally */                                     
 *define GETMAXY 767    /* Last pixel vertically    */ 
 fdefine DISTANCE 1.0   /* Distance from eye to screen */                                 
 'define WIDTH 0.6      /* Width of the screen. The distance  from the                    
                        /* eye to the screen is 1.0, and this WIDTH
                        /* will tell you how much the picture is zoomed                   
                        /* in.                                                            

 fdefine FILENAME "SPHERES.SCR" /* File to which to save the image */                     
                                                                                          
 fifndef FALSE                                                                            
 'define FALSE 0                                                                          
 fendif                                                                                   
fifndef TRUE                                                                              
fdefine TRUE ("FALSE)                                                                     
fendif
                                                                                          
!define pmin (-2.0) /* The next five are the parameters for the Mandelbrot */             
fdefine pmax 1.0 /* set which is one of the surfaces available. */                        
fdefine qmin (-1.5)                                                                       
fdefine qmax 1.5                                                                          
'define MAXCOUNT 100 /* Max number of iterations for the Mandelbrot set */                
                                                                                          
fdefine CHECKERBOARD 0 /* Constants for the surface */                                    
'define MANDELBROT 1                                                                      
fdefine WATER 2          /* WATER does not work yet */                                    

'define RIPPLE 0.04 /* Frequency of ripples of water */                                   

'define AMP 4        /* Amplitude of the waves */
                                                                                          
                 /* Constants for bobject.reflect */
'define DULL 0   /* The object does not reflect anything */
fdefine SHINE 1 /* The object reflects all light rays to it */
fdefine GLASS 2 /* We can see through the object and the light is refracted */ 
'define MATTE 3 /* Like DULL but has some diffused shine from the light */
                                                                                          
'define SPHEREOBJECT 0 /* Constants for bobject.objecttype *1
fdefine TORUSOBJECT 1
'define CONEOBJECT 2
fdefine ELLIPSELOIDOBJECT 3 
"define PARABOLOIDOBEJECT 4 
fdefine EGGOBJECT 5
'define HYPERBOLOID 6
'define CYLINDER 7
                                                                                          
fdefine SILVER 1    /* Make bobject[i].Red-SILVER to color the object silver */
                                                                                          
fifndef DOGRAPH
typedef unsigned char RGBvalue; 
fendif
                                                                                          
char *OBJECTNAME[]-("Sphere","Torus","Cone ","Ellipseloid","Paraboloid","Egg ","Hyperbol 
oid","Cylinder");
char *SURFACENAME()-("Dull","Shine","Glass","Matte");
char *PLATFORMNAMEN("Checkerboard","Mandelbrot","Water");
                                                                                          
struct BOBJECT
                                                                                          
    int objecttype; /* See constants above */
    float x,y,z,r,A,B,C,sina,cosa,tana,top,bottom,ax,ay,az; 
    RGBvalue Red,Green,Blue; /* Color at brightest point */ 
    int reflect; /* See constants above */
  );
                                                                                          
struct RAY
                                                                                          
    float X1,Y1,X2,Y2,Z1,Z2; /* x-X1*t+X2; y-Yl*t+Y2; z-Z1*t+Z2 */
   ;
                                                                                          
struct BOBJECT bobject[200]; /* 0-99 are objects, and 100-199 are lights */ 
int inside[200);   /* TRUE if we are looking at the inside of an object */
                                                                                          
float getpower();
float cuberoot();
float sqr();
float getintersection(); 
float gettorusdistance(); 
float solvequarticequation(); 
float checkifinshadow();
                                                                                          
float HEIGHT - (WIDTH*GETMAXY/GETMAXX);
int done;
int xstart,ystart,xend,yend;
FILE *f;
int WAXED,TODISK,NUMBOBJECTS,ANTIALIASING,KEEPIMAGE,ZOOM,AMBIANT,NUMLIGHTS; 
float WINDOWX1,WINDOWY1,WINDOWX2,WINDOWY2;
int ZOOMX1,ZOOMY1,ZOOMX2,200MY2;
int SURFACE,MODIFIED,GID;
float EYEX,EYEY,EYEZ,CENTERX,CENTERY,CENTERZ; /* View points */ 
char str[80); /* All-purpose string */
                                                                                          
main (argc, argv)
int argc;                                                                                
char *argv[);                                                                            

#ifdef DOGRAPH                                                                           
  /*if (ismex())                                                                         
                                                                                         
    printf ("%cCannot run this program from MEX.  Quit MEX and try again.\n",7);         
    exit (1);                                                                            
  1*/                                                                                    
fendif                                                                                   
  initallvars();                                                                         
  if (argc > 1)                                                                          
                                                                                         
    TODISK-TRUE;                                                                         
    loadthisworld(argv[1]); 
    drawpicture();                                                                       
                                                                                         
  else                                                                                   
    while (menu()) ;
                                                                                         
                                                                                         
initallvars ()                                                                           
                                                                                         
  reseteverything();
  WAXED-FALSE;    /* WAXED means that the surface reflects */                            
  NUMBOBJECTS-0; /* Number of objects in this world        */
  TODISK-FALSE;   /* TRUE would send the output directly to disk */                      
  ANTIALIASING-TRUE; /* Will anti-alias the final image */                               
  KEEPIMAGE-FALSE;    /* TRUE would save the image after it is generated                 
  ZOOM-0;      /* Used when zooming into parts of the picture */                         
  SURFACE-CHECKERBOARD; /* Initial surface */
  AMBIANT-100;    /* Ambiant light. i.e., light in shadows, etc. Out of 255 /            
  MODIFIED-FALSE; /* TRUE-This world was modified and not saved to disk */ 
tifndef DOGRAPH                                                                          
  TODISK-TRUE;                                                                           
#endif                                                                                   
                                                                                         
                                                                                         
int menu() 
                                                                                         
  char ch;                                                                               
                                                                                         
  clrscr();                                                                              
  printf ("\n\n                                   Ray Tracer\n");                        
  printf ("\n\n                                       by\n");                            
  printf (    "                               Lawrence Kesteloot\n");                    
  printf (    "                                      and\n");                            
  printf (    "                                 Fredrik Fatemi\n");                      
  printf ("\n\n");                                                                       
  printf ("    G - Go ahead and draw the image\n");                                      
  printf ("    D - To disk/to screen (currently to 4s)\n",TODISK?"DISK":"SCREEN");       
  printf ("    K - Toggle Keep (save image to file '%s' - Currently 4s)\n",FILENAME,KEEP 
IMAGE?"ON":"OFF");                                                                       
  printf ("    L - Load a graphic file for display\n");
  printf ("    M - Modify this world\n");
  printf ("    N - Load a New World\n");                                                 
  printf ("    P - Parameters (Platform-4d, Ambiant-id,               Center4d,4d,4d)\n
",SURFACE,AMBIANT,(int)EYEX,(int)EYEY,(int)EYEZ,(int)CENTERX,(int)CENTERY,(int)CENTERZ); 
  printf ("    R - Reset window\n");
  printf ("    S - Save this world\n");                                                  
  printf ("    W - Toggle Waxed (currently 4s)\n",WAXED?"ON":"OFF");                     
  printf ("    Q - Quit Ray Tracer\n");
  printf ("\n\n>");
  gets(str);                                                                             
  ch = toupper(str[0]); 
  switch (ch)
                                                                                       
    case 'G': sleep(2); drawpicture(); break; /* SLEEP is necessary */
    case 'D': TODISK-ITODISK; break;
    case 'K': KEEPIMAGE..!KEEPIMAGE; break;
    case 'L': loadfile(); break; 
    case 'M': modifyworld(); break; 
    case 'N': loadworld(); break; 
    case 'P': changeparameters(); 
    case 'R': resetwindow(); break; 
    case 'S': saveworld(); break;
    case 'W': WAXED-IWAXED; MODIFIED-TRUE; break;
    case 'Cr: if (MODIFIED)
                                                                                       
                printf ("WORLD was modified. Are you sure you want to quit? (y/N] "); 
                gets(str);
                if (toupper(str[0]).--'Y') return (FALSE);
                                                                                       
              else return (FALSE);
  1 
  return(TRUE);
1
                                                                                       
drawpicture ()
                                                                                       
  if (ITODISK) initialize(); else opendiskfile(); 
  drawimage();
  if (ITODISK) cleanup(); else closediskfile();
}
                                                                                       
initialize ()
                                                                                       
       DOGRAPH
       IRIS4D
  prefposition (0,1023,0,767); 
  foreground();
  GID-winopen ("Ray Tracer");
                                                                                       
  ginit () ;
  cursoff(); 
#endif
  tpoff();
  RGBmode(); 
  gconfig(); 
  RGBcolor(0,0,0);
  clear();
  qdevice (LEFTMOUSE);
  qdevice (MIDDLEMOUSE);
  qdevice (RIGHTMOUSE);
  qdevice (KEYBD);
  qreset(); 
#endif
                                                                                       
                                                                                       
opendiskfile()
                                                                                       
  if ((f-fopen(FILENAME,"w"))--NULL)
                                                                                       
    printf ("RAY.C: Unable to open file 4s for output.",FILENAME); 
    exit(1);
drawimage() 
 {                                                             
  float height,width,x;                                        

  initothervars();                                             
  do
   ZOOM-0;                                                     
   drawbobjects();                                             
   if (ZOOM--3)
                                                               
     width-WINDOWX2-WINDOWX1;                                  
     height-WINDOWY2-WINDOWY1;                                 
     WINDOWY2-(1.0*zooMY2/GETMAXY)*height+WINDOWyl; 
     WINDOWY1-(1.0*ZOOMY1/GETMAXY)*height+WINDOWY1; 
     WINDOWX2-(1.0*ZOOMX2/GETMAXX)*width+WINDOWX1;             
     WINDOWX1-(1.0*ZOOMX1/GETMAXX)*wldth+WINDOWX1; 
     x-(WINDOWX1+WINDOWX2)/2.0; 
     height-WINDOWY2-WINDOWY1;                                 
     width-height*GETMAXX/GETMAXY; 
     WINDOWX1-x-width/2;                                       
     WINDOWX2-x+width/2;                                       
                                                               
                                                               
 while (ZOOM--3);                                              
 if ((ldone)WITODISK))                                         
                                                               
   if (ANTIALIASING) antialias();                              
fifdef DOGRAPH                                                 
   if (KEEPIMAGE) saveimage (FILENAME);                        
fendif                                                         
  }                                                            
j                                                              
                                                               
cleanup ()                                                     

fifdef DOGRAPH 
 if (!done)                                                    
   ringbell();                                                 
   ringbell();                                                 
   while (qtest()--0);                                         
                                                               
 greset(); 
 color(BLACK); 
 clear();                                                      
 tpon();                                                       
fifdef IRIS4D                                                  
 winclose(GID);                                                
felse                                                          
 gexit();                                                      
fendif
fendif                                                         
                                                               
                                                               
closediskfile()                                                
{                                                              
 fclose(f);                                                    
fifdef DOGRAPH                                                 
 ringbell();                                                   
fendif
                                                               
                                                               
initothervars()                                                
  /* This routine defines some often-used variables so that they don't 
  /* have to be calculated at every pixel */
                                                                              
  int 1;
                                                                              
  for (1..0;i<NUMBOBJECTS;1++)
   if (bobject(il.objecttype--CONEOBJECT)
                                                                              
     bobject(1].sina-sin(bobject(i).A); 
     bobject[1].cosa-cos(bobject(1].A); 
     bobject(1).tana-tan(bobject[1].A);
                                                                              
                                                                              
  ZOOM- 0; 
                                                                              
                                                                              
reseteverything()
                                                                              
  NUMLIGHTS.4;
  bobject[100].x-s -300; 
  bobject(100].y-500; 
  bobject(100).z..400;
  bobject(100).r-100; /* Radius of bulb */ 
  bobject[100].reflect..10000; /* BRIGHTNESS of light 
  bobject(100].objecttype..SPHEREOBJECT;
  resetwindow();
  CENTERX-0; /* Point you are looking at in the world 
  CENTERY-0;
  CENTERZ.. -300;
  EYEX-0; /* Coordinate of eye */
  EYEY-0;
  EYEZ-150;
  SURFACE-0; /* Checkerboard */
                                                                              
                                                                              
resetwindow()
{ WINDOWX1.. -WIDTH/2; /* Set ZOOM back to default */ 
  WINDOWY1- -HEIGHT/2;
  WINDOWX2- WIDTH/2;
  WINDOWY2.. HEIGHT/2;
                                                                              
                                                                              
drawbobjects()
{
  int xx,yy,x,y,step,R,G,B,val,skip;
  short d;
  float X,Y,height,width;
  RGBvalue RA(1024),GA(1024),BAt1024],downRA(1024],downGA(1024],downBA[1024];
                                                                              
  xstart-0;
  xend-GETMAXX; 
  ystart-0;
  yend-GETMAXY; 
  done-FALSE;
  skip-FALSE;
  width-WINDOWX2-WINDOWX1;
  height-WINDOWY2-WINDOWY1;
                                                                              
  step-INITSTEP;
  if (TODISK) step-1; 
  do
                                                                                              
     for(y-ystart;(y<-yend)WIdone);y+-step)                                                   
       if (TODISKWy>ystart))                                                                  
                                                                                              
         memcpyldownRA,RA,1024);                                                              
         memcpy(downGA,GA,1024);                                                              
         memcpy(downBA,BA,1024); 
       1                                                                                      
       Y WINDOWY1+(height*y/GETMAXY);                                                         
       for (x-xstart;xc-xend;x+-step)                                                         
       {
         if (((step--INITSTEP)II(x%(step*2)>0)11(Y%(step*2)>0)    IlskiplITODISK)             

          X - WINDOWX1+(width*x/GETMAXX); 
          getpixelcolor(X,Y,&R,iG,&B);                                                        
          if (TODISK)                                                                         
                                                                                              
             RA[x)-(char)R;                                                                   
             GA(x)-(char)G; 
             BA[xj-(char)B;                                                                   
           1                                                                                  
          else                                                                                
                                                                                              
#ifdef DOGRAPH                                                                                
             RGBcolor(R,G,B);
             if (step--1)
              pnt2i(x,y);                                                                     
             else                                                                             
               rectfi(x,y,x+step-1,y+step-1);                                                 
fendif                                                                                        

                                                                                              
                                                                                              
#ifdef DOGRAPH                                                                                
       if (1TODISK&&gtest())

         d -1; 
         do                                                                                   
                                                                                              
          val-gread(&d);                                                                      
          if (val--KEYBD)
                                                                                              
             if (d--'z') ZOOM-l;                                                              

          if (d--1) cursors(};                                                                
          if (d--0)
                                                                                              
             cursoff();                                                                       
             xx..getvaluator(MOUSEX);                                                         
             yr.getvaluator(MOUSEY);                                                          
             if (val--LEFTMOUSE)
                                                                                              
              if (ZOOM--1)                                                                    
                                                                                              
                 ZOOMX1 -xx;                                                                  
                 ZOOMY1 -yy;                                                                  
                 ZOOM-2;                                                                      
              1
              if ((xx<xstart)II(yy<ystart)) skip-TRUE;                                        
              xstart-xx-xx*step;                                                              
              ystart-yy-yy%step;                                                              
              x-xstart;                                                                       
                   if (val--RIGHTMOUSE) 
                   (
                     if (ZOOM - -2)
                        ZOOMX2-xx; 
                        ZOOMY2-yy; 
                        ZOOM-3;
                                                                                
                     if ((xx>xend)II(yy>yend)) skip-TRUE;
                     xend-xx-xx%step; 
                     yend■yy-yy%step;
                                                                                
                   if (val...MIDDLEMOUSE) done-TRUE;
                                                                                
                                                                                
               while ((d1-0)&&(vall■KEYBD));
             1
       fendif
             if (TODISK)
                                                                                
               if (ANTIALIASINGWy>ystart)) 
                 antialiasrow(RA,GA,BA,downRA,downGA,downBA,y);
               fwrite (downRA,1,1024,f); 
               fwrite (downGA,1,1024,f); 
               fwrite (downBA,1,1024,f);
                                                                                
                                                                                
           step/-2;
         1
         while ((step     ENDSTEP)WZOOM1-3));
         if (TODISK)
                                                                                
           fwrite (RA,1,1024,f); 
           fwrite (GA,1,1024,f); 
           fwrite (BA,1,1024,f);
                                                                                
                                                                                
                                                                                
       getpixelcolor(X,Y,RR,GG,BB) 
       float X,Y;
       int *RR,*GG,*BB;
                                                                                
         struct RAY ray;
         float r,rr,th,a,XX,YY,ZZ,XXX,YYY,ZZZ;
                                                                                
         /* The next few lines are the transformation to move the window around
                                                                                
         rr=1.0/sqrt(sqr(EYEX-CENTERX)+sgr(EYEY-CENTERY)+sgr(EYEZ-CENTERZ)); 
         XX-EYEX-(EYEX-CENTERX)*rr;
         YY-EYEY-(EYEY-CENTERY)*rr; 
         ZZ.-EYEZ-(EYEZ-CENTERZ)*rr;
                                                                                
         XXX-EYEX-XX; 
         YYYEYEY-YY; 
         ZZZ-EYEZ-ZZ;
       /* ray.X1-(X)*sgrt(ZZZ*ZZZ+YYY*YYY)+(-Y)*XXX*YYY-XXX;
         ray.Y1- (Y) *sgrt (ZZZ*ZZZ+XXX*XXX) -YYY;
         ray.Z1-(-X)*XXX+(-Y)*YYY-ZZZ;*/
                                                                                
         rr-sgrt(XXX*XXX+ZZZ*ZZZ);
         ray.X1-(Y*XXX*YYY+X*ZZZI/rr-XXX; 
         ray.Y1-Y*rr-YYY;
         ray.Z1..(-X*XXX+Y*ZZZ*YYY)/rr-ZZZ;
  ray.X2-EYEX; 
  ray.Y2-EYEY;                                                                           
  ray. Z2-EYEZ;                                                                          
                                                                                         
  *RR..(ini)((-Y+HEIGHT/2)/HEIGHT*255);                                                  
  if (*RR > 255) *RR-255;                                                                
  if (*RR < 0) *RR-0;
  *GG- *RR;                                                                              
  *BB- 255;                                                                              
                                                                                         
  getraycolor (ray,RR,GG,BB,FALSE);                                                      
                                                                                         
                                                                                         
getraycolor (ray,RR,GG,BB,inglass)                                                       
struct RAY ray;
int *RR,*GG,*BB,inglass;                                                                 
{                                                                                        
  /* getraycolor returns the color of the object at the point at which the */            
  /* ray intersects the nearest object. If the object is reflective, then */ 
  /* getraycolor is called recursively with the reflected or refracted ray */            
                                                                                         
  int r,g,b,i,j,RRR,GGG,BBB,closest,intersect,isinside;                                  
  float x,y,z,t,A,B,C,q,cx,cy,cz,a,nx,ny,nz,lx,ly,lz,rr,rad,inshadow,temp,mrad,ma;       
  struct RAY newray;                                                                     
                                                                                         
  rr-1.0/sqrt(ray.Xl*ray.Xl+ray.Y1*ray.Y1+ray.Z1*ray.Z1);                                
  ray.X1*■rr;                                                                            
  ray.Y1*∎rr;                                                                            
  ray.21*--rr;
  ray.X2+-.ray.X1;                                                                       
  ray.Y2+-ray.Yl;                                                                        
  ray.22+..ray.21;                                                                       
  intersect-FALSE;                                                                       
  cr99999999999;                                                                         
  for (1-0;i<NUMBOBJECTS;i++) /* Find nearest object */                                  
                                                                                         
    t.-getintersection(i,ray,inglass); 
    if ((t < q) ts (t > 0))                                                              
                                                                                         
     q-t;                                                                                
     closest-i;                                                                          
     intersect-TRUE;                                                                     
                                                                                         
  1                                                                                      
  if (intersect)
                                                                                         
    t -q;                                                                                
    1-closest;                                                                           
    isinside-inside[i];                                                                  
    {                                                                                    
     lo.ray.Xl*t+ray.X2;                                                                 
     p.ray.Y1*t+ray.Y2;                                                                  
     z..ray.21*t+ray.22;                                                                 
     inshadow-checkifinshadow(x,y,z): /* Returns brightness at that point */             
     getnormalvector(i,x,y,z,&nx,&ny,&nz);                                               
     if (bobject[i].Red....SILVER) 
                                                                                         
       *RR.. *GG- *BB-150;                                                               

     else                                                                                
                                                                                         
       *RR-bobject[i].Red;                                                               
       *GG-bobject[i].Green;                                                        nglas
       *BB-bobject[i].Blue;                                                              
                  if (val--RIGHTMOUSE) 
                  (
                    if (ZOOM - -2)
                       ZOOMX2-xx; 
                       ZOOMY2-yy; 
                       ZOOM-3;
                                                                               
                    if ((xx>xend)II(yy>yend)) skip-TRUE;
                    xend-xx-xx%step; 
                    yend■yy-yy%step;
                                                                               
                  if (val...MIDDLEMOUSE) done-TRUE;
                                                                               
                                                                               
              while ((d1-0)&&(vall■KEYBD));
            1
      fendif
            if (TODISK)
                                                                               
              if (ANTIALIASINGWy>ystart)) 
                antialiasrow(RA,GA,BA,downRA,downGA,downBA,y);
              fwrite (downRA,1,1024,f); 
              fwrite (downGA,1,1024,f); 
              fwrite (downBA,1,1024,f);
                                                                               
                                                                               
          step/-2;
        1
        while ((step     ENDSTEP)WZOOM1-3));
        if (TODISK)
                                                                               
          fwrite (RA,1,1024,f); 
          fwrite (GA,1,1024,f); 
          fwrite (BA,1,1024,f);
                                                                               
                                                                               
                                                                               
      getpixelcolor(X,Y,RR,GG,BB) 
      float X,Y;
      int *RR,*GG,*BB;
                                                                               
        struct RAY ray;
        float r,rr,th,a,XX,YY,ZZ,XXX,YYY,ZZZ;
                                                                               
        /* The next few lines are the transformation to move the window around
                                                                               
        rr=1.0/sqrt(sqr(EYEX-CENTERX)+sgr(EYEY-CENTERY)+sgr(EYEZ-CENTERZ)); 
        XX-EYEX-(EYEX-CENTERX)*rr;
        YY-EYEY-(EYEY-CENTERY)*rr; 
        ZZ.-EYEZ-(EYEZ-CENTERZ)*rr;
                                                                               
        XXX-EYEX-XX; 
        YYYEYEY-YY; 
        ZZZ-EYEZ-ZZ;
      /* ray.X1-(X)*sgrt(ZZZ*ZZZ+YYY*YYY)+(-Y)*XXX*YYY-XXX;
        ray.Y1- (Y) *sgrt (ZZZ*ZZZ+XXX*XXX) -YYY;
        ray.Z1-(-X)*XXX+(-Y)*YYY-ZZZ;*/
                                                                               
        rr-sgrt(XXX*XXX+ZZZ*ZZZ);
        ray.X1-(Y*XXX*YYY+X*ZZZI/rr-XXX; 
        ray.Y1-Y*rr-YYY;
        ray.Z1..(-X*XXX+Y*ZZZ*YYY)/rr-ZZZ;
  ray.X2-EYEX; 
  ray.Y2-EYEY;                                                                        
  ray. Z2-EYEZ;                                                                       
                                                                                      
  *RR..(ini)((-Y+HEIGHT/2)/HEIGHT*255);                                               
  if (*RR > 255) *RR-255;                                                             
  if (*RR < 0) *RR-0;
  *GG- *RR;                                                                           
  *BB- 255;                                                                           
                                                                                      
  getraycolor (ray,RR,GG,BB,FALSE);                                                   
                                                                                      
                                                                                      
getraycolor (ray,RR,GG,BB,inglass)                                                    
struct RAY ray;
int *RR,*GG,*BB,inglass;                                                              
{                                                                                     
  /* getraycolor returns the color of the object at the point at which the */         
  /* ray intersects the nearest object. If the object is reflective, then */ 
  /* getraycolor is called recursively with the reflected or refracted ray */         
                                                                                      
  int r,g,b,i,j,RRR,GGG,BBB,closest,intersect,isinside;                               
  float x,y,z,t,A,B,C,q,cx,cy,cz,a,nx,ny,nz,lx,ly,lz,rr,rad,inshadow,temp,mrad,ma;    
  struct RAY newray;                                                                  
                                                                                      
  rr-1.0/sqrt(ray.Xl*ray.Xl+ray.Y1*ray.Y1+ray.Z1*ray.Z1);                             
  ray.X1*■rr;                                                                         
  ray.Y1*∎rr;                                                                         
  ray.21*--rr;
  ray.X2+-.ray.X1;                                                                    
  ray.Y2+-ray.Yl;                                                                     
  ray.22+..ray.21;                                                                    
  intersect-FALSE;                                                                    
  cr99999999999;                                                                      
  for (1-0;i<NUMBOBJECTS;i++) /* Find nearest object */                               
                                                                                      
    t.-getintersection(i,ray,inglass); 
    if ((t < q) ts (t > 0))                                                           
                                                                                      
     q-t;                                                                             
     closest-i;                                                                       
     intersect-TRUE;                                                                  
                                                                                      
  1                                                                                   
  if (intersect)
                                                                                      
    t -q;                                                                             
    1-closest;                                                                        
    isinside-inside[i];                                                               
    {                                                                                 
     lo.ray.Xl*t+ray.X2;                                                              
     p.ray.Y1*t+ray.Y2;                                                               
     z..ray.21*t+ray.22;                                                              
     inshadow-checkifinshadow(x,y,z): /* Returns brightness at that point */          
     getnormalvector(i,x,y,z,&nx,&ny,&nz);                                            
     if (bobject[i].Red....SILVER) 
                                                                                      
       *RR.. *GG- *BB-150;                                                            

     else                                                                             
                                                                                      
       *RR-bobject[i].Red;                                                            
       *GG-bobject[i].Green;                                                        
       *BB-bobject[i].Blue;                                                           
  rr-1.0/sqrt(nx*nx+ny*ny+nz*nz);
  if (isinside) rr- -rr; /* We're looking at the INSIDE of an object */ 
  nx*-rr; ny*..rr; nz*-rr;
  a-ma-0;
  for (j-100; j< (100+NUMLIGHTS) ; j++)
                                                                                     
   lx-bobject[j].x-x; 
   ly..bobject[j].y-y; 
   lz-bobject[j].z-z; 
   temp-1.0/sqrt(lx*lx+ly*ly+lz*lz);
   lx*-temp; ly*..temp; lz*..temp;
   rad-lx*nx+ly*ny+lz*nz;
   if (bobject[i].reflect--MATTE)
                                                                                     
     getreflection(ray.X1,ray.Y1,ray.21,nx,ny,nz,‘newray.X1,&newray.Y1,&newray.Z1); 
     mrad-newray.Xl*lx+newray.Y1*ly+newrayal*lz;
     ma+-mrad;
                                                                                     
   if (rad > 0) a+-rad;
  1
  a/-NUMLIGHTS;
  if (a<0.0)
  if (a>1.0)
  *RR...(int)(*RR*a*inshadow); 
  *GG-(int)(*GG*a*inshadow); 
  *BB.-(int)(*BB*a*inshadow); 
  if (bobject[i).reflect==MATTE)
                                                                                     
   ma/=NUMLIGHTS;
   if (ma<0.0) ma-0.0;
   if (ma>1.0) ma-1.0;
   ma*-ma*ma;
   *RR-(int)(*RR+(255-*RR)*ma); 
   *GG-(int)(*GG+(255-*GG)*ma); 
   *BB...(int)(*BB+(255-*BB)*ma);
                                                                                     
  if ((bobject[i].reflect )- DULL)&4(bobjectti].reflect 1= MATTE))
  f
   getreflection(ray.X1,ray.Y1,ray.Z1,nx,ny,nz,&newray.X1,‘newray.Y1,&newray.21); 
   newray.X2-x;
   newray.Y2-y; 
   newray.22..z;
   if (bobject[ii.Red”SILVER)
                                                                                     
     RRR- *RR;
     GGG■ *GG;
     BBB.* *BB;
     getraycolor(newray,iRRR,I,GGG,038B,inglass);
     *RR-(RRR+*RR)*0.5; 
     *GG-(GGG+*GG)*0.5; 
     *BB-(BBB+*BB)*0.5;
     if (*RR >255) *RR-255;
     if (*GG >255) *GG-255;
     if (*BB >255) *BB-255;
                                                                                     
   else
   if (bobject[i].reflect■■GLASS)
                                                                                     
     if (iinglass)
       getraycolor(newray,RR,GG,BB,inglass); 
     getrefraction(ray.X1,ray.Y1,ray.21,nx,ny,nz,4newray.X1,&newray.Y1,&newray.Z1,&inglass);
     if (inglass)
                                                                                         
            RRR- *RR;                                                                    
            GGG- *GG;                                                                    
            BBB... *BB;
            getraycolor(newray,5RRR,&GGG,5BBB,inglass);                                  
            *RR-(3*RRR+*RR)/4;                                                           
            *GG-(3*GGG+*GG)/4;                                                           
            *BB-(3*BBB+.148)/4;
            if (*RR > 255) *RR-255;                                                      
            if (*GG > 255) *GG-255;
            if (*BB > 255) *BB-255;                                                      
                                                                                         
          else                                                                           

            getraycolor(newray,RR,GO,BB,inglass);                                        
            if (*RR > 255) *RR-255;                                                      
            if (*GG > 255) *GG-255;                                                      
            if (*BB > 255) *BB-255;                                                      
                                                                                         

       else                                                                              
        (
          M.* *RR;                                                                       
          GGG- *GG;                                                                      
          BBB- *BB;                                                                      
          getraycolor(newray,5RRR,5000,5BBB,inglass);
          *RR-(5*RRR+*RR)/6;                                                             
          *GG-(5*GGG+*GG)/6; 
          *BB-(5*BBB+*BB)/6;                                                             
          if (*RR > 255) *RR-255;
          if (*GG > 255) *GG-255;                                                        
          if (*BB > 255) *B111*.255;
                                                                                         
                                                                                         

                                                                                         
  cy- -50.0; /* Y position of surface */                                                 
  if (ray.Y1 1- 0)                                                                       

   t-(cy-ray.Y2)/ray.Y1;                                                                 
   if ((t > 0) 55 (t < q))

     q-t;                                                                                
     cx■ray.Xl*t+ray.X2; 
     cz■ray.Z1*t+ray.Z2;                                                                 
     if ((es < 0) si (cz > -400) 55 (fabs(cx) < 120))                                    

       intersect-TRUE;                                                                   
       inshadow-checkifinshadow(cx,cy,cz);                                               
       t-0;                                                                              
       for (1-100;1<(100+NUMLIGHTS);i++)                                                 
                                                                                         
          q..(bobject[i].y-cy)/sqrt(sqr(bobject[i].x-cx)+sqr(bobject[i].y-cy)+sqr(bobject
[i].z-cz));                                                                              
          if (bobject[il.reflect > 5000) q-q/2+0.5;                                      
          t+-q;                                                                          
                                                                                         
       inshadow*...t/NUMLIGHTS; 
       if (WAXED)                                                                        
                                                                                         
          if (SURFACE--WATER)                                                            
                                                                                         
           rad-sqrt(cx*cx+cz*cz):                                                        
           newray.Y1■1.0/(1.0+RIPPLE*RIPPLE*AMP*AMP*sqr(cos(RIPPLE*rad)));               
            newray.X1-cx*sqrt(1.0-newray.Y1*newray.Y1)/rad; 
            newray.Z1-cz*sqrt(1.0-newray.Y1*newray.Y1)/rad; 
            if (cos(rad) > 0)
                                                                                           
              newray.X1... -newray.X1; 
              newray.Z1- -newray.Z1;
            1
                                                                                           
          else
                                                                                           
            newray.X1-ray.X1; 
            newray.Y1- -ray.Y1; 
            newray.Z1-ray.Z1;
                                                                                           
          newray.X2-cx; 
          newray.Y2-cy; 
          newray.Z2-cz; 
          RRR-GGG-BBB-0; 
          getraycolor(newray,5RRR,4GGG,5BBB,inglass);
                                                                                           
        if (SURFACE - -CHECKERBOARD)
                                                                                           
          cx-(cx+150)/20.0;
          cz/-20.0;
          if (((int)(cx) + (int)(cz)) 4 2)
                                                                                           
            *RR-255; *GG...0; *BB=0; 
                                                                                           
          else
                                                                                           
            *R.R..255; *GG-255; *BB-255;
                                                                                           
        }
        if (SURFACE - -MANDELBROT)
                                                                                           
          cx..(pmax-pmin)*(cx+100.0)/200.0+pmin; 
          cz-(qmax-qmin)*(cz+400.0)/400.0+qmin; 
          getmandelbrot(cx,cz,RR,GG,BB);
                                                                                           
        if (SURFACE--WATER) /* Not working yet */ 
                                                                                           
                                                                                           
          for (j-100;j<(100+NUMLIGHTS);j++)
                                                                                           
            a+-((bobject[j].x-cx)*newray.X1+(bobject[jj.y-cy)*newray.Y1+(bobject[j].z-cz) 
*newray.Z1)/(sqrt(sqr(bobject[j].x-cx) + sqr(bobject[j].y-cy) + sqr(bobject[j].z-cz)));
                                                                                           
          a/-NUMLIGHTS;
          if (a<0.0) a-0.0; 
          if (a>1.0) a-1.0; 
          *GC-, *RR- *BB-(int)(255*a);
          printf ("%f 4f 4f 4f\n",a,newray.X1,newray.Y1,newray.Z1);

        *RR-(int)(*RR*inshadow); 
        *GG-.(int)(*GG*inshadow); 
        *BB-(int)(*BB*inshadow); 
        if (WAXED)
                                                                                           
          *RR-(4**RR+RRR)/5; 
          *GG-(4**GG+GGG)/5; 
          *BB...(4**BB+BBB)/5;
          if (*RR > 255) *RR-255;
          if (*GG > 255) *GG-255;
          if (*BB > 255) *BB-255;
      )
    )                                                                             
   )                                                                              
  for (j-100;j<(100+NUMLIGHTS);j++) /* This checks to see if the ray              
   (                              /* intersects any light bulbs                   
    t-getintersection(j,ray,FALSE); /* Light */                                   
    if (t > 0)                                                                    
    (                                                                             
      inshadow-0;                                                                 
      for (i-0;i<NUMBOBJECTS;i++)                                                 
      ( t=getintersection(1,ray,FALSE);                                           
        if (t > 0) inshadow-1;
      1                                                                           
      if (inshadow--0)                                                            
      (                                                                           
        *RR-1000; /* 1000 out of 255 means the bulb blooms. This is done */       
        *GG-1000; /* so that if it is averaged with any other color, it */        
        *BB-200; /* will still look bright white. BB-200- it is yellow */         
        intersect-TRUE;                                                           
    1 1                                                                           
  )                                                                               
  return;                                                                         
}                                                                                 
                                                                                  
getnormalvector (1,x,y,z,nx,ny,nz)                                                
int i;                                                                            
float x,y,z,*nx,*ny,*nz;                                                          
(                                                                                 
  /* Returns the normal vector to the surface of object "1" at point x,y,z */     
                                                                                  
  float t;                                                                        
                                                                                  
  *nx-x-bobject[1].x;                                                             
  *ny-y-bobject[i].y;                                                             
  *nz-z-bobject[1].z;                                                             
  switch (bobject[1].objecttype)                                                  
  {                                                                               
    case SPHEREOBJECT:                                                            
      break;                                                                      
    case TORUSOBJECT:                                                             
      t-sqrt(*nx**nx+*ny**ny);                                                    
      *nx--(bobject[1].A**nx)/t;                                                  
      *ny--(bobject[i].A**ny)/t;                                                  
      break;                                                                      
    case ELLIPSELOIDOBJECT:                                                       
      *nx*-bobject[i].C*bobject[1].B/bobject[1].A;                                
      *ny*-bobject[1).A*bobject[1].C/bobject[1].B;                                
      *nz*-bobject[1].A*bobject[1].B/bobject(1).C;                                
      break;                                                                      
    case CONEOBJECT:                                                              
      *nx*- -bobject[1].cosa/(*ny*bobject[1).tana);                               
      *nz*- -bobject[1].cosa/(*ny*bobject(1).tana);                               
      *ny= bobject[i).sina;                                                       
      break;                                                                      
    case HYPERBOLOID:                                                             
      Any*,.. -(bobject[i).A*bobject[1].A);                                       
      break;                                                                      
    case CYLINDER:                                                                
      *ny-O;                                                                      
      break;                                                                      
  1                                                                               
float getintersection (1,ray,inglass) 
int i;
struct RAY ray;
int inglass;
(
  /* The program spends 33% of its time in this one procedure. Amazing. */ 
  /* Anyway, it checks to see if the ray "ray" intersects the object "1" */ 
  /* and if so, it will return the distance to that object in terms of */ 
  /* lengths of "ray". */
  float d,t,tt,A,B,C,AA,BB,CC,X1,Y1,21,X2,Y2,22,tangent;
                                                                             
  Xl-ray.X1;
  Y1-ray.Yl;
  21-ray.21;
  X2-ray.X2-bobject[1).x; 
  Y2-ray.Y2-bobject[11.y; 
  Z2-ray.22-bobject[1).z; 
  inside[1]-FALSE;
  (
    switch (bobject[1].objecttype)
    (
      case ELLIPSELOIDOBJECT:
        AA-1.0/(bobject[i1.A*bobject[1].A); 
        BB-1.0/(bobject[1].B.bobject[1].B); 
        CC-1.0/(bobject[1].C*bobject(1).C);
        A-Xl*Xl*AA+Yl*Y1*BB+Z1*Z1*CC;
        B-2*(X1*X2*AA+Y1*Y2*BB+Z1*Z2*CC);
        C.,X2*X2*AA+Y2*Y2*BB+Z2*Z2*CC-1;
        break;
      case SPHEREOBJECT:
      case TORUSOBJECT:
        A=X1*Xl+Yl*Y1+Z1*Z1;
        B..2*(X1*X2+Y1*Y2+Z1*Z2); 
        C-X2*X2+Y2*Y2+Z2*Z2-bobject[1].r*bobject[i].r;
        break;
      case CONEOBJECT:
        tangent-sqr(bobject(1).tana);
        A-Xl*X1+21.21-Y1*Y1*tangent;
        B-2*(X1*X2+Z1*Z2-Y1*Y2*tangent);
        C-X2*X2+22*Z2-Y2*Y2*tangent;
        break;
      case HYPERBOLOID:
        AA-sqr(bobject[1).A);
        BB-sqr(bobject[1].B);
        A-Xl*Xl+Z1*Z1-AA*Y1*Y1;
        B-2*(X1*X2+Z1*Z2-AA*Y1*Y2);
        C-X2*X2+Z2*Z2-AA*Y2*Y2-BB;
        break;
      case CYLINDER:
        A-X1*X1+21*21;
        B-2*(X1*X2+Z1*22);
        C-X2*X2+Z2*Z2-bobject[1].r*bobject[11.r;
        break;
    }
    d-B*B-4*A*C; 
    if (d > 0)
    (
      If (bobject[1].objecttype--TORUSOBJECT) 
      i
        t..(-B-sqrt(d))/(A+A);

       ray.X2-ray.Xl*t+ray.X2; 
       ray.Y2..ray.Y1*t+ray.Y2; 
       ray.12-ray.Z1*t+ray.Z2;                                                 
       tt-gettorusdistance (i,ray);                                            
       if (tt < 0) t-tt; else t+-tt;
     else                                                                      
                                                                               
       if (inglass)                                                            
         t-(-B+sqrt(d))/(A+A);                                                 
       else                                                                    
         t-(-B-sqrt(d))/(A+A);
       if ((bobject(I].top 1- 0)11(bobject(il.bottom 1- 0))
                                                                               
         if ((Y1*t+y2 < bobject(i].bottom)11(Y1*t+Y2 > bobject[i].toP)I1(t<0)) 
          if (linglass)                                                        
          {                                                                    
            t...(-B+sqrt(d))/(A+A);
            if ((Y1*t+Y2 < bobject(il.bottom)11(Y1*t+Y2 > bobject[i].top))     
              t- -1.0;                                                         
            else
              inside[i]-TRUE;                                                  
                                                                               
                                                                               
                                                                               
   1                                                                           
   else                                                                        
     t -d;                                                                     
                                                                               
 return (t);                                                                   
                                                                               
                                                                               
float gettorusdistance (i,ray)                                                 
int i;
struct RAY ray;                                                                
                                                                               
 /* This will return the distance, as in Getlntersection, from the             
 /* current ray to its intersection with the torus of object "i",              
 /* in terms of lengths of ray "ray". Since a ray can intersect 
 /* a torus in four places (think about it), the equation is a                 
 /* quartic equation, and the routine solvequacticequation is used             
 /* to find the lowest positive root. Since finding the roots of               
 /* a quartic equation is real math-intensive, round-off problems              
 /* sometimes cause messy toruses (or tori?)
                                                                               
 float A,B,C,D,t,AA,BB,X1,X2,Y1,Y2,Z1,Z2,PartA,PartAAA,H;                      
                                                                               
 X1..ray.X1; X2-ray.X2-bobject(i].x;                                           
 Y1-,ray.Y1; Y2..ray.Y2-bobject(i).y;                                          
 Zl-ray.Z1; Z2-ray.Z2-bobject(i).z;                                            
 AA-bobject[i].A;                                                              
 BB-bobject(i].B;
                                                                               
 PartA-4.0*(Xl*X2+Yl*Y2+21*22);                                                
 PartAAA-1.0*(X2*X2+Y2*Y2+22*22+AA*AA-BB*BB);
 H-4.0*AA*AA;                                                                  
 A-PartA;
 12-0.25*PartA*PartA+2*PartAAA-H*(X1*X1+Y1*Y1);                                
 C-PartA*PartAAA-2*H*(Xl*X2+Yl*Y2);                                            
 D-PartAAA*PartAAA-H*(X2*X2+Y2*Y2);                                            
                                                                               
 t-solvequarticequation (A,B,C,D);                                             

 return (t);                                                                   
float cuberoot (x) 
float x;
                                                                            
 /* Returns the cube root of X */
 if (x--0) return (0.0►; 
 if (x < 0)
   return (-exp(log(-x)/3)); 
 return (exp(log(x)/3));
                                                                            
                                                                            
float solvequarticequation(A,B,C,D) 
float A,B,C,D;
{
 /* Don't even try to figure out what's going on in this function A/
                                                                            
 float partl,cosPhi,TanPhi,rootl,disc,LL,KK,GGG,asqd,part2,P,10,R,G,HH,PP; 
 float DDDD,H,Phi,RR,SS,R1,R2,R3,R4,t;
                                                                            
 DDD1›.4.0*D; 
 asqd-A*A;
 P- -B;
 Q-A*C-DDDD;
 R- -asqd*D+B*DDDD-C*C;
 PP..B*B;
 G..(340-PP)/3; 
 GGG-G*G*G; 
 H-P*(2*PP-9*0)/27+R;
 HH■11/2;
 disc-41*H/4+GGG/27;
 if (disc>-0)
                                                                            
   disc-sqrt(disc); 
   rootl-cuberoot(-HH+diac)-cuberoot(HH+disc);
 1
 if (disc<0)
                                                                            
   cosPhi- -HH*sqrt(-27/GGG);
   TanPhi-sqrt(1-cosPhi*cosPhi)/cosPhi; 
   Phi•atan(TanPhi);
   root1-2*sqrt(-0/3)*cos(Phi/3);
                                                                            
 rootl--P/3;
 RR-asqd/4+P+rootl; 
 if (RR<0)
   return (-1.0); 
 R1-R2-R3-R4- -1.0; 
 SS-sqrt(RR);
 if (fabs(SS) < 0.00001)
                                                                            
   part1-3*A*A/4-2*B; 
   part2-2*sqrt(rootl*root1-4*D);
                                                                            
 else
                                                                            
   part1-3*asqd/4-RR+2*P; 
   part2-.(4*A*B-8*C-asqd*A)/(4*SS);
 }
 KKnoart1+part2; 
 if (KK>-0)
                                                                            
   KK-sqrt(KK);
     R1- -A/4+SS/2+KK/2;                                                              
     R2- -A/4+SS/2-KK/2;                                                              
   1
  LL-partl-part2;                                                                     
  if (LL>-0)                                                                          
                                                                                      
     IX.sqrt(LL);                                                                     
     R3- -A/4-SS/2+LL/2;                                                              
     R4- -A/4-SS/2-LL/2;                                                              
  1                                                                                   
  t-40000;                                                                            
  if  ((R1 >0) &&  (R1 < t))
  if  ((R2 >0) cs  (R2 < t))t-R2;
  if  ((R3 >0) z4  (R3 < t))t-R3;                                                     
  if  ((R4 >0) &&  (R4 < t))t-R4;                                                     
  if  (t--40000)                                                                      
    return (-2.0); 
  return (t);                                                                         
                                                                                      
                                                                                      
float checkifinshadow(x,y,z)                                                          
float x,y,z;
                                                                                      
  /* This function will return a number from 0 to 1, indicating the 
  /* brightness of the point x,y,z, according to which lights it is                   
  /* being lit by, and its distance to those lights.                                  
                                                                                      
  int i,j,intersect;                                                                  
  float A,B,C,t,tt,rr;                                                                
  struct RAY ray;                                                                     
                                                                                      
  tt-0.0;                                                                             
  for (j-100; j< (100+NUMLIGHTS) ; j++)                                               
                                                                                      
    ray.X1-bobject[j].x-x;                                                            
    ray.Y1-bobject[j].y-y; 
    ray.Z1-bobject[j].z-z;                                                            
    rr-1.0/sqrt(ray.Xlaray.Xl+ray.Ylaray.Y1+ray.Zlaray.Z1);                           
    ray.X2..x+ray.Xlarr;                                                              
    ray.Y2..y+ray.Ylarr;                                                              
    ray.12-z+ray.E1arr;                                                               
    intersect-FALSE;
    for (1-0;i<NUMBOBJECTS;i++) 
                                                                                      
      t..getintersection(i,ray,FALSE);                                                
      if ((t > 0)“(t < 1)) intersect-TRUE;                                            

    if ((intersect)                                                                   

      rr*-rr;   /* Square rr to get square of distance */                             
      rra..1.0*bobject[j].reflectabobject[j].reflect; 
      if (rr > 1.0) rr-1.0;                                                           
      tt+-rr;                                                                         

  1
  tt/..NUMLIGHTS;
  tt..1.0*AMBIANT/255+tt* (255.0-AMBIANT) /255.0;                                     
  return(tt);                                                                         


getreflection (A1,B1,C1,A2,132,C2,A3,B3,C3)                                           
float A1,81,C1,A2,B2,C2,*A3,*B3,*C3;                                                  
  /* Given incoming vector Al,B1,C1 and normal vector A2,B2,C2, it will */            
    /* return reflected vector A3,B3,C3 */ 
    float t;
                                                                            
    t..1.0/sqrt(A1*Al+Bl*B1+C1*C1);
    Al*-t; B1*-t; C1*..t; 
    t...1.0/sqrt(A2*A2+B2*B2+C2*C2);
    A2*-t; B2*-t; C2*-t;
    t-2.0*(A1*A2+Bl*B2+C1*C2);
    *A3-Al-t*A2; 
    *B3....B1-t*B2; 
    *C3-C1-t*C2;
                                                                            
                                                                            
  getrefraction (I1,12,I3,N1,N2,N3,R1,R2,R3,inglass) 
  float 11,12,13,N1,N2,N3,*R1,*R2,*R3;
  int *inglass;
                                                                            
    /* Given incoming vector 11,12,13, and normal vector N1,N2,N3, it will 
    /* return the refracted vector R1,R2,R3. If singlass• is false, then 
    /* it will refract according to the ratio from air to glass, otherwise 
    /* it will refract according to the ratio from glass to air.
                                                                            
    float t,a,th,n1-1.0,n21.52,m,o;
                                                                            
    t..1.0/sqrt(sqr(II)+sqr(I2)+sqr(I3)); 
    I1*-t; I2*-t; I3*-t;
    t...1.0/sqrt(sqr(N1)+sqr(N2)+sqr(N3)); 
    if Winglass) t- -t;
    N1*-t; N2*-t; N3*-t;
    th-acos(Il*N1412*N2+I3*N3);
    if (ainglass)
      m-sin(th)an2/n1;
    else
      m-sin(th)anl/n2;
    a■asin(m);
                                                                            
    *inglass- lainglass;
    o..sin(th-a); 
    *R1-Ilam+o*N1; 
    *R2-I2*m+o*N2; 
    *R3-13*m+o*N3;
                                                                            
                                                                            
  float getpower(x,y) 
  float x;
  int y;
                                                                            
    /* Returns x^y */
                                                                            
    float r;
                                                                            
    r - 1.0; 
    do
                                                                            
                                                                            
                                                                            
    while (--y > 0); 
    return (r);
                                                                            
                                                                            
  float sqr(x)
  float x;
  1 /* This seems simple enough */
                                                                                          
   return (x*x);                                                                          


antialias()                                                                               
                                                                                          
   int y;                                                                                 
   RGBvalue red[1024],green[1024],blue[1024],downred[1024],downgreen[1024],downblue[1024)
                                                                                          

fifdef DOGRAPH                                                                            
   ringbell();
   for (y-ystart+l;y<-yend;y++)                                                           
                                                                                          
     cmov21(0,y);                                                                         
     readRGB (1024,red,green,blue);                                                       
     cmov21(0,y-1);                                                                       
     readRGB (1024,downred,downgreen,downblue);                                           
     antialiasrow(red,green,blue,downred,downgreen,downblue,y);
     cmov21(0,y-1);
     writeRGB (1024,downred,downgreen,downblue);                                          

fendif                                                                                    

                                                                                          
antialiasrow(red,green,blue,downred,downgreen,downblue,y)                                 
RGBvalue red[],green[],bluel],downred[],downgreen[],downblue[];                           
int y;
                                                                                          
  /* Will anti-alias a row of graphics by checking to see if the neighboring */           
  /* pixels are different. If they are, then the current pixel is assumed */              
  /* to be on a border, and is broken down into 16 sub-pixels and the colors */           
  /* are averaged. This creates an illusion and edges look smoother. */

  int x,R,G,B,r,g,b;                                                                      
  float xx,yy,X,Y,width,height;
  RGBvalue newred[1024],newgreen[1024],newblue[1024];                                     
                                                                                          
  y--;                                                                                    
  width-WINDOWX2-WINDOWX1; 
  height-WINDOWY2-WINDOWY1;                                                               
  memcpy (newred, downred, 1024);                                                         
  memcpy (newgreen,downgreen,1024);                                                       
  memcpy inewblue,downblue,1024); 
  for (x-xstart;x<xend;x++)                                                               
     if (diff(red,downred,x) II diff(green,downgreen,x) II diff(blue,downblue,x))         
     [                                                                                    
      r-g-b-0;                                                                            
      for (xx-0.0;xx<1.0;xx+-0.25)                                                        
                                                                                          
        X - WINDOWX1+(width*(x+xx)/GETMAXX);                                              
        for (yy..0.0;yy<1.0;yy+-0.25)
                                                                                          
          Y   WINDOWY1+(height*(y+yy)/GETMAXY);                                           
          getpixelcolor(X,Y,aR,&G,&B);                                                    
          r+-R; g+-G; b+-B;                                                               
                                                                                          
                                                                                          
      r/-16; g/-16; b/-16;                                                                
      newred[x]-r;
      newgreen[x]-g;                                                                      
      newblue[x]-b;                                                                       
                                                                                          
  memcpy(downred,newred,1024);                                                            
   memcpy(downgreen,newgreen,1024); 
   memcpy(downblue,newblue,1024);
                                                                          
                                                                          
 int diff(c,downc,x) 
 RGBvalue c[],downc[]; 
 int x;
                                                                          
   /* Checks to see difference among four pixels */ 
                                                                          
   int d-10;
                                                                          
   if ((abs(c[xl-downc[x]) > d) II (abs(c[x]-c[x+11) > d) II 
       (abs(c[x+1]-downc[x+1]) > d) 31 (abs(downc[x]-downc[x+1)) > d) H
       (abs(c[xl-downc[x+1)) > d) II (abs(downc[xl-c[x+1]) > do)
     return(TRUE);
   else
     return(FALSE);
                                                                          
                                                                          
 loadfile() 
                                                                          
   char a[130);
                                                                          
   clrscr();
   system ("ls -1 *.SCR");
   printf ("\n\nName of file to load: ");
                                                                          
   gets(str);
   if (str--"") return;
   strcpy (a,"disprgb "); 
   system (strcat (a,str));
                                                                          
                                                                          
 modifyworld()
                                                                          
   /* This function allows the user to modify the current world. It is 
   /* real messy cause I wrote it two days ago. Just assume it works and 
   /* skip it, trust me. */
                                                                          
   int i,j,val,done,xx,yy,f,HANDINPUT,MGID; 
   short d;
   char ch,s[130];
                                                                          
 •ifdef IRIS4D
   prefposition (0,1023,0,767); 
   foreground();
   MGID-winopen ("Ray Tracer"); 
 felse
   ginit();
 lendif
                                                                          
   tpoff();
   qdevice (LEFTMOUSE); 
   qdevice (RIGHTMOUSE); 
   greset();
   done-FALSE;
   HANDINPUT-FALSE;
   do
                                                                          
     color (BLACK); 
     clear();
     color (WHITE); 
     cmov2i(20,750);
            charstr ("Current World: ") ;                                                                                                                                                                                                                   
            if (NUMBOBJECTS-0)                                                                                                                                                                                                                              
            {                                                                                                                                                                                                                                               
                  cmov21(20,730);                                                                                                                                                                                                                           
                  charstr ("No objects defined.");
                                                                                                                                                                                                                                                            
            else                                                                                                                                                                                                                                            
            {                                                                                                                                                                                                                                               
                  cmov2i (20, 730) ; 
                  charstr (" # Object                                                            X             I             Z R Red Grn Blu Reflect                                                         A              B            C To               
p Bttm" ) ;
                  cmov2i (20, 710) ; 
                  charstr (" 
                                                                                                                                                                                                                                                            
                  for (i-0; i<NUMBOBJECTS; i++)                                                                                                                                                                                                             
                                                                                                                                                                                                                                                            
                      cmov2i (20,680-16*i) ;                                                                                                                                                                                                                
                      sprint f (s, "%2d it-12sic4dt5d%5d#,4d%40i4d#14d %-8s1k4d1i5d1i5d45d%5d", is OBJECTNAME [bob                                                                                                                                          
ject[i].objecttype],(int)bobject[i].x,(int)bobject[iJ.y,(int)bobject[1].z,(int)bobject[i 
) . r, ( int) bobject [I ) . Red, ( int ) bobject [ ) .Green, ( int) bobject [1] . Blue, SURFACENAME [bobject                                                                                                                                               
    . reflect ] , (int) bobject [i ) . A, (int ) bobject [1 ] .B, (int ) bobject [i].C, (int)bobject [1] .top, (in
t) bobject [ . bottom) ;                                                                                                                                                                                                                                    
                    charstr (s) ;                                                                                                                                                                                                                           
                                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                                            
            cmov2i (20, 60);                                                                                                                                                                                                                                
            charstr (" Click on action:                                                             <ADD OBJECT> <DELETE OBJECT> <HAND-INPUT> <QUIT>m)                                                                                                      
                                                                                                                                                                                                                                                            
            curson () ;                                                                                                                                                                                                                                     
            do                                                                                                                                                                                                                                              
                                                                                                                                                                                                                                                            
                  val..ciread (&d) ;                                                                                                                                                                                                                        
            }                                                                                                                                                                                                                                               
            while (d=^0)                                                                                                                                                                                                                                    
            cursoff () ;                                                                                                                                                                                                                                    
            xx-getvaluator (MOUSEX) ;                                                                                                                                                                                                                       
            yy-getvaluator (MOUSEY) ;                                                                                                                                                                                                                       
            if (yy < 100)                                                                                                                                                                                                                                   
                                                                                                                                                                                                                                                            
                  if ( (xx > 215) && (xx < 311) )                                                                                                                                                                                                           
                                                                                                                                                                                                                                                            
                      NUMBOBJECTS++;                                                                                                                                                                                                                        
                      i-NUMBOBJECTS-1;
                      bobject [1].x..bobject [1] .y-bobject [i ) • z-bobject [1] .r-bobject (I) .Red-bobject [1] .Gr                                                                                                                                        
eenbobject [i ) . Blue-bobject [ 1] .A-bobject 11 .B-bobject [ . C-bobject [ 1] .top-bobject [I ] .bot                                                                                                                                                      
tom-0;
                      bobject [ ] . objecttype-SPHEREOBJECT;                                                                                                                                                                                                
                      bobject [i ] . reflect-SHINE;                                                                                                                                                                                                         
                      MODIFIED-TRUE;                                                                                                                                                                                                                        
                  }
                  if ( (xx > 338) 64 (xx < 458) )

                      cmov2i (20,100) ;
                      charstr ("Click on the object to delete, or <Right Button> to quit.") ;                                                                                                                                                               
                      curson ( ) ;                                                                                                                                                                                                                          
                      do                                                                                                                                                                                                                                    
                                                                                                                                                                                                                                                            
                           val-gread (&d) ;                                                                                                                                                                                                                 
                                                                                                                                                                                                                                                            
                      while (d--0) ;                                                                                                                                                                                                                        
                      cursoff () ;                                                                                                                                                                                                                          
                      if (val                         LEFTMOUSE)                                                                                                                                                                                            
                                                                                                                                                                                                                                                            
                           yy-getvaluator (MOUSEY) ;                                                                                                                                                                                                        
                              if (yy < 696)
                              {
                                    i-(696-yy) /16;
                                    if ( (i >- 0) && (i < NUMBOBJECTS) )
                                                                                                          
                                          NUMBOBJECTS--;
                                          for (j-i;j<NUMBOBJECTS;j++) 
                                               bobject[j]-bobject(j+1);
                                                                                                          
                                    MODIFIED-TRUE;
                                                                                                          
                                                                                                          
                                                                                                          
                    if ((xx > 491) && (xx < 590))
                         HANDINPUT-TRUE;
                    if ((xx > 612) && (xx < 662)) done-TRUE;
              }
              else
                                                                                                          
                    if       (yy < 696)
                                                                                                          
                         if (val--RIGHTMOUSE)                                      HANDINPUT-TRUE;
                                (696-yy) /16;
                         f-0;
                         if       (xx <           726)          f-14;
                         if       (xx <           680)          f-13;
                         if       (xx <           637)          f-12;
                         if       (xx <           591)          f-11;
                         if       (xx <           548)          f-10;
                         if       (xx <           502)          f-9;
                         if       (xx <           429)          f-8;
                         if       (xx <           392)          f-7;
                         if       (xx <           356)          f-6;
                         if       (xx <           320)          f-5;
                         if       (xx <           286)          f-4;
                         if       (xx <           236)          f-3;
                         if       (xx <           194)          f-2;
                         if       (xx <           145)          f-1;
                         if       (xx <           47)        f-0;
                         if       ( (f      >     0) && (i >- 0)                   && (i < NUMBOBJECTS) )
                         {
                              if (HANDINPUT)
                                                                                                          
                                    handinputfield (i,f); 
                                    HANDINPUT-FALSE;
                                                                                                          
                              else
                                  editfield (i, f) ; 
                              MODIFIED-TRUE;
                                                                                                          
                                                                                                          
                                                                                                          
                                                                                                          
        while ( !done) ; 
        color (BLACK) ; 
        clear () ;
        greset () ;
        tpon () ;
   fifdef IRIS4D
        winclose (MGID) ; 
   (else
        gexit () ;
   fendif
                                                            
edit field (1, f)                                           
int 1, f;                                                   
                                                            
  int xx,yy,oldyy,orgyy,val,v1,ft,orgi,dy;                  
  short d;                                                  
  float vf,orgf;
  char s[100],ss[100);                                      
                                                            
 switch (f)
  {                                                         
   case 1: ft-1; vi-bobject[1].objecttype; xx-3; break; 
   case 2: ft-2; vf-bobject[i).x; xx-15; break;             
   case 3: ft-2; vf-bobject[i).y; xx-20; break;             
   Case 4: ft-2; vf-bobject[i].z; xx-25; break; 
   case 5: ft-2; vf-bobject(i].r; xx-29; break;             
   case 6: ft-1; vi-bobject[1].Red; xx-33; break;
   case 7: ft-1; vi-bobject(i].Green; xx-37; break;         
   case 8: ft-1; vi-bobject[i].Blue; xx-41; break;          
   case 9: ft-1; vi-bobject[1].reflect; xx-46; break; 
   case 10: ft-2; vf-bobject[1].A; xx-54; break;            
   case 11: ft-2; vf-bobject[1].B; xx-59; break; 
   case 12: ft -2; vf-bobject[1].C; xx-64; break;           
   case 13: ft-2; vf-bobject(i].top; xx-69; break;          
   case 14: ft-2; vf..bobject[1].bottom; xx-74; break;

                                                            
 oldyy-getvaluator(MOUSEY)                                  
 if (ft--2) vi-vf;                                          
 orgi-vi;                                                   
 orgf-vf;                                                   
 dy-0;                                                      
 cursoff () ;                                               
 do                                                         
                                                            
   switch (f)

                                                            
    case 1: sprintf (s,"4-12s",OBJECTNAME[vi)); break;      
    case 9: sprintf (s,"4-8s",SURFACENAME(vil); break;      
    default: sprintf (s,"%4d",vi); break;                   
                                                            
   cmov2i (20+9*xx,680-16*1):                               
   color (YELLOW);                                          
   charstr (s);                                             
   if (ft--2)                                               
   {                                                        
    sprintf (ss,"Real value: t.lf",vf);                     
    cmov2l (20,100);                                        
    charstr (ss);                                           
                                                            
  curson();                                                 
  do
   {
    yy-getvaluator(MOUSEY);                                 
                                                            
  while ((oldyy--yy)&&(qtest()--0));                        
  cursoff();                                                
  if (oldyy !- yy)                                          
                                                            
    cmov2i (20+9*xx,680-16*1);                              
    color (BLACK);
    charstr (s);                                            
    if (ft--2)                                              


     cmov2i (20,100);                                       
       charstr (ss);
     1
     dy+-yy-oldyy;
     vi-orgi+dy/10;
     if (ft--2) vf-orgf+dy/10.0; 
     if (f-1)
                                                                      
       if (vi > 7) vI%-8;
       if (vi < 0) vi- 7-(-(v1+1)%8);
                                                                      
     if (f--9)
                                                                      
       if (vi > 3) vi%-4;
       if (vi < 0) vi- 3-(-(v1+1)%4);
                                                                      
     if ((f >- 6) && (f <- 8))
                                                                      
       if (vi > 255) vi%-8;
       if (vi < 0) vi- 255-(-0/1+1)4256);
                                                                      
     if (f--5)
                                                                      
       if (vi < 0) vi-0; 
       if (vf < 0) of-0;
                                                                      
                                                                      
     if (yy < 100) ( yy-760; setvaluator (MOUSEY,760,0,767); 
     if (yy > 760) ( yy-100; setvaluator (MOUSEY,100,0,767); 
     oldyy-yy;
     MODIFIED-TRUE;
    )
  1
  while (qtest()--0); 
  cursoff () ;
  switch (f)
                                                                      
                                                                      
    case 1: bobject[i].objecttype-vi; break;
    case 2: bobject[1].x-vf; break; 
    case 3: bobject[i].y-vf; break; 
    case 4: bobject[i].z-vf; break; 
    case 5: bobject[1].r-vf; break; 
    case 6: bobject[i].Red-vi; break; 
    case 7: bobject[1].Green..vi; break; 
    case 8: bobject[1].Blue-vi; break;
    case 9: bobject[i].reflect-vi; break; 
    case 10: bobject(i].A-vf; break;
    case 11: bobject[1].B..vf; break; 
    case 12: bobject[I).0-vf; break; 
    case 13: bobject[i].top-vf; break;
    case 14: bobject[1].bottom-vf; break;
  )
                                                                      
                                                                      
 handinputfield (1,f) 
 int 1,f;
 {
  char s[100],c;
  short d;
  int t,x,val;
  float ff;
                                                                      
  gdevice (KEYBD); 
  switch (f)
                                                                      
                                                                      
    case 2: sprintf (s,"Current value of X: %f",bobject[1].x); break;
     case 3: sprintf (s,"Current value of Y: %f",bobject[i].y); break;             
     case 4: sprintf (s,"Current value of 2: %f",bobject[i].z); break;             
     case 5: sprintf (s,"Current value of R: %f",bobject[i].r); break;             
     case 10: sprintf (s,"Current value of A: %f",bobject[I].A); break;            
     case 11: sprintf (s,"Current value of B: %f",bobject[1].B); break;            
     case 12: sprintf (s,"Current value of C: %f",bobject[i].C); break;            
     case 13: sprintf (s,"Current value of top: %f",bobject[i].top); break;
     case 14: sprintf (s,"Current value of bottom: %f",bobject[i].bottom); break; 
     default: return; /* No point in hand-entering an integer */                   
                                                                                   
   color {YELLOW); 
   cmov2i (20,120);                                                                
   charstr (s);                                                                    
   cmov2i (20,100);
   charstr ("Enter new value, or simply <ENTER> to keep as it is: ");              
   t-0;
   do                                                                              
                                                                                   
     val-gread(sd);                                                                
     if (val    KEYBD)
     {                                                                             
       c-d;                                                                        
       if {((c   '0')Wc <-                                                         
       {                                                                           
        s[t++]..c;
        s[t)-0;                                                                    
        cmov2i(53*9+20,100);                                                       
        charstr(s);                                                                
                                                                                   
       if ((c   8)&s(t > 0))                                                       
                                                                                   
        color (BLACK);                                                             
        cmov21(53.9+20,100); 
        charstr(s);                                                                
        s[--t]-0;                                                                  
        color (YELLOW);                                                            
        cmov21(53*9+20,100);                                                       
        charstr(s);                                                                

     1
                                                                                   
  while (d !- 13); 
  if (t > 0)                                                                       
                                                                                   
     ffatof(s); 
     switch (f)                                                                    
                                                                                   
      case 2: bobject[i].x-ff; break;                                              
      case 3: bobject[i].y-ff; break;                                              
      case 4: bobject[i].z-ff; break;                                              
      case 5: bobject[i].r-ff; break; 
      case 10: bobject(1].A-ff; break;                                             
      case 11: bobject[1].B-ff; break;                                             
      case 12: bobject[i].C-ff; break;                                             
      case 13: bobject[i].top-ff; break;
      case 14: bobject[i].bottom-ff; break;                                        
                                                                                   
                                                                                   
  uncidevice (KEYBD);                                                              


                                                                                   
loadworld()                                                                        
                                                                                   
  clrscr();                                                                        
   system ("ls *.world");
   printf ("\nName of the file from which to load a world: ");
   gets(str);
   if (str[0]--0) return; 
   loadthisworld(str); 
   MODIFIED-FALSE;
                                                                                             
                                                                                             
 loadthisworld(str) 
 char *str;
                                                                                             
   int igrr,gg,bb; 
   FILE *f;
                                                                                             
   if ((f-fopen(str,"r"))--NULL)
                                                                                             
     printf ("File '%s' not found. Strike <RETURN> to continue.",str); 
     gets(str);
     return;
                                                                                             
   fscanf (f,"%d %d\n",&NUMBOBJECTS,&WAXED);
   fscanf (f,"%f %f %f %f %f kf %d %d 4f %f 4f %f\n",&EYEX,&EYEY,&EYEZ,4CENTERX,SCENTERY,i 
 CENTERZ,LSURFACE,&AMBIANT,iWINDOWX1,&WINDOWY1,LWINDOWX2,4WINDOWY2);
   for (1-0;i<NUMBOBJECTS;i++)
                                                                                             
     fscanf (f,"%f %f %f 4f %d %d 4d %d ird %f %f %f kf %f %f 4f 4f\n",0object[1].x,Ebobje 
 ct[i].y,‘bobject[i].z,ibobject[i].r,irr,‘gg,ibb,ibobject[i].reflect,cbobject[i].objecttyp 
 e,sbobject[i].A,&bobject[i].B,‘bobject[i].C,fibobject[i].top,ibobject[i].bottom,ibobject[i 
 I.ax,6bobject[i].ay,&bobject(i).az);
     bobject[i].Red-rr;
     bobject[i].Green-gg;
     bobject(i).Blue-bb;
                                                                                             
   fscanf (f,"%d\n",6NUMLIGHTS);
   for (1-100;1<(100+NUMLIGHTS);i++)
     fscanf (f,"%f %f tf tf 4d\n",thobject[1].x,6.bobject[i].y,ibobject[i].z,ibobject[i].r, 
 Gbobject[i].reflect);
   fclose(f);
                                                                                             
                                                                                             
 saveworld()
                                                                                             
   FILE *f; 
   int 1;
                                                                                             
   clrscr();
   printf ("Name of the file to which to save this world: "); 
   gets(str);
   if (str[0]....0) return;
   if ((f-fopen(str,"w"))--NULL)
                                                                                             
     printf ("Unable to open file for output  . Strike <RETURN> to continue.");
     gets(str);
     ret urn;
                                                                                             
   fprintf (f,"%d %d\n",NUMBOBJECTS,WAXED);
   fprintf (f,"%f %f %f %f %f %f %d id %f 4f %f %f\n",EYEX,EYEY,EYEZ,CENTERX,CENTERY,CENTE 
 RZ,SURFACE,AMBIANT,WINDOWX1,WINDOWY1,WINDOWX2,WINDOWY2);
   for (1,-D;i<NUMBOBJECTS;1++)
                                                                                             
                                                                                             
     fprintf (f,"%f if tf %f %d %d %d %d id %f tf kf %f %f %f %f %f\n",bobject[i).x,bobjec 
 t[i].y,bobject[i].z,bobject[i].r,(int)bobject[i].Red,(int)bobject[i].Green,(int)bobject[i 
 ].Blue,(int)bobject[i].reflect,bobject[i].objecttype,bobject[1].Agbobject[i].B,bobject[i] 
 .C,bobject[i].top,bobject[i].bottom,bobject[i].ax,bobject[i].ay,bobject[i].az);

                                                                                                              
    fprintf (f,"%d\n",NUMLIG}(TS);                                                                            
    for (1-100;1<(100+NUMLIGHTS);i++)                                                                         
       fprintf (f,"%f 4f if 4f 4d\n",bobject(i).x,bobject[i].y,bobject[i].z,bobjectli].r,bo                   
bject[i].reflect);                                                                                            
    fclose(f);                                                                                                
    MODIFIED-FALSE;                                                                                           
                                                                                                              
                                                                                                              
clrscr()                                                                                                      
                                                                                                              
    /* Is there a built-in command to do this? */                                                             
                                                                                                              
    system ("clear");                                                                                         
                                                                                                              
                                                                                                              
getmandelbrot(p,q,R,G,B)                                                                                      
float p,q;                                                                                                    
int *R,*G,*B;                                                                                                 
                                                                                                              
    /* Returns the color of the Mandelbrot set at point p,q */                                                
                                                                                                              
         float boundary,x,y,oldx; 
         int count;

         count-0;
         boundary-0;
         x..0; 
         y-0; 
         do

           oldx-x;
           x-x*x-y*y+p;
           y-oldx*y+oldx*y+g; 
           boundary=x*x+y*y; 
           count++;

         while ((boundary <- 4) 64 (count <- MAXCOUNT)); 
         if (count        MAXCOUNT)

           .R-255;
           *G-{count*20)4256; 
           * B-0;

         else

                 *G- *B-0;



changeparameters()

  /* Lets the user change the viewpoint, centerpoint, ambient light and */ 
  /* surface */

  clrscr();
  printf ("Current eye:               <4d,%dd4d>\n",(int)EYEX,(int)EYEY,(int)EYEZ);
  printf ("Current center: <4d,fed,%d>\n",(int)CENTERX,(int)CENTERY,(int)CENTERZ);
  printf ("\nEnter new coordinates, or ENTER to keep:\n\n");
  printf ("EYEX: "); 
  gets (str);
  if (str[0]1-0) EYEX-atoi(str);
  printf ("EYEY: "); 
  gets (str);                                                                                                 
if (str(0)1-0) EYEY-atoi(str);
printf ("EYEZ: ");
gets (str);
if (str[0]I-0) EYEZ-atoi{str);
printf ("CENTERX: ");
gets (str);
if (str[0]I-0) CENTERX-atoi(str);
printf ("CENTERY: ");
gets (str);
if (str[0]I-0) CENTERY-atoi(str);
printf ("CENTERZ: ");
gets (str);
if (str[0]1-0) CENTERZ-atoi(str);
printf ("\nSurface currently td:                0-Checkerboard, l-Mandelbrot, 2-Water\n",SURFACE);
printf ("SURFACE: ");
gets (str);
if (str[0]I-0) SURFACE-atoi(str);
printf ("\nAmbiant light currently id out of 255.\n",AMBIANT);
printf ("AMBIANT: ");
gets {str);
if (str[0]I-0) AMBIANT-atoi(str);
MODIFIED-TRUE;