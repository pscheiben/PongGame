//******************************************************************************
//  Pong.c
//
//  Description: Starting point for the Pong game coursework
//  Core functionality is completed (timed operation)
//  ACLK = REFO = LFXT1 32kHz, MCLK = SMCLK = 1MHz
//
//  M. MATA
//  GCU
//  November 2017
//MSP430:  Flash/FRAM usage is 11740 bytes. RAM usage is 4088 bytes.

//******************************************************************************
#include    "msp430f5438a.h"
#include	"hal_UCS.h"
#include 	"hal_PMM.h"
#include    "hal_lcd.h"
#include    "general_settings.h"
#include    "ball_movement.h"
#include    "informationboard.h"
#include    "gamemenu.h"

/* 5xx functions / variables */ 
void halBoardInit(void);
void halADCInit(void);
void halAccPortInit(void);
void LCDInit(void);
void TimerB0Init(void);
void TimerA1Init(void);
void GameStartInit(void);
void LCD_update(void);
void DrawBall(int, int);
void DrawBallTrail(int, int);
void ClearBall(int, int);
void ReadAccX(void);
void ReadAccY(void);


volatile unsigned int LCD_intervals = 0; //count number of base intervals elapsed
volatile unsigned int Ball_intervals = 0; //count number of base intervals elapsed
//volatile =  compiler will not optimize these variables

////This is kept as an ADC12 example, and allows battery monitoring
//volatile unsigned long temp_vcc;
//void format_voltage_string(unsigned int v);
//char LCD_string[5]="x.xV\0"; //test only

