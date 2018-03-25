#ifndef BUTTONS_HANDLER_C
#define BUTTONS_HANDLER_C


//==========================================================================
void
UserButton_Init(void) {
        /* Enable the BUTTON Pin Clock */
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);

        /* Configure button Pin, Expclicit setting NO PULL MODE  */
        LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_0, LL_GPIO_MODE_INPUT);
        LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_0, LL_GPIO_PULL_NO);

        LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);

        /* Connecting PA0 to EXTI0 Line  */
        LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE0);

        /* Set Pending bit to 1   */
        LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_0);


        /* Set interruption to fallling from 1 to 0 */
        LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_0);
        LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_0);


        NVIC_EnableIRQ(EXTI0_1_IRQn);
        NVIC_SetPriority(EXTI0_1_IRQn, 0);
}

void
FirstButton_Init(void) {

        /* Activating and configurating button pin */
        LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_13, LL_GPIO_MODE_INPUT);    // First button
        LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_13, LL_GPIO_PULL_DOWN);

        /* Enable clocking to SYSCFG */
        LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);

        /* Connecting PC13 to EXTI13 Line  */
        LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE13);

        /* Set Pending bit to 1   */
        LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_13);

        /* Set interruption to fallling from 1 to 0 */
        LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_13);
        LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_13);

        /* Configurating Interruption*/
        NVIC_SetPriority(EXTI4_15_IRQn, 0);
        NVIC_EnableIRQ(EXTI4_15_IRQn);

}

void
SecondButton_Init(void) {

        /* commented because we already clocked GPIOC*/
        //LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);

        /* Activating and configurating button pin */
        LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_14, LL_GPIO_MODE_INPUT);    // Second button
        LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_14, LL_GPIO_PULL_DOWN);

        /* Enable clocking to SYSCFG */
        LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);

        /* Connecting PC13 to EXTI13 Line  */
        LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE14);

        /* Set Pending bit to 1   */
        LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_14);

        /* Set interruption to fallling frsom 1 to 0 */
        LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_14);
        LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_14);


        /* Configurating Interruption*/
        NVIC_SetPriority(EXTI4_15_IRQn, 0);
        NVIC_EnableIRQ(EXTI4_15_IRQn);

}

/*button interrupt handler*/
void
EXTI0_1_IRQHandler(void)
{
        /* call exti_finite_state_machine */
        exti_finite_state_machine(LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_0), BUTTON_USER);

        /*don't forget to add this line at the end*/
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_0);
}




/* One Handler for lines form 4 to 15*/
void
EXTI4_15_IRQHandler(void)
{
    /* Check, which button is pressed*/
    if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_13)) {

        /* call exti_finite_state_machine */
        exti_finite_state_machine(LL_GPIO_IsInputPinSet(GPIOC, LL_GPIO_PIN_13), BUTTON_LEFT);

        /* essential procedure*/
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_13);
    }

    /* Check, which button is pressed*/
      if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_14)) {

        /* call exti_finite_state_machine */
        exti_finite_state_machine(LL_GPIO_IsInputPinSet(GPIOC, LL_GPIO_PIN_14), BUTTON_RIGHT);

        /* essential procedure*/
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_14);

    }
}

