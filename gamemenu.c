/*
 * gamemenu.c
 *
 *  Created on: 28 Nov 2022
 *      Author: pschei200
 */


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
    game_mode_id = 0;
    ctrl_id = 1;
    active_menu_id = 0;
    PrintMainMenu();

    while(1)
     {
        active_menu_id=MenuSelection(active_menu_id);
        switch(active_menu_id)
            {
                case 0:
                    if((P2IN&BIT6) == 0)
                    {

                        halLcdClearScreen(); //CLEAR SCREEN
                        return(active_menu_id);
                    }
                    __bis_SR_register(LPM3_bits + GIE);
                    __no_operation(); //for debug
                    break;
                case 1:
                    if((P2IN&BIT6) == 0)
                    {
                        PrintGameMode();
                        while(P2IN&BIT6)
                        {
                            game_mode_id=MenuSelection(game_mode_id);
                            __bis_SR_register(LPM3_bits + GIE);
                            __no_operation(); //for debug
                        }
                        halLcdClearScreen(); //CLEAR SCREEN
                        PrintMainMenu();

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
                                           ctrl_id = 1;
                                       }
                                       ActiveMenuDraw(ctrl_id);
                                   }


                            __bis_SR_register(LPM3_bits + GIE);
                            __no_operation(); //for debug

                        }
                        halLcdClearScreen(); //CLEAR SCREEN
                        PrintMainMenu();
                        if(ctrl_id = 2) halLcdPrintLine("ACC is calibrated", 7, OVERWRITE_TEXT);//PRINT MESSAGE
                        accx_offset = accx;
                        accy_offset = accy;
                    }
                    break;
            }


     __bis_SR_register(LPM3_bits + GIE);
     __no_operation(); //for debug

     }
}

void ActiveMenuDraw(int menu_id)
{
    halLcdHLine(3,LCD_COL-3,(9+menu_id*24),PIXEL_ON);
    halLcdHLine(3,LCD_COL-3,(26+menu_id*24),PIXEL_ON);
    halLcdVLine(3,(10+menu_id*24),(26+menu_id*24),PIXEL_ON);
    halLcdVLine(LCD_COL-4,(10+menu_id*24),(26+menu_id*24),PIXEL_ON);
}

void OldMenuClear(int menu_id)
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
    ActiveMenuDraw(game_mode_id);
}

void PrintCTRLSel(void)
{
    halLcdClearScreen(); //CLEAR SCREEN
    halLcdPrintLine(" P1 and P2 CTRLs", 1, OVERWRITE_TEXT);//PRINT MESSAGE
    halLcdPrintLine(" SW and Joysitck", 3, OVERWRITE_TEXT);//PRINT MESSAGE
    halLcdPrintLine("  Accelerometer ", 5, OVERWRITE_TEXT);//PRINT MESSAGE
    halLcdPrintLine(" Select     Next ", 8, INVERT_TEXT);  //PRINT MESSAGE
    ActiveMenuDraw(ctrl_id);
}

int MenuSelection(int menu_id)
{
    if((P2IN&BIT7) == 0)
           {
               OldMenuClear(menu_id);
               menu_id ++;

               if(menu_id > 2)
               {
                   menu_id = 0;
               }
               ActiveMenuDraw(menu_id);
           }

     return menu_id;
}