//main function
void main(void)
{

  WDTCTL = WDTPW+WDTHOLD; // Stop WDT
  
  // Initialize board
  halBoardInit();

  //ACC Power up and X, Y channel config
  halAccPortInit();

  /* Initialize ADC12 & REF module to sample battery voltage */
  halADCInit();

  /* Initialize TimerB0, it drives ADC12 and times input updates */
  TimerB0Init();

  /* Initialize TimerA1, it times ball and LCD updates */
  TimerA1Init();

  /* Initialize LFXT1, it will drive TA1 and TB0 */
  P7SEL |= BIT1+BIT0;    // Enable crystal pins
  LFXT_Start(XT1DRIVE_0);        

  //Initialize LCD and backlight
  LCDInit();

  //Initialize Menu elemets
  active_menu_id=GameMenuInit();
  //Initialize game variables
  GameStartInit();


  switch(game_mode_id) //according to the game mode selection
   {
       case EASY:
               if(ctrl_id==1)  //SW and JSTICK are selected
               {
                   while(1) //infinite main loop
                                      {
                                        // Bit-set (LPM3_bits + GIE) in SR register to enter LPM3 mode
                                        __bis_SR_register(LPM3_bits + GIE);
                                        __no_operation(); //for debug

                                        //CPU CONTINUES HERE WHEN IT IS AWAKEN AT THE END OF ADC12 or TimerA1 ISR

                                        if(InputUpdatePending)
                                        {
                                            InputUpdatePending=0;
                                            if(!(P2IN & BIT6)) //SW1 pressed
                                                {
                                                    if (xR1 > INF_SLIDE_WIDTH + 1) //avoid overwriting left wall
                                                        {
                                                            xR1=xR1-4;
                                                            InputChangePending = 1;
                                                        }
                                                }

                                            if(!(P2IN & BIT7)) //SW2 pressed
                                                {
                                                    if (xR1 < LCD_COL-2-HALF_RACKET_SIZE*2-INF_SLIDE_WIDTH) //avoid overwriting right wall
                                                        {
                                                            xR1=xR1+4;
                                                            InputChangePending = 1;
                                                        }
                                                }

                                            if(xBall<(xR2 + HALF_RACKET_SIZE))
                                                     {
                                                         if (xR2 > INF_SLIDE_WIDTH + 1) //avoid overwriting left wall
                                                                 {
                                                                     xR2=xR2-2; //move racket 1 pixel left
                                                                     InputChangePending = 1;
                                                                 }
                                                     }
                                                     else
                                                     {
                                                         if(xBall>(xR2 + HALF_RACKET_SIZE))
                                                         {
                                                             if (xR2 < LCD_COL-2-HALF_RACKET_SIZE*2-INF_SLIDE_WIDTH) //avoid overwriting left wall
                                                                             {
                                                                                 xR2=xR2+2; //move racket 1 pixel left
                                                                                 InputChangePending = 1;
                                                                             }
                                                         }
                                                     }

                                        }
                                        if(BallUpdatePending)
                                        {
                                         BallUpdatePending=0;
                                         ball_update();
                                        }
                                        if(LCDUpdatePending)
                                        {
                                         LCDUpdatePending=0;
                                         LCD_update();
                                        }

                                      } //while loop
               }
               else
               {
                   while(1) //infinite main loop
                                      {
                                        // Bit-set (LPM3_bits + GIE) in SR register to enter LPM3 mode
                                        __bis_SR_register(LPM3_bits + GIE);
                                        __no_operation(); //for debug

                                        //CPU CONTINUES HERE WHEN IT IS AWAKEN AT THE END OF ADC12 or TimerA1 ISR

                                        if(InputUpdatePending)
                                        {
                                            InputUpdatePending=0;
                                            ReadAccX();
                                            if(accdx>16) //Accelerometer sensitive enough
                                                {
                                                    if (xR1 > INF_SLIDE_WIDTH + 1) //avoid overwriting left wall
                                                        {
                                                            xR1=xR1-4;
                                                            InputChangePending = 1;
                                                        }
                                                }

                                            if(accdx<-16) //SW2 pressed
                                                {
                                                    if (xR1 < LCD_COL-2-HALF_RACKET_SIZE*2-INF_SLIDE_WIDTH) //avoid overwriting right wall
                                                        {
                                                            xR1=xR1+4;
                                                            InputChangePending = 1;
                                                        }
                                                }

                                            if(xBall<(xR2 + HALF_RACKET_SIZE))
                                                     {
                                                         if (xR2 > INF_SLIDE_WIDTH + 1) //avoid overwriting left wall
                                                                 {
                                                                     xR2=xR2-2; //move racket 1 pixel left
                                                                     InputChangePending = 1;
                                                                 }
                                                     }
                                                     else
                                                     {
                                                         if(xBall>(xR2 + HALF_RACKET_SIZE))
                                                         {
                                                             if (xR2 < LCD_COL-2-HALF_RACKET_SIZE*2-INF_SLIDE_WIDTH) //avoid overwriting left wall
                                                                             {
                                                                                 xR2=xR2+2; //move racket 1 pixel left
                                                                                 InputChangePending = 1;
                                                                             }
                                                         }
                                                     }

                                        }
                                        if(BallUpdatePending)
                                        {
                                         BallUpdatePending=0;
                                         ball_update();
                                        }
                                        if(LCDUpdatePending)
                                        {
                                         LCDUpdatePending=0;
                                         LCD_update();
                                        }

                                      } //while loop
               }
               break;
       case HARD:
               if(ctrl_id==1)  //SW and JSTICK are selected
               {
                   while(1) //infinite main loop
                                      {
                                        // Bit-set (LPM3_bits + GIE) in SR register to enter LPM3 mode
                                        __bis_SR_register(LPM3_bits + GIE);
                                        __no_operation(); //for debug

                                        //CPU CONTINUES HERE WHEN IT IS AWAKEN AT THE END OF ADC12 or TimerA1 ISR

                                        if(InputUpdatePending)
                                        {
                                            InputUpdatePending=0;
                                            if(!(P2IN & BIT6)) //SW1 pressed
                                                {
                                                    if (xR1 > INF_SLIDE_WIDTH + 1) //avoid overwriting left wall
                                                        {
                                                            xR1=xR1-2;
                                                            InputChangePending = 1;
                                                        }
                                                }

                                            if(!(P2IN & BIT7)) //SW2 pressed
                                                {
                                                    if (xR1 < LCD_COL-2-HALF_RACKET_SIZE*2-INF_SLIDE_WIDTH) //avoid overwriting right wall
                                                        {
                                                            xR1=xR1+2;
                                                            InputChangePending = 1;
                                                        }
                                                }

                                            if(xBall<(xR2 + HALF_RACKET_SIZE))
                                                     {
                                                         if (xR2 > INF_SLIDE_WIDTH + 1) //avoid overwriting left wall
                                                                 {
                                                                     xR2=xR2-2; //move racket 1 pixel left
                                                                     InputChangePending = 1;
                                                                 }
                                                     }
                                                     else
                                                     {
                                                         if(xBall>(xR2 + HALF_RACKET_SIZE))
                                                         {
                                                             if (xR2 < LCD_COL-2-HALF_RACKET_SIZE*2-INF_SLIDE_WIDTH) //avoid overwriting left wall
                                                                             {
                                                                                 xR2=xR2+2; //move racket 1 pixel left
                                                                                 InputChangePending = 1;
                                                                             }
                                                         }
                                                     }

                                        }
                                        if(BallUpdatePending)
                                        {
                                         BallUpdatePending=0;
                                         ball_update();
                                        }
                                        if(LCDUpdatePending)
                                        {
                                         LCDUpdatePending=0;
                                         LCD_update();
                                        }

                                      } //while loop
               }
               else
               {
                   while(1) //infinite main loop
                                      {
                                        // Bit-set (LPM3_bits + GIE) in SR register to enter LPM3 mode
                                        __bis_SR_register(LPM3_bits + GIE);
                                        __no_operation(); //for debug

                                        //CPU CONTINUES HERE WHEN IT IS AWAKEN AT THE END OF ADC12 or TimerA1 ISR

                                        if(InputUpdatePending)
                                        {
                                            InputUpdatePending=0;
                                            ReadAccX();
                                            if(accdx>16) //SW1 pressed
                                                {
                                                    if (xR1 > INF_SLIDE_WIDTH + 1) //avoid overwriting left wall
                                                        {
                                                            xR1=xR1-2;
                                                            InputChangePending = 1;
                                                        }
                                                }

                                            if(accdx<-16) //SW2 pressed
                                                {
                                                    if (xR1 < LCD_COL-2-HALF_RACKET_SIZE*2-INF_SLIDE_WIDTH) //avoid overwriting right wall
                                                        {
                                                            xR1=xR1+2;
                                                            InputChangePending = 1;
                                                        }
                                                }


                                            if(xBall<(xR2 + HALF_RACKET_SIZE))
                                                     {
                                                         if (xR2 > INF_SLIDE_WIDTH + 1) //avoid overwriting left wall
                                                                 {
                                                                     xR2=xR2-2; //move racket 1 pixel left
                                                                     InputChangePending = 1;
                                                                 }
                                                     }
                                                     else
                                                     {
                                                         if(xBall>(xR2 + HALF_RACKET_SIZE))
                                                         {
                                                             if (xR2 < LCD_COL-2-HALF_RACKET_SIZE*2-INF_SLIDE_WIDTH) //avoid overwriting left wall
                                                                             {
                                                                                 xR2=xR2+2; //move racket 1 pixel left
                                                                                 InputChangePending = 1;
                                                                             }
                                                         }
                                                     }



                                        }
                                        if(BallUpdatePending)
                                        {
                                         BallUpdatePending=0;
                                         ball_update();
                                        }
                                        if(LCDUpdatePending)
                                        {
                                         LCDUpdatePending=0;
                                         LCD_update();
                                        }

                                      } //while loop
               }
               break;
       case MULTI:
               if(ctrl_id==1)  //SW and JSTICK are selected
               {
                   while(1) //infinite main loop
                                      {
                                        // Bit-set (LPM3_bits + GIE) in SR register to enter LPM3 mode
                                        __bis_SR_register(LPM3_bits + GIE);
                                        __no_operation(); //for debug

                                        //CPU CONTINUES HERE WHEN IT IS AWAKEN AT THE END OF ADC12 or TimerA1 ISR

                                        if(InputUpdatePending)
                                        {
                                            InputUpdatePending=0;
                                            if(!(P2IN & BIT6)) //SW1 pressed
                                                {
                                                    if (xR1 > INF_SLIDE_WIDTH + 1) //avoid overwriting left wall
                                                        {
                                                            xR1=xR1-2;
                                                            InputChangePending = 1;
                                                        }
                                                }

                                            if(!(P2IN & BIT7)) //SW2 pressed
                                                {
                                                    if (xR1 < LCD_COL-2-HALF_RACKET_SIZE*2-INF_SLIDE_WIDTH) //avoid overwriting right wall
                                                        {
                                                            xR1=xR1+2;
                                                            InputChangePending = 1;
                                                        }
                                                }

                                            if(!(P2IN & BIT1)) //JS left pressed
                                                {
                                                   if (xR2 > INF_SLIDE_WIDTH + 1) //avoid overwriting left wall
                                                       {
                                                           xR2=xR2-2; //move racket 2 pixel left
                                                           InputChangePending = 1;
                                                       }
                                                }

                                            if(!(P2IN & BIT2)) //JS right pressed
                                                {
                                                   if (xR2 < LCD_COL-2-HALF_RACKET_SIZE*2-INF_SLIDE_WIDTH) //avoid overwriting right wall
                                                       {
                                                           xR2=xR2+2; //move racket 2 pixel right
                                                           InputChangePending = 1;
                                                       }
                                                }

                                        }
                                        if(BallUpdatePending)
                                        {
                                         BallUpdatePending=0;
                                         ball_update();
                                        }
                                        if(LCDUpdatePending)
                                        {
                                         LCDUpdatePending=0;
                                         LCD_update();
                                        }

                                      } //while loop
                   }
               else
               {
                   while(1) //infinite main loop
                   {
                     // Bit-set (LPM3_bits + GIE) in SR register to enter LPM3 mode
                     __bis_SR_register(LPM3_bits + GIE);
                     __no_operation(); //for debug

                     //CPU CONTINUES HERE WHEN IT IS AWAKEN AT THE END OF ADC12 or TimerA1 ISR

                     if(InputUpdatePending)
                     {
                         InputUpdatePending=0;
                         ReadAccX();
                         ReadAccY();

                         if(accdx>16) //SW1 pressed
                             {
                                 if (xR1 > INF_SLIDE_WIDTH + 1) //avoid overwriting left wall
                                     {
                                         xR1=xR1-2;
                                         InputChangePending = 1;
                                     }
                             }

                         if(accdx<-16) //SW2 pressed
                             {
                                 if (xR1 < LCD_COL-2-HALF_RACKET_SIZE*2-INF_SLIDE_WIDTH) //avoid overwriting right wall
                                     {
                                         xR1=xR1+2;
                                         InputChangePending = 1;
                                     }
                             }

                         if(accdy>16) //JS left pressed
                             {
                                if (xR2 > INF_SLIDE_WIDTH + 1) //avoid overwriting left wall
                                    {
                                        xR2=xR2-2; //move racket 2 pixel left
                                        InputChangePending = 1;
                                    }
                             }

                         if(accdy<-16) //JS right pressed
                             {
                                if (xR2 < LCD_COL-2-HALF_RACKET_SIZE*2-INF_SLIDE_WIDTH) //avoid overwriting right wall
                                    {
                                        xR2=xR2+2; //move racket 2 pixel right
                                        InputChangePending = 1;
                                    }
                             }

                     }
                     if(BallUpdatePending)
                     {
                      BallUpdatePending=0;
                      ball_update();
                     }
                     if(LCDUpdatePending)
                     {
                      LCDUpdatePending=0;
                      LCD_update();
                     }

                   } //while loop
               }
               break;

   }



}

