/*********************************************************
 * informationboard.h
 *
 *  Created on: 27 Nov 2022
 *      Author: Sejbi
 **********************************************************/

#ifndef INFORMATIONBOARD_H_
#define INFORMATIONBOARD_H_


#define INF_BRD_WIDTH 12 //information board width
#define MAX_PLAYER_LIFE 5 //players life
#define LIFE_IND_RADIUS 3 //players life




void information_board_draw(void);
void information_board_clean(void);
void generate_life_score(int number_of_life); //init life generation
void draw_life(int x, int y);      //draws a life symbol into a certain position


volatile int p1_life_counter, p2_life_counter;  //actual life of the players

#endif /* INFORMATIONBOARD_H_ */


