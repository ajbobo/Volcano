#include "stdafx.h"
#include "string.h"
#include "Player.h"
#include "VPiece.h"

Player::Player(char *InName)
{
	strcpy(Name,InName);
	for (int x = 0; x < 25; x++)
		Pieces[x] = NULL;
	ColorsCollected = 0;
	Score = 0;
}

Player::~Player()
{
	for (int x = 0; x < 25; x++)
	{
		if (Pieces[x])
			delete Pieces[x];
	}
}

char *Player::GetName()
{
	return Name;
}

int Player::GetScore()
{
	return Score;
}

void Player::SetScore(int InScore)
{
	Score = InScore;
}

void Player::AddPieceToScore(VPiece *inPiece)
{
	// Add the piece to the player's inventory and recalculate the score
	delete inPiece;
	Score++;
}