//LCD initialization
void LCDInit(void)
{
 halLcdInit();
 halLcdBackLightInit();
 halLcdSetBackLight(LCD_BACKLIGHT_LEVEL);
 halLcdSetContrast(LCD_CONTRAST_LEVEL);
 halLcdClearScreen();
}

//This function can be used to initialise game variables at boot-up
//Most variables are  declared into general_settings.h
void GameStartInit()
{
    accx_offset = accx;//for semi-random ballstate
    accy_offset = accy;


 // ball init to avoid the left top corner visual artifact
 xBall = LCD_COL >> 1;
 yBall = LCD_ROW >> 1;
 xBall_old2 = xBall;
 yBall_old2 = yBall;
 xBall_old = xBall;
 yBall_old = yBall;


 //Initial position of racket 1 player one bottom side
 xR1 = (LCD_COL >>1) - HALF_RACKET_SIZE;
 yR1 = LCD_ROW - 2;
 yR1_old = yR1; //trail init is the same as the actual position
 xR1_old = xR1;

 //Initial position of racket 2 player two top side
 xR2 = (LCD_COL >>1) - HALF_RACKET_SIZE;
 yR2 = 0;
 yR2_old = yR2;//trail init is the same as the actual position
 xR2_old = xR2;

 InputUpdatePending = 1; //avoid fast starting ball movement
 InputChangePending = 1;
 BallUpdatePending = 1;
 LCDUpdatePending = 1;

 ballState = 0; //initial ball state


 //Draw left and right walls

 halLcdVLine(INF_SLIDE_WIDTH-1, 0, LCD_ROW, PIXEL_ON); //halLcdVLine is more convenient then halLcdLine, it does not draw LCD_ROW pixel
 halLcdVLine(LCD_COL-1-INF_SLIDE_WIDTH, 0, LCD_ROW, PIXEL_ON);
 information_board_draw();

//Draw the two racket
 halLcdHLine(xR1, xR1 + HALF_RACKET_SIZE*2+1, yR1, PIXEL_ON);    //Drawing the 1st racket at init
 halLcdHLine(xR1, xR1 + HALF_RACKET_SIZE*2+1, yR1 + 1, PIXEL_ON);

 halLcdHLine(xR2, xR2 + HALF_RACKET_SIZE*2+1, yR2, PIXEL_ON); //Drawing the 2nd racket at the init
 halLcdHLine(xR2, xR2 + HALF_RACKET_SIZE*2+1, yR2 + 1, PIXEL_ON);
}

