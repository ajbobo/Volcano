#pragma once

void SetupBoard();
void SelectForCap(int Row,int Col);
void MakeLegalMove(int EndRow,int EndCol);
bool MoveFrom(int StartRow, int StartCol, int EndRow, int EndCol);