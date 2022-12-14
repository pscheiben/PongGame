//*******************************************************
//* Main menu for Pong Game
//* gamemenu.h
//*
//*  Created on: 28 Nov 2022
//*      Author: Peter Scheibenhoffer
//*******************************************************

#ifndef GAMEMENU_H_
#define GAMEMENU_H_

int GameMenuInit(void);
void PrintMainMenu(void);
void PrintGameMode(void);
void PrintCTRLSel(void);
void ActiveMenuDraw(int);
void OldMenuClear(int);
int MenuSelection(int);

#endif /* GAMEMENU_H_ */
