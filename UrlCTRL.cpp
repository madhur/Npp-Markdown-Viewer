//this file is part of notepad++
//Copyright (C)2003 Don HO ( donho@altern.org )
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include "URLCtrl.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
// #include <tchar.h>
#include <Windows.h>

static BYTE XORMask[128] =
{
  0xff, 0xff, 0xff, 0xff,
  0xf9, 0xff, 0xff, 0xff, 
  0xf0, 0xff, 0xff, 0xff, 
  0xf0, 0xff, 0xff, 0xff, 
  0xf0, 0xff, 0xff, 0xff,
  0xf0, 0xff, 0xff, 0xff, 
  0xf0, 0x24, 0xff, 0xff, 
  0xf0, 0x00, 0x7f, 0xff, 
  0xc0, 0x00, 0x7f, 0xff,
  0x80, 0x00, 0x7f, 0xff, 
  0x80, 0x00, 0x7f, 0xff, 
  0x80, 0x00, 0x7f, 0xff, 
  0x80, 0x00, 0x7f, 0xff,
  0x80, 0x00, 0x7f, 0xff, 
  0xc0, 0x00, 0x7f, 0xff, 
  0xe0, 0x00, 0x7f, 0xff, 
  0xf0, 0x00, 0xff, 0xff,
  0xf0, 0x00, 0xff, 0xff, 
  0xf0, 0x00, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 
};

/* AND mask for hand cursor */
/* Generated by HexEdit */
static BYTE ANDMask[128] =
{
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 
  0x06, 0x00, 0x00, 0x00, 
  0x06, 0x00, 0x00, 0x00, 
  0x06, 0x00, 0x00, 0x00,
  0x06, 0x00, 0x00, 0x00, 
  0x06, 0x00, 0x00, 0x00, 
  0x06, 0xdb, 0x00, 0x00,
  0x06, 0xdb, 0x00, 0x00, 
  0x36, 0xdb, 0x00, 0x00, 
  0x36, 0xdb, 0x00, 0x00, 
  0x37, 0xff, 0x00, 0x00, 
  0x3f, 0xff, 0x00, 0x00,
  0x3f, 0xff, 0x00, 0x00, 
  0x1f, 0xff, 0x00, 0x00, 
  0x0f, 0xff, 0x00, 0x00, 
  0x07, 0xfe, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 
};


void URLCtrl::create(HWND itemHandle, LPTSTR link, COLORREF linkColor)
{
	// turn on notify style
    ::SetWindowLongPtr(itemHandle, GWL_STYLE, ::GetWindowLong(itemHandle, GWL_STYLE) | SS_NOTIFY);

	// set the URL text (not the display text)
	if (link)
		lstrcpy(_URL, link);

	// set the hyperlink colour
    _linkColor = linkColor;

	// set the visited colour
	_visitedColor = RGB(128,0,128);

	// subclass the static control
    _oldproc = (WNDPROC)::SetWindowLongPtr(itemHandle, GWL_WNDPROC, (LONG)URLCtrlProc);

	// associate the URL structure with the static control
    ::SetWindowLongPtr(itemHandle, GWL_USERDATA, (LONG)this);
}

LRESULT URLCtrl::runProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) 
{
    switch(Message)
    {
	    // Free up the structure we allocated
	    case WM_NCDESTROY:
		    //HeapFree(GetProcessHeap(), 0, url);
		    break;
    	
	    // Paint the static control using our custom
	    // colours, and with an underline text style
	    case WM_PAINT:
        {
		    DWORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);
		    DWORD dwDTStyle = DT_SINGLELINE;
    		
		    //Test if centered horizontally or vertically
		    if(dwStyle & SS_CENTER)	     dwDTStyle |= DT_CENTER;
		    if(dwStyle & SS_RIGHT)		 dwDTStyle |= DT_RIGHT;
		    if(dwStyle & SS_CENTERIMAGE) dwDTStyle |= DT_VCENTER;

	        RECT		rect;
            ::GetClientRect(hwnd, &rect);

            PAINTSTRUCT ps;
            HDC hdc = ::BeginPaint(hwnd, &ps);
    		
            ::SetTextColor(hdc, _linkColor);
            ::SetBkColor  (hdc, ::GetSysColor(COLOR_3DFACE));
    		
		    // Create an underline font 
		    if(_hfUnderlined == 0)
		    {
			    // Get the default GUI font
			    LOGFONT lf;
                HFONT hf = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);

			    // Add UNDERLINE attribute
			    GetObject(hf, sizeof lf, &lf);
                lf.lfUnderline = TRUE;
    			
			    // Create a new font
                _hfUnderlined = ::CreateFontIndirect(&lf);
		    }
    		
		    HANDLE hOld = SelectObject(hdc, _hfUnderlined);

		    // Draw the text!
            TCHAR szWinText[_MAX_PATH];
            ::GetWindowText(hwnd, szWinText, sizeof szWinText);
            ::DrawText(hdc, szWinText, -1, &rect, dwDTStyle);
    		
            ::SelectObject(hdc, hOld);

            ::EndPaint(hwnd, &ps);

		    return 0;
        }

	    case WM_SETTEXT:
        {
            LRESULT ret = ::CallWindowProc(_oldproc, hwnd, Message, wParam, lParam);
            ::InvalidateRect(hwnd, 0, 0);
            return ret;
        }
	    // Provide a hand cursor when the mouse moves over us
	    //case WM_SETCURSOR:
        case WM_MOUSEMOVE:
        {
            if (_hCursor == 0)
                _hCursor = ::CreateCursor(::GetModuleHandle(0), 5, 2, 32, 32, XORMask, ANDMask);
    	
            SetCursor(_hCursor);
            return TRUE;
        }
		    
	    case WM_LBUTTONDOWN:
		    _clicking = true;
		    break;

	    case WM_LBUTTONUP:
		    if(_clicking)
		    {
			    _clicking = false;
			    _linkColor = _visitedColor;
    			
                ::InvalidateRect(hwnd, 0, 0);
                ::UpdateWindow(hwnd);

			    // Open a browser
			    if(_URL[0])
			    {
                    ::ShellExecute(NULL, L"open", _URL, NULL, NULL, SW_SHOWNORMAL);
			    }
			    else
			    {
                    TCHAR szWinText[_MAX_PATH];
                    ::GetWindowText(hwnd, szWinText, sizeof szWinText);
                    ::ShellExecute(NULL, L"open", szWinText, NULL, NULL, SW_SHOWNORMAL);
			    }
		    }

		    break;
    		
	    // A standard static control returns HTTRANSPARENT here, which
	    // prevents us from receiving any mouse messages. So, return
	    // HTCLIENT instead.
	    case WM_NCHITTEST:
		    return HTCLIENT;
    }
    return ::CallWindowProc(_oldproc, hwnd, Message, wParam, lParam);
}
