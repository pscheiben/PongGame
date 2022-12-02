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



void GameMenuInit(void)
{
    halLcdClearScreen(); //CLEAR SCREEN
    halLcdPrintLine(" One Player Easy", 1, OVERWRITE_TEXT);//PRINT MESSAGE
    halLcdPrintLine(" One Player Hard", 3, OVERWRITE_TEXT);//PRINT MESSAGE
    halLcdPrintLine("   Multi Player ", 5, OVERWRITE_TEXT);//PRINT MESSAGE
    halLcdPrintLine(" Select     Next ", 8, INVERT_TEXT);  //PRINT MESSAGE

    active_menu_id = 0;
    ActiveMenuDraw(active_menu_id);

    while(P2IN&BIT6) //until SW1 Select is not pressed (pull up) constant pulling
     {
       if((P2IN&BIT7) == 0)
       {
           OldMenuClear(active_menu_id);
           active_menu_id ++;

           if(active_menu_id > 2)
           {
               active_menu_id = 0;
           }
           ActiveMenuDraw(active_menu_id);
       }

     __bis_SR_register(LPM3_bits + GIE);
     __no_operation(); //for debug

     }
    halLcdClearScreen(); //CLEAR SCREEN

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


