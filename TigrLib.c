
#include <pocketlang.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "tigr.h"

//
//	TODO font loading stuff 
//

//	TMouse class

Tigr *main_window;
typedef struct 
{
	int x,y,b;
} tkMouse;

PkHandle* cls_TMouse = NULL;

static void* _TMouseNew(PKVM* vm) {
  tkMouse* ptr = pkRealloc(vm, NULL, sizeof(tkMouse));
	ptr->x=0;
	ptr->y=0;
	ptr->b=0;
  return ptr;
}

static void _TMouseDelete(PKVM* vm, void* ptr) {
  pkRealloc(vm, ptr, 0);
}

void _TMouseGetter(PKVM* vm) {
  uint32_t hash = pkGetSlotHash(vm, 1);
  tkMouse* self = pkGetSelf(vm);
	int x,y,b;
	tigrMouse(main_window,&x,&y,&b);
  switch (hash) {
		//	x 
		case 0xfd0c5087:
		{ 
      pkSetSlotNumber(vm, 0, (float)x);
      return;
    }
		//	y 
    case 0xfc0c4ef4:
		{ 
      pkSetSlotNumber(vm, 0, (float)y);
      return;
    }
		case 0x124aec70: 	//	left
		{
			pkSetSlotBool(vm,0,b&1);
			return;
		}
		case 0xc982a718: 	//	middle
		{
			pkSetSlotBool(vm,0,b&2);
			return;
		}
		case 0x78e32de5: 	//	right
		{
			pkSetSlotBool(vm,0,b&4);
			return;
		}
		default:
		{
			printf("unsupported field hash %x\n",hash);
			return;
		}
 }
}

//	TPixel class
PkHandle* cls_TPixel = NULL;

static void* _TPixelNew(PKVM* vm) {
  TPixel* ptr = pkRealloc(vm, NULL, sizeof(TPixel));
	ptr->r=255;
	ptr->g=0;
	ptr->b=255;
	ptr->a=255;
  return ptr;
}

void _TPixelInit(PKVM* vm) {
  double r, g, b, a;
	int argc = pkGetArgc(vm);
	if (argc==4)
	{
		if (!pkValidateSlotNumber(vm, 1, &r)) return;
		if (!pkValidateSlotNumber(vm, 2, &g)) return;
		if (!pkValidateSlotNumber(vm, 3, &b)) return;
		if (!pkValidateSlotNumber(vm, 4, &a)) return;
		TPixel* self = (TPixel*) pkGetSelf(vm);
		self->r = (int)r;
		self->g = (int)g;
		self->b = (int)b;
		self->a = (int)a;
	}
}

void _TPixelStr(PKVM* vm) {
	char tostr[256];
 	TPixel* self = (TPixel*)pkGetSelf(vm);
 	sprintf(tostr,"Pixel\t[%d, %d, %d, %d]", self->r, self->g,self->b,self->a);
  pkSetSlotString(vm, 0, tostr);
}

static void _TPixelDelete(PKVM* vm, void* ptr) {
  pkRealloc(vm, ptr, 0);
}

void _TPixelGetter(PKVM* vm) {
  uint32_t hash = pkGetSlotHash(vm, 1);
  TPixel* self = pkGetSelf(vm);
  switch (hash) {
		//	red 
		case 0x40f480dc:
    case 0xf70c4715:
		{ 
      pkSetSlotNumber(vm, 0, self->r);
      return;
    }
		//	green 
		case 0xe20c2606:
    case 0x11decbc:
		{ 
      pkSetSlotNumber(vm, 0, self->g);
      return;
    }
		//	blue 
		case 0xe70c2de5:
    case 0x82fbf5cd:
		{ 
      pkSetSlotNumber(vm, 0, self->b);
      return;
    }
		//	alpha
		case 0xe40c292c:
    case 0x5d8b6dab:
		{ 
      pkSetSlotNumber(vm, 0, self->a);
      return;
    }
  }
}

void _TPixelSetter(PKVM* vm) {
  uint32_t hash = pkGetSlotHash(vm, 1);
  TPixel* self = pkGetSelf(vm);
	double var = 0.0f;
  if (!pkValidateSlotNumber(vm, 2, &var)) return;

  switch (hash) {
		//	red 
		case 0x40f480dc:
    case 0xf70c4715:
		{ 
			self->r = (int)var;
      return;
    }
		//	green 
		case 0xe20c2606:
    case 0x11decbc:
		{ 
			self->g = (int)var;
//      pkSetSlotNumber(vm, 0, self->g);
      return;
    }
		//	blue 
		case 0xe70c2de5:
    case 0x82fbf5cd:
		{ 
			self->b = (int)var;
 //     pkSetSlotNumber(vm, 0, self->b);
      return;
    }
		//	alpha
		case 0xe40c292c:
    case 0x5d8b6dab:
		{ 
			self->a = (int)var;
  //    pkSetSlotNumber(vm, 0, self->a);
      return;
    }
  }
}

