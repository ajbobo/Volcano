#pragma once
#include "VPiece.h"
#include "BoardSpace.h"
#include "Player.h"

enum GameMode {Mode_Pick_Cap, Mode_Pick_Space, Mode_Inspect};

class GameData
{
public:
	Player **Players; // Holds a dynamically-sized array of Players (1 or more)
	BoardSpace *Board[5][5]; // Holds the largest allowed board
	BoardSpace *CurInspect; // Holds a pointer to the space currently being inspected
	GameMode Mode;
	int SelRow, SelCol; // Holds the row and column of the selected space
	bool Over;
	int NumPlayers;
	int CurrPlayer;

	GameData(int InNumPlayers);
	~GameData();

	void InitializePlayer(int PlayerNum, char *PlayerName);

};
