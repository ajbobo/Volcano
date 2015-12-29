#include "stdafx.h"
#include "VPiece.h"

VPiece::VPiece(int InSize, int InColor)
{
	Size = InSize;
	Color = InColor;
	Above = NULL;
}

VPiece::~VPiece()
{
	if (Above)
		delete Above;
}

VPiece *VPiece::GetAbove()
{
	return Above;
}

VPiece *VPiece::ClearAbove()
{
	VPiece *temp = Above;
	Above = NULL;
	return temp;
}

void VPiece::SetAbove(VPiece *InPiece)
{
	if (Above)
		Above->SetAbove(InPiece);
	else
		Above = InPiece;
}

int VPiece::GetSize()
{
	return Size;
}

int VPiece::GetColor()
{
	return Color;
}