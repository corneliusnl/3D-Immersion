/******************************************************************//*  Program: TERRAIN                                              */
/*  Author : Paul Tyma                                            */
/*  Date   : February 1995                                        */
/******************************************************************/

// some constants#define RAD(z) ((float)(z))*.0174
#define SHEIGHT 200
#define SWIDTH  320
#define DI 110

// include#include <windows.h>
#include <windowsx.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <wing.h>
#include "terrain.rh"

// structure of 3D points, i.e. triangle verticesstruct pt3d {int x,y,z; } point3d[25][25];

// global variablesHBITMAP offBitmap = 0;
HDC hDC,offhDC;
HMENU hMenu;
int GRIDSIZE,s1=0,s=0,d1=0;
int hide=0,movetype=0,filltype=0;

// function prototypes
HDC CreateWinGDC(void);
void DestroyWinGDC(HDC hWinGDC);
void initfield(int GRIDSIZE);
void DoTerrain (void);
void flyby(void);
void fluid(void);
void living(void);
float dotproduct(struct pt3d A, struct pt3d B, struct pt3d C,
								int respectx, int respecty,int respectz);
long FAR PASCAL WndProc(HWND hWnd, WORD msg,WORD wParam, LONG lParam);

int PASCAL WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
									 LPSTR lpszCmdParam, int nCmdShow)
{
	MSG msg;
	HWND hWnd;
	WNDCLASS WndClass;

	//randomize();

// define a window class	if(!hPrevInstance)
	{
		WndClass.cbClsExtra = 0;
		WndClass.cbWndExtra = 0;
		WndClass.hbrBackground = GetStockObject(WHITE_BRUSH);
		WndClass.hCursor = NULL;
		WndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(TERRICON));
		WndClass.hInstance = hInstance;
		WndClass.lpfnWndProc = WndProc;
		WndClass.lpszClassName = "TerrClass";
		WndClass.lpszMenuName = MAKEINTRESOURCE(TERR_MENU);
		WndClass.style = CS_VREDRAW | CS_HREDRAW;

		if(!RegisterClass(&WndClass))
			return FALSE;
	}
	else
		return FALSE;

	hWnd = CreateWindow ("TerrClass",
				"Black Art Terrain",
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT,CW_USEDEFAULT,
				SWIDTH,SHEIGHT+20,
				(HWND)NULL, NULL, hInstance, (LPSTR)NULL);

	if(!hWnd)
		return (FALSE);

	ShowWindow (hWnd, SW_SHOWNORMAL);
	hDC = GetDC(hWnd);
	hMenu = GetMenu(hWnd);

	// setup WinG DC
	offhDC = CreateWinGDC();
	initfield(15);

	// enter msg loop
	while (1)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
				return msg.wParam;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			DoTerrain();
		}
  }
}

