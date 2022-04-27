/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>	//Not a fan of this library, but it is needed for Keyboard and HID libraries to work
/*End of auto generated code by Atmel studio */

//Beginning of Auto generated function prototypes by Atmel Studio
#include <Keyboard.h>	//Used to send button presses to PC via serial
//End of Auto generated function prototypes by Atmel Studio

#include <stdbool.h>	//Added support for C
#include "util/delay.h"	//Delay library

#define R_SOLENOID 4	//Right solenoid connected to PIND4
#define L_SOLENOID 7	//Left solenoid connected to PIND7
#define ON true			//ON set to bool to avoid confusion
#define OFF false		//OFF set to bool to avoid confusion

/**
  * @brief Function to read button inputs and change variable state to True or False
  * @param Pointers of bool variables that store state of R, L, & B button
  * @retval None
  */
void read_button(int pin, bool *BTN_POINT, unsigned long current_time, unsigned long *previous_time)
{
	if ((PINB & (1<<pin)) == 0) //Checks to see if any of the buttons are pressed (Pin switches from high to low)
	{
		if ((*previous_time == 0) && (*BTN_POINT != true))
		{
			*previous_time = current_time;
		}
		else if ((current_time - *previous_time >= 50))
		{
			if ((PINB & (1<<pin)) == 0)	//Checks right button
			{
				*BTN_POINT = true;		//Set right button as pressed
				*previous_time = 0;
			}
			else
			{
				*previous_time = 0;
				*BTN_POINT = false;		//Set right button as not pressed
			}
		}
	}
	else
	{
		if (*BTN_POINT == true)
		{	
			if ((*previous_time == 0))
			{
				*previous_time = current_time;
			}
			else if ((current_time - *previous_time >= 50))
			{
				*previous_time = 0;
				*BTN_POINT = false;		//Set right button as not pressed
			}
		}
		else
		{
			*BTN_POINT = false;		//Set right button as not pressed
		}
	}
}



/**
  * @brief Function to trigger relay which power on desired solenoid
  * @param uint32_t Solenoid pin number, bool state of solenoid (on or off)
  * @retval None
  */
void solenoid_ctrl(uint32_t SOLENOID, bool STATE)
{
	if (SOLENOID == R_SOLENOID)			//If right solenoid state needs to be changed
	{
		if (STATE == ON)				//If solenoid needs to be powered on
		{
			PORTD |= (1<<R_SOLENOID);	//Set output PIND4 to 1 to trigger relay (Power on solenoid 12V)
		}
		else							//I used "else" instead of "if else (STATE == OFF)" to prevent mistakes that keep the solenoid power on too long. Solenoid can burn out if left on too long (8s or more)
		{
			PORTD &= ~(1<<R_SOLENOID);	//Set output PIND4 to 0 to turn off relay (Power off solenoid 12V)
		}
	}
	else if (SOLENOID == L_SOLENOID)	//If left solenoid state needs to be changed
	{
		if (STATE == ON)				//If solenoid needs to be powered on
		{
			PORTD |= (1<<L_SOLENOID);	//Set output PIND7 to 1 to trigger relay
		}
		else
		{
			PORTD &= ~(1<<L_SOLENOID);	//Set output PIND7 to 0 to turn off relay
		}
	}
}

/**
  * @brief Function to initialize AVR peripherals (Required by Arduino.h library)
  * @param None
  * @retval None
  */
void setup()
{
	Keyboard.begin();		//Initialize the library.
	delay(1000);			//1 sec delay for Keyboard init
	
	DDRB &= ~(1<<PINB4);	//Set PINB4 to Input
	DDRB &= ~(1<<PINB5);	//Set PINB5 to Input
	DDRB &= ~(1<<PINB6);	//Set PINB6 to Input
	DDRB &= ~(1<<PINB7);	//Set PINB7 to Input
	
	PORTB |= (1<<PINB4);	//Enable internal pull up resistors for PINB4
	PORTB |= (1<<PINB5);	//Enable internal pull up resistors for PINB5
	PORTB |= (1<<PINB6);	//Enable internal pull up resistors for PINB6
	PORTB |= (1<<PINB7);	//Enable internal pull up resistors for PINB7
	
	DDRD |= (1<<PIND4);		//Set PIND4 to output
	DDRD |= (1<<PIND7);		//Set PIND7 to output
	PORTD &= ~(1<<PIND4);	//Set PIND4 output to 0 (Not needed but good practice)
	PORTD &= ~(1<<PIND7);	//Set PIND7 output to 0
}

/**
  * @brief Main loop for AVR ("loop()" must be used instead of "main()" because Arduino.h library requires it)
  * @param None
  * @retval None
  */
