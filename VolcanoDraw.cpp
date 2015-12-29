#include "stdafx.h"
#include "DialogCode.h"
#include "GameData.h"
#include "VPiece.h"
#include "VolcanoDraw.h"
#include "VolcanoRules.h"

#define BOARD_SIZE 300
#define BOARD_OFFSET 12
#define INSPECT_OFFSETX 325
#define INSPECT_OFFSETY 12
#define INSPECT_WIDTH 60
#define INSPECT_HEIGHT 328
#define INSPECT_PIECE_HEIGHT 10
#define BUTTON_OFFSET 3
#define BUTTON_WIDTH (BOARD_SIZE / 4 - 2 * BUTTON_OFFSET)
#define ONE_OFFSET 10
#define TWO_OFFSET 18
#define THREE_OFFSET 25
#define BRUSHES 9

extern HINSTANCE hInst;
extern GameData *gdata;

HBRUSH Brushes[BRUSHES];

void InitDraw()
{
	Brushes[0] = CreateSolidBrush(RGB(255,0,0));		// Red
	Brushes[1] = CreateSolidBrush(RGB(0,255,0));		// Green
	Brushes[2] = CreateSolidBrush(RGB(0,0,255));		// Blue
	Brushes[3] = CreateSolidBrush(RGB(255,255,0));	// Yellow
	Brushes[4] = CreateSolidBrush(RGB(180,0,180));	// Purple
	Brushes[5] = CreateSolidBrush(RGB(0,0,0));		// Black
	Brushes[6] = CreateSolidBrush(RGB(128,128,128));// Grey
	Brushes[7] = CreateSolidBrush(RGB(192,192,192));// Light Grey
	Brushes[8] = CreateSolidBrush(RGB(255,0,255));	// Magenta
}

void DeinitDraw()
{
	for (int x = 0; x < BRUSHES; x++)
		DeleteObject(Brushes[x]);
}

void MainDraw(HDC hdc)
{
	DrawBoard(hdc);
	DrawInspectArea(hdc);
	DrawButtons(hdc);
}

void DrawBoard(HDC hdc)
{
	HBRUSH oldbrush;
	// Outline of board area
	Rectangle(hdc,BOARD_OFFSET,BOARD_OFFSET,BOARD_SIZE + BOARD_OFFSET,BOARD_SIZE + BOARD_OFFSET);
	BoardSpace *curspace = NULL;
	VPiece *curpiece = NULL;
	for (int row = 0; row < 5; row++)
	{
		for (int col = 0; col < 5; col++)
		{
			int CenterX = (BOARD_SIZE / 5 * col) + (BOARD_SIZE / 10) + BOARD_OFFSET;
			int CenterY = (BOARD_SIZE / 5 * row) + (BOARD_SIZE / 10) + BOARD_OFFSET;
			int Offset;
			curspace = gdata->Board[row][col];
			curpiece = curspace->GetBottomPiece();
			if (curspace->GetInspect())
			{
				Offset = BOARD_SIZE / 10;
				oldbrush = (HBRUSH)SelectObject(hdc,Brushes[6]);
				Rectangle(hdc,CenterX - Offset, CenterY - Offset, CenterX + Offset, CenterY + Offset);
				SelectObject(hdc,oldbrush);
			}
			if (row == gdata->SelRow && col == gdata->SelCol)
			{
				Offset = BOARD_SIZE / 10;
				oldbrush = (HBRUSH)SelectObject(hdc,Brushes[8]);
				Rectangle(hdc,CenterX - Offset, CenterY - Offset, CenterX + Offset, CenterY + Offset);
				SelectObject(hdc,oldbrush);
			}
			while (curpiece != NULL)
			{
				oldbrush = (HBRUSH)SelectObject(hdc,Brushes[curpiece->GetColor() - 1]);
				if (curpiece->GetSize() == 1) 
					Offset = ONE_OFFSET;
				else if (curpiece->GetSize() == 2) 
					Offset = TWO_OFFSET;
				else if (curpiece->GetSize() == 3) 
					Offset = THREE_OFFSET;
				Rectangle(hdc,CenterX - Offset, CenterY - Offset, CenterX + Offset, CenterY + Offset);
				SelectObject(hdc,oldbrush);
				curpiece = curpiece->GetAbove();
			}
			if (curspace->HasCap())
			{
				oldbrush = (HBRUSH)SelectObject(hdc,Brushes[5]);
				Rectangle(hdc,CenterX - ONE_OFFSET,CenterY - ONE_OFFSET,CenterX + ONE_OFFSET,CenterY + ONE_OFFSET);
				SelectObject(hdc,oldbrush);
			}
		}
	}
}

