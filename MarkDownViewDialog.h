//this file is part of Hex Edit Plugin for Notepad++
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

#ifndef HEXDLG_DEFINE_H
#define HEXDLG_DEFINE_H

#include "StaticDialog.h"
#include "PluginDefinition.h"
//#include <string>
#include <vector>
//#include <algorithm>
//#include <shlwapi.h>
//#include <zmouse.h>
#include <windows.h>
//#include <windowsx.h>
//#include <commctrl.h>



#include <exdisp.h>		// Defines of stuff like IWebBrowser2. This is an include file with Visual C 6 and above
#include <mshtml.h>		// Defines of stuff like IHTMLDocument2. This is an include file with Visual C 6 and above
#include <crtdbg.h>		// for _ASSERT()


using namespace std;


#define HEX_FIRST_TIME_VIS	(0xFFFFFFFF)

#define DT_HEX_VIEW			(DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS | DT_NOPREFIX)
#define	VIEW_ROW			(_pCurProp->columns * _pCurProp->bits)
#define FACTOR				((_pCurProp->isBin == TRUE)?8:2)
#define	SUBITEM_LENGTH		(_pCurProp->bits * FACTOR)
#define FULL_SUBITEM		((_pCurProp->cursorItem * VIEW_ROW + (_pCurProp->cursorSubItem * _pCurProp->bits)) <= _currLength)
#define DUMP_FIELD			(_pCurProp->columns + 1)




class MarkDownViewDialog : public StaticDialog //, private SciSubClassWrp
{
public:
	MarkDownViewDialog(void);
	~MarkDownViewDialog(void);
    void init(HINSTANCE hInst, NppData nppData, LPCTSTR iniFilePath=NULL);

	   	void doDialog(BOOL toggle=FALSE);

			tMarkProp GetMarkProp(void)
	{
		if (_pCurProp != NULL) {
			//GetLineVis();
			return *_pCurProp;
		} 
		tMarkProp prop;
		return prop;
	};



		void SetMarkProp(tMarkProp prop)
	{
		if (_pCurProp != NULL)
		{
			*_pCurProp = prop;

		

			if (_pCurProp->isVisible == TRUE) {
		
				//::RedrawWindow(_hListCtrl, NULL, NULL, TRUE);
			}
		}
	};

			void SetParentNppHandle(HWND hWnd, UINT cont)
	{
		/* restore subclasses */
		//SciSubClassWrp::CleanUp();

		/* store given parent handle */
		_hParentHandle = hWnd;

		/* intial subclassing */
		/*if (cont == MAIN_VIEW) {
			SciSubClassWrp::Init(hWnd, wndParentProc0);
		} else {
			SciSubClassWrp::Init(hWnd, wndParentProc1);
		}*/
	};

   	//void doDialog(BOOL toggle = FALSE);



protected :
	virtual BOOL CALLBACK run_dlgProc( UINT Message, WPARAM wParam, LPARAM lParam);

private:
	
void MarkDownViewDialog::MoveView(void);


	NppData				_nppData;
	/* handle of parent handle (points to scintilla main view) */
	HWND				_hParentHandle;
	HHOOK				_hParentHook;

	/* properties of open files */
	tMarkProp*			_pCurProp;
	vector<tMarkProp>	_MarkProp;

};





#endif // HEXDLG_DEFINE_H