//	Tigr ( bitmap ) class
PkHandle* cls_Tigr = NULL;

static void* _TigrNew(PKVM* vm) {
  Tigr* ptr = NULL;
	int argc = pkGetArgc(vm);

	//	filename
	if (argc==1)
	{
		const char* fname;
		if (!pkValidateSlotString(vm, 1, &fname, NULL)) return NULL;
		return tigrLoadImage(fname);
	}
	//	width,height
	if (argc==2)
	{
		int width;
		if (!pkValidateSlotInteger(vm, 1, &width)) return NULL;
		int height;
		if (!pkValidateSlotInteger(vm, 2, &height)) return NULL;
		ptr = tigrBitmap(width,height);
		return ptr;
	}
	//	width,height,title,flags
	if (argc==4)
	{
		int width;
		if (!pkValidateSlotInteger(vm, 1, &width)) return NULL;
		int height;
		if (!pkValidateSlotInteger(vm, 2, &height)) return NULL;
		const char* title;
		if (!pkValidateSlotString(vm, 3, &title, NULL)) return NULL;
		int flags;
		if (!pkValidateSlotInteger(vm, 4, &flags)) return NULL;
		ptr = tigrWindow(width,height,title,flags);
		main_window = ptr;
		return ptr;
	}
  return NULL;
}

void _TigrGetter(PKVM* vm) {
  uint32_t hash = pkGetSlotHash(vm, 1);
  Tigr* self = pkGetSelf(vm);
  switch (hash) {
    case 0x95876e1f: { // width
      pkSetSlotNumber(vm, 0, self->w);
      return;
    }
		case 0xd5bdbb42: { // height
      pkSetSlotNumber(vm, 0, self->h);
		}
  }
}

void _TigrSetter(PKVM* vm) {
  uint32_t hash = pkGetSlotHash(vm, 1);
  Tigr* self = pkGetSelf(vm);
}


void _TigrStr(PKVM* vm) {
	char tostr[256];
 	Tigr* self = (Tigr*)pkGetSelf(vm);
 	sprintf(tostr,"Bitmap\t[%d, %d]", self->w, self->h);
  pkSetSlotString(vm, 0, tostr);
}

static void _TigrDelete(PKVM* vm, void* ptr) {
	tigrFree(ptr);
}

//	functions
// Initialize window and OpenGL context
//	expects width,height,"title",flags
static void _tigrWindow(PKVM* vm) {
  pkSetSlotHandle(vm, 0, cls_Tigr);
  if (!pkNewInstance(vm, 0, 0, 4, 2)) return;
  Tigr* r = pkGetSlotNativeInstance(vm, 0);
}
//	create bitmap 
//	expects width,height
static void _tigrBitmap(PKVM* vm) {
  pkSetSlotHandle(vm, 0, cls_Tigr);
  if (!pkNewInstance(vm, 0, 0, 2, 2)) return;
  Tigr* r = pkGetSlotNativeInstance(vm, 0);
}

//	load PNG 
//	expects filename
static void _tigrLoadImage(PKVM* vm) {
  pkSetSlotHandle(vm, 0, cls_Tigr);
  if (!pkNewInstance(vm, 0, 0, 1, 2)) return;
  Tigr* r = pkGetSlotNativeInstance(vm, 0);
}

//	save PNG 
//	expects bitmap,"filename"
static void _tigrSaveImage(PKVM* vm) {
  pkSetSlotHandle(vm, 0, cls_Tigr);
  if (!pkValidateSlotInstanceOf(vm, 1, 0)) return;
 	Tigr* tigr = pkGetSlotNativeInstance(vm, 1);
	const char* fname;
	if (!pkValidateSlotString(vm, 2, &fname, NULL)) return;
	tigrSaveImage(fname,tigr);
}

// Returns non-zero if the user requested to close a window.
static void _tigrClosed(PKVM* vm) {
  pkSetSlotHandle(vm, 0, cls_Tigr);
  if (!pkValidateSlotInstanceOf(vm, 1, 0)) return;
 	Tigr* tigr = pkGetSlotNativeInstance(vm, 1);
	pkSetSlotBool(vm,0,tigrClosed(tigr)!=0);
}

