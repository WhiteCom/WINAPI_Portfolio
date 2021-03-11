﻿#include "iStd.h"

#include "iWindow.h"

static int keys = 0;

iSize devSize;
iRect viewport;

void setKeyDown(iKeyStat stat, int c)
{
	if (stat == iKeyStatBegan)
	{
		if (c == 'a' || c == 'A')
			keys |= keysA;
		else if (c == 's' || c == 'S')
			keys |= keysS;
		else if (c == 'd' || c == 'D')
			keys |= keysD;
		else if (c == 'w' || c == 'W')
			keys |= keysW;
		else if (c == ' ')
			keys |= keysSpace;
	}
	
	else if (stat == iKeyStatEnded)
	{
		if (c == 'a' || c == 'A')
			keys &= ~keysA;
		else if (c == 's' || c == 'S')
			keys &= ~keysS;
		else if (c == 'd' || c == 'D')
			keys &= ~keysD;
		else if (c == 'w' || c == 'W')
			keys &= ~keysW;
		else if (c == ' ')
			keys &= ~keysSpace;
	}
}

int getKeyDown()
{
	return keys;
}

void appInitialize()
{
	srand(time(NULL));
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

int igImageWidth(igImage* ig)
{
	return ((Image*)ig)->GetWidth();
}
int igImageHeight(igImage* ig)
{
	return ((Image*)ig)->GetHeight();
}

igImage* createImage(const char* szFormat, ...)
{
	char szText[1024];
	va_start_end(szText, szFormat);

	wchar_t* ws = utf8_to_utf16(szText);
	Image* img = Image::FromFile(ws, false);
	delete ws;

	return img;
}
void freeImage(igImage* img)
{
	delete (Image*)img;
}

void drawImage(igImage* _img, float x, float y, int anc,
	float ix, float iy, float iw, float ih,
	float rx, float ry,
	int xyz, float degree)
{
	Image* img = (Image*)_img;
	float w = img->GetWidth() * rx, h = img->GetHeight() * ry;

	switch (anc)
	{
	case TOP | LEFT:										break;
	case TOP | RIGHT:			x -= w;						break;
	case TOP | HORIZONTAL:		x -= w / 2;					break;

	case BOTTOM | LEFT:						y -= h;			break;
	case BOTTOM | RIGHT:		x -= w;		y -= h;			break;
	case BOTTOM | HORIZONTAL:	x -= w / 2; y -= h;			break;

	case VERTICAL | LEFT:					y -= h / 2;		break;
	case VERTICAL | RIGHT:		x -= w;		y -= h / 2;		break;
	case VERTICAL | HORIZONTAL:	x -= w / 2;	y -= h / 2;		break;
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

	graphics->DrawImage(img, (PointF*)dstPoint, 3,
		ix, iy, iw, ih, UnitPixel, &attr);

}
void drawImage(igImage* _img, float x, float y, int anc)
{
	Image* img = (Image*)_img;

	drawImage(img, x, y, anc,
		0, 0, img->GetWidth(), img->GetHeight(),
		1.0f, 1.0f,
		2, 0);
}
