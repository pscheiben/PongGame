/*********************************************************
 * ball_movement.c
 *
 * Implements the state diagram for the ball's movement
 * Only a few states are completed, refer to the CW description
 * to find out about the remaining states
 *
 * Created on: Nov 2017
 * Author: M MATA (GCU)
 *********************************************************/
#include "ball_movement.h"
#include "general_settings.h"
#include "hal_lcd.h"
#include "msp430f5438a.h"
#include "informationboard.h"

int top_wall_reached()
{
    if(yBall <= BALL_RADIUS + INF_BRD_WIDTH) //up wall reached
    {
        yBall = 1 + BALL_RADIUS + INF_BRD_WIDTH; //do not overwrite the wall
        return 1;
    }
    else return 0;
}

int bottom_wall_reached()
{
    if(yBall >= LCD_ROW-1-BALL_RADIUS) //bottom wall reached
    {
        yBall = LCD_ROW-2-BALL_RADIUS; //do not overwrite the wall
        return 1;
    }
    else return 0; //bottom wall not reached
}

int right_wall_reached()
{
    if(xBall >= LCD_COL-1-BALL_RADIUS) //right wall reached
    {
        xBall = LCD_COL-2-BALL_RADIUS; //do not overwrite right wall/racket
        return 1;
    }
    else return 0; //right wall not reached
}

int left_wall_reached()
{
    if(xBall <= BALL_RADIUS)  //left wall reached
    {
        xBall = 1+BALL_RADIUS; //do not overwrite wall/racket1
        return 1;
    }
    else return 0; //right wall not reached
}

int P1_racket_hit() //check ball vs left racket
{
 if( (yBall <= (yR1 + HALF_RACKET_SIZE)) && (yBall >= (yR1 - HALF_RACKET_SIZE)) )
     return 1;
 else
     return 0;
}

int P2_racket_hit() //check ball vs right racket
{
 if( (yBall <= (yR2 + HALF_RACKET_SIZE)) && (yBall >= (yR2 - HALF_RACKET_SIZE)) )
     return 1;
 else
     return 0;
}