// Displays a window's contents on-screen and updates input.
static void _tigrUpdate(PKVM* vm) {
  pkSetSlotHandle(vm, 0, cls_Tigr);
  if (!pkValidateSlotInstanceOf(vm, 1, 0)) return;
 	Tigr* tigr = pkGetSlotNativeInstance(vm, 1);
	tigrUpdate(tigr);
}

// Helper for reading pixels. bitmap,x,y
// For high performance, just access bmp->pix directly.
static void _tigrGet(PKVM* vm) {
	double dx,dy;
  pkSetSlotHandle(vm, 0, cls_Tigr);
  if (!pkValidateSlotInstanceOf(vm, 1, 0)) return;
 	Tigr* dest = pkGetSlotNativeInstance(vm, 1);

  pkReserveSlots(vm, 6); // Now we have slots [0, 1, 2, 3, 4, 5].
  if (!pkValidateSlotNumber(vm, 2, &dx)) return;
  if (!pkValidateSlotNumber(vm, 3, &dy)) return;
	TPixel got = tigrGet(dest,(int)dx,(int)dy);
	pkSetSlotNumber(vm, 2, got.r);
	pkSetSlotNumber(vm, 3, got.g);
	pkSetSlotNumber(vm, 4, got.b);
	pkSetSlotNumber(vm, 5, got.a);

  pkSetSlotHandle(vm, 0, cls_TPixel);
  if (!pkNewInstance(vm, 0, 0, 4, 2)) return;
//	printf("got %d %d %d %d\n",got.r,got.g,got.b,got.a);
}
// Plots a pixel.
// Clips and blends.
// For high performance, just access bmp->pix directly.
static void _tigrPlot(PKVM* vm) {
  pkSetSlotHandle(vm, 0, cls_Tigr);
  if (!pkValidateSlotInstanceOf(vm, 1, 0)) return;
 	Tigr* tigr = pkGetSlotNativeInstance(vm, 1);
	double x,y;
  double r,g,b,a;
  if (!pkValidateSlotNumber(vm, 2, &x)) return;
  if (!pkValidateSlotNumber(vm, 3, &y)) return;

	int argc = pkGetArgc(vm);
	if (argc==7)
	{
		if (!pkValidateSlotNumber(vm, 4, &r)) return;
		if (!pkValidateSlotNumber(vm, 5, &g)) return;
		if (!pkValidateSlotNumber(vm, 6, &b)) return;
		if (!pkValidateSlotNumber(vm, 7, &a)) return;
	//	printf("tigrPlot %x,%f,%f (%f,%f,%f,%f)\n",tigr,x,y,r,g,b,a);
		tigrPlot(tigr,x,y,tigrRGBA((int)r,(int)g,(int)b,(int)a));
	}
	else if (argc==4)
	{
	  pkSetSlotHandle(vm, 1, cls_TPixel);
	  if (!pkValidateSlotInstanceOf(vm, 4, 1)) return;
 		TPixel* px = pkGetSlotNativeInstance(vm, 4);
		tigrPlot(tigr,x,y,*px);
	}
}

// Clears a bitmap to a color.
// No blending, no clipping.
static void _tigrClear(PKVM* vm) {
  pkSetSlotHandle(vm, 0, cls_Tigr);
  if (!pkValidateSlotInstanceOf(vm, 1, 0)) return;
 	Tigr* tigr = pkGetSlotNativeInstance(vm, 1);
  double r,g,b;
  if (!pkValidateSlotNumber(vm, 2, &r)) return;
  if (!pkValidateSlotNumber(vm, 3, &g)) return;
  if (!pkValidateSlotNumber(vm, 4, &b)) return;
	tigrClear(tigr,tigrRGB((int)r,(int)g,(int)b));
}