// DOTERRAIN FUNCTION// Handle all off screen operations then Blit
void DoTerrain ()
{
	int g,h;
	POINT tri[4];

	// clean offscreen buffer	Rectangle(offhDC,0,0,SWIDTH,SHEIGHT);

	 // call appropriate mesh manipulator	switch(movetype)
	{
		case 0:
			flyby();
			break;

		case 1:
			fluid();
			break;

		case 2:
			living();
			break;
	}
	// loop through each triangle and draw if needed
	for (h=GRIDSIZE-2; h>=0; --h)
	{
		for (g=0; g<GRIDSIZE-1; ++g)
		{
			// populate tri array to use polygon command
			tri[0].x = DI*point3d[g][h].x /  point3d[g][h].z + SWIDTH/2;
			tri[0].y = DI*point3d[g][h].y /  point3d[g][h].z + SHEIGHT/2 ;
			tri[2].x = DI*point3d[g+1][h+1].x / point3d[g+1][h+1].z + SWIDTH/2;
			tri[2].y = DI*point3d[g+1][h+1].y / point3d[g+1][h+1].z + SHEIGHT/2 ;
			tri[3].x = tri[0].x;
			tri[3].y = tri[0].y;

			// if hidden surface removal doesnt argue, draw the triangle
			if(!hide || (dotproduct(point3d[g][h],point3d[g+1][h+1],point3d[g+1][h],
						0,0,0) < 0.0))
			{
				tri[1].x = DI*point3d[g+1][h].x /  point3d[g+1][h].z + SWIDTH/2;
				tri[1].y = DI*point3d[g+1][h].y /  point3d[g+1][h].z + SHEIGHT/2 ;

				// wireframe or filled?
				if(filltype)
					Polygon(offhDC,tri,4);
				else
					Polyline(offhDC,tri,4);
			}

			// do opposing triangle with same corner pt
			if (!hide || (dotproduct(point3d[g][h],point3d[g][h+1],point3d[g+1][h+1],
						0,0,0) < 0.0))
			{
				tri[1].x = DI*point3d[g][h+1].x / point3d[g][h+1].z + SWIDTH/2;
				tri[1].y = DI*point3d[g][h+1].y / point3d[g][h+1].z + SHEIGHT/2;

				// wireframe or filled
				if(filltype && (h < GRIDSIZE - 4))
					Polygon(offhDC,tri,4);
				else
					Polyline(offhDC,tri,4);
			}
		}
	}

	 // Offscreen buffer done! Blit it!
	 WinGBitBlt (hDC, 0, 0, SWIDTH,SHEIGHT, offhDC, 0, 0);
}

// FLYBY// "scroll" all points down one then set make new row of terrain
void flyby()
{
	int g,h;

	// scroll	for(g=0; g<GRIDSIZE; ++g)
		for(h=0; h<GRIDSIZE-1; ++h)
		  point3d[g][h].y = point3d[g][h+1].y + 3;

	// "pseudo" random new terrain - allows for flowing valleys
	for (h=0;h<GRIDSIZE;++h)
		// bigger this number the less mountains are produced - i.e. %10
	{
		if (!(rand()%10))
			point3d[h][GRIDSIZE-1].y = (-GRIDSIZE*2)-(rand()%12) - (!(rand()%5)*(rand()%20));
		else
			point3d[h][GRIDSIZE-1].y = (point3d[abs(h-1)][GRIDSIZE-1].y +
												 point3d[(h+1)%GRIDSIZE][GRIDSIZE-1].y +
												 (point3d[h][GRIDSIZE-2].y-3) ) /3;
	}
}

// Code to produce "fluid" motion// i.e. colliding sine waves
void fluid()
{
	int g,h,d;
// s controls one sine wave, d the other
// start s at an external counter value (X sine wave position)
	 s = s1;

	 s1 += GRIDSIZE;
// loop through all points	 for (g=0;g<GRIDSIZE;++g)
	 { d = d1++;
		for (h=0;h<GRIDSIZE;++h)
				 { // start point3d at standard place
					 point3d[g][h].y = (GRIDSIZE-h*3);

					 // add in wave offsets, 6 defines wave amplitudes					 point3d[g][h].y +=  6*sin(RAD((s%360)));
					 point3d[g][h].y +=  6*sin(RAD((d%360)));

					 // changing these values will change wave frequency
					 s+=1;

					 d+=GRIDSIZE;				 }
			}
	 }

	// Living earth - looks like a puppy under a rug
//void living()
{  int g,h;
	 float gs,hs;

	 // s1 create random motion
	 s1 += rand()%3-1;
	 d1 += rand()%3-1;
	 // govern s1 & d1
	 if (s1<0) s1=0;
	 if (s1>GRIDSIZE-1) s1=GRIDSIZE-1;

	 if (d1<0) d1=0;
	 if (d1>GRIDSIZE-1) s1=GRIDSIZE-1;

	 // loop through gridpoints
	 for (g=0;g<GRIDSIZE;++g)
	  for (h=0;h<GRIDSIZE;++h)
		  {
			 // follow sine wave
			 gs = sin(RAD(((g+s1)*180/GRIDSIZE)+(rand()%10)-5));
			 hs = sin(RAD(((h+d1)*180/GRIDSIZE)+(rand()%10)-5));
			 gs = gs*hs;
          point3d[g][h].y = GRIDSIZE-h*3;
          // only allow the tip of a hi frequency sine wave to show
          if (gs>0.9)  point3d[g][h].y -= 150*(gs-.9);
         }

 }

