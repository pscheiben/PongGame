/*********************************************************
 * informationboard.h
 *
 *  Created on: 27 Nov 2022
 *      Author: Peter Scheibenoffer
 **********************************************************/

#ifndef INFORMATIONBOARD_H_
#define INFORMATIONBOARD_H_

#define INF_BRD_WIDTH 12 //information board width
#define MAX_PLAYER_LIFE 5 //players life
#define LIFE_IND_RADIUS 3 //life indicator circle radius

void information_board_draw(void);              //Draws the structure of the information board
void generate_life_score(int p1_life, int p2_life); //Init life generation
void draw_life(int x, int y, int player);      //draws a life symbol into a certain position
void clear_life(int x, int y, int player);   //clear the middle of the symbol
void clear_player1_score(int playerscore);  //check player 1 score
void clear_player2_score(int playerscore);  //check player 2 score

volatile int p1_life_counter, p2_life_counter;  //actual life of the players

#endif /* INFORMATIONBOARD_H_ */


