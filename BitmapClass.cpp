// BitmapClass.cpp

#include "BitmapClass.h"

Bitmap::Bitmap()
{
	// Initialise member variables
	ZeroMemory( &m_hBitmap, sizeof( m_hBitmap ) );

	// Clear width and height
	m_nWidth	= 0;
	m_nHeight	= 0;

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

		// Update member variables
		m_nWidth	= nWidth;
		m_nHeight	= nHeight;

		// Update return value
		bResult = TRUE;

	} // End of successfully created bitmap

	return bResult;

} // End of function Bitmap::Create

int Bitmap::GetHeight()
{
	// Get height
	return m_nHeight;

} // End of function Bitmap::GetHeight

int Bitmap::GetWidth()
{
	// Get width
	return m_nWidth;

} // End of function Bitmap::GetWidth

BOOL Bitmap::Load( HWND hWnd, LPCTSTR lpszFileName )
{
	BOOL bResult = FALSE;

	HANDLE hFile;

	// Open file
	hFile = CreateFile( lpszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );

	// Ensure that file was opened
	if( hFile != INVALID_HANDLE_VALUE )
	{
		// Successfully opened file
		DWORD dwFileSize;

		// Get file size
		dwFileSize = GetFileSize( hFile, NULL );

		// Ensure that file size was got
		if( dwFileSize != INVALID_FILE_SIZE )
		{
			// Successfully got file size
			HGLOBAL hGlobal;

			// Allocate global memory
			hGlobal = GlobalAlloc( GPTR, dwFileSize );

			// Ensure that global memory was allocated
			if( hGlobal )
			{
				// Successfully allocated global memory
				LPVOID lpGlobal;

				// Get pointer to global memory
				lpGlobal = ( LPVOID )hGlobal;

				// Read file
				if( ReadFile( hFile, lpGlobal, dwFileSize, NULL, NULL ) )
				{
					// Successfully read file
					IStream *lpStream;

					// Create stream
					CreateStreamOnHGlobal( hGlobal, false, &lpStream );

					// Ensure that stream was created
					if( lpStream )
					{
						// Successfully created stream
						IPicture *lpPicture;

						// Load picture
						OleLoadPicture( lpStream, 0, false, IID_IPicture, ( LPVOID * )&lpPicture );

						// Ensure that picture was loaded
						if( lpPicture )
						{
							// Successfully loaded picture
							HBITMAP hBitmapTemporary = 0;

							// Store picture to temporary bitmap handle
							lpPicture->get_Handle( ( LPUINT )&hBitmapTemporary );

							// Copy image from temporary bitmap handle into member variable
							m_hBitmap = ( HBITMAP )CopyImage( hBitmapTemporary, IMAGE_BITMAP, 0, 0, LR_COPYRETURNORG );

							// Note that the temporary bitmap handle will be destroyed when the picture is released,
							// which is why we can't just copy straight to the member variable

							// Ensure that bitmap is valid
							if( m_hBitmap )
							{
								// Bitmap is valid
								HDC hdcWindow;
								BITMAP bitmap;

								// Get window dc
								hdcWindow = GetDC( hWnd );

								// Create memory dc
								m_hdcMemory = CreateCompatibleDC( hdcWindow );

								// Select bitmap into memory
								SelectObject( m_hdcMemory, m_hBitmap );

								// Copy bitmap handle into a bitmap structure
								GetObject( m_hBitmap, sizeof( bitmap ), &bitmap );

								// Update member variables
								m_nWidth	= bitmap.bmWidth;
								m_nHeight	= bitmap.bmHeight;

								// Update return value
								bResult = TRUE;

							} // End of bitmap is valid

							lpPicture->Release();

						} // End of successfully loaded picture

						// Release stream
						lpStream->Release();

					} // End of successfully created stream

				} // End of successfully read file

				// Free global memory
				GlobalFree( hGlobal );

			} // End of successfully allocated global memory

		} // End of successfully got file size

		// Close file
		CloseHandle( hFile );

	} // End of successfully opened file

	return bResult;

} // End of function Bitmap::Load

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

