// BitmapClass.cpp

#include "BitmapClass.h"

Bitmap::Bitmap()
{
	// Initialise member variables
	ZeroMemory( &m_hBitmap, sizeof( m_hBitmap ) );

} // End of function Bitmap::Bitmap

Bitmap::~Bitmap()
{
	// Clear member variables
	ZeroMemory( &m_hBitmap, sizeof( m_hBitmap ) );

} // End of function Bitmap::~Bitmap

Bitmap& Bitmap::operator = ( HBITMAP hBitmap )
{
	// Update member variables
	m_hBitmap = hBitmap;

	return *this;

} // End of function Bitmap::operator =

BOOL Bitmap::operator == ( HBITMAP hBitmap )
{
	BOOL bResult = FALSE;

	// See if item equals member item
	if( hBitmap == m_hBitmap )
	{
		// Item equals member item

		// Update return value
		bResult = TRUE;

	} // End of item equals member item

	return bResult;

} // End of function Bitmap::operator ==

BOOL Bitmap::operator != ( HBITMAP hBitmap )
{
	BOOL bResult = FALSE;

	// See if item is different to member item
	if( hBitmap != m_hBitmap )
	{
		// Item is different to member item

		// Update return value
		bResult = TRUE;

	} // End of item is different to member item

	return bResult;

} // End of function Bitmap::operator !=

Bitmap::operator HBITMAP()
{
	return m_hBitmap;

} // End of function Bitmap::operator HBITMAP()

BOOL Bitmap::Create( HWND hWnd, int nWidth, int nHeight, BYTE bRed, BYTE bGreen, BYTE bBlue )
{
	BOOL bResult = FALSE;

	HBRUSH hBrushOriginal;
	HBRUSH hBrushBackground;
	HDC hdcWindow;

	// Get window dc
	hdcWindow = GetDC( hWnd );

	// Create memory dc
	m_hdcMemory = CreateCompatibleDC( hdcWindow );

	// Create bitmap
	m_hBitmap = CreateCompatibleBitmap( hdcWindow, nWidth, nHeight );

	// Ensure that bitmap was created
	if( m_hBitmap )
	{
		// Successfully created bitmap

		// Select bitmap into memory
		SelectObject( m_hdcMemory, m_hBitmap );

		// Create background brush
		hBrushBackground = CreateSolidBrush( RGB( bRed, bGreen, bBlue ) );

		// Select background brush into memory
		hBrushOriginal = ( HBRUSH )SelectObject( m_hdcMemory, hBrushBackground );

		// Paint background
		PatBlt( m_hdcMemory, 0, 0, nWidth, nHeight, PATCOPY );

		// Select original brush back into memory
		SelectObject( m_hdcMemory, hBrushOriginal );

		// Release window dc
		ReleaseDC( hWnd, hdcWindow );

		// Update return value
		bResult = TRUE;

	} // End of successfully created bitmap

	return bResult;

} // End of function Bitmap::Create

BOOL Bitmap::Paint( HWND hWnd )
{
	BOOL bResult;

	PAINTSTRUCT ps;
	int nPaintWidth;
	int nPaintHeight;

	// Begin painting
	HDC hdcWindow = BeginPaint( hWnd, &ps );

	// Store paint size
	nPaintWidth		= ( ps.rcPaint.right - ps.rcPaint.left );
	nPaintHeight	= ( ps.rcPaint.bottom - ps.rcPaint.top );

	// Paint from memory onto window
	bResult = BitBlt( hdcWindow, ps.rcPaint.left, ps.rcPaint.top, nPaintWidth, nPaintHeight, m_hdcMemory, ps.rcPaint.left, ps.rcPaint.top, SRCCOPY );

	// End painting
	EndPaint( hWnd, &ps );

	return bResult;

} // End of function Bitmap::Paint

BOOL Bitmap::Update( LPARAM lParam, BOOL( *lpUpdateFunction )( HDC hdc, LPARAM lParam ) )
{
	BOOL bResult;

	int nOriginalDc;

	// save the current dc state
	nOriginalDc = SaveDC( m_hdcMemory );

	// Call update function
	bResult = ( *lpUpdateFunction )( m_hdcMemory, lParam );

	// Restore original dc into memory
	RestoreDC( m_hdcMemory, nOriginalDc );

	return bResult;

} // End of function Bitmap::Update

/*
Bitmap::
{
} // End of function Bitmap::
*/
