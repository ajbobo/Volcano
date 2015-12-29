#pragma once
#include "VPiece.h"

class Player
{
private:
	char Name[255];
	int Score;
	int ColorsCollected; // Each bit respresents a certain color
	VPiece *Pieces[25]; // Holds the maximum possible number of trees (there's probably a better way...)
public:
	Player(char *InName);
	~Player();

	int GetScore();
	void SetScore(int InScore);
	char *GetName();
	void AddPieceToScore(VPiece *inPiece);
};