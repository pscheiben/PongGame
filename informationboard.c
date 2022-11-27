/*********************************************************
 *
 * informationboard.c
 *
 * Create the score and information display
 *
 *  Created on: 27 Nov 2022
 *      Author: Sejbi
 *
 **********************************************************/


#include "ball_movement.h"
#include "general_settings.h"
#include "hal_lcd.h"
#include "msp430f5438a.h"
#include "informationboard.h"


//Draws the structure of the information board
void information_board_draw(void)
{
    halLcdVLine(LCD_COL>>1, 0, INF_BRD_WIDTH, PIXEL_ON);
    generate_life_score(MAX_PLAYER_LIFE);
}

//Draws a life symbol into a certain position
//Currently 3 circle with decreasing radius
void draw_life(x,y)
{
     halLcdCircle(x,y,LIFE_IND_RADIUS,2);
     halLcdCircle(x,y,LIFE_IND_RADIUS-1,2);
     halLcdCircle(x,y,LIFE_IND_RADIUS-2,2);

  }


void generate_life_score(int number_of_life)
{
    int i;
    p1_life_counter = number_of_life; //Player 1 life counter set
    p2_life_counter = number_of_life; //Player 2 life counter set

    for(i=0;i<p1_life_counter;i++)
    {
        draw_life(((LCD_COL>>1)-((i+1)<<3)),6); //P1's life
    }

    for(i=0;i<p2_life_counter;i++)
    {
        draw_life(((LCD_COL>>1)+((i+1)<<3)),6); //P1's life
    }

}

void information_board_clean(void)
{

}