void
exti_finite_state_machine(enum EDGE edge, enum BUTTONS button)
{
    switch (button)
    {
      case BUTTON_USER :
        {
            static enum BUTTON_STATES user_button_state =  USER_BUTTON_RELEASED;
            static uint64_t user_button_timestamp = 0;
            static uint64_t user_button_press_time = 0;
            switch(user_button_state)
                {

                    case USER_BUTTON_RELEASED :
                        if ((edge == RISING) && (abs(TIME - user_button_timestamp) > 50))
                        {
                            /* situation when we notice, that button is pressed, but dont know type of press*/

                            user_button_state = USER_BUTTON_PRESSED;
                            user_button_timestamp = TIME;
                            user_button_press_time = TIME;

                        }
                        break;


                    case USER_BUTTON_PRESSED :
                        if((edge == FALLING) && (abs(TIME - user_button_timestamp) > 50))
                        {
                            /* situaton, when button is pressed for a long time*/
                            if (abs(TIME - user_button_press_time) > 250) {
                                alarm_finite_state_machine(USER_BUTTON_LONG_PRESSED);
                            }

                            /* situaton, when button is usually pressed*/
                            else {
                                alarm_finite_state_machine(USER_BUTTON_PRESSED);
                            }

                            user_button_state = USER_BUTTON_RELEASED;
                            user_button_timestamp = TIME;
                            user_button_press_time = 0;
                        }
                        break;



                        default:
                            while(1);
                }
        }
      break;

      case BUTTON_LEFT :
        {
            static enum BUTTON_STATES left_button_state =  LEFT_BUTTON_RELEASED;
            static uint64_t left_button_timestamp = 0;
            static uint64_t left_button_press_time = 0;
            switch(left_button_state)
              {

                  case LEFT_BUTTON_RELEASED :
                      if ((edge == RISING) && (abs(TIME - left_button_timestamp) > 50))
                      {
                          /* situation when we notice, that button is pressed*/
                          left_button_state = LEFT_BUTTON_PRESSED;
                          left_button_timestamp = TIME;
                          left_button_press_time = TIME;
                      }
                      break;


                  case LEFT_BUTTON_PRESSED :
                      if((edge == FALLING) && (abs(TIME - left_button_timestamp) > 50))
                      {
                          /* situaton, when button is pressed for a long time*/
                          if (abs(TIME - left_button_press_time) > 250) {
                              alarm_finite_state_machine(LEFT_BUTTON_LONG_PRESSED);
                          }
                          /* situaton, when button is usually pressed*/
                          else {
                              alarm_finite_state_machine(LEFT_BUTTON_PRESSED);
                          }

                          left_button_state = LEFT_BUTTON_RELEASED;
                          left_button_timestamp = TIME;
                          left_button_press_time = 0;
                      }
                      break;

                      default:
                          while(1);
              }
        }
        break;

      case BUTTON_RIGHT :
        {
            static enum BUTTON_STATES right_button_state = RIGHT_BUTTON_RELEASED;
            static uint64_t right_button_timestamp = 0;
            static uint64_t right_button_press_time = 0;
            switch(right_button_state)
              {
                  case RIGHT_BUTTON_RELEASED :
                      if ((edge == RISING) && (abs(TIME - right_button_timestamp) > 50))
                      {
                          /* situation when we notice, that button is pressed*/
                          right_button_state = RIGHT_BUTTON_PRESSED;
                          right_button_timestamp = TIME;
                          right_button_press_time = TIME;
                      }
                      break;


                  case RIGHT_BUTTON_PRESSED :
                      if((edge == FALLING) && (abs(TIME - right_button_timestamp) > 50))
                      {
                          /* situaton, when button is pressed for a long time*/
                          if(abs(TIME - right_button_press_time) > 250){
                              alarm_finite_state_machine(RIGHT_BUTTON_LONG_PRESSED);
                          }
                          /* situaton, when button is usually pressed*/
                          else {
                              alarm_finite_state_machine(RIGHT_BUTTON_PRESSED);
                          }
                          right_button_state = RIGHT_BUTTON_RELEASED;
                          right_button_timestamp = TIME;
                          right_button_press_time = 0;
                      }
                      break;

                      default:
                          while(1);
              }
        }
        break;

        default:
            while(1);

    }
}

void
alarm_finite_state_machine(enum BUTTON_STATES button_state)
{
    static enum ALARM_STATES alarm_state = CLOCK;
    global_alarm_state = alarm_state;
    switch (alarm_state)
      {
          case CLOCK :
            {
                if (button_state == USER_BUTTON_LONG_PRESSED)
                {
                    TIME_X = 0;
                    alarm_state = TUNE;
                }
            }
            break;

          case TUNE :
            {
                if (button_state == USER_BUTTON_LONG_PRESSED)
                {
                    alarm_state = CLOCK;
                }
                else
                {
                    tune_finite_state_machine(button_state);
                }
            }
            break;

          case ALARM :
            {
                if (button_state == USER_BUTTON_PRESSED)
                {
                    alarm_state  = CLOCK;
                }
            }
            break;

          default:
              while(1);
      }
      global_alarm_state = alarm_state;
}


void
tune_finite_state_machine(enum BUTTON_STATES button_state)
{
  static enum TUNE_STATES tune_state = TUNE_HOUR;
  switch (tune_state)
    {
        case TUNE_MINUTE :
          {
              if (button_state == LEFT_BUTTON_LONG_PRESSED)
                {
                  tune_state = TUNE_HOUR;
                }
              else
                {
                    if (button_state == LEFT_BUTTON_PRESSED)
                      {
                          TIME_X = TIME_X - 60*1000;
                      }
                    else if (button_state == RIGHT_BUTTON_PRESSED)
                      {
                          TIME_X = TIME_X + 60*1000;
                      }
                  }
          }
          break;

        case TUNE_HOUR :
          {
              if (button_state == RIGHT_BUTTON_LONG_PRESSED)
                {
                  tune_state = TUNE_MINUTE;
                }
              else
                {
                    if (button_state == LEFT_BUTTON_PRESSED)
                      {
                          TIME_X = TIME_X - 60*60*1000;
                      }
                    else if (button_state == RIGHT_BUTTON_PRESSED)
                      {
                          TIME_X = TIME_X + 60*60*1000;
                      }
                }
          }
          break;

        default:
            while(1);
    }
}

//===============================================================
#endif //BUTTONS_HANDLER_C