void loop()
{
	unsigned long current_time;
	unsigned long R_previous_time = 0;
	unsigned long L_previous_time = 0;
	unsigned long B_previous_time = 0;
	unsigned long E_previous_time = 0;
	unsigned long LS_previous_time = 0;
	unsigned long RS_previous_time = 0;
	unsigned long SOL_Safe_time = 0;
	unsigned long KEY_Safe_time = 0;
	
	//Button bools init to false
	bool R_BTN_PRESS = false;
	bool L_BTN_PRESS = false;
	bool B_BTN_PRESS = false;
	bool E_BTN_PRESS = false;
	
	//Button state bools init to false (These vars as used to turn off solenoids if buttons are held. This prevents the solenoids from burning out)
	bool R_BUTTONSTATE = false;
	bool L_BUTTONSTATE = false;
	bool B_BUTTONSTATE = false;
	bool E_BUTTONSTATE = false;

	while(1) //Infinite loop (Acts as AVRs OS after init)
	{
		current_time = millis();
		
		
		read_button(PINB4, &R_BTN_PRESS, current_time, &R_previous_time);
		read_button(PINB5, &L_BTN_PRESS, current_time, &L_previous_time);
		read_button(PINB6, &B_BTN_PRESS, current_time, &B_previous_time);
		read_button(PINB7, &E_BTN_PRESS, current_time, &E_previous_time);
		
		
		//read_buttons(&R_BTN_PRESS, &L_BTN_PRESS, &B_BTN_PRESS);	//Refresh button states each loop
		
		if (R_BTN_PRESS != R_BUTTONSTATE)			//Checks to see if right button state has changed (Pressed or released)
		{
			R_BUTTONSTATE = R_BTN_PRESS;			//Update right button state
			if (R_BTN_PRESS == true)				//If right button is currently being pressed
			{
				KEY_Safe_time = current_time;
				RS_previous_time = current_time;
				solenoid_ctrl(R_SOLENOID, ON);		//Turn on right solenoid
				Keyboard.press(KEY_RIGHT_ARROW);	//Send right arrow key press to PC (continuously being sent until Keyboard.release is used
			}
			else									//Right button is released
			{
				Keyboard.release(KEY_RIGHT_ARROW);	//Stop sending right arrow key press to PC
			}
		}
		if (current_time - RS_previous_time >= 50)										//Right button is being held or not pressed
		{
			solenoid_ctrl(R_SOLENOID, OFF);			//Turn off right solenoid
			//RS_previous_time = current_time;
		}
		
		if (L_BTN_PRESS != L_BUTTONSTATE)			//Checks to see if left button state has changed (Pressed or released)
		{
			L_BUTTONSTATE = L_BTN_PRESS;			//Update left button state
			if (L_BTN_PRESS == true)				//If left button is currently being pressed
			{
				KEY_Safe_time = current_time;
				LS_previous_time = current_time;
				solenoid_ctrl(L_SOLENOID, ON);		//Turn on left solenoid
				Keyboard.press(KEY_LEFT_ARROW);		//Send left arrow key press to PC
			}
			else									//Left button is released
			{
				Keyboard.release(KEY_LEFT_ARROW);	//Stop sending left arrow key press to PC
			}
		}
		if (current_time - LS_previous_time >= 50)		//Left button is being held or not pressed
		{
			solenoid_ctrl(L_SOLENOID, OFF);			//Turn off left solenoid
			//LS_previous_time = current_time;
		}
		
		if (B_BTN_PRESS != B_BUTTONSTATE)			//Checks to see if shoot ball button state has changed
		{
			B_BUTTONSTATE = B_BTN_PRESS;			//Update shoot ball button state
			if (B_BTN_PRESS == true)				//If shoot ball button is currently being pressed
			{
				KEY_Safe_time = current_time;
				Keyboard.press(KEY_RETURN);		//Send left arrow key press to PC ***THIS NEEDS TO BE CHANGED TO NEW KEY***
			}
			else									//Shoot ball button is being held or not pressed
			{
				RS_previous_time = current_time;
				LS_previous_time = current_time;
				solenoid_ctrl(R_SOLENOID, ON);		//Turn on right solenoid
				solenoid_ctrl(L_SOLENOID, ON);		//Turn on left solenoid
				Keyboard.release(KEY_RETURN);	//Stop sending left arrow key press to PC ***THIS NEEDS TO BE CHANGED TO NEW KEY***
			}
		}
		
		if (E_BTN_PRESS != E_BUTTONSTATE)			//Checks to see if left button state has changed (Pressed or released)
		{
			E_BUTTONSTATE = E_BTN_PRESS;			//Update e button state
			if (E_BTN_PRESS == true)				//If e button is currently being pressed
			{
				R_BTN_PRESS = false;
				L_BTN_PRESS = false;
				B_BTN_PRESS = false;
				E_BTN_PRESS = false;
				//Keyboard.press(KEY_RETURN);		//Send e arrow key press to PC
			}
			else									//e button is released
			{
				//Keyboard.release(KEY_RETURN);	//Stop sending e arrow key press to PC
			}
		}
		
		
		if (current_time - SOL_Safe_time >= 4000)
		{
			solenoid_ctrl(R_SOLENOID, OFF);			//Turn off right solenoid
			solenoid_ctrl(L_SOLENOID, OFF);			//Turn off left solenoid
			SOL_Safe_time = current_time;
		}
		
		if (R_BTN_PRESS == false)
		{
			Keyboard.release(KEY_RIGHT_ARROW);
		}
		if (L_BTN_PRESS == false)
		{
			Keyboard.release(KEY_LEFT_ARROW);
		}
		if (B_BTN_PRESS == false)
		{
			Keyboard.release(KEY_RETURN);
		}		
		if (E_BTN_PRESS == false)
		{
			//Keyboard.release(KEY_RETURN);
		}
	}
}