// setup initial parameters for fields of varying granularityvoid initfield(int s)
{ int g,h,j,k;


  // set GRIDSIZE variable so all functions know of the change  GRIDSIZE = s;

  // pick the new granularity  switch (GRIDSIZE)
  { case 10 : j = 15;
						k = 60;
						break;
	  case 15 : j = 10;
						k = 75;
						break;
	  case 20:  j = 7;
						k = 75;
						break;
	  case 25 : j = 6;
						k = 73;
						break;
	  }

	 // init the field	 for (g=0;g<GRIDSIZE;++g)
	  for (h=0;h<GRIDSIZE;++h)
		{ point3d[g][h].x = (g * j - k);
			point3d[g][h].y = GRIDSIZE-h*3;
			point3d[g][h].z = h *3 + GRIDSIZE + 5;

			}	 }

// DOTPRODUCT fctn (a bit beyond)
// Calculate the angle of incidence
float dotproduct(struct pt3d A, struct pt3d B, struct pt3d C,
								int respectx, int respecty,int respectz)
{  // pt3d structure is int x,y,z;
	 struct pt3d normal,midpt,vect;
	 float dotp;
	 int j,k;

	 // calculate normal for plane (triangle)
	 normal.x = (B.y - A.y) * (A.z - C.z) - (B.z - A.z) * (A.y - C.y);
	 normal.y = (B.z - A.z) * (A.x - C.x) - (B.x - A.x) * (A.z - C.z);
	 normal.z = (B.x - A.x) * (A.y - C.y) - (B.y - A.y) * (A.x - C.x);

	 // calculate a representative point in the triangle	 midpt.x = (A.x + B.x + C.x)/3;
	 midpt.y = (A.y + B.y + C.y)/3;
	 midpt.z = (A.z + B.z + C.z)/3;

	 // calculate the vector from the viewpoint to midpoint
	 vect.x = midpt.x - respectx;
	 vect.y = midpt.y - respecty;
	 vect.z = midpt.z - respectz;

	// get vector lengths
	 j = abs(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);
	 k = abs(vect.x*vect.x + vect.y*vect.y + vect.z*vect.z);
	 j = sqrt(j);
	 k = sqrt(k);

	 // dot product
	 dotp = (normal.x * vect.x) + (normal.y * vect.y) + (normal.z * vect.z);
	 dotp /= j;
	 dotp /= k;

	 return dotp;
}

