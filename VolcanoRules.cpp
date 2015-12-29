#include "stdafx.h"
#include "VolcanoRules.h"
#include "GameData.h"

extern GameData *gdata;

void SetupBoard()
{
	int ColorLayout[5][5]= {
		{1,2,3,3,3},
		{4,1,2,2,3},
		{5,4,1,2,3},
		{5,4,4,1,2},
		{5,5,5,4,1}};

	// Create the BoardSpace objects and fill them with nests of pieces
	for (int x = 0; x < 5; x++)
	{
		for (int y = 0; y < 5; y++)
		{
			gdata->Board[x][y] = new BoardSpace();
			gdata->Board[x][y]->AddNest(ColorLayout[x][y]);
		}
	}

	// Set the caps on the spaces on the main diagonal
	for (int x = 0; x < 5; x++)
		gdata->Board[x][x]->GiveCap(true);
}

void SelectForCap(int Row,int Col)
{
	if (!gdata->Board[Row][Col]->HasCap())
	{
		MessageBox(NULL,"You must select a space with a cap","Volcano",MB_OK);
		return;
	}
	gdata->SelRow = Row;
	gdata->SelCol = Col;
	gdata->Mode = Mode_Pick_Space;
}

void MakeLegalMove(int EndRow,int EndCol)
{
	int StartRow = gdata->SelRow;
	int StartCol = gdata->SelCol;
	if (StartRow == EndRow && StartCol == EndCol) // Clicked on the same space
	{
		gdata->SelCol = -1; 
		gdata->SelRow = -1;
		gdata->Mode = Mode_Pick_Cap;
		return;
	}
	if (gdata->Board[EndRow][EndCol]->HasCap()) // Clicked on a space with a cap
	{
		MessageBox(NULL,"That space already has a cap","Volcano",MB_OK);
		return;
	}
	if (abs(EndRow - StartRow) > 1 || abs(EndCol - StartCol) > 1) // Trying to move too far
	{
		MessageBox(NULL,"That is not a valid space","Volcano",MB_OK);
		return;
	}
	if (MoveFrom(gdata->SelRow,gdata->SelCol,EndRow,EndCol)) // returns true if a real piece (non-cap) moves
	{
		gdata->CurrPlayer++;
		if (gdata->CurrPlayer > gdata->NumPlayers)
			gdata->CurrPlayer = 1;
	}
	gdata->Mode = Mode_Pick_Cap;
	gdata->SelRow = -1;
	gdata->SelCol = -1;
}

bool MoveFrom(int StartRow, int StartCol, int EndRow, int EndCol)
{
	bool Eruption = false;
	BoardSpace *CurrSpace = gdata->Board[StartRow][StartCol];
	int ColDiff = EndCol - StartCol;
	int RowDiff = EndRow - StartRow;
	gdata->Board[StartRow][StartCol]->GiveCap(false); // Remove the cap from the old space
	gdata->Board[StartRow + RowDiff][StartCol + ColDiff]->GiveCap(true); // Move it to the new one
	int row = StartRow + 2 * RowDiff; // Start 1 space past where the cap goes
	int col = StartCol + 2 * ColDiff;
	for (;row >= 0 && row < 5 && col >=0 && col < 5 && !gdata->Board[row][col]->HasCap();
		   row += RowDiff, col +=ColDiff) // Move the pieces underneath until reaching a cap or the board's edge
	{
		VPiece *CurPiece = gdata->Board[StartRow][StartCol]->GetTopPiece(true);
		VPiece *TargetTop = gdata->Board[row][col]->GetTopPiece(false);
		if (CurPiece && TargetTop && CurPiece->GetSize() == TargetTop->GetSize()) // Pieces are the same size - give the moving one to the player
		{
			MessageBox(NULL,"Captured a piece","Volcano",MB_OK);
			gdata->Players[gdata->CurrPlayer - 1]->AddPieceToScore(CurPiece);
		}
		else // Different sizes - Put it on the board
		{
			gdata->Board[row][col]->AddPiece(CurPiece);
		}
		if (CurPiece)
			Eruption = true;
	}

	return Eruption;
}