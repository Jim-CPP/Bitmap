// Bitmap.cpp

#include "Bitmap.h"

// Global variables
Bitmap g_bitmap;

BOOL BitmapUpdateFunction( HDC hdc, LPARAM lParam )
{
	BOOL bResult;

	int nRectangleLeft;
	int nRectangleTop;
	int nRectangleRight;
	int nRectangleBottom;
	HPEN hPen;
	HBRUSH hBrush;

	// Create pen
	hPen = CreatePen( PS_SOLID, 0, RGB( 255, 0, 0 ) );

	// Create brush
	hBrush = CreateSolidBrush( RGB( 0, 255, 0 ) );

	// Select pen into memory
	SelectObject( hdc, hPen );

	// Select brush into memory
	SelectObject( hdc, hBrush );

	// Store mouse position
	nRectangleLeft		= LOWORD( lParam );
	nRectangleTop		= HIWORD( lParam );
	nRectangleRight		= ( nRectangleLeft + 100 );
	nRectangleBottom	= ( nRectangleTop + 100 );

	// Draw rectangle into memory
	bResult = Rectangle( hdc, nRectangleLeft, nRectangleTop, nRectangleRight, nRectangleBottom );

	// Delete pen
	DeleteObject( hPen );

	// Delete brush
	DeleteObject( hBrush );

	return bResult;

} // End of function BitmapUpdateFunction

LRESULT CALLBACK MainWindowProcedure( HWND hWndMain, UINT uMessage, WPARAM wParam, LPARAM lParam )
{
	LRESULT lResult = 0;

	// Select message
	switch ( uMessage )
	{
		case WM_CREATE:
		{
			// A create message

			// Load bitmap
			if( !( g_bitmap.Load( hWndMain, "House.jpg" ) ) )
			{
				// Unable to load bitmap

				// Create bitmap
				g_bitmap.Create( hWndMain, DEFAULT_WIDTH, DEFAULT_HEIGHT );

			} // End of unable to load bitmap

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

			// Paint bitmap onto windoe
			g_bitmap.Paint( hWndMain );

			// Break out of switch
			break;

		} // End of a paint message
		case WM_LBUTTONDOWN:
		{
			// A left button down message

			// Update bitmap
			g_bitmap.Update( lParam, &BitmapUpdateFunction );

			// force repaint
			InvalidateRect( hWndMain, NULL, TRUE);

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
		case WM_CLOSE:
		{
			// A close message

			// Save bitmap
			g_bitmap.Save( SAVE_FILE_NAME );

			// Destroy main window
			DestroyWindow( hWndMain );

			// Break out of switch
			break;

		} // End of a close message
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