//Update the state and position of the ball
//(CPU is awaken by TimerA1 interval ints)
void ball_update(void)
{
 //calculate new position and bouncing
 switch(ballState)
 {
 case 0: //"Start" state, init ball position
         yBall = (LCD_ROW + INF_BRD_WIDTH) >> 1;
         xBall = LCD_COL >> 1;
         xBall_old2 = xBall;
         yBall_old2 = yBall;
         xBall_old = xBall;
         yBall_old = yBall;
         //choose next state to start ball movement
         ballState = 1;
         break;

 case 1: //move Right
             xBall = xBall + 2;
             //check right wall reached
             if(right_wall_reached())
             {
                 //If racket is here bounce, otherwise it's a goal
                            if(P2_racket_hit())
                            {
                               if(R2Dir == UP) //up effect on ball
                               { ballState = 9; }
                               else
                               {
                                   if(R2Dir == DOWN) //down effect on ball
                                    {   ballState = 7; }
                                   else //no effect, normal bounce
                                    {   ballState = 8; }
                               }
                            }
                            else //Goal! Player 2 missed the ball
                             { ballState = 16; }
              }

             break;

 case 2: //move Right and a bit Up
             xBall = xBall + 2;
             yBall = yBall - 1;
             //Check top wall bounce
             if(top_wall_reached())
                 ballState = 14; //Top wall hit, bounce to 14
             //check right wall reached
             else if(right_wall_reached())
             {
                 //If racket is here bounce, otherwise it's a goal
                            if(P2_racket_hit())
                            {
                               if(R2Dir == UP) //up effect on ball
                               { ballState = 8; }
                               else
                               {
                                   if(R2Dir == DOWN) //down effect on ball
                                    {   ballState = 6; }
                                   else //no effect, normal bounce
                                    {   ballState = 7; }
                               }
                            }
                            else //Goal! Player 2 missed the ball
                             { ballState = 16; }
              }

             break;

 case 3: //move Right and Up
             xBall = xBall + 2;
             yBall = yBall - 2;
             //Check top wall bounce
             if(top_wall_reached())
                 ballState = 14; //Top wall hit, bounce to 14
             //check right wall reached
             else if(right_wall_reached())
             {
                 //If racket is here bounce, otherwise it's a goal
                            if(P2_racket_hit())
                            {
                               if(R2Dir == UP) //up effect on ball
                               { ballState = 7; }
                               else
                               {
                                   if(R2Dir == DOWN) //down effect on ball
                                    {   ballState = 5; }
                                   else //no effect, normal bounce
                                    {   ballState = 6; }
                               }
                            }
                            else //Goal! Player 2 missed the ball
                             { ballState = 16; }
              }

             break;

 case 4: //move a bit Right and Up
             xBall = xBall + 1;
             yBall = yBall - 2;
             //Check top wall bounce
             if(top_wall_reached())
                 ballState = 12; //Top wall hit, bounce to 14
             //check right wall reached
             else if(right_wall_reached())
             {
                 //If racket is here bounce, otherwise it's a goal
                            if(P2_racket_hit())
                            {
                               if(R2Dir == UP) //up effect on ball
                               { ballState = 6; }
                               else
                               {
                                   if(R2Dir == DOWN) //down effect on ball
                                    {   ballState = 5; }
                                   else //no effect, normal bounce
                                    {   ballState = 5; }
                               }
                            }
                            else //Goal! Player 2 missed the ball
                             { ballState = 16; }
              }

             break;

 case 5: //move a bit Left and Up
             xBall = xBall + 1;
             yBall = yBall - 2;
             //Check top wall bounce
             if(top_wall_reached())
                 ballState = 11; //Top wall hit, bounce to 14
             //check right wall bounce
             else if(left_wall_reached())
             {
                 //If racket is here bounce, otherwise it's a goal
                            if(P1_racket_hit())
                            {
                               if(R1Dir == UP) //up effect on ball
                               { ballState = 2; }
                               else
                               {
                                   if(R1Dir == DOWN) //down effect on ball
                                    {   ballState = 14; }
                                   else //no effect, normal bounce
                                    {   ballState = 1; }
                               }
                            }
                            else //Goal! Player 1 missed the ball
                             { ballState = 15; }
               ballState = 4;  //Right wall is here, bounce to direction 4
             }

             break;

 case  6: //move left and  UP
             xBall = xBall - 2;
             yBall = yBall - 2;
             //Check up wall bounce
             if(top_wall_reached())
                 ballState = 10; //up wall hit, bounce to 10
             //check left wall reached
             else if(left_wall_reached())
             {
                 //If racket is here bounce, otherwise it's a goal
                            if(P1_racket_hit())
                            {
                               if(R1Dir == UP) //up effect on ball
                               { ballState = 2; }
                               else
                               {
                                   if(R1Dir == DOWN) //down effect on ball
                                    {   ballState = 4; }
                                   else //no effect, normal bounce
                                    {   ballState = 3; }
                               }
                            }
                            else //Goal! Player 1 missed the ball
                             { ballState = 15; }
              }

             break;


 case  7: //move left and a bit UP
             xBall = xBall - 2;
             yBall = yBall - 1;
             //Check up wall bounce
             if(top_wall_reached())
                 ballState = 9; //up wall hit, bounce to 9
             //check left wall reached
             else if(left_wall_reached())
             {
                 //If racket is here bounce, otherwise it's a goal
                            if(P1_racket_hit())
                            {
                               if(R1Dir == UP) //up effect on ball
                               { ballState = 1; }
                               else
                               {
                                   if(R1Dir == DOWN) //down effect on ball
                                    {   ballState = 3; }
                                   else //no effect, normal bounce
                                    {   ballState = 2; }
                               }
                            }
                            else //Goal! Player 1 missed the ball
                             { ballState = 15; }
              }

             break;

 case 8: //move Left
         xBall = xBall - 2;
         //check left wall reached
         if(left_wall_reached())
         {
           //If racket is here bounce, otherwise it's a goal
           if(P1_racket_hit())
           {
              if(R1Dir == UP) //up effect on ball
              { ballState = 14; }
              else
              {
                  if(R1Dir == DOWN) //down effect on ball
                   {   ballState = 2; }
                  else //no effect, normal bounce
                   {   ballState = 1; }
              }
           }
           else //Goal! Player 1 missed the ball
            { ballState = 15; }
         }
         break;

 case  9: //move left and a bit Down
             xBall = xBall - 2;
             yBall = yBall + 1;
             //Check bottom wall bounce
             if(bottom_wall_reached())
                 ballState = 7; //bottom wall hit, bounce to 5
             //check left wall reached
             else if(left_wall_reached())
             {
                 //If racket is here bounce, otherwise it's a goal
                            if(P1_racket_hit())
                            {
                               if(R1Dir == UP) //up effect on ball
                               { ballState = 13; }
                               else
                               {
                                   if(R1Dir == DOWN) //down effect on ball
                                    {   ballState = 1; }
                                   else //no effect, normal bounce
                                    {   ballState = 14; }
                               }
                            }
                            else //Goal! Player 1 missed the ball
                             { ballState = 15; }
              }

             break;


 case 10: //move left and Down
             xBall = xBall - 2;
             yBall = yBall + 2;
             //Check bottom wall bounce
             if(bottom_wall_reached())
                 ballState = 6; //bottom wall hit, bounce to 5
             //check left wall reached
             else if(left_wall_reached())
             {
                 //If racket is here bounce, otherwise it's a goal
                            if(P1_racket_hit())
                            {
                               if(R1Dir == UP) //up effect on ball
                               { ballState = 12; }
                               else
                               {
                                   if(R1Dir == DOWN) //down effect on ball
                                    {   ballState = 14; }
                                   else //no effect, normal bounce
                                    {   ballState = 13; }
                               }
                            }
                            else //Goal! Player 1 missed the ball
                             { ballState = 15; }
              }

             break;

 case 11: //move a bit left and Down
             xBall = xBall - 1;
             yBall = yBall + 2;
             //Check bottom wall bounce
             if(bottom_wall_reached())
                 ballState = 5; //bottom wall hit, bounce to 5
             //check left wall reached
             else if(left_wall_reached())
             {
                 //If racket is here bounce, otherwise it's a goal
                            if(P1_racket_hit())
                            {
                               if(R1Dir == UP) //up effect on ball
                               { ballState = 12; }
                               else
                               {
                                   if(R1Dir == DOWN) //down effect on ball
                                    {   ballState = 13; }
                                   else //no effect, normal bounce
                                    {   ballState = 12; }
                               }
                            }
                            else //Goal! Player 1 missed the ball
                             { ballState = 15; }
              }

             break;


 case 12: //move a bit Right and Down
             xBall = xBall + 1;
             yBall = yBall + 2;
             //Check bottom wall bounce
             if(bottom_wall_reached())
                 ballState = 4; //bottom wall hit, bounce to 4
             //check right wall reached
             else if(right_wall_reached())
             {
                 //If racket is here bounce, otherwise it's a goal
                            if(P2_racket_hit())
                            {
                               if(R2Dir == UP) //up effect on ball
                               { ballState = 11; }
                               else
                               {
                                   if(R2Dir == DOWN) //down effect on ball
                                    {   ballState = 10; }
                                   else //no effect, normal bounce
                                    {   ballState = 11; }
                               }
                            }
                            else //Goal! Player 2 missed the ball
                             { ballState = 16; }
              }

             break;

 case 13: //move Right and Down
             xBall = xBall + 2;
             yBall = yBall + 2;
             //Check bottom wall bounce
             if(bottom_wall_reached())
                 ballState = 3; //bottom wall hit, bounce to 3
             //check right wall reached
             else if(right_wall_reached())
             {
                 //If racket is here bounce, otherwise it's a goal
                            if(P2_racket_hit())
                            {
                               if(R2Dir == UP) //up effect on ball
                               { ballState = 11; }
                               else
                               {
                                   if(R2Dir == DOWN) //down effect on ball
                                    {   ballState = 9; }
                                   else //no effect, normal bounce
                                    {   ballState = 10; }
                               }
                            }
                            else //Goal! Player 2 missed the ball
                             { ballState = 16; }
              }
             break;


 case 14: //move Right and a bit Down
             xBall = xBall + 2;
             yBall = yBall + 1;
             //Check bottom wall bounce
             if(bottom_wall_reached())
                 ballState = 2; //bottom wall hit, bounce to 7
             //check right wall reached
             else if(right_wall_reached())
             {
                 //If racket is here bounce, otherwise it's a goal
                            if(P2_racket_hit())
                            {
                               if(R2Dir == UP) //up effect on ball
                               { ballState = 10; }
                               else
                               {
                                   if(R2Dir == DOWN) //down effect on ball
                                    {   ballState = 8; }
                                   else //no effect, normal bounce
                                    {   ballState = 9; }
                               }
                            }
                            else //Goal! Player 2 missed the ball
                             { ballState = 16; }
              }

             break;


 case 15:  //Left-hand player missed the ball!
         p1_life_counter --;
         check_player1_score(p1_life_counter); //reduce the lives on the information board
         if(p1_life_counter>0)
         {
             ballState = 0;
         }
         else
         {
                       //A very simplistic game end handling
                       halLcdClearScreen(); //CLEAR SCREEN
                       halLcdPrintLine("     Game Over", 2, OVERWRITE_TEXT);//PRINT MESSAGE
                       halLcdPrintLine(" Middle Joystick", 6, OVERWRITE_TEXT);//PRINT MESSAGE
                       halLcdPrintLine("  to restart ", 7, OVERWRITE_TEXT);//PRINT MESSAGE
                       //stop TimerA1. This prevents new LCD and ball updates
                       //but user input is operational because is driven by TimerB0
                       TA1CTL= TA1CTL & ~(BIT5 + BIT4); //MC=00 (bits 5,4) 0b11001111
                       ballState = 17;
         }
         break;

 case 16:  //Right-hand player missed the ball!

         p2_life_counter --;  //decrease the Player 2 lifecount
         check_player2_score(p2_life_counter); //reduce the lives on the information board
         if(p2_life_counter>0)
         {
             ballState = 0;
         }
         else
         {
                       //A very simplistic game end handling
                       halLcdClearScreen(); //CLEAR SCREEN
                       halLcdPrintLine("    Game Over", 2, OVERWRITE_TEXT);//PRINT MESSAGE
                       halLcdPrintLine(" Middle Joystick", 6, OVERWRITE_TEXT);//PRINT MESSAGE
                       halLcdPrintLine("   to restart ", 7, OVERWRITE_TEXT);//PRINT MESSAGE
                       //stop TimerA1. This prevents new LCD and ball updates
                       //but user input is operational because is driven by TimerB0
                       TA1CTL= TA1CTL & ~(BIT5 + BIT4); //MC=00 (bits 5,4) 0b11001111
                       ballState = 17;

         }
         break;
 case 17:  //Right-hand player missed the ball!

         WDTCTL=1;
         break;
}
}
