// Bitmap.cpp

#include "Bitmap.h"

LRESULT CALLBACK MainWindowProcedure( HWND hWndMain, UINT uMessage, WPARAM wParam, LPARAM lParam )
{
	LRESULT lResult = 0;

	// Static variables
	static HDC s_hdcMemory;
	static HBITMAP s_hBitmap;

	// Select message
	switch ( uMessage )
	{
		case WM_CREATE:
		{
			// A create message
			HBRUSH hBrushOriginal;
			HBRUSH hBrushBackground;
			int nScreenWidth;
			int nScreenHeight;
			HDC hdcWindow;

			// Store screen size
			nScreenWidth	= GetSystemMetrics( SM_CXSCREEN );
			nScreenHeight	= GetSystemMetrics( SM_CYSCREEN );

			// Get window dc
			hdcWindow = GetDC( hWndMain );

			// Create memory dc
			s_hdcMemory = CreateCompatibleDC( hdcWindow );

			// Create bitmap
			s_hBitmap = CreateCompatibleBitmap( hdcWindow, nScreenWidth, nScreenHeight );

			// Select bitmap into memory
			SelectObject( s_hdcMemory, s_hBitmap );

			// Create background brush
			hBrushBackground = CreateSolidBrush( RGB( 255, 255, 255 ) );

			// save old brush and select new brush
			hBrushOriginal = ( HBRUSH )SelectObject( s_hdcMemory, hBrushBackground );

			// Paint background
			PatBlt( s_hdcMemory, 0, 0, nScreenWidth, nScreenHeight, PATCOPY );

			// Select original brush back into memory
			SelectObject( s_hdcMemory, hBrushOriginal );

			// Release window dc
			ReleaseDC( hWndMain, hdcWindow );

			// Break out of switch
			break;

		} // End of a create message
		case WM_GETMINMAXINFO:
		{
			// A get min max info message
			MINMAXINFO FAR *lpMinMaxInfo;

			// Get min max info structure
			lpMinMaxInfo = ( MINMAXINFO FAR * )lParam;

			// Update min max info structure
			lpMinMaxInfo->ptMinTrackSize.x = MAIN_WINDOW_MINIMUM_WIDTH;
			lpMinMaxInfo->ptMinTrackSize.y = MAIN_WINDOW_MINIMUM_HEIGHT;

			// Break out of switch
			break;

		} // End of a get min max info message
		case WM_PAINT:
		{
			// A paint message
			PAINTSTRUCT ps;
			int nPaintWidth;
			int nPaintHeight;

			// Begin painting
			HDC hdcWindow = BeginPaint( hWndMain, &ps );

			// Store paint size
			nPaintWidth		= ( ps.rcPaint.right - ps.rcPaint.left );
			nPaintHeight	= ( ps.rcPaint.bottom - ps.rcPaint.top );

			// Paint from memory onto window
			BitBlt( hdcWindow, ps.rcPaint.left, ps.rcPaint.top, nPaintWidth, nPaintHeight, s_hdcMemory, ps.rcPaint.left, ps.rcPaint.top, SRCCOPY );

			// End painting
			EndPaint( hWndMain, &ps );

			// Break out of switch
			break;

		} // End of a paint message
		case WM_LBUTTONDOWN:
		{
			// A left button down message
			int nOriginalDc;
			int nRectangleLeft;
			int nRectangleTop;
			int nRectangleRight;
			int nRectangleBottom;
			HPEN hPen;
			HBRUSH hBrush;

			// save the current dc state
			nOriginalDc = SaveDC( s_hdcMemory );

			// Create pen
			hPen = CreatePen( PS_SOLID, 0, RGB( 255, 0, 0 ) );

			// Create brush
			hBrush = CreateSolidBrush( RGB( 0, 255, 0 ) );

			// Select pen into memory
			SelectObject( s_hdcMemory, hPen );

			// Select brush into memory
			SelectObject( s_hdcMemory, hBrush );

			// Store mouse position
			nRectangleLeft		= LOWORD( lParam );
			nRectangleTop		= HIWORD( lParam );
			nRectangleRight		= ( nRectangleLeft + 100 );
			nRectangleBottom	= ( nRectangleTop + 100 );

			// Draw rectangle into memory
			Rectangle( s_hdcMemory, nRectangleLeft, nRectangleTop, nRectangleRight, nRectangleBottom );

			// force repaint
			InvalidateRect( hWndMain, NULL, TRUE);

			// Restore original dc into memory
			RestoreDC( s_hdcMemory, nOriginalDc );

			// Delete pen
			DeleteObject( hPen );

			// Delete brush
			DeleteObject( hBrush );

			// Break out of switch
			break;

		} // End of a left button down message
		case WM_DESTROY:
		{
			// A destroy message

			// Terminate thread
			PostQuitMessage( 0 );

			// Break out of switch
			break;

		} // End of a destroy message
		default:
		{
			// Default message

			// Call the default window procedure
			lResult = DefWindowProc( hWndMain, uMessage, wParam, lParam );

			// Break out of switch
			break;

		} // End of default message

	}; // End of selection for message

	return lResult;

} // End of function MainWindowProcedure

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPTSTR, int nCmdShow )
{
	MSG msg;

	WNDCLASSEX wcMain;

	// Clear message structure
	ZeroMemory( &msg, sizeof( msg ) );

	// Clear main window class structure
	ZeroMemory( &wcMain, sizeof( wcMain ) );

	// Initialise main window class structure
	wcMain.cbSize			= sizeof( WNDCLASSEX );
	wcMain.hInstance		= hInstance;
	wcMain.lpfnWndProc		= MainWindowProcedure;
	wcMain.lpszClassName	= MAIN_WINDOW_CLASS_NAME;
	wcMain.style			= MAIN_WINDOW_CLASS_STYLE;
	wcMain.hIcon			= MAIN_WINDOW_CLASS_ICON;
	wcMain.hIconSm			= MAIN_WINDOW_CLASS_ICON_SMALL;
	wcMain.hCursor			= MAIN_WINDOW_CLASS_CURSOR;
	wcMain.lpszMenuName		= MAIN_WINDOW_CLASS_MENU_NAME;
	wcMain.hbrBackground	= MAIN_WINDOW_CLASS_BACKGROUND;

	// Register main window class
	if( RegisterClassEx( &wcMain ) )
	{
		// Successfully registered main window class
		HWND hWndMain;

		// Create main window
		hWndMain = CreateWindowEx( MAIN_WINDOW_EXTENDED_STYLE, MAIN_WINDOW_CLASS_NAME, MAIN_WINDOW_TEXT, MAIN_WINDOW_STYLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, HWND_DESKTOP, NULL, hInstance, NULL );

		// Ensure that main window was created
		if( hWndMain )
		{
			// Successfully created main window

			// Show main window
			ShowWindow ( hWndMain, nCmdShow );

			// Main message loop
			while( GetMessage( &msg, NULL, 0, 0 ) )
			{
				// Translate message
				TranslateMessage( &msg );

				// Dispatch message
				DispatchMessage( &msg );

			}; // End of main message loop

		} // End of successfully created main window

	} // End of successfully registered main window class

	return msg.wParam;

} // End of function WinMain
