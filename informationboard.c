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
    halLcdVLine(LCD_COL>>1, 0, INF_BRD_WIDTH, PIXEL_ON);  //division line drawing
    generate_life_score(MAX_PLAYER_LIFE, MAX_PLAYER_LIFE); //generate lives indicator for both player

}

//Draws a life symbol into a certain position
//Currently 3 circle with decreasing radius
void draw_life(x,y)
{

    halLcdCircle(x,y,LIFE_IND_RADIUS,PIXEL_ON);
    halLcdCircle(x,y,LIFE_IND_RADIUS-1,PIXEL_DARK);
    halLcdCircle(x,y,LIFE_IND_RADIUS-2,PIXEL_LIGHT);


}


//clearing the certain life from the screen
void clear_life(x,y)
{
    int i;
    for(i=0;i<(LIFE_IND_RADIUS);i++)
    {
        halLcdHLine((x-LIFE_IND_RADIUS),(x+LIFE_IND_RADIUS),y+i,PIXEL_OFF);
        halLcdHLine((x-LIFE_IND_RADIUS),(x+LIFE_IND_RADIUS),y-i,PIXEL_OFF);
    }
    halLcdCircle(x,y,LIFE_IND_RADIUS,3);  //creating an empty circle
}

////check the player 1 score and update
void check_player1_score(int playerscore)
{

    clear_life(((LCD_COL>>1)-(8*(playerscore+1))),6); //clear the middle of the circle of life

}

//check the player 2 score and update
void check_player2_score(int playerscore)
{

    clear_life(((LCD_COL>>1)+(8*(playerscore+1))),6); //clear the middle of the circle of life

}


//generate the starting lives
void generate_life_score(int p1_life, int p2_life)
{
    int i;
    p1_life_counter = p1_life; //Player 1 life counter set
    p2_life_counter = p2_life; //Player 2 life counter set

    for(i=0;i<p1_life_counter;i++)
    {
        draw_life(((LCD_COL>>1)-((i+1)<<3)),6); //P1's life
    }

    for(i=0;i<p2_life_counter;i++)
    {
        draw_life(((LCD_COL>>1)+((i+1)<<3)),6); //P2's life
    }

}

void information_board_clean(void)
{

}
