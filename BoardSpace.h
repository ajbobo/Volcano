#pragma once
#include "VPiece.h"

class BoardSpace
{
private:
	VPiece *Piece;
	bool Cap;
	bool Inspect;
public:
	BoardSpace();
	~BoardSpace();

	void GiveCap(bool incap);
	bool HasCap();
	void SetInspect(bool inval);
	bool GetInspect();
	void AddNest(int InColor);
	void AddPiece(VPiece *InPiece);
	VPiece *GetTopPiece(bool Remove);
	VPiece *GetBottomPiece();

};