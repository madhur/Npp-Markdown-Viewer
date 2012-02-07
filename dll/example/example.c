/* example.c
 
This is a Win32 C application (ie, no MFC, WTL, nor even any C++ -- just plain C) that demonstrates
how to embed a browser "control" (actually, an OLE object) in your own window (in order to display a
web page, or an HTML file on disk). The bulk of the OLE/COM code is in DLL.c which creates a DLL that
we use in this simple app. Furthermore, we use LoadLibrary and GetProcAddress, so our DLL is not
actually loaded until/unless we need it.

NOTE: The DLL we create does not normally use UNICODE strings. If you compile this example as UNICODE,
then you should do the same with DLL.C.
*/


#include <windows.h>
#include "..\CWebPage.h"	/* Declarations of the functions in DLL.c */
#include "MarkdownDialog.h"




// A running count of how many windows we have open that contain a browser object
unsigned char WindowCount = 0;

// The class name of our Window to host the browser. It can be anything of your choosing.
static const TCHAR	ClassName[] = L"Browser Example";

// Where we store the pointers to CWebPage.dll's functions
//EmbedBrowserObjectPtr		*lpEmbedBrowserObject;
//UnEmbedBrowserObjectPtr		*lpUnEmbedBrowserObject;
//DisplayHTMLPagePtr			*lpDisplayHTMLPage;
//DisplayHTMLStrPtr			*lpDisplayHTMLStr;




/****************************** WindowProc() ***************************
 * Our message handler for our window to host the browser.
 */

LRESULT CALLBACK WindowProc1(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	
	
	// NOTE: If you want to resize the area that the browser object occupies when you
	// resize the window, then handle WM_SIZE and use the IWebBrowser2's put_Width()
	// and put_Height() to give it the new dimensions.

	 return(DefWindowProc(hwnd, uMsg, wParam, lParam));
}




/****************************** WindowProc() ***************************
 * Our message handler for our window to host the browser.
 */

INT_PTR CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_INITDIALOG)
	{
		// Embed the browser object into our host window. We need do this only
		// once. Note that the browser object will start calling some of our
		// IOleInPlaceFrame and IOleClientSite functions as soon as we start
		// calling browser object functions in EmbedBrowserObject().
		
			//	EmbedBrowserObject(hwnd);
	//	DisplayHTMLPage(hwnd,L"http://www.microsoft.com");
	//	DisplayHTMLStr(msg.hwnd, L"<H2><CENTER>HTML string test</CENTER></H2><P><FONT COLOR=RED>This is a <U>HTML string</U> in memory.</FONT>");

		

	//	(*lpDisplayHTMLStr)(hwnd, "<H2><CENTER>HTML string test</CENTER></H2><P><FONT COLOR=RED>This is a <U>HTML string</U> in memory.</FONT>");
		// Another window created with an embedded browser object
		++WindowCount;

		// Success
			return(TRUE);
		
	}

	if (uMsg == WM_DESTROY)
	{
		// Detach the browser object from this window, and free resources.
	//	(*lpUnEmbedBrowserObject)(hwnd);

		// One less window
		--WindowCount;

		// If all the windows are now closed, quit this app
		if (!WindowCount) PostQuitMessage(0);

		return(TRUE);
	}

	return FALSE;
	// NOTE: If you want to resize the area that the browser object occupies when you
	// resize the window, then handle WM_SIZE and use the IWebBrowser2's put_Width()
	// and put_Height() to give it the new dimensions.

	// return(DefWindowProc(hwnd, uMsg, wParam, lParam));
}





/****************************** WinMain() ***************************
 * C program entry point.
 *
 * This creates a window to host the web browser, and displays a web
 * page.
 */

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hInstNULL, LPSTR lpszCmdLine, int nCmdShow)
{
	//HINSTANCE		cwebdll;
	MSG				msg;
	WNDCLASSEX		wc;
	int i;
	HWND hwnd;

	// Load our DLL containing the OLE/COM code. We do this once-only. It's named "cwebpage.dll"
	// if ((cwebdll = LoadLibrary(L"cwebpage.dll")))
	// {
		// Get pointers to the EmbedBrowserObject, DisplayHTMLPage, DisplayHTMLStr, and UnEmbedBrowserObject
		// functions, and store them in some globals.

		// Get the address of the EmbedBrowserObject() function. NOTE: Only Reginald has this one
		//lpEmbedBrowserObject = (EmbedBrowserObjectPtr *)GetProcAddress((HINSTANCE)cwebdll, "EmbedBrowserObject");

		//// Get the address of the UnEmbedBrowserObject() function. NOTE: Only Reginald has this one
		//lpUnEmbedBrowserObject = (UnEmbedBrowserObjectPtr *)GetProcAddress((HINSTANCE)cwebdll, "UnEmbedBrowserObject");

		//// Get the address of the DisplayHTMLPagePtr() function
		//lpDisplayHTMLPage = (DisplayHTMLPagePtr *)GetProcAddress((HINSTANCE)cwebdll, "DisplayHTMLPage");

		//// Get the address of the DisplayHTMLStr() function
		//lpDisplayHTMLStr = (DisplayHTMLStrPtr *)GetProcAddress((HINSTANCE)cwebdll, "DisplayHTMLStr");

		// Register the class of our window to host the browser. 'WindowProc' is our message handler
		// and 'ClassName' is the class name. You can choose any class name you want.
		ZeroMemory(&wc, sizeof(WNDCLASSEX));
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.hInstance = hInstance;
		wc.lpfnWndProc = WindowProc1;
		wc.lpszClassName = &ClassName[0];
		RegisterClassEx(&wc);

	/*	hwnd = CreateWindowEx(0, &ClassName[0], L"An HTML string", WS_OVERLAPPEDWINDOW,
							CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
							HWND_DESKTOP, NULL, hInstance, 0);*/

		msg.hwnd=CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_MARK_DLG),0,(DLGPROC)WindowProc, 0);

		EmbedBrowserObject(msg.hwnd);
	//	DisplayHTMLPage(msg.hwnd,L"http://www.microsoft.com");
		DisplayHTMLStr(msg.hwnd, L"<H2><CENTER>HTML string test</CENTER></H2><P><FONT COLOR=RED>This is a <U>HTML string</U> in memory.</FONT>");
		// Create a window. NOTE: We embed the browser object duing our WM_CREATE handling for
		// this window.
	/*	if ((msg.hwnd = CreateWindowEx(0, &ClassName[0], L"An HTML string", WS_OVERLAPPEDWINDOW,
							CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
							HWND_DESKTOP, NULL, hInstance, 0)))*/

		//     i=GetLastError();
		//{
			// For this window, display a string in the BODY of a web page.
		//	(*lpDisplayHTMLStr)(msg.hwnd, "<H2><CENTER>HTML string test</CENTER></H2><P><FONT COLOR=RED>This is a <U>HTML string</U> in memory.</FONT>");

			// Show the window.
		//	ShowWindow(hwnd, nCmdShow);
			ShowWindow(msg.hwnd, nCmdShow);
			//UpdateWindow(msg.hwnd);
	//	}

		// Create another window with another browser object embedded in it.
	

		// Do a message loop until WM_QUIT.
		while (GetMessage(&msg, 0, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Free the DLL.
	//	FreeLibrary(cwebdll);

		return(0);
	//}

	MessageBox(0, L"Can't open cwebpage.dll!", L"ERROR", MB_OK);
	return(-1);
}
