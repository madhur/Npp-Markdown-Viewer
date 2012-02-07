//this file is part of notepad++
//Copyright (C)2003 Don HO <donho@altern.org>
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

#include "PluginDefinition.h"
#include "menuCmdID.h"
#include "MarkdownDialog.h"
#include "MarkDownViewDialog.h"
#include "HelpDialog.h"

FuncItem funcItem[nbFunc];
 NppData nppData;
HANDLE			g_hModule;

//HANDLE g_hMod;
HelpDlg			helpDlg;
MarkDownViewDialog markDialog1;
MarkDownViewDialog markDialog2;
UINT			currentSC	= MAIN_VIEW;

/* for subclassing */
WNDPROC	wndProcNotepad = NULL;

/* handle of current used edit */
MarkDownViewDialog*		currentView		= NULL;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  reasonForCall, 
                       LPVOID lpReserved )
{
	g_hModule = hModule;
    switch (reasonForCall)
    {
      case DLL_PROCESS_ATTACH:
		  commandMenuInit();
      //  pluginInit(hModule);
        break;

      case DLL_PROCESS_DETACH:
        pluginCleanUp();
        break;

      case DLL_THREAD_ATTACH:
        break;

      case DLL_THREAD_DETACH:
        break;
    }

    return TRUE;
}


extern "C" __declspec(dllexport) void setInfo(NppData notpadPlusData)
{
	nppData = notpadPlusData;
	// helpDlg.init((HINSTANCE)g_hModule, notpadPlusData);
		
	pluginInit(g_hModule,notpadPlusData);

	/* Subclassing for Notepad */
	// wndProcNotepad = (WNDPROC)SetWindowLongPtr(nppData._nppHandle, GWL_WNDPROC, (LPARAM)SubWndProcNotepad);

	currentView=&markDialog1;

	
}

extern "C" __declspec(dllexport) const TCHAR * getName()
{
	return NPP_PLUGIN_NAME;
}

extern "C" __declspec(dllexport) FuncItem * getFuncsArray(int *nbF)
{
	*nbF = nbFunc;
	return funcItem;
}


extern "C" __declspec(dllexport) void beNotified(SCNotification *notifyCode)
{
	switch (notifyCode->nmhdr.code) 
	{
		case NPPN_SHUTDOWN:
		{
			commandMenuCleanUp();
		}
		break;

		default:
			return;
	}
}


// Here you can process the Npp Messages 
// I will make the messages accessible little by little, according to the need of plugin development.
// Please let me know if you need to access to some messages :
// http://sourceforge.net/forum/forum.php?forum_id=482781
//
extern "C" __declspec(dllexport) LRESULT messageProc(UINT Message, WPARAM wParam, LPARAM lParam)
{/*
	if (Message == WM_MOVE)
	{
		::MessageBox(NULL, "move", "", MB_OK);
	}
*/
	return TRUE;
}

#ifdef UNICODE
extern "C" __declspec(dllexport) BOOL isUnicode()
{
    return TRUE;
}
#endif //UNICODE


//
// Initialize your plugin data here
// It will be called while plugin loading   
void pluginInit(HANDLE hModule, NppData nppData)
{
	// g_hMod=hModule;
	//::nppData=nppData;
	helpDlg.init((HINSTANCE)hModule, nppData);
	markDialog1.init((HINSTANCE)hModule, nppData);		
	markDialog2.init((HINSTANCE)hModule, nppData);		
	markDialog1.SetParentNppHandle(nppData._scintillaMainHandle, MAIN_VIEW);
	markDialog2.SetParentNppHandle(nppData._scintillaSecondHandle, SUB_VIEW);

	
}

//
// Here you can do the clean up, save the parameters (if any) for the next session
//
void pluginCleanUp()
{
}

