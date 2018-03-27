#ifndef BUTTONS_HANDLER_H
#define BUTTONS_HANDLER_H

//=========================================================
/* enums for exti_finite_state_machine */


enum BUTTON_STATES
{
  USER_BUTTON_RELEASED = 0,
  USER_BUTTON_PRESSED,
  USER_BUTTON_LONG_PRESSED,
  LEFT_BUTTON_RELEASED,
  LEFT_BUTTON_PRESSED,
  LEFT_BUTTON_LONG_PRESSED,
  RIGHT_BUTTON_RELEASED,
  RIGHT_BUTTON_PRESSED,
  RIGHT_BUTTON_LONG_PRESSED,
  BUTTON_STATES_DEFAULT
};

enum EDGE
{
  FALLING = 0,
  RISING,
  EDGE_DEFAULT
};

enum BUTTONS
{
  BUTTON_USER = 0,
  BUTTON_LEFT,
  BUTTON_RIGHT,
  BUTTONS_DEFAULT
};

/* enum for alarm_finite_state_machine */
enum ALARM_STATES
{
  CLOCK = 0,
  TUNE,
  ALARM,
  ALARM_STATES_DEFAULT
};

/* enum for tune_finite_state_machine */
enum TUNE_STATES
{
  TUNE_HOUR = 0,
  TUNE_MINUTE,
  TUNE_STATES_DEFAULT
};



/* some procedures downstairs*/

/*Interruption Handlers*/
void EXTI0_1_IRQHandler(void);
void EXTI4_15_IRQHandler(void);


/* Buttons initialisation*/
void UserButton_Init(void);
void FirstButton_Init(void);
void SecondButton_Init(void);

/* Finite State Machine for interruptions */
void exti_finite_state_machine(enum EDGE edge, enum BUTTONS button);

/* Finite State Machine for our Alarm */
void alarm_finite_state_machine(enum BUTTON_STATES button_state);

/* Finite State Machine for Tuning mode for ssour Alarm */
void tune_finite_state_machine(enum BUTTON_STATES button_state);



/* TIME of ALARM!!!! */
uint64_t TIME_X = 500000000;

/* variable to make noise during ALARMING!!!  */
enum ALARM_STATES global_alarm_state = CLOCK;

//===============================================================================

#endif //BUTTONS_HANDLER_H
