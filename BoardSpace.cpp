#include "stdafx.h"
#include "BoardSpace.h"

BoardSpace::BoardSpace()
{
	Cap = false;
	Piece = NULL;
	Inspect = false;
}

BoardSpace::~BoardSpace()
{
	if (Piece)
		delete Piece;
}

void BoardSpace::GiveCap(bool incap)
{
	Cap = incap;
}

bool BoardSpace::HasCap()
{
	return Cap;
}

void BoardSpace::SetInspect(bool inval)
{
	Inspect = inval;
}

bool BoardSpace::GetInspect()
{
	return Inspect;
}

void BoardSpace::AddNest(int InColor)
{
	Piece = new VPiece(1,InColor);
	Piece->SetAbove(new VPiece(2,InColor));
	Piece->SetAbove(new VPiece(3,InColor));
}

void BoardSpace::AddPiece(VPiece *InPiece)
{
	if (Piece)
		Piece->SetAbove(InPiece);
	else
		Piece = InPiece;
}

VPiece *BoardSpace::GetTopPiece(bool Remove)
{
	// Return a pointer to the VPiece on top - if Remove is true, remove it from the list
	VPiece *CurPiece = Piece;
	if (CurPiece == NULL) // The space had no pieces
		return NULL;
	if (Piece->GetAbove() == NULL) // There is only one piece in the space
	{
		if (Remove)
			Piece = NULL;
		return CurPiece;
	}
	VPiece *AbovePiece = CurPiece->GetAbove();
	while (AbovePiece->GetAbove() != NULL)
	{
		AbovePiece = AbovePiece->GetAbove();
		CurPiece = CurPiece->GetAbove();
	}

	if (Remove)
		return CurPiece->ClearAbove();

	return CurPiece->GetAbove();
}

VPiece *BoardSpace::GetBottomPiece()
{
	return Piece;
}