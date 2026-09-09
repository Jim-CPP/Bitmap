// BitmapClass.h

#pragma once

#include <windows.h>
#include <ocidl.h>
#include <olectl.h>

#define BITMAP_DEFAULT_BACKGROUND_RED											0xFF
#define BITMAP_DEFAULT_BACKGROUND_GREEN											0xFF
#define BITMAP_DEFAULT_BACKGROUND_BLUE											0xFF

class Bitmap
{
public:
	Bitmap();
	~Bitmap();

	Bitmap& operator = ( HBITMAP hBitmap );

	BOOL operator == ( HBITMAP hBitmap );

	BOOL operator != ( HBITMAP hBitmap );

	operator HBITMAP();

	BOOL Create( HWND hWnd, int nWidth, int nHeight, BYTE bRed = BITMAP_DEFAULT_BACKGROUND_RED, BYTE bGreen = BITMAP_DEFAULT_BACKGROUND_GREEN, BYTE bBlue = BITMAP_DEFAULT_BACKGROUND_BLUE );

	int GetHeight();

	int GetWidth();

	BOOL Load( HWND hWnd, LPCTSTR lpszFileName );

	BOOL Paint( HWND hWnd );

	BOOL Save( LPCTSTR lpszFileName );

	BOOL Update( LPARAM lParam, BOOL( *lpUpdateFunction )( HDC hdc, LPARAM lParam ) );

protected:
	HBITMAP m_hBitmap;
	HDC m_hdcMemory;
	int m_nWidth;
	int m_nHeight;

}; // End of class Bitmap
