#pragma once

class VPiece
{
private:
	VPiece *Above;
	int Size;
	int Color;
public:
	VPiece(int InSize, int InColor);
	~VPiece();

	VPiece *GetAbove();
	VPiece *ClearAbove();
	void SetAbove(VPiece *InPiece);
	int GetSize();
	int GetColor();
};