////////////////////////////////////////////////////////////////////////////long FAR PASCAL WndProc(HWND hWnd, WORD msg, WORD wParam,LONG lParam)
{
	int g;

	switch(msg)
	{
		case WM_DESTROY:
			DestroyWinGDC(offhDC);
			PostQuitMessage(0);
			return 0;

	case WM_COMMAND:
		switch(GET_WM_COMMAND_ID(wParam, lParam))
		{
			case CM_FILEEXIT:
				DestroyWindow(hWnd);
				return 0;

		  // handle new movement
			case CM_MOVERAND:				CheckMenuItem(hMenu,CM_MOVERAND+movetype,MF_UNCHECKED);				movetype = 0;
				CheckMenuItem(hMenu,CM_MOVERAND,MF_CHECKED);
				return 0;

			case CM_MOVESINE:				CheckMenuItem(hMenu,CM_MOVERAND+movetype,MF_UNCHECKED);				movetype = 1;
				CheckMenuItem(hMenu,CM_MOVESINE,MF_CHECKED);
				return 0;

			case CM_MOVELIVE:				CheckMenuItem(hMenu,CM_MOVERAND+movetype,MF_UNCHECKED);				movetype = 2;
				CheckMenuItem(hMenu,CM_MOVELIVE,MF_CHECKED);
				return 0;

			// handle new granularity			case CM_GRID10:
				g = GRIDSIZE/5;
				g = g + CM_GRID10 -2;
				CheckMenuItem(hMenu,g,MF_UNCHECKED);
				CheckMenuItem(hMenu,CM_GRID10,MF_CHECKED);
				initfield(10);
				return 0;

			case CM_GRID15:				g = GRIDSIZE/5;				g = g + CM_GRID10 -2;
				CheckMenuItem(hMenu,g,MF_UNCHECKED);
				CheckMenuItem(hMenu,CM_GRID15,MF_CHECKED);
				initfield(15);
				return 0;

			case CM_GRID20:				g = GRIDSIZE/5;				g = g + CM_GRID10 -2;
				CheckMenuItem(hMenu,g,MF_UNCHECKED);
				CheckMenuItem(hMenu,CM_GRID20,MF_CHECKED);
				initfield(20);
				return 0;

		  case CM_GRID25:      g = GRIDSIZE/5;
												g = g + CM_GRID10 -2;
												CheckMenuItem(hMenu,g,MF_UNCHECKED);
												CheckMenuItem(hMenu,CM_GRID25,MF_CHECKED);
												initfield(25);
												return 0;



		  case CM_FILLWIRE:      filltype = 0;
												CheckMenuItem(hMenu,CM_FILLFILL,MF_UNCHECKED);
												CheckMenuItem(hMenu,CM_FILLWIRE,MF_CHECKED);
												return 0;

		  case CM_FILLFILL:    filltype = 1;
												CheckMenuItem(hMenu,CM_FILLFILL,MF_CHECKED);
												CheckMenuItem(hMenu,CM_FILLWIRE,MF_UNCHECKED);
												return 0;

        case CM_HIDDEN:    hide = 1 - hide;
                                    if (!hide) CheckMenuItem(hMenu,CM_HIDDEN,MF_UNCHECKED);
                                            else CheckMenuItem(hMenu,CM_HIDDEN,MF_CHECKED);
                                    return 0;
         }

     default:                   return(DefWindowProc(hWnd, msg, wParam, lParam));
          }
     }




HDC CreateWinGDC(void)
{
    HDC hWinGDC;
    HBITMAP hBitmapNew;
    struct {
        BITMAPINFOHEADER InfoHeader;
        RGBQUAD ColorTable[256];
    } Info;
    void far *pSurfaceBits;

    // Set up an optimal bitmap
    if (WinGRecommendDIBFormat((BITMAPINFO far *)&Info) == FALSE)
        return 0;

    // Set the width and height of the DIB but preserve the
    // sign of biHeight in case top-down DIBs are faster
    Info.InfoHeader.biHeight *= SHEIGHT;

    Info.InfoHeader.biWidth = SWIDTH;

    // Create a WinGDC and Bitmap, then select away
    hWinGDC = WinGCreateDC();
    if (hWinGDC)
    {
        hBitmapNew = WinGCreateBitmap(hWinGDC,
            (BITMAPINFO far *)&Info, &pSurfaceBits);
        if (hBitmapNew)
        {
            offBitmap = (HBITMAP)SelectObject(hWinGDC,
                hBitmapNew);
        }
        else
        {
            DeleteDC(hWinGDC);
            hWinGDC = 0;

        }
    }

    return hWinGDC;
}

void DestroyWinGDC(HDC hWinGDC)
{
    HBITMAP hBitmapOld;

    if (hWinGDC && offBitmap)
    {
        // Select the stock 1x1 monochrome bitmap back in
        hBitmapOld = (HBITMAP)SelectObject(hWinGDC,offBitmap);
        DeleteObject(hBitmapOld);
        DeleteDC(hWinGDC);
    }
}


