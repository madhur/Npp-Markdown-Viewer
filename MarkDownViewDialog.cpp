//this file is part of Function List Plugin for Notepad++
//Copyright (C)2006 Jens Lorenz <jens.plugin.npp@gmx.de>
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

#include "MarkDownViewDialog.h"
#include "MarkdownDialog.h"
extern "C"
{
#include "cwebpage.h"
}
// #include "ColumnDialog.h"
// #include "ModifyMenu.h"
#include "Scintilla.h"
#include "resource.h"

#include <windows.h>
#include <commctrl.h>


extern char	hexMask[256][3];





MarkDownViewDialog::MarkDownViewDialog(void)
{
	
}

MarkDownViewDialog::~MarkDownViewDialog(void)
{
}

void MarkDownViewDialog::doDialog(BOOL toggle)
{
   // if (_pCurProp == NULL)
	//	return;

	/* toggle view if user requested */
	

		MoveView();

	//::ShowWindow(this->_hSelf, 5);
}



void MarkDownViewDialog::init(HINSTANCE hInst, NppData nppData, LPCTSTR iniFilePath)
{
	_nppData = nppData;
	Window::init(hInst, nppData._nppHandle);
	

	tMarkProp hexProp={};
	hexProp.isModified=FALSE;
	hexProp.isVisible=TRUE;
	//_tcscpy(hexProp.szFileName,"madhur");
	_pCurProp=&hexProp;
	
}


void MarkDownViewDialog::MoveView(void)
{
	HINSTANCE cwebdll;
	//if (_openDoc == -1)
//{
	//	::ShowWindow(_hParentHandle, SW_HIDE);
	//	::ShowWindow(_hSelf, SW_HIDE);
		// ::ShowWindow(_hListCtrl, SW_HIDE);
//	}
	//else
	//{
		//if ((_pCurProp != NULL) && (_pCurProp->isVisible == TRUE))
	//	{
		/*	RECT	rc;
			
			::GetWindowRect(_hParentHandle, &rc);
			ScreenToClient(_nppData._nppHandle, &rc);

			INT	iNewHorDiff = rc.right - rc.left;
			INT	iNewVerDiff = rc.bottom - rc.top;

			::SetWindowPos(_hSelf, NULL, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, FALSE);*/

	
    if (!isCreated())
	{
		create(IDD_MARK_DLG, false, false);
	}

		    if (::IsWindowVisible(_hParentHandle)==TRUE)
			{

				

				 
			
			::ShowWindow(_hSelf, SW_SHOW);
			//::ShowWindow(_hListCtrl, SW_SHOW);
			::ShowWindow(_hParentHandle, SW_HIDE);
			_pCurProp->isVisible=FALSE;
			}

			else if (::IsWindowVisible(_hParentHandle) == FALSE)
			{
		//	_iOldHorDiff = iNewHorDiff;
		//	_iOldVerDiff = iNewVerDiff;
	//	}
		//else if (::IsWindowVisible(_hParentHandle) == FALSE)
		//{
			::ShowWindow(_hParentHandle, SW_SHOW);
			::SetFocus(_hParentHandle);
			::ShowWindow(_hSelf, SW_HIDE);
			_pCurProp->isVisible=TRUE;
			
			//::ShowWindow(_hListCtrl, SW_HIDE);
			}
		//}
	
}

BOOL CALLBACK MarkDownViewDialog::run_dlgProc( UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message) 
	{
		case WM_INITDIALOG:
		{
				// EmbedBrowserObject(_hSelf);
			//	DisplayHTMLStr(_hSelf, L"<H2><CENTER>HTML string test</CENTER></H2><P><FONT COLOR=RED>This is a <U>HTML string</U> in memory.</FONT>");
			//if (EmbedBrowserObject(this->_hSelf)) return(-1);

		// Another window created with an embedded browser object
		// ++WindowCount;

		// Success
		return(TRUE);
			//EmbedBrowserObject(this->_hSelf);
			//DisplayHTMLPage(this->_hSelf,L"http://www.microsoft.com");
			//DisplayHTMLStr(this->_hSelf, L"<H2><CENTER>HTML string test</CENTER></H2><P><FONT COLOR=RED>This is a <U>HTML string</U> in memory.</FONT>");
			
			break;
		}

	
		

		default:
			break;
	}

	return FALSE;
}