// Draws a line.
// Start and end pixels are drawn.
// Clips and blends.
static void _tigrLine(PKVM* vm) {
  pkSetSlotHandle(vm, 0, cls_Tigr);
  if (!pkValidateSlotInstanceOf(vm, 1, 0)) return;
 	Tigr* tigr = pkGetSlotNativeInstance(vm, 1);
	double x,y,x1,y1;
  double r,g,b,a;
  if (!pkValidateSlotNumber(vm, 2, &x)) return;
  if (!pkValidateSlotNumber(vm, 3, &y)) return;
  if (!pkValidateSlotNumber(vm, 4, &x1)) return;
  if (!pkValidateSlotNumber(vm, 5, &y1)) return;
	int argc = pkGetArgc(vm);
	if (argc==9)
	{
		if (!pkValidateSlotNumber(vm, 6, &r)) return;
		if (!pkValidateSlotNumber(vm, 7, &g)) return;
		if (!pkValidateSlotNumber(vm, 8, &b)) return;
		if (!pkValidateSlotNumber(vm, 9, &a)) return;
//		tigrPlot(tigr,x,y,tigrRGBA((int)r,(int)g,(int)b,(int)a));
		tigrLine(tigr,x,y,x1,y1,tigrRGBA((int)r,(int)g,(int)b,(int)a));
	}
	else if (argc==6)
	{
	  pkSetSlotHandle(vm, 1, cls_TPixel);
	  if (!pkValidateSlotInstanceOf(vm, 6, 1)) return;
 		TPixel* px = pkGetSlotNativeInstance(vm, 6);
		tigrLine(tigr,x,y,x1,y1,*px);
	}
}

static void _tigrRect(PKVM* vm) {
  pkSetSlotHandle(vm, 0, cls_Tigr);
  if (!pkValidateSlotInstanceOf(vm, 1, 0)) return;
 	Tigr* tigr = pkGetSlotNativeInstance(vm, 1);
	double x,y,w,h;
  double r,g,b,a;
  if (!pkValidateSlotNumber(vm, 2, &x)) return;
  if (!pkValidateSlotNumber(vm, 3, &y)) return;
  if (!pkValidateSlotNumber(vm, 4, &w)) return;
  if (!pkValidateSlotNumber(vm, 5, &h)) return;
	int argc = pkGetArgc(vm);
	if (argc==9)
	{
		if (!pkValidateSlotNumber(vm, 6, &r)) return;
		if (!pkValidateSlotNumber(vm, 7, &g)) return;
		if (!pkValidateSlotNumber(vm, 8, &b)) return;
		if (!pkValidateSlotNumber(vm, 9, &a)) return;
//		tigrPlot(tigr,x,y,tigrRGBA((int)r,(int)g,(int)b,(int)a));
		tigrRect(tigr,x,y,w,h,tigrRGBA((int)r,(int)g,(int)b,(int)a));
	}
	else if (argc==6)
	{
	  pkSetSlotHandle(vm, 1, cls_TPixel);
	  if (!pkValidateSlotInstanceOf(vm, 6, 1)) return;
 		TPixel* px = pkGetSlotNativeInstance(vm, 6);
		tigrRect(tigr,x,y,w,h,*px);
	}
	else 
	{
		printf("%d\n",argc);
	}
}

static void _tigrFillRect(PKVM* vm) {
  pkSetSlotHandle(vm, 0, cls_Tigr);
  if (!pkValidateSlotInstanceOf(vm, 1, 0)) return;
 	Tigr* tigr = pkGetSlotNativeInstance(vm, 1);
	double x,y,w,h;
  double r,g,b,a;
  if (!pkValidateSlotNumber(vm, 2, &x)) return;
  if (!pkValidateSlotNumber(vm, 3, &y)) return;
  if (!pkValidateSlotNumber(vm, 4, &w)) return;
  if (!pkValidateSlotNumber(vm, 5, &h)) return;
	int argc = pkGetArgc(vm);
	if (argc==9)
	{
		if (!pkValidateSlotNumber(vm, 6, &r)) return;
		if (!pkValidateSlotNumber(vm, 7, &g)) return;
		if (!pkValidateSlotNumber(vm, 8, &b)) return;
		if (!pkValidateSlotNumber(vm, 9, &a)) return;
		tigrFillRect(tigr,x,y,w,h,tigrRGBA((int)r,(int)g,(int)b,(int)a));
	}
	else if (argc==6)
	{
	  pkSetSlotHandle(vm, 1, cls_TPixel);
	  if (!pkValidateSlotInstanceOf(vm, 6, 1)) return;
 		TPixel* px = pkGetSlotNativeInstance(vm, 6);
		tigrFillRect(tigr,x,y,w,h,*px);
	}
	else 
	{
		printf("%d\n",argc);
	}
}

