#include "stdafx.h"
#include <stdio.h>
#include "DialogCode.h"
#include "GameData.h"

/***************************
 * A.J.'s Global Variables *
 ***************************/
int NumPlayers = 0;
extern GameData *gdata;

// Message handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}

// Message handler for Setup Dialog box
LRESULT CALLBACK SetupGame(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND PlayerDropDown = GetDlgItem(hDlg,IDC_NUMPLAYERS);
	switch(message)
	{
	case WM_INITDIALOG:
	{
		NumPlayers = 0;
		// Set up the dialog controls correctly
		SetWindowPos(PlayerDropDown,NULL,110,10,95,100,0);
		SendMessage(PlayerDropDown,CB_RESETCONTENT,NULL,NULL);
		SendMessage(PlayerDropDown,CB_ADDSTRING,NULL,(LPARAM)"1");
		SendMessage(PlayerDropDown,CB_ADDSTRING,NULL,(LPARAM)"2");
		SendMessage(PlayerDropDown,CB_ADDSTRING,NULL,(LPARAM)"3");
		SendMessage(PlayerDropDown,CB_ADDSTRING,NULL,(LPARAM)"4");
		return true;
	}
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			// Set GameData
			if (NumPlayers <= 0)
			{
				MessageBox(hDlg,"Enter the number of players","Volcano",MB_OK);
			}
			else
			{
				char *temp = new char[255];
				if (gdata)
					delete gdata;
				gdata = new GameData(NumPlayers);
				switch(NumPlayers)
				{
				case 4:
					GetDlgItemText(hDlg,IDC_NAME4,temp,255);
					gdata->InitializePlayer(4,temp);
				case 3:
					GetDlgItemText(hDlg,IDC_NAME3,temp,255);
					gdata->InitializePlayer(3,temp);
				case 2:
					GetDlgItemText(hDlg,IDC_NAME2,temp,255);
					gdata->InitializePlayer(2,temp);
				case 1:
					GetDlgItemText(hDlg,IDC_NAME1,temp,255);
					gdata->InitializePlayer(1,temp);
				}
				delete[] temp;
				EndDialog(hDlg,LOWORD(wParam));
				return true;
			}
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg,LOWORD(wParam));
			return false;
		}
		else if (HIWORD(wParam) == CBN_SELCHANGE) // The dropdown list selection has changed
		{
			UINT NameEdits[4] = {IDC_NAME1, IDC_NAME2, IDC_NAME3, IDC_NAME4};
			NumPlayers = (int)SendMessage(PlayerDropDown,CB_GETCURSEL,0,0); // Set NumPlayers zero-based
			for (int x = 0; x < 4; x++)
			{
				if (x <= NumPlayers)
				{
					char temp[255];
					if (GetWindowText(GetDlgItem(hDlg,NameEdits[x]),temp,255) == 0)
					{
						sprintf(temp,"Player %i",x + 1);
						SetWindowText(GetDlgItem(hDlg,NameEdits[x]),temp);
					}
					EnableWindow(GetDlgItem(hDlg,NameEdits[x]),true);
				}
				else
				{
					SetWindowText(GetDlgItem(hDlg,NameEdits[x]),"");
					EnableWindow(GetDlgItem(hDlg,NameEdits[x]),false);
				}
			}
			NumPlayers++; // Set NumPlayers to be one-based
		}
		break;
	}
	return false;
}

LRESULT CALLBACK Scores(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		UINT names[4] = {IDC_NAME1, IDC_NAME2, IDC_NAME3, IDC_NAME4};
		UINT scores[4] = {IDC_SCORE1, IDC_SCORE2, IDC_SCORE3, IDC_SCORE4};
		for (int x = 0; x < 4; x++)
		{
			if (x < gdata->NumPlayers)
			{
				SetDlgItemText(hDlg,names[x],(LPCTSTR)gdata->Players[x]->GetName());
				char temp[4] = "000";
				sprintf(temp,"%i",gdata->Players[x]->GetScore());
				SetDlgItemText(hDlg,scores[x],(LPCTSTR)temp);
			}
			else
			{
				SetDlgItemText(hDlg,names[x],"");
				SetDlgItemText(hDlg,scores[x],"");
			}
		}
		return TRUE;
	}
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}