//
// Initialization of your plugin commands
// You should fill your plugins commands here
void commandMenuInit()
{

    //--------------------------------------------//
    //-- STEP 3. CUSTOMIZE YOUR PLUGIN COMMANDS --//
    //--------------------------------------------//
    // with function :
    // setCommand(int index,                      // zero based number to indicate the order of command
    //            TCHAR *commandName,             // the command name that you want to see in plugin menu
    //            PFUNCPLUGINCMD functionPointer, // the symbol of function (function pointer) associated with this command. The body should be defined below. See Step 4.
    //            ShortcutKey *shortcut,          // optional. Define a shortcut to trigger this command
    //            bool check0nInit                // optional. Make this menu item be checked visually
    //            );

	ShortcutKey *sk=new ShortcutKey();
	sk->_isAlt=TRUE;
	sk->_isCtrl=TRUE;
	sk->_isShift=TRUE;
	sk->_key=0x4A;

    setCommand(0, TEXT("&View Markdown in HTML"), viewmarkdowninhtml, sk, false);
    setCommand(1, TEXT("&About"), about, NULL, false);
}

//
// Here you can do the clean up (especially for the shortcut)
//
void commandMenuCleanUp()
{
	// Don't forget to deallocate your shortcut here
}


//
// This function help you to initialize your plugin commands
//
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool check0nInit) 
{
    if (index >= nbFunc)
        return false;

    if (!pFunc)
        return false;

    lstrcpy(funcItem[index]._itemName, cmdName);
    funcItem[index]._pFunc = pFunc;
    funcItem[index]._init2Check = check0nInit;
    funcItem[index]._pShKey = sk;

    return true;
}

//----------------------------------------------//
//-- STEP 4. DEFINE YOUR ASSOCIATED FUNCTIONS --//
//----------------------------------------------//
void viewmarkdowninhtml()
{
    // GetShortCuts(nppData._nppHandle);
//	pCurHexEdit->doDialog(TRUE);
//	DialogUpdate();
//	setMenu();
	 // Open a new document
//    ::SendMessage(nppData._nppHandle, NPPM_MENUCOMMAND, 0, IDM_FILE_NEW);
	currentView->doDialog(TRUE);

}


/**************************************************************************
 *	SubWndProcNotepad
 */
LRESULT CALLBACK SubWndProcNotepad(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT			ret = 0;

	switch (message)
	{
		case WM_ACTIVATE:
		{
			ret = ::CallWindowProc(wndProcNotepad, hWnd, message, wParam, lParam);
			if (currentSC == MAIN_VIEW)
			{
				::SendMessage(markDialog1.getHSelf(), message, wParam, lParam);
				::SendMessage(markDialog2.getHSelf(), message, ~wParam, lParam);
			}
			else
			{
				::SendMessage(markDialog1.getHSelf(), message, ~wParam, lParam);
				::SendMessage(markDialog2.getHSelf(), message, wParam, lParam);
			}
			break;
		}
     
		case WM_COMMAND:
		{
			
			switch (LOWORD(wParam))
			{				
				case IDM_VIEW_GOTO_ANOTHER_VIEW:
				case IDM_VIEW_CLONE_TO_ANOTHER_VIEW:
				{
					tMarkProp markProp = currentView->GetMarkProp();

					ret = ::CallWindowProc(wndProcNotepad, hWnd, message, wParam, lParam);
//					OutputDebugString(_T("TO_ANOTHER_VIEW\n"));
					//SystemUpdate();
					currentView->SetMarkProp(markProp);
					currentView->doDialog();
					//currentView->SetStatusBar();
					break;
				}
				case IDM_VIEW_SWITCHTO_OTHER_VIEW:
				{
					ret = ::CallWindowProc(wndProcNotepad, hWnd, message, wParam, lParam);
					//currentView->SetStatusBar();
					break;
				}
				default:
					ret = ::CallWindowProc(wndProcNotepad, hWnd, message, wParam, lParam);
					break;
			}
			break;
		}
		
			
		
		default:
			ret = ::CallWindowProc(wndProcNotepad, hWnd, message, wParam, lParam);
			break;
	}

	return ret;
}



INT_PTR CALLBACK abtDlgProc(HWND hwndDlg,UINT uMsg,WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
		
		return TRUE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hwndDlg,wParam);
			return TRUE;
		}
	}
	return FALSE;
}

void about()
{
	
   // HWND userDialog=::CreateDialog((HINSTANCE)g_hMod,MAKEINTRESOURCE(IDD_HELP_DLG),nppData._nppHandle,abtDlgProc);
	// int error=::GetLastError();
	helpDlg.doDialog();
	
}

void ActivateWindow(void)
{
	/* activate correct window */
		//::SendMessage(markDialog.getHSelf(), WM_ACTIVATE, TRUE, 0);
}