static void _tigrCircle(PKVM* vm) {
  pkSetSlotHandle(vm, 0, cls_Tigr);
  if (!pkValidateSlotInstanceOf(vm, 1, 0)) return;
 	Tigr* tigr = pkGetSlotNativeInstance(vm, 1);
	double x,y,rad;
  double r,g,b,a;
  if (!pkValidateSlotNumber(vm, 2, &x)) return;
  if (!pkValidateSlotNumber(vm, 3, &y)) return;
  if (!pkValidateSlotNumber(vm, 4, &rad)) return;
	int argc = pkGetArgc(vm);
	if (argc==8)
	{
		if (!pkValidateSlotNumber(vm, 5, &r)) return;
		if (!pkValidateSlotNumber(vm, 6, &g)) return;
		if (!pkValidateSlotNumber(vm, 7, &b)) return;
		if (!pkValidateSlotNumber(vm, 8, &a)) return;
		tigrCircle(tigr,x,y,rad,tigrRGBA((int)r,(int)g,(int)b,(int)a));
	}
	else if (argc==5)
	{
	  pkSetSlotHandle(vm, 1, cls_TPixel);
	  if (!pkValidateSlotInstanceOf(vm, 5, 1)) return;
 		TPixel* px = pkGetSlotNativeInstance(vm, 5);
		tigrCircle(tigr,x,y,rad,*px);
	}
	else 
	{
		printf("%d\n",argc);
	}
}

static void _tigrFillCircle(PKVM* vm) {
  pkSetSlotHandle(vm, 0, cls_Tigr);
  if (!pkValidateSlotInstanceOf(vm, 1, 0)) return;
 	Tigr* tigr = pkGetSlotNativeInstance(vm, 1);
	double x,y,rad;
  double r,g,b,a;
  if (!pkValidateSlotNumber(vm, 2, &x)) return;
  if (!pkValidateSlotNumber(vm, 3, &y)) return;
  if (!pkValidateSlotNumber(vm, 4, &rad)) return;
	int argc = pkGetArgc(vm);
	if (argc==8)
	{
		if (!pkValidateSlotNumber(vm, 5, &r)) return;
		if (!pkValidateSlotNumber(vm, 6, &g)) return;
		if (!pkValidateSlotNumber(vm, 7, &b)) return;
		if (!pkValidateSlotNumber(vm, 8, &a)) return;
		tigrFillCircle(tigr,x,y,rad,tigrRGBA((int)r,(int)g,(int)b,(int)a));
	}
	else if (argc==5)
	{
	  pkSetSlotHandle(vm, 1, cls_TPixel);
	  if (!pkValidateSlotInstanceOf(vm, 5, 1)) return;
 		TPixel* px = pkGetSlotNativeInstance(vm, 5);
		tigrFillCircle(tigr,x,y,rad,*px);
	}
	else 
	{
		printf("%d\n",argc);
	}
}


static void _tigrBlit(PKVM* vm) {
	double dx,dy,sx,sy,w,h;

  pkSetSlotHandle(vm, 0, cls_Tigr);
  if (!pkValidateSlotInstanceOf(vm, 1, 0)) return;
  if (!pkValidateSlotInstanceOf(vm, 2, 0)) return;
 	Tigr* dest = pkGetSlotNativeInstance(vm, 1);
 	Tigr* source = pkGetSlotNativeInstance(vm, 2);

  if (!pkValidateSlotNumber(vm, 3, &dx)) return;
  if (!pkValidateSlotNumber(vm, 4, &dy)) return;
  if (!pkValidateSlotNumber(vm, 5, &sx)) return;
  if (!pkValidateSlotNumber(vm, 6, &sy)) return;
  if (!pkValidateSlotNumber(vm, 7, &w)) return;
  if (!pkValidateSlotNumber(vm, 8, &h)) return;

	tigrBlit(dest, source, (int)dx, (int)dy, (int)sx, (int)sy, (int)w, (int)h);
}


static void _tigrBlitAlpha(PKVM* vm) {
	double dx,dy,sx,sy,w,h,alpha;

  pkSetSlotHandle(vm, 0, cls_Tigr);
  if (!pkValidateSlotInstanceOf(vm, 1, 0)) return;
  if (!pkValidateSlotInstanceOf(vm, 2, 0)) return;
 	Tigr* dest = pkGetSlotNativeInstance(vm, 1);
 	Tigr* source = pkGetSlotNativeInstance(vm, 2);

  if (!pkValidateSlotNumber(vm, 3, &dx)) return;
  if (!pkValidateSlotNumber(vm, 4, &dy)) return;
  if (!pkValidateSlotNumber(vm, 5, &sx)) return;
  if (!pkValidateSlotNumber(vm, 6, &sy)) return;
  if (!pkValidateSlotNumber(vm, 7, &w)) return;
  if (!pkValidateSlotNumber(vm, 8, &h)) return;
  if (!pkValidateSlotNumber(vm, 9, &alpha)) return;

	tigrBlitAlpha(dest, source, (int)dx, (int)dy, (int)sx, (int)sy, (int)w, (int)h,(float)alpha);
}

