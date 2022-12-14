//*******************************************************
//
//*
// informationboard.c
//*
//* Create the score and information display
//*
//*  Created on: 27 Nov 2022
//*      Author: Peter Scheibenhoffer
//*
//*******************************************************


#include "ball_movement.h"
#include "general_settings.h"
#include "hal_lcd.h"
#include "msp430f5438a.h"
#include "informationboard.h"
const unsigned int heart_symbol[] = {0x3030, 0xfcfc, 0xefec, 0xebac, 0xfabc, 0x3ab0, 0x3ef0, 0x0fc0, 0x0300};           //drawing of the heart symbol
const unsigned int empty_heart_symbol[] = {0x3030, 0xfcfc, 0xcfcc, 0xc30c, 0xf03c, 0x3030, 0x3cf0, 0x0fc0, 0x0300};     //drawing of lost lives
const unsigned int heart_symbol_up[] = {0x0300, 0x0fc0, 0x3ef0, 0x3ab0, 0xfabc, 0xebac, 0xefec, 0xfcfc, 0x3030};        //up-side down heart symbol for the opposite player
const unsigned int empty_heart_symbol_up[] = {0x0300, 0x0fc0, 0x3cf0, 0x3030, 0xf03c, 0xc30c, 0xcfcc, 0xfcfc, 0x3030};  //up-side down drawing of lost lives for the opposite player

//Draws the structure of the information board
void information_board_draw(void)
{
    halLcdHLine(0, INF_SLIDE_WIDTH, LCD_ROW>>1, PIXEL_ON);  //division line drawing
    halLcdHLine(LCD_COL-INF_SLIDE_WIDTH, LCD_COL, LCD_ROW>>1, PIXEL_ON);  //division line drawing
    generate_life_score(MAX_PLAYER_LIFE, MAX_PLAYER_LIFE); //generate lives indicator for both player

}

//Draws a life symbol into a certain position, and the correct symbol according to the player
void draw_life(x,y,player)
{

    if(player==1)
    halLcdImage(heart_symbol, 1, 9, x, y);
    else
    halLcdImage(heart_symbol_up, 1, 9, x, y);
}

//clearing the certain life the correct symbol according to the player from the screen
void clear_life(x,y,player)
{
    if(player==1)
    halLcdImage(empty_heart_symbol, 1, 9, x, y);
    else
    halLcdImage(empty_heart_symbol_up, 1, 9, x, y);
}

//check the player 1 score and update
void clear_player1_score(int playerscore)
{
    if(game_mode_id==1) clear_life(1, ((LCD_ROW>>1)+((2-playerscore)*10))+3, 1); //clear the middle of the heart of life in hard mode there are just three life
    else clear_life(1, ((LCD_ROW>>1)+((MAX_PLAYER_LIFE-playerscore-1)*10))+3, 1); //clear the middle of the heart of life on a certain position

}

//check the player 2 score and update
void clear_player2_score(int playerscore)
{
    clear_life(1, ((LCD_ROW>>1)-((MAX_PLAYER_LIFE-playerscore-1)*10))-11, 2); //clear the middle of the heart of life on a certain position
}

//generate the starting lives 5 or hardmode 3 for player one
void generate_life_score(int p1_life, int p2_life)
{
    int i;
    p1_life_counter = p1_life; //Player 1 life counter set
    p2_life_counter = p2_life; //Player 2 life counter set
    if(game_mode_id==1) p1_life_counter=p1_life_counter-2; //reducing the Player 1 life by two in hard mode
    for(i=0;i<p1_life_counter;i++)
    {
        draw_life(1, ((LCD_ROW>>1)+((i)*10))+3, 1); //P1's life
    }
    for(i=0;i<p2_life_counter;i++)
    {
        draw_life(1, ((LCD_ROW>>1)-((i)*10))-11, 2); //P2's life
    }
}
