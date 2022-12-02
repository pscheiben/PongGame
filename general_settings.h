/*
 * general_settings.h
 *
 *  Created on: Nov 2017
 *      Author: M Mata
 */

#ifndef GENERAL_SETTINGS_H_
#define GENERAL_SETTINGS_H_

#define LCD_BACKLIGHT_LEVEL 5 //0 to 16
#define LCD_CONTRAST_LEVEL 90 //70 to 127


#define HALF_RACKET_SIZE 7 //racket width in pixels
#define BALL_RADIUS 1 //ball radius in pixels


//Timing intervals for the game
#define TIMING_BASE_mS 5 //base timing interval for all timed execution
#define LCD_INTERVAL_mS 40 //timing interval for updating LCD
#define INPUT_INTERVAL_mS 100 //timing interval for reading inputs
#define BALL_INTERVAL_mS 50 //timing interval for updating ball position

//Tracking the menu and the settings for the game
volatile int active_menu_id;


//flags marking when updates must be done
volatile unsigned int InputUpdatePending;
volatile unsigned int BallUpdatePending;
volatile unsigned int LCDUpdatePending;

//Ball handling
volatile unsigned int ballState; //Current ball state
volatile int xBall, yBall; //Current ball position
volatile int xBall_old, yBall_old; //For ball trail position
volatile int xBall_old2, yBall_old2; //To delete old ball position


//Racket1's variables
volatile int xR1, yR1; //Current racket 1 position
volatile int yR1_old; //To delete old racket position

//Racket2's variables
volatile int xR2, yR2; //Current racket 2 position
volatile int yR2_old; //To delete old racket position

#endif /* GENERAL_SETTINGS_H_ */
