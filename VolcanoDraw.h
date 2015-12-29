#pragma once
#include "stdafx.h"
#include "GameData.h"

void InitDraw();
void DeinitDraw();
void MainDraw(HDC hdc);
void DrawBoard(HDC hdc);
void DrawInspectArea(HDC hdc);
void DrawButtons(HDC hdc);
void HandleLClick(HWND hWnd, int x, int y);
void HandleRClick(HWND hWnd, int x, int y);