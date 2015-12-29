#include "stdafx.h"
#include "GameData.h"

GameData::GameData(int InNumPlayers)
{
	// Player data
	NumPlayers = InNumPlayers;
	Players = new Player*[NumPlayers];
	for (int x = 0; x < NumPlayers; x++)
		Players[x] = NULL;
	CurrPlayer = 1; // Players are numbered starting at 1 (not 0)

	// Board data
	for (int x = 0; x < 5; x++)
		for (int y = 0; y < 5; y++)
			Board[x][y] = NULL;
	CurInspect = NULL;
	SelRow = SelCol = -1;

	// Game play data
	Mode = Mode_Pick_Cap;
	Over = false;
}

GameData::~GameData()
{
	delete[] Players;
	for (int x = 0; x < 5; x++)
		for (int y = 0; y < 5; y++)
			delete Board[x][y];
}

void GameData::InitializePlayer(int PlayerNum, char *PlayerName)
{
	Players[PlayerNum - 1] = new Player(PlayerName);
}
	