BOOL Bitmap::PlaceBitmap( Bitmap sourceBitmap, int nLeft, int nTop )
{
	// Place source bitmap
	return BitBlt( m_hdcMemory, nLeft, nTop, sourceBitmap.m_nWidth, sourceBitmap.m_nHeight, sourceBitmap.m_hdcMemory, 0, 0, SRCCOPY );

} // End of function Bitmap::PlaceBitmap

BOOL Bitmap::Save( LPCTSTR lpszFileName )
{
	BOOL bResult = FALSE;

	LPBITMAPINFO lpBitmapInformation = NULL;

	BITMAP bmp;

	// Convert bitmap handle into a bitmap structure
	if( GetObject( m_hBitmap, sizeof( BITMAP ), ( LPSTR )&bmp ) )
	{
		// Successfully converted bitmap handle into a bitmap structure
		WORD wBitCount;
		LPBITMAPINFOHEADER lpBitmapInformationHeader;
		LPBYTE lpBits;

		// Calculate bit count
		wBitCount = ( WORD )( bmp.bmPlanes * bmp.bmBitsPixel );

		// Ensure that bit count is valid
		if( wBitCount == 1 )
		{
			// Bit count equals 1

			// Keep bit count at 1
			wBitCount = 1;

		} // End of bit count equals 1
		else if( wBitCount <= 4 )
		{
			// Bit count is not greater than 4

			// Set bit count to 4
			wBitCount = 4;

		} // End of bit count is not greater than 4
		else if( wBitCount <= 8)
		{
			// Bit count is not greater than 8

			// Set bit count to 8
			wBitCount = 8;

		} // End of bit count is not greater than 8
		else if( wBitCount <= 16)
		{
			// Bit count is not greater than 16

			// Set bit count to 16
			wBitCount = 16;

		} // End of bit count is not greater than 16
		else if( wBitCount <= 24)
		{
			// Bit count is not greater than 24

			// Set bit count to 24
			wBitCount = 24;

		} // End of bit count is not greater than 24
		else
		{
			// Bit count is greater than 24

			// Set bit count to 32
			wBitCount = 32;

		} // End of bit count is greater than 24

		// See if an rgb quad structure is required
		if( wBitCount < 24 )
		{
			// Bit count is less than 24, so an rgb quad structure is required

			// Allocate memory for bitmap information structure
			lpBitmapInformation = ( LPBITMAPINFO )LocalAlloc( LPTR, ( sizeof( BITMAPINFOHEADER ) + ( sizeof(RGBQUAD) * ( 1 << wBitCount ) ) ) );

		} // End of bit count is less than 24, so an rgb quad structure is required
		else
		{
			// Bit count is greater than or equal to 24, so no rgb quad structure is required

			// Allocate memory for bitmap information structure
			lpBitmapInformation = ( LPBITMAPINFO )LocalAlloc( LPTR, sizeof( BITMAPINFOHEADER ) );

		} // End of bit count is greater than or equal to 24, so no rgb quad structure is required

		// Clear bitmap information structure
		ZeroMemory( lpBitmapInformation, sizeof( *lpBitmapInformation ) );

		// Initialise bitmap information structure
		lpBitmapInformation->bmiHeader.biSize			= sizeof( BITMAPINFOHEADER );
		lpBitmapInformation->bmiHeader.biWidth			= bmp.bmWidth;
		lpBitmapInformation->bmiHeader.biHeight			= bmp.bmHeight;
		lpBitmapInformation->bmiHeader.biPlanes			= bmp.bmPlanes;
		lpBitmapInformation->bmiHeader.biBitCount		= bmp.bmBitsPixel;
		lpBitmapInformation->bmiHeader.biCompression	= BI_RGB;
		lpBitmapInformation->bmiHeader.biSizeImage		= ( ( ( ( lpBitmapInformation->bmiHeader.biWidth * wBitCount +31 ) & ~31 ) / 8 ) * lpBitmapInformation->bmiHeader.biHeight );
		lpBitmapInformation->bmiHeader.biClrImportant	= 0;

		// See if number of colors used is relevant
		if( wBitCount < 24 )
		{
			// Bit count is greater than 24, so number of colors used is relevant

			// Calculate number of colors used
			lpBitmapInformation->bmiHeader.biClrUsed = ( 1 << wBitCount );

		} // End of bit count is greater than 24, so number of colors used is relevant

		// Get pointer to bitmap information header
		lpBitmapInformationHeader = ( LPBITMAPINFOHEADER )lpBitmapInformation;

		// Allocate bit mempry
		lpBits = ( LPBYTE )GlobalAlloc( GMEM_FIXED, lpBitmapInformationHeader->biSizeImage );

		// Ensure that bit memory was allocated
		if( lpBits )
		{
			// Successfully allocated bit memory

			// Get bit data
			if( GetDIBits( m_hdcMemory, m_hBitmap, 0, ( WORD )( lpBitmapInformationHeader->biHeight ), lpBits, lpBitmapInformation, DIB_RGB_COLORS ) )
			{
				// Successfully got bit data
				HANDLE hFile;

				// Create file
				hFile = CreateFile( lpszFileName, ( GENERIC_READ | GENERIC_WRITE ), ( DWORD ) 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, ( HANDLE )NULL );

				// Ensure that file was created
				if( hFile != INVALID_HANDLE_VALUE )
				{
					// Successfully created file
					BITMAPFILEHEADER bitmapFileHeader;
					DWORD dwBitmapFileHeaderOffsetBits;
					DWORD dwBitmapFileHeaderSize;

					// Calculate offset bits
					dwBitmapFileHeaderOffsetBits = ( sizeof( BITMAPFILEHEADER ) + lpBitmapInformationHeader->biSize + ( lpBitmapInformationHeader->biClrUsed * sizeof ( RGBQUAD ) ) );

					// Calculate bitmap file header size
					dwBitmapFileHeaderSize = ( dwBitmapFileHeaderOffsetBits + lpBitmapInformationHeader->biSizeImage );

					// Clear bitmap file header
					ZeroMemory( &bitmapFileHeader, sizeof( bitmapFileHeader ) );

					// Initialise bitmap file header
					bitmapFileHeader.bfType			= 0x4d42;
					bitmapFileHeader.bfSize			= dwBitmapFileHeaderSize;
					bitmapFileHeader.bfReserved1	= 0;
					bitmapFileHeader.bfReserved2	= 0;
					bitmapFileHeader.bfOffBits		= dwBitmapFileHeaderOffsetBits;

					// Write bitmap file header to file
					if( WriteFile( hFile, ( LPVOID )&bitmapFileHeader, sizeof( BITMAPFILEHEADER ), NULL,  NULL ) )
					{
						// Successfully wrote bitmap file header to file

						// Write bitmap information header and rgb quad array to file
						if( WriteFile( hFile, ( LPVOID )lpBitmapInformationHeader, sizeof( BITMAPINFOHEADER )+ lpBitmapInformationHeader->biClrUsed * sizeof (RGBQUAD), NULL, NULL ) )
						{
							// Successfully wrote bitmap information header and rgb quad array to file

							// Write bit data to file
							if( WriteFile( hFile, ( LPSTR )lpBits, ( int )lpBitmapInformationHeader->biSizeImage, NULL, NULL ) )
							{
								// Successfully wrote bit data to file

								// Update return value
								bResult = TRUE;

							} // End of successfully wrote bit data to file

						} // End of successfully wrote bitmap information header and rgb quad array to file

					} // End of successfully wrote bitmap file header to file

					// Close file
					CloseHandle( hFile );

				} // End of successfully created file

			} // End of successfully got bit data

			// Free bit memory
			GlobalFree( ( HGLOBAL )lpBits );

		} // End of successfully allocated bit memory

	} // End of successfully converted bitmap handle into a bitmap structure

	return bResult;

} // End of function Bitmap::Save

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