static void _tigrBlitTint(PKVM* vm) {
	double dx,dy,sx,sy,w,h;
  pkSetSlotHandle(vm, 0, cls_Tigr);
  if (!pkValidateSlotInstanceOf(vm, 1, 0)) return;
  if (!pkValidateSlotInstanceOf(vm, 2, 0)) return;
 	Tigr* dest = pkGetSlotNativeInstance(vm, 1);
 	Tigr* source = pkGetSlotNativeInstance(vm, 2);

  if (!pkValidateSlotNumber(vm, 3, &dx)) return;
  if (!pkValidateSlotNumber(vm, 4, &dy)) return;
  if (!pkValidateSlotNumber(vm, 5, &sx)) return;
  if (!pkValidateSlotNumber(vm, 6, &sy)) return;
  if (!pkValidateSlotNumber(vm, 7, &w)) return;
  if (!pkValidateSlotNumber(vm, 8, &h)) return;

	int argc = pkGetArgc(vm);

	if (argc==12)
	{
	  double r,g,b,a;
		if (!pkValidateSlotNumber(vm, 9, &r)) return;
		if (!pkValidateSlotNumber(vm, 10, &g)) return;
		if (!pkValidateSlotNumber(vm, 11, &b)) return;
		if (!pkValidateSlotNumber(vm, 12, &a)) return;
		tigrBlitTint(dest,source,(int)dx,(int)dy,(int)sx,(int)sy,(int)w,(int)h,tigrRGBA((int)r,(int)g,(int)b,(int)a));
//		tigrFillCircle(tigr,x,y,rad,tigrRGBA((int)r,(int)g,(int)b,(int)a));
	}
	else if (argc==9)
	{
	  pkSetSlotHandle(vm, 1, cls_TPixel);
	  if (!pkValidateSlotInstanceOf(vm, 9, 1)) return;
 		TPixel* px = pkGetSlotNativeInstance(vm, 9);
		tigrBlitTint(dest,source,(int)dx,(int)dy,(int)sx,(int)sy,(int)w,(int)h,*px);
	}
}

static void _tigrPrint(PKVM* vm) {
	double dx,dy;
	const char* title;
  pkSetSlotHandle(vm, 0, cls_Tigr);
  if (!pkValidateSlotInstanceOf(vm, 1, 0)) return;
 	Tigr* dest = pkGetSlotNativeInstance(vm, 1);

  if (!pkValidateSlotNumber(vm, 3, &dx)) return;
  if (!pkValidateSlotNumber(vm, 4, &dy)) return;

	pkSetSlotHandle(vm, 1, cls_TPixel);
	if (!pkValidateSlotInstanceOf(vm, 5, 1)) return;
	TPixel* px = pkGetSlotNativeInstance(vm, 5);

	if (!pkValidateSlotString(vm, 6, &title, NULL)) return;
	int argc = pkGetArgc(vm);
	int x = (int)dx;
	int y = (int)dy;
	tigrPrint(dest,tfont,x,y,*px,title);
//	printf("%d %x %d %d %s (%x%x%x%x)\n",argc,dest,x,y,title,px->r,px->g,px->b,px->a);
}


static void _tigrTime(PKVM* vm) {
  // Return value to the pocket VM.
  pkSetSlotNumber(vm, 0, tigrTime());
}

static void _tigrKeyDown(PKVM* vm) {
	int key;
  pkSetSlotHandle(vm, 0, cls_Tigr);
  if (!pkValidateSlotInstanceOf(vm, 1, 0)) return;
 	Tigr* dest = pkGetSlotNativeInstance(vm, 1);

	int slot = pkGetSlotType(vm,2);
	
	if (slot==PK_STRING)
	{
		const char *str;
		if (!pkValidateSlotString(vm, 2, &str, NULL)) return;
		key = str[0];
	}
	else
	{
		if (!pkValidateSlotInteger(vm, 2, &key)) return;
	}
  
  // Return value to the pocket VM.
  pkSetSlotBool(vm, 0, tigrKeyDown(dest,key)!=0);
}

static void _tigrKeyHeld(PKVM* vm) {
	int key;
  pkSetSlotHandle(vm, 0, cls_Tigr);
  if (!pkValidateSlotInstanceOf(vm, 1, 0)) return;
 	Tigr* dest = pkGetSlotNativeInstance(vm, 1);
	
	int slot = pkGetSlotType(vm,2);
	
	if (slot==PK_STRING)
	{
		const char *str;
		if (!pkValidateSlotString(vm, 2, &str, NULL)) return;
		key = str[0];
	}
	else
	{
		if (!pkValidateSlotInteger(vm, 2, &key)) return;
	}
	
  // Return value to the pocket VM.
  pkSetSlotBool(vm, 0, tigrKeyHeld(dest,key)!=0);
}