////Read user inputs here (CPU is awaken by ADC12 conversion)
//void UserInputs_update(void)
//{
//
// if(!(P2IN & BIT6)) //SW1 pressed
// {
//  if (xR1 > INF_SLIDE_WIDTH + 1) //avoid overwriting left wall
//  {
//
//   if(game_mode_id==0)
//       xR1=xR1-3;
//   else
//     xR1=xR1-2; //move racket1 2 pixel left
//   InputChangePending = 1;
//  }
// }
//
// if(!(P2IN & BIT7)) //SW2 pressed
// {
//  if (xR1 < LCD_COL-2-HALF_RACKET_SIZE*2-INF_SLIDE_WIDTH) //avoid overwriting right wall
//  {
//
//   if(game_mode_id==0)
//       xR1=xR1+2;
//   else
//       xR1=xR1+2; //move racket1 2 pixel right
//   InputChangePending = 1;
//  }
// }
//
// switch(game_mode_id)
//  {
//  case 0: //"Start" state, init ball position
//          if(xBall<(xR2 + HALF_RACKET_SIZE))
//          {
//              if (xR2 > INF_SLIDE_WIDTH + 1) //avoid overwriting left wall
//                      {
//                          xR2=xR2-2; //move racket 1 pixel left
//                          InputChangePending = 1;
//                      }
//          }
//          else
//          {
//              if(xBall>(xR2 + HALF_RACKET_SIZE))
//              {
//                  if (xR2 < LCD_COL-2-HALF_RACKET_SIZE*2-INF_SLIDE_WIDTH) //avoid overwriting left wall
//                                  {
//                                      xR2=xR2+2; //move racket 1 pixel left
//                                      InputChangePending = 1;
//                                  }
//              }
//          }
//          break;
//  case 1: //"Start" state, init ball position
//      if(xBall<(xR2 + HALF_RACKET_SIZE))
//      {
//          if (xR2 > INF_SLIDE_WIDTH + 1) //avoid overwriting left wall
//                  {
//                      xR2=xR2-2; //move racket 2 pixel left
//                      InputChangePending = 1;
//                  }
//      }
//      else
//      {
//          if(xBall>(xR2 + HALF_RACKET_SIZE))
//          {
//              if (xR2 < LCD_COL-2-HALF_RACKET_SIZE*2-INF_SLIDE_WIDTH) //avoid overwriting left wall
//                              {
//                                  xR2=xR2+2; //move racket 2 pixel left
//                                  InputChangePending = 1;
//                              }
//          }
//      }
//      break;
//  case 2: //"Start" state, init ball position
//         if(!(P2IN & BIT1)) //JS left pressed
//         {
//             if (xR2 > INF_SLIDE_WIDTH + 1) //avoid overwriting left wall
//             {
//                 xR2=xR2-2; //move racket 2 pixel left
//                 InputChangePending = 1;
//             }
//         }
//
//         if(!(P2IN & BIT2)) //JS right pressed
//         {
//             if (xR2 < LCD_COL-2-HALF_RACKET_SIZE*2-INF_SLIDE_WIDTH) //avoid overwriting right wall
//             {
//                xR2=xR2+2; //move racket 2 pixel right
//                InputChangePending = 1;
//             }
//         }
//         break;
//  }
//}



