//*******************************************************
//* Main menu for Pong Game
//* gamemenu.c
//*
//*  Created on: 28 Nov 2022
//*      Author: Peter Scheibenhoffer
//*******************************************************


#include    "msp430f5438a.h"
#include    "hal_UCS.h"
#include    "hal_PMM.h"
#include    "hal_lcd.h"
#include    "general_settings.h"
#include    "ball_movement.h"
#include    "informationboard.h"
#include    "gamemenu.h"

int GameMenuInit(void)
{
    game_mode_id = 0; //tracks the gamemode option
    ctrl_id = 1;      //tracks the control mode options (only two, so starts from 1)
    active_menu_id = 0;  //main menu track
    PrintMainMenu();

    while(1)
    {
        active_menu_id=MenuSelection(active_menu_id); //checks is there any change in the highlighted menu
        switch(active_menu_id)                        //3 main menu cases and what to do if select is pressed
        {
        case 0:
            if((P2IN&BIT6) == 0)
            {
                halLcdClearScreen(); //CLEAR SCREEN
                return(active_menu_id);             //return from GameMenuInit --> Starts the game
            }
            __bis_SR_register(LPM3_bits + GIE);
            __no_operation(); //for debug
            break;
        case 1:
            if((P2IN&BIT6) == 0)
            {
                PrintGameMode();                    //game mode menu
                while(P2IN&BIT6)
                {
                    game_mode_id=MenuSelection(game_mode_id);  //saving game mode
                    __bis_SR_register(LPM3_bits + GIE);
                    __no_operation(); //for debug
                }
                halLcdClearScreen(); //CLEAR SCREEN
                PrintMainMenu();                               //back to main menu

            }

            break;
        case 2:
            if((P2IN&BIT6) == 0)
            {
                PrintCTRLSel();
                while(P2IN&BIT6)
                {
                    if((P2IN&BIT7) == 0)
                    {
                        OldMenuClear(ctrl_id);
                        ctrl_id ++;
                        if(ctrl_id > 2)
                        {
                            ctrl_id = 1;  //special case, because the program dont want to highlight the title
                        }
                        ActiveMenuDraw(ctrl_id);
                    }
                    __bis_SR_register(LPM3_bits + GIE);
                    __no_operation(); //for debug
                }
                halLcdClearScreen(); //CLEAR SCREEN
                PrintMainMenu(); //get back to the main menu
                if(ctrl_id == 2)
                {
                    halLcdPrintLine("ACC is calibrated", 7, OVERWRITE_TEXT);//PRINT MESSAGE
                    accx_offset = accx; //saving the offset values
                    accy_offset = accy;
                }
            }
            break;
        }
        __bis_SR_register(LPM3_bits + GIE);
        __no_operation(); //for debug

    }
}

void ActiveMenuDraw(int menu_id)  //draws the highlight rectangle
{
    halLcdHLine(3,LCD_COL-3,(9+menu_id*24),PIXEL_ON);
    halLcdHLine(3,LCD_COL-3,(26+menu_id*24),PIXEL_ON);
    halLcdVLine(3,(10+menu_id*24),(26+menu_id*24),PIXEL_ON);
    halLcdVLine(LCD_COL-4,(10+menu_id*24),(26+menu_id*24),PIXEL_ON);
}

void OldMenuClear(int menu_id)  //deletes the previous menu point highlight rectangle
{
    halLcdHLine(3,LCD_COL-3,(9+menu_id*24),PIXEL_OFF);
    halLcdHLine(3,LCD_COL-3,(26+menu_id*24),PIXEL_OFF);
    halLcdVLine(3,(10+menu_id*24),(26+menu_id*24),PIXEL_OFF);
    halLcdVLine(LCD_COL-4,(10+menu_id*24),(26+menu_id*24),PIXEL_OFF);
}


void PrintMainMenu(void)
{
    halLcdClearScreen(); //CLEAR SCREEN
    halLcdPrintLine("   Start Game", 1, OVERWRITE_TEXT);//PRINT MESSAGE
    halLcdPrintLine("    Game Mode", 3, OVERWRITE_TEXT);//PRINT MESSAGE
    halLcdPrintLine("  Control Option ", 5, OVERWRITE_TEXT);//PRINT MESSAGE
    halLcdPrintLine(" Select     Next ", 8, INVERT_TEXT);  //PRINT MESSAGE
    ActiveMenuDraw(active_menu_id);
}

void PrintGameMode(void)
{
    halLcdClearScreen(); //CLEAR SCREEN
    halLcdPrintLine(" One Player Easy", 1, OVERWRITE_TEXT);//PRINT MESSAGE
    halLcdPrintLine(" One Player Hard", 3, OVERWRITE_TEXT);//PRINT MESSAGE
    halLcdPrintLine("   Multi Player ", 5, OVERWRITE_TEXT);//PRINT MESSAGE
    halLcdPrintLine(" Select     Next ", 8, INVERT_TEXT);  //PRINT MESSAGE
    ActiveMenuDraw(game_mode_id);   //global variable for game mode
}

void PrintCTRLSel(void)
{
    halLcdClearScreen(); //CLEAR SCREEN
    halLcdPrintLine(" P1 and P2 CTRLs", 1, OVERWRITE_TEXT);//PRINT MESSAGE
    halLcdPrintLine(" SW and Joysitck", 3, OVERWRITE_TEXT);//PRINT MESSAGE
    halLcdPrintLine("  Accelerometer ", 5, OVERWRITE_TEXT);//PRINT MESSAGE
    halLcdPrintLine(" Select     Next ", 8, INVERT_TEXT);  //PRINT MESSAGE
    ActiveMenuDraw(ctrl_id);    //global variable for control options
}

int MenuSelection(int menu_id)   //this tracks the active menu point in the main and sub menus
{
    if((P2IN&BIT7) == 0)    //checks if the "NEXT" SW2 is pressed
    {
        OldMenuClear(menu_id);
        menu_id ++;
        if(menu_id > 2)     //After the last menu point, it jumps to the first
        {
            menu_id = 0;
        }
        ActiveMenuDraw(menu_id);    //draw the highlight rectangle
    }
    return menu_id;                 //returns the selected menupoint
}
