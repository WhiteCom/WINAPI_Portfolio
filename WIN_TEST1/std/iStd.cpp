﻿#include "iStd.h"

#include "iWindow.h"

iSize devSize;
iRect viewport;

bool* keyBuf;

void appInitialize()
{
	srand(time(NULL));
	keyBuf = new bool[256];
	memset(keyBuf, 0xff, 256);
}

int keyStat = 0, keyDown = 0; //stat : 계속 누름, down : 한번누름
void setKeys(int& keys, iKeyStat stat, unsigned int c);

void setKeyStat(iKeyStat stat, unsigned int c)
{
	setKeys(keyStat, stat, c);
}

void setKeyDown(iKeyStat stat, unsigned int c)
{
	//just one time
	if (stat == iKeyStatBegan)
	{
		if (keyBuf[c])
			return;
		keyBuf[c] = true;
		setKeys(keyDown, stat, c);
	}
	else
	{
		keyBuf[c] = false;
	}
}

void setKeys(int& keys, iKeyStat stat, unsigned int c)
{
	if (stat == iKeyStatBegan)
	{
		if (c == 'a' || c == 'A' || c == VK_LEFT)
			keys |= keysA;
		else if (c == 's' || c == 'S' || c == VK_DOWN)
			keys |= keysS;
		else if (c == 'd' || c == 'D' || c == VK_RIGHT)
			keys |= keysD;
		else if (c == 'w' || c == 'W' || c == VK_UP)
			keys |= keysW;
		else if (c == ' ')
			keys |= keysSpace;
	}
	
	else if (stat == iKeyStatEnded)
	{
		if (c == 'a' || c == 'A' || c == VK_LEFT)
			keys &= ~keysA;
		else if (c == 's' || c == 'S' || c == VK_DOWN)
			keys &= ~keysS;
		else if (c == 'd' || c == 'D' || c == VK_RIGHT)
			keys &= ~keysD;
		else if (c == 'w' || c == 'W' || c == VK_UP)
			keys &= ~keysW;
		else if (c == ' ')
			keys &= ~keysSpace;
	}
}

int random()
{
	return rand();
}

float _cos(float degree)
{
	return cos(degree * M_PI / 180);
}
float _sin(float degree)
{
	return sin(degree * M_PI / 180);
}

float uint8ToFloat(uint8 c)
{
	return c / 255.0f;
}

uint8 floatToUint8(float c)
{
	return (uint8)(c * 0xFF);
}

static float _r = 1.0f, _g = 1.0f, _b = 1.0f, _a = 1.0f;

void setRGBA(iColor4f c)
{
	_r = c.r;
	_g = c.g;
	_b = c.b;
	_a = c.a;
}

void setRGBA(iColor4b c)
{
	_r = uint8ToFloat(c.r);
	_g = uint8ToFloat(c.g);
	_b = uint8ToFloat(c.b);
	_a = uint8ToFloat(c.a);
}
void setRGBA(float r, float g, float b, float a)
{
	_r = r;
	_g = g;
	_b = b;
	_a = a;
}

void getRGBA(float& r, float& g, float& b, float& a)
{
	r = _r;
	g = _g;
	b = _b;
	a = _a;
}


void clearRect()
{
	graphics->Clear(Color(
		floatToUint8(_a),
		floatToUint8(_r),
		floatToUint8(_g),
		floatToUint8(_b)
	));
}

void drawRect(iRect rt, float radius)
{
	drawRect(rt.origin.x, rt.origin.y, rt.size.width, rt.size.height, radius);
}
void drawRect(float x, float y, float width, float height, float radius)
{
	Pen pen(Color(
		floatToUint8(_a),
		floatToUint8(_r),
		floatToUint8(_g),
		floatToUint8(_b)
	));

	pen.SetWidth(1.0f);

	GraphicsPath path;
	path.AddLine(x + radius, y, x + width - (radius * 2), y);
	path.AddArc(x + width - (radius * 2), y, radius * 2, radius * 2, 270, 90);
	path.AddLine(x + width, y + radius, x + width, y + height - (radius * 2));
	path.AddArc(x + width - (radius * 2), y + height - (radius * 2), radius * 2, radius * 2, 0, 90);
	path.AddLine(x + width - (radius * 2), y + height, x + radius, y + height);
	path.AddArc(x, y + height - (radius * 2), radius * 2, radius * 2, 90, 90);
	path.AddLine(x, y + height - (radius * 2), x, y + radius);
	path.AddArc(x, y, radius * 2, radius * 2, 180, 90);
	path.CloseFigure();

	graphics->DrawPath(&pen, &path);
}

void fillRect(iRect rt, float radius)
{
	fillRect(rt.origin.x, rt.origin.y, rt.size.width, rt.size.height, radius);
}
void fillRect(float x, float y, float width, float height, float radius)
{
	SolidBrush brush(Color(
		floatToUint8(_a),
		floatToUint8(_r),
		floatToUint8(_g),
		floatToUint8(_b)
	));

	GraphicsPath path;
	path.AddLine(x + radius, y, x + width - (radius * 2), y);
	path.AddArc(x + width - (radius * 2), y, radius * 2, radius * 2, 270, 90);
	path.AddLine(x + width, y + radius, x + width, y + height - (radius * 2));
	path.AddArc(x + width - (radius * 2), y + height - (radius * 2), radius * 2, radius * 2, 0, 90);
	path.AddLine(x + width - (radius * 2), y + height, x + radius, y + height);
	path.AddArc(x, y + height - (radius * 2), radius * 2, radius * 2, 90, 90);
	path.AddLine(x, y + height - (radius * 2), x, y + radius);
	path.AddArc(x, y, radius * 2, radius * 2, 180, 90);
	path.CloseFigure();

	graphics->FillPath(&brush, &path);
}