//Update drawings in LCD screen (CPU is awaken by TimerA1 interval ints)
void LCD_update(void)
{

 //Clear oldest ball
 ClearBall(xBall_old2,yBall_old2);
 //Draw ball trail
 DrawBallTrail(xBall_old,yBall_old);
 //Draw new ball
 DrawBall(xBall,yBall);
 //update older positions for drawing ball trail and deleting old ball
 xBall_old2=xBall_old;
 yBall_old2=yBall_old;
 xBall_old=xBall;
 yBall_old=yBall;

 if((InputChangePending==1) || (ballState == 0)) //only if there was movement redraw both racket
 {

         //Clear old racket2
         halLcdHLine(xR1_old, xR1_old + HALF_RACKET_SIZE*2+1, yR1, PIXEL_OFF);    //Drawing the 1st racket at init
         halLcdHLine(xR1_old, xR1_old + HALF_RACKET_SIZE*2+1, yR1 + 1, PIXEL_OFF);
         //Draw new racket1
         halLcdHLine(xR1, xR1 + HALF_RACKET_SIZE*2+1, yR1, PIXEL_ON);    //Drawing the 1st racket at init
         halLcdHLine(xR1, xR1 + HALF_RACKET_SIZE*2+1, yR1 + 1, PIXEL_ON);
         //update older positions to clear old racket1
         xR1_old = xR1;

         //Clear old racket2
         halLcdHLine(xR2_old, xR2_old + HALF_RACKET_SIZE*2+1, yR2, PIXEL_OFF); //Drawing the 2nd racket at the init
         halLcdHLine(xR2_old, xR2_old + HALF_RACKET_SIZE*2+1, yR2 + 1, PIXEL_OFF);
         //Draw new racket2
         halLcdHLine(xR2, xR2 + HALF_RACKET_SIZE*2+1, yR2, PIXEL_ON); //Drawing the 2nd racket at the init
         halLcdHLine(xR2, xR2 + HALF_RACKET_SIZE*2+1, yR2 + 1, PIXEL_ON);
         //update older positions to clear old racket2
         xR2_old = xR2;

 }
 else
 {

 }
 InputChangePending==0;
}



void DrawBall(int x, int y)
{
    halLcdPixel(x, y, PIXEL_ON);
    halLcdPixel(x-1, y, PIXEL_ON);
    halLcdPixel(x+1, y, PIXEL_ON);
    halLcdPixel(x, y-1, PIXEL_ON);
    halLcdPixel(x, y+1, PIXEL_ON);
}