void DrawInspectArea(HDC hdc)
{
	HBRUSH oldbrush;
	// Outline of Viewing area
	Rectangle(hdc,INSPECT_OFFSETX,INSPECT_OFFSETY,INSPECT_OFFSETX + INSPECT_WIDTH,INSPECT_OFFSETY + INSPECT_HEIGHT);
	if (!gdata->CurInspect)
		return;

	int CenterX = INSPECT_OFFSETX + (INSPECT_WIDTH / 2); // The center of the inspect area
	int CurrBase = INSPECT_OFFSETY + INSPECT_HEIGHT - INSPECT_PIECE_HEIGHT; // The base of the next piece to draw
	int Offset;
	VPiece *curpiece = gdata->CurInspect->GetBottomPiece();
	while (curpiece != NULL) // Draw all of the pieces in the current Space
	{
		oldbrush = (HBRUSH)SelectObject(hdc,Brushes[curpiece->GetColor() - 1]);
		if (curpiece->GetSize() == 1) 
			Offset = ONE_OFFSET;
		else if (curpiece->GetSize() == 2) 
			Offset = TWO_OFFSET;
		else if (curpiece->GetSize() == 3) 
			Offset = THREE_OFFSET;
		Rectangle(hdc,CenterX - Offset, CurrBase, CenterX + Offset, CurrBase + INSPECT_PIECE_HEIGHT);
		SelectObject(hdc,oldbrush);
		CurrBase -= INSPECT_PIECE_HEIGHT;
		curpiece = curpiece->GetAbove();
	}
	if (gdata->CurInspect->HasCap())
	{
		oldbrush = (HBRUSH)SelectObject(hdc,Brushes[5]);
		Offset = ONE_OFFSET;
		Rectangle(hdc,CenterX - Offset, CurrBase, CenterX + Offset, CurrBase + INSPECT_PIECE_HEIGHT);
		SelectObject(hdc,oldbrush);
		CurrBase -= INSPECT_PIECE_HEIGHT;
	}

}

void DrawButtons(HDC hdc)
{
	HBRUSH oldbrush;
	// Draw each button
	char *labels[4] = {"Select","Inspect","Inventory","Scores"};
	int yloc = BOARD_OFFSET + BOARD_SIZE + 5;
	for (int button = 0; button < 4; button++)
	{
		bool highlight = false;
		if ((button == 0 && gdata->Mode == Mode_Pick_Space) ||
	 		 (button == 0 && gdata->Mode == Mode_Pick_Cap) ||
			 (button == 1 && gdata->Mode == Mode_Inspect))
		{
			highlight = true;
		}
		if (highlight)
			oldbrush = (HBRUSH)SelectObject(hdc,Brushes[7]);
		int xloc = BOARD_OFFSET + (BUTTON_WIDTH + 2 * BUTTON_OFFSET) * button + BUTTON_OFFSET;
		Rectangle(hdc,xloc,yloc,xloc + BUTTON_WIDTH,yloc + 22);
		// Add text to the button
		if (highlight)
			SelectObject(hdc,oldbrush);
	}
}

void HandleLClick(HWND hWnd, int x, int y)
{
	if (x < BOARD_OFFSET || x > BOARD_OFFSET + BOARD_SIZE) // Clicked outside the valid range
		return;
	if (y < BOARD_OFFSET + BOARD_SIZE) // Clicked on the board
	{
		// Find the space being clicked on, respond based on the current mode
		int modx = x - BOARD_OFFSET;
		int mody = y - BOARD_OFFSET;
		int spacesize = BOARD_SIZE / 5;
		int Row = mody / spacesize;
		int Col = modx / spacesize;
		if (gdata->Mode == Mode_Pick_Cap)
		{
			SelectForCap(Row,Col);
		}
		else if (gdata->Mode == Mode_Pick_Space)
		{
			MakeLegalMove(Row,Col);
		}
		else if (gdata->Mode == Mode_Inspect)
		{
			for (int row = 0; row < 5; row++)
				for (int col = 0; col < 5; col++)
					gdata->Board[row][col]->SetInspect(false);
			gdata->CurInspect = gdata->Board[Row][Col];
			gdata->CurInspect->SetInspect(true);
		}
	}
else // Clicked on a button
	{
		// Find the button that was clicked on
		int modx = x - BOARD_OFFSET;
		int buttonsize = BOARD_SIZE / 4;
		int button = modx / buttonsize;
		if (button == 0) // Select Mode
			gdata->Mode = Mode_Pick_Cap;
		else if (button == 1) // Inspect Mode
		{
			gdata->Mode = Mode_Inspect;
			gdata->SelRow = -1; gdata->SelCol = -1;
		}
		else if (button == 2)
			MessageBox(NULL,"Inventory","Volcano",MB_OK);
		else if (button == 3)
			DialogBox(hInst,(LPCTSTR)IDD_SCORES,hWnd,(DLGPROC)Scores);
	}
}

void HandleRClick(HWND hWnd, int x, int y)
{
	for (int row = 0; row < 5; row++)
		for (int col = 0; col < 5; col++)
			gdata->Board[row][col]->SetInspect(false);

	// Make sure the user clicked on the game board
	if (x < BOARD_OFFSET || x > BOARD_OFFSET + BOARD_SIZE ||
		 y < BOARD_OFFSET || y > BOARD_OFFSET + BOARD_SIZE)
	{
		gdata->CurInspect = NULL;
		return;
	}

	// Calculate which space was clicked on here
	int modx = x - BOARD_OFFSET;
	int mody = y - BOARD_OFFSET;
	int SpaceSize = BOARD_SIZE / 5;
	int Row = mody / SpaceSize;
	int Col = modx / SpaceSize;

	gdata->CurInspect = gdata->Board[Row][Col];
	gdata->CurInspect->SetInspect(true);
}