static void _tigrMouse(PKVM* vm) {
  // Return value to the pocket VM.
  pkSetSlotNumber(vm, 0, 1);
  pkSetSlotNumber(vm, 1, 2);
  pkSetSlotNumber(vm, 2, 3);
}


#define pkEnum(vm,x) pkSetSlotNumber(vm,1,x); pkSetAttribute(vm,0,#x,1);

PK_EXPORT PkHandle* pkExportModule(PKVM* vm) {
  PkHandle* tigrLib = pkNewModule(vm, "Tigr");

  pkReserveSlots(vm, 1);
  pkSetSlotHandle(vm, 0, tigrLib);
	pkEnum(vm,TK_PAD0);
	pkEnum(vm,TK_PAD1);
	pkEnum(vm,TK_PAD2);
	pkEnum(vm,TK_PAD3);
	pkEnum(vm,TK_PAD4);
	pkEnum(vm,TK_PAD5);
	pkEnum(vm,TK_PAD6);
	pkEnum(vm,TK_PAD7);
	pkEnum(vm,TK_PAD8);
	pkEnum(vm,TK_PAD9);
	pkEnum(vm,TK_PADMUL);
	pkEnum(vm,TK_PADADD);
	pkEnum(vm,TK_PADENTER);
	pkEnum(vm,TK_PADSUB);
	pkEnum(vm,TK_PADDOT);
	pkEnum(vm,TK_PADDIV);
	pkEnum(vm,TK_F1);
	pkEnum(vm,TK_F2);
	pkEnum(vm,TK_F3);
	pkEnum(vm,TK_F4);
	pkEnum(vm,TK_F5);
	pkEnum(vm,TK_F6);
	pkEnum(vm,TK_F7);
	pkEnum(vm,TK_F8);
	pkEnum(vm,TK_F9);
	pkEnum(vm,TK_F10);
	pkEnum(vm,TK_F11);
	pkEnum(vm,TK_F12);
	pkEnum(vm,TK_BACKSPACE);
	pkEnum(vm,TK_TAB);
	pkEnum(vm,TK_RETURN);
	pkEnum(vm,TK_SHIFT);
	pkEnum(vm,TK_CONTROL);
	pkEnum(vm,TK_ALT);
	pkEnum(vm,TK_PAUSE);
	pkEnum(vm,TK_CAPSLOCK);
	pkEnum(vm,TK_ESCAPE);
	pkEnum(vm,TK_SPACE);
	pkEnum(vm,TK_PAGEUP);
	pkEnum(vm,TK_PAGEDN);
	pkEnum(vm,TK_END);
	pkEnum(vm,TK_HOME);
	pkEnum(vm,TK_LEFT);
	pkEnum(vm,TK_RIGHT);
	pkEnum(vm,TK_UP);
	pkEnum(vm,TK_DOWN);
	pkEnum(vm,TK_INSERT);
	pkEnum(vm,TK_DELETE);
	pkEnum(vm,TK_LWIN);
	pkEnum(vm,TK_RWIN);
	pkEnum(vm,TK_NUMLOCK);
	pkEnum(vm,TK_SCROLL);
	pkEnum(vm,TK_LSHIFT);
	pkEnum(vm,TK_RSHIFT);
	pkEnum(vm,TK_LCONTROL);
	pkEnum(vm,TK_RCONTROL);
	pkEnum(vm,TK_LALT);
	pkEnum(vm,TK_RALT);
	pkEnum(vm,TK_SEMICOLON);
	pkEnum(vm,TK_EQUALS);
	pkEnum(vm,TK_COMMA);
	pkEnum(vm,TK_MINUS);
	pkEnum(vm,TK_DOT);
	pkEnum(vm,TK_SLASH);
	pkEnum(vm,TK_BACKTICK);
	pkEnum(vm,TK_LSQUARE);
	pkEnum(vm,TK_BACKSLASH);
	pkEnum(vm,TK_RSQUARE);
	pkEnum(vm,TK_TICK);

	pkEnum(vm,TIGR_FIXED);
	pkEnum(vm,TIGR_AUTO);
	pkEnum(vm,TIGR_2X);
	pkEnum(vm,TIGR_3X);
	pkEnum(vm,TIGR_4X);
	pkEnum(vm,TIGR_FULLSCREEN);

  cls_Tigr = pkNewClass(vm, "Tigr", NULL, tigrLib, _TigrNew, _TigrDelete, "Tigr");
  pkClassAddMethod(vm, cls_Tigr, "@getter",  _TigrGetter, 1, NULL);
  pkClassAddMethod(vm, cls_Tigr, "@setter",  _TigrSetter, 2, NULL);
	pkClassAddMethod(vm, cls_Tigr, "_str", 		_TigrStr, 0, NULL);

  cls_TPixel = pkNewClass(vm, "RGBA", NULL, tigrLib, _TPixelNew, _TPixelDelete, "RGBA");
	pkClassAddMethod(vm, cls_TPixel, "@getter",  _TPixelGetter, 1, NULL);
 	pkClassAddMethod(vm, cls_TPixel, "@setter",  _TPixelSetter, 2, NULL);
  pkClassAddMethod(vm, cls_TPixel, "_init",   _TPixelInit,   -1, NULL);
	pkClassAddMethod(vm, cls_TPixel, "_str", 		_TPixelStr, 0, NULL);


  cls_TMouse = pkNewClass(vm, "TMouse", NULL, tigrLib, _TMouseNew, _TMouseDelete, "TMouse");
	pkClassAddMethod(vm, cls_TMouse, "@getter",  _TMouseGetter, 1, NULL);

// void pkModuleAddFunction(PKVM* vm, PkHandle* module, const char* name, pkNativeFn fptr, int arity, const char* docstring)  
	pkModuleAddFunction(vm, tigrLib, "Window", _tigrWindow,4,"(Width,Height,Title,FLAGS)");
	pkModuleAddFunction(vm, tigrLib, "Bitmap", _tigrBitmap,2,"(Width,Height)");
	pkModuleAddFunction(vm, tigrLib, "LoadImage", _tigrLoadImage,1,"(Filename)");
	pkModuleAddFunction(vm, tigrLib, "SaveImage", _tigrSaveImage,2,"(Target,Filename)");
	pkModuleAddFunction(vm, tigrLib, "Closed", _tigrClosed,1,"(Target)");
 	pkModuleAddFunction(vm, tigrLib, "Update", _tigrUpdate,1,"(Target");
 	pkModuleAddFunction(vm, tigrLib, "Clear", _tigrClear,4,"(Target,red,green,blue)");
 	pkModuleAddFunction(vm, tigrLib, "Plot", _tigrPlot,-1,"(Target,x,y,<color>)");
 	pkModuleAddFunction(vm, tigrLib, "Rect", _tigrRect,-1,"(Target,x,y,width,height,<color>)");
 	pkModuleAddFunction(vm, tigrLib, "FillRect", _tigrFillRect,-1,"(Target,x,y,width,height,<color>)");
 	pkModuleAddFunction(vm, tigrLib, "Circle", _tigrCircle,-1,"(Target,x,y,radius,<color>)");
 	pkModuleAddFunction(vm, tigrLib, "FillCircle", _tigrFillCircle,-1,"(Target,x,y,radius,<color>)");
 	pkModuleAddFunction(vm, tigrLib, "Line", _tigrLine,-1,"(Target,x0,y0,x1,y1,<color>)");
 	pkModuleAddFunction(vm, tigrLib, "Blit", _tigrBlit,8,"(Target,source,target X,target Y,source X,source Y,width,height)");
 	pkModuleAddFunction(vm, tigrLib, "BlitAlpha", _tigrBlitAlpha,9,"(Target,source,target X,target Y,source X,source Y,width,height,alpha (0.0-1.0))");
 	pkModuleAddFunction(vm, tigrLib, "BlitTint", _tigrBlitTint,-1,"(Target,source,target X,target Y,source X,source Y,width,height,<color>)");
 	pkModuleAddFunction(vm, tigrLib, "Print", _tigrPrint,-1,"");
 	pkModuleAddFunction(vm, tigrLib, "Time", _tigrTime,0,"");
 	pkModuleAddFunction(vm, tigrLib, "Mouse", _tigrMouse,0,"");
 	pkModuleAddFunction(vm, tigrLib, "KeyDown", _tigrKeyDown,2,"");
 	pkModuleAddFunction(vm, tigrLib, "KeyHeld", _tigrKeyHeld,2,"");
 	pkModuleAddFunction(vm, tigrLib, "Get", _tigrGet,3,"Get(bitmap,x,y)");
  return tigrLib;
}

void main()
{
	
}