void DrawBallTrail(int x, int y)
{
    halLcdPixel(x, y, PIXEL_LIGHT);
    halLcdPixel(x-1, y, PIXEL_LIGHT);
    halLcdPixel(x+1, y, PIXEL_LIGHT);
    halLcdPixel(x, y-1, PIXEL_LIGHT);
    halLcdPixel(x, y+1, PIXEL_LIGHT);
}

void ClearBall(int x, int y)
{
    halLcdPixel(x, y, PIXEL_OFF);
    halLcdPixel(x-1, y, PIXEL_OFF);
    halLcdPixel(x+1, y, PIXEL_OFF);
    halLcdPixel(x, y-1, PIXEL_OFF);
    halLcdPixel(x, y+1, PIXEL_OFF);
}

void ReadAccX(void)
{
    accdx = accx-accx_offset;

}

void ReadAccY(void)
{
     accdy = accy-accy_offset;
}

// create string with voltage measurement
//void format_voltage_string(unsigned int voltage)
//{
//  unsigned char ones = '0';
//  unsigned char tenths = '0';
//  while(voltage >= 10)
//  {
//    ones++;
//    voltage -= 10;
//  }
//  tenths += voltage;
//  LCD_string[0] = ones;
//  LCD_string[2] = tenths;
//}

//void balanceBall(void)
//{
//    int x, y, dx, dy, dz, redrawCounter;
//    unsigned char quit = 0;
//
//    halAccelerometerInit();
//
//    halButtonsInterruptDisable(BUTTON_ALL);
//    halButtonsInterruptEnable(BUTTON_SELECT);
//    buttonsPressed  = 0;
//
//    //Starting location of the Ball
//    x = 80;
//    y = 16;
//
//    halLcdClearScreen();
//    halLcdImage(TI_BUG, 14, 106, 10, 0);
//
//    RTCExit64Hz = 1;
//    RTCPS0CTL |= RT0PSIE;
//    redrawCounter = 0;
//    halAdcSetQuitFromISR(0);
//
//    while (!quit)
//    {
//        halAdcStartRead();
//
//        __bis_SR_register(LPM0_bits + GIE);
//        __no_operation();
//
//        if (buttonsPressed & BUTTON_SELECT || buttonsPressed & BUTTON_S1)
//            quit = 1;
//
//        halAccelerometerRead(&dx, &dy, &dz);
//        dx >>= 5;
//        dy >>= 5;
//        if ((ABS(dx) > 2) || (ABS(dy) > 2))
//        {
//            halLcdCircle(x, y, 7, PIXEL_OFF);
//            halLcdCircle(x, y, 6, PIXEL_OFF);
//            halLcdCircle(x, y, 5, PIXEL_OFF);
//            halLcdCircle(x, y, 4, PIXEL_OFF);
//            halLcdCircle(x, y, 3, PIXEL_OFF);
//            halLcdCircle(x, y, 2, PIXEL_OFF);
//            halLcdCircle(x, y, 1, PIXEL_OFF);
//            x -= dx;
//            y += dy;
//
//            if (x < 0 || x >= LCD_COL)
//                x += dx;
//            if (y < 0 || y >= LCD_ROW)
//                y -= dy;
//
//            halLcdCircle(x, y, 2, PIXEL_ON);
//            halLcdCircle(x, y, 3, PIXEL_ON);
//            halLcdCircle(x, y, 4, PIXEL_ON);
//            halLcdCircle(x, y, 5, PIXEL_ON);
//            halLcdCircle(x, y, 6, PIXEL_ON);
//        }
//        if (++redrawCounter == 320)             //Redraw canvas after ~5s
//        {
//            halLcdClearScreen();
//            halLcdImage(TI_BUG, 14, 106, 10, 0);
//            redrawCounter = 0;
//            halLcdCircle(x, y, 2, PIXEL_ON);
//            halLcdCircle(x, y, 3, PIXEL_ON);
//            halLcdCircle(x, y, 4, PIXEL_ON);
//            halLcdCircle(x, y, 5, PIXEL_ON);
//            halLcdCircle(x, y, 6, PIXEL_ON);
//        }
//    }
//
//    RTCPS0CTL &= ~RT0PSIE;
//    RTCExit64Hz = 0;
//}



/*********************************************************
 ********** System initializations ***********************
 *********************************************************/