int igImageWidth(igImage* ig)
{
	return ((Image*)ig)->GetWidth();
}
int igImageHeight(igImage* ig)
{
	return ((Image*)ig)->GetHeight();
}

Texture** createImageDivide(int numX, int numY, const char* szFormat, ...)
{
	char szText[1024];
	va_start_end(szText, szFormat);

	wchar_t* ws = utf8_to_utf16(szText);
	Bitmap* bmp = Bitmap::FromFile(ws, false);
	delete ws;

	// lock
	int width = bmp->GetWidth();
	int height = bmp->GetHeight();
	Rect rect(0, 0, width, height);
	BitmapData bmpData;
	bmp->LockBits(&rect, ImageLockModeRead, PixelFormat32bppARGB, &bmpData);

	int stride = bmpData.Stride / 4;
	uint32* rgba = (uint32*)bmpData.Scan0;

	// to do...
	int numXY = numX * numY;
	Texture** texs = new Texture * [numXY];
	int w = width / numX;
	int h = height / numY;
	for (int i = 0; i < numXY; i++)
	{
		int x = i % numX;
		int y = i / numX;

		Bitmap* b = new Bitmap(w, h, PixelFormat32bppARGB);
		Rect rt(0, 0, w, h);
		BitmapData bd;
		b->LockBits(&rt, ImageLockModeWrite, PixelFormat32bppARGB, &bd);

		int srd = bd.Stride / 4;
		uint32* color = (uint32*)bd.Scan0;
		for (int j = 0; j < h; j++)
		{
			memcpy(&color[srd * j],
				&rgba[stride * (h * y + j) + w * x],
				sizeof(uint32) * w);
		}

		b->UnlockBits(&bd);

		Texture* tex = new Texture;
		tex->texID = b;
		tex->width = w;
		tex->height = h;
		tex->potWidth = w;
		tex->potHeight = h;
		tex->retainCount = 1;

		texs[i] = tex;
	}

	// unlock
	bmp->UnlockBits(&bmpData);

	return texs;
}

Texture* createImage(const char* szFormat, ...)
{
	char szText[1024];
	va_start_end(szText, szFormat);

	wchar_t* ws = utf8_to_utf16(szText);
	Image* img = Image::FromFile(ws, false);
	delete ws;

	Texture* tex = new Texture;
	tex->texID = img;
	tex->width = img->GetWidth();
	tex->height = img->GetHeight();
	tex->potWidth = tex->width;
	tex->potHeight = tex->height;
	return tex;
}

void freeImage(Texture* tex)
{
	if (tex->retainCount > 1)
	{
		tex->retainCount--;
		return;
	}
	delete (Image*)tex->texID;
	delete tex;
}

void drawImage(Texture* tex, float x, float y, int anc,
	float ix, float iy, float iw, float ih,
	float rx, float ry,
	int xyz, float degree, int reverse)
{
	//Image* img = (Image*)_img;
	float w = iw * rx, h = ih * ry;

	switch (anc)
	{
	case TOP | LEFT:										break;
	case TOP | RIGHT:			x -= w;						break;
	case TOP | HCENTER:		x -= w / 2;					break;

	case BOTTOM | LEFT:						y -= h;			break;
	case BOTTOM | RIGHT:		x -= w;		y -= h;			break;
	case BOTTOM | HCENTER:	x -= w / 2; y -= h;			break;

	case VCENTER | LEFT:					y -= h / 2;		break;
	case VCENTER | RIGHT:		x -= w;		y -= h / 2;		break;
	case VCENTER | HCENTER:	x -= w / 2;	y -= h / 2;		break;
	}

	iPoint dstPoint[3] = {
		{x, y}, {x + w, y}, { x, y + h }
	};
	iPoint t = iPointMake(x + w / 2, y + h / 2);
	if (xyz == 0)
	{
		dstPoint[0].y =
		dstPoint[1].y = y + h / 2 - h / 2 * _cos(degree);
		dstPoint[2].y = y + h / 2 + h / 2 * _sin(degree);
	}
	else if (xyz == 1)
	{
		dstPoint[0].x =
		dstPoint[1].x = x + w / 2 - w / 2 * _cos(degree);
		dstPoint[2].x = x + w / 2 + w / 2 * _sin(degree);
	}

	else //if(xyz == 2)
	{
		for (int i = 0; i < 3; i++)
			dstPoint[i] = iPointRotate(dstPoint[i], t, degree);
	}

	if (reverse == REVERSE_NONE);
	else if (reverse == REVERSE_WIDTH) //좌우 반전
	{
		//dstPoint[0]	dstPoint[1]
		//dstPoint[2]	dstPoint[3]

		float t = dstPoint[0].x;
		dstPoint[0].x = dstPoint[1].x;
		dstPoint[1].x = t;

		dstPoint[2].x += w;
	}

	else if (reverse == REVERSE_HEIGHT) //상하 반전
	{
		//dstPoint[0]	dstPoint[1]
		//dstPoint[2]	dstPoint[3]
		float t = dstPoint[0].y;
		dstPoint[0].y = dstPoint[2].y;
		dstPoint[2].y = t;

		dstPoint[1].y += h;	
	}


	ColorMatrix matrix = {
		1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,   _a, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};
	ImageAttributes attr;
	attr.SetColorMatrix(&matrix,
		ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);

	graphics->DrawImage((Image*)tex->texID, (PointF*)dstPoint, 3,
		ix, iy, iw, ih, UnitPixel, &attr);

}
void drawImage(Texture* tex, float x, float y, int anc)
{
	//Image* img = (Image*)_img;

	drawImage(tex, x, y, anc,
		0, 0, tex->width, tex->height,
		1.0f, 1.0f,
		2, 0);
}