//MSP430 pins initialization
void halBoardInit(void)
{
  // Tie unused ports and pins as LOW outputs to save power
    //PA = P1 and P2 (16 bits)
    //PB = P3 and P4 (16 bits)
    //PC = P5 and P6 (16 bits)
    //PD = P7 and P8 (16 bits)
    //PE = P9 and P10 (16 bits)
  PAOUT  = 0; PADIR  = 0xFFFF; PASEL  = 0;
  PBOUT  = 0; PBDIR  = 0xFFFF; PBSEL  = 0;
  PCOUT  = 0; PCDIR  = 0xFFFF; PCSEL  = 0;
  PDOUT  = 0; PDDIR  = 0xFFFF; PDSEL  = 0;
  // P10.0 to USB RST pin, if enabled with J5
  PEOUT  = 0; PEDIR  = 0xFEFF; PESEL  = 0;
  PFOUT  = 0; PFDIR  = 0xFEFF; PFSEL  = 0;
  P11OUT = 0; P11DIR = 0xFF;   P11SEL = 0;
  PJOUT  = 0; PJDIR  = 0xFF;

  P6OUT = 0x40;      // Shut down audio output amp
  P5DIR &= ~0x80;    // USB RX Pin, Input with pull-down
  P5OUT &= ~0x80;
  P5REN |= 0x80;

  //Now configure SW1+SW2 (P2.6+P2.7) as input with pull-down (example)
  P2DIR &= ~(BIT6+BIT7); //pin 6+7 input
  P2REN = P2REN | (BIT6+BIT7); //pin 6+7 internal pull R enabled
  P2OUT = P2OUT | (BIT6+BIT7); //pin 6+7 pull-down
//  P2IES |= (BIT6+BIT7); //switches interrupt edge is high to low
//  P2IFG &= ~(BIT6+BIT7); //clear interrupt flags for the switches
//  P2IE |= (BIT6+BIT7); //Interrupt enable for the switches

  //Now configure joystick UP/Down Left/Right Middle (P2.4+P2.5)(P2.1+P2.2)(P2.3) as input with pull-down (example)
  P2DIR &= ~(BIT4+BIT5+BIT1+BIT2+BIT3); //pin 1 to 5 input
  P2REN = P2REN | (BIT4+BIT5+BIT1+BIT2+BIT3); //pin 1 to 5 internal pull R enabled
  P2OUT = P2OUT | (BIT4+BIT5+BIT1+BIT2+BIT3); //pin 1 to 5 pull-down
//  P2IES |= (BIT4+BIT5+BIT1+BIT2+BIT3); //pin 1 to 5 interrupt edge is high to low
//  P2IFG &= ~(BIT4+BIT5+BIT1+BIT2+BIT3); //clear interrupt flags for the joystick
//  P2IE |= (BIT4+BIT5+BIT1+BIT2+BIT3); //Interrupt enable for the joystick

   //Now configure LED1 and LED2 (P1.0+P1.1) as output
   P1DIR |= (BIT0+BIT1); //pin 1+2 output
}

void halAccPortInit(void)
{
    //Configure ACC Power port 6.0
//    P6SEL |= BIT0; //GPIO function
    P6DIR |= BIT0; //Power pin output
    P6OUT |= BIT0; //ACC power - 6.0 out HIGH

    //Configure Accx, Accy, which are port 6.1, 6.2
    P6SEL |= (BIT1 + BIT2); //GPIO function
    P6DIR &= ~(BIT1 + BIT2); //accx, accy as input




}

//Inits ADC12 to read battery voltage triggered from TB0.OUT3
void halADCInit(void)
{
    ADC12CTL0 &= ~ADC12ENC;                // Ensure ENC is clear
    // 5.4MHz (MODOSC_max) * 30 us (t_sensor)= 162 cycles min sampling time
  ADC12CTL0 = ADC12SHT0_7+ADC12ON+ADC12MSC;     // Set sample time to 192 cycles
  // Enable TB0.OUT1 as ADC trigger (ADC12SHS_3), enable sequence-of-channels
  ADC12CTL1 = ADC12SHS_3 + ADC12CONSEQ_1+ADC12SHP; // ... also enable sample timer
  ADC12CTL2 = ADC12RES_2; //12 bit conversion
  //define conversion sequence (just Batt voltage so only ADC12MEM0 used)
  ADC12MCTL0 = ADC12INCH_1;// ADC input ch A1 = ACCx
  ADC12MCTL1 = ADC12INCH_2 + ADC12EOS;// ADC input ch A2 = ACCy + EndOfSequence

//  ADC12MCTL2 = ADC12SREF_1 + ADC12INCH_3 + ADC12EOS;// ADC input ch A3 = ACCz

  ADC12IE |= BIT1;                             //Enable ADC12MEM0IFG interrupt

  /* Initialize the shared reference module */
//  REFCTL0 |= REFMSTR + REFVSEL_1 + REFON;      // Configure internal 2.0V reference
//  ADC12CTL0 |= ADC12ENC;                // Enable conversions to configure REF
}

//NOTE: TimerA0 is initialized inside hal_lcd.c because it's already used for the LCD backlight PWM
//Initialize TimerA1 as basic intervals generator
void TimerA1Init(void)
{
  //TA1CCR0   = TIMING_BASE_mS*(Faclk/1000);
  // TIMING_BASE_mS*(32.768) ~ TIMING_BASE_mS*(33)
  TA1CCR0   = TIMING_BASE_mS*33;
  TA1CCTL0 = CCIE; //Enable TA1CCR0 CCIFG int
  TA1CTL    = TASSEL_1 + MC_1 + TACLR; // ACLK, UP mode. Do not enable TAIE (rollover int)
}

//Initialize TimerB0 to drive the ADC12 and inputs reading
void TimerB0Init(void)
{
  //TB0CCR0   = INPUT_INTERVAL_mS*(Faclk/1000);
  // INPUT_INTERVAL_mS*(32.768) ~ INPUT_INTERVAL_mS*(33)
  TB0CCR0   = INPUT_INTERVAL_mS*33; //set timer interval
  // Provide CCR1 value, time until CCR1 match must be greater than 75 us!
  TB0CCR1   = 3;         //this time will be 3 * 1/32768kHZ (> 75usec)
  TB0CCTL1  = OUTMOD_3;  // CCR1 output mode => SET/RESET. Do not enable TB0CCR3 INT
  // ACLK, UP mode, clear TBR, enable rollover INT
  TB0CTL    = TBSSEL_1 + MC_1 + TBCLR + TBIE;
}

/***********************************************************************
 *************** Interrupt Service Routines (ISRs) *********************
 ***********************************************************************/
// Timer A1 CCR0 interrupt service routine
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
{
 //interrupt flag CCIFG is automatically cleared when servicing this ISR
  if(!LCDUpdatePending) //if update still pending, skip this interval
 {
   LCD_intervals++;
   if(LCD_intervals>=(LCD_INTERVAL_mS/TIMING_BASE_mS)) //time to run the ball update
   {
      LCDUpdatePending = 1; //warn the CPU that LCD update is required
      LCD_intervals = 0;
   }
 }

 if(!BallUpdatePending) //if update still pending, skip this interval
 {
   Ball_intervals++;
   if(Ball_intervals>=(BALL_INTERVAL_mS/TIMING_BASE_mS)) //time to run the ball update
   {
     BallUpdatePending = 1; //warn the CPU that ball update is required
     Ball_intervals = 0;
   }
 }

 if(BallUpdatePending || LCDUpdatePending)
 { //keep CPU active after ISR to process updates in main loop
   __bic_SR_register_on_exit(LPM3_bits);
 }
}



// Timer B0 overflow and CCRx (x>0) interrupt service routine
#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void)
{
  switch(__even_in_range(TB0IV,14))
  {
    case  0: break;                // No interrupt
    case  2: break;                // TB0CCR1 not used
    case  4: break;                // TB0CCR2 not used
    case  6: break;                // TB0CCR3 not used
    case  8: break;                // TB0CCR4 not used
    case 10: break;                // TB0CCR5 not used
    case 12: break;                // TB0CCR6 not used
    case 14:                       // TBIFG overflow, enable new ADC
//            REFCTL0 |= REFON;      // Enable internal reference
            ADC12CTL0 |= ADC12ENC; // Enable conversions to configure REF
            break;
  }
}

//ADC12 interrupt service routine
#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR (void)
{
  switch(__even_in_range(ADC12IV,34))
  {
  case  0: break;                  // Vector  0:  No interrupt
  case  2: break;                  // Vector  2:  ADC overflow
  case  4: break;                  // Vector  4:  ADC timing overflow
  case  6:                         // Vector  6:  ADC12IFG0, last conversion now

  case  8:                         // Vector  8:  ADC12IFG1
          //Data is ready (both MEM0 and MEM1 converted)
          ADC12CTL0 &= ~ADC12ENC;  // Disable conversions to disable VREF
//          REFCTL0 &= ~REFON;       // Disable internal reference
          accx = ADC12MEM0;
          accy = ADC12MEM1;
          //accz = ADC12MEM2;    //accz digitalisation

          InputUpdatePending = 1;  //warn the CPU that input update is required
          //Keep CPU active on exit to process user inputs in main loop
          __bic_SR_register_on_exit(LPM3_bits);

           break;
  case 10: break;                  // Vector 10:  ADC12IFG2
  case 12: break;                  // Vector 12:  ADC12IFG3
  case 14: break;                  // Vector 14:  ADC12IFG4
  case 16: break;                  // Vector 16:  ADC12IFG5
  case 18: break;                  // Vector 18:  ADC12IFG6
  case 20: break;                  // Vector 20:  ADC12IFG7
  case 22: break;                  // Vector 22:  ADC12IFG8
  case 24: break;                  // Vector 24:  ADC12IFG9
  case 26: break;                  // Vector 26:  ADC12IFG10
  case 28: break;                  // Vector 28:  ADC12IFG11
  case 30: break;                  // Vector 30:  ADC12IFG12
  case 32: break;                  // Vector 32:  ADC12IFG13
  case 34: break;                  // Vector 34:  ADC12IFG14
  }
}


