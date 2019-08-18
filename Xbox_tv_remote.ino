
//#include <ir_Lego_PF_BitStreamEncoder.h>
//#include <IRremoteInt.h>
#include <IRremote.h>
#include <boarddefs.h>
#include "ClickButton.h"
#include <avr/sleep.h>
#include <avr/interrupt.h>

#define BUTTON_LEFT_PIN 2
#define BUTTON_RIGHT_PIN 3
#define IR_SEND_PIN 1

#define STANDARD 0
#define LEFT_ONLY 1
#define RIGHT_ONLY 2

IRsend irsend;

// the Button

ClickButton ButtonLeft(BUTTON_LEFT_PIN, LOW, CLICKBTN_PULLUP);
ClickButton ButtonRight(BUTTON_RIGHT_PIN, LOW, CLICKBTN_PULLUP);

int last_button_state_left;
bool last_pressed_state_left;
long long hold_start_left, hold_stop_left;
long long wake_up_time, actual_time;
short mode;

void setup()
{
	ADCSRA &= ~_BV(ADEN); // disable ADC

	// Setup button timers (all in milliseconds / ms)
	// (These are default if not set, but changeable for convenience)
	ButtonLeft.debounceTime = 50;   // Debounce timer in ms
	ButtonLeft.multiclickTime = 250;  // Time limit for multi clicks
	ButtonLeft.longClickTime = 1000; // time until "held-down clicks" register

	ButtonRight.debounceTime = 50;   // Debounce timer in ms
	ButtonRight.multiclickTime = 250;  // Time limit for multi clicks
	ButtonRight.longClickTime = 1000; // time until "held-down clicks" register

	 // Serial.begin(9600);

	mode = STANDARD;
}


void loop()
{
	// Update button state
	ButtonLeft.Update();
	ButtonRight.Update();

	/*
	actual_time = millis();
	if (actual_time - wake_up_time > 10000)
		sleep();
	
	clickPrintLeft();
	holdPrintLeft();
	pressPrintLeft();
	holdTimeLeft();
	*/


	if (mode == STANDARD)
	{
		//------------------ LEFT BUTTON -------------------------
		if (ButtonLeft.clicks == 1 || ButtonLeft.clicks == -1)
		{
			//Serial.println("Sent Vol- full");
			irsend.sendRC5(0x10011, 12);
		}

		if (ButtonLeft.clicks == 2 || ButtonLeft.clicks == -2)
		{
			// Serial.println("Sent P-");
			irsend.sendRC5(0x10021, 12);
		}
		if (ButtonLeft.clicks == 3 || ButtonLeft.clicks == -3)
		{
			//Serial.println("Sent POWER");
			irsend.sendRC5(0x1000C, 12);
		}
		if (ButtonLeft.clicks == 4 || ButtonLeft.clicks == -4) // workaround if right button fails - there are voltage dropdowns 
			
		{
			//Serial.println("Sent Vol+ full");
			irsend.sendRC5(0x10010, 12);
		}

		//------------------ RIGHT BUTTON ------------------------
		if (ButtonRight.clicks == 1 || ButtonRight.clicks == -1)
		{
			//Serial.println("Sent Vol+ full");
			irsend.sendRC5(0x10010, 12);
		}

		if (ButtonRight.clicks == 2 || ButtonRight.clicks == 2)
		{
			//Serial.println("Sent P+ full");
			irsend.sendRC5(0x10020, 12);
		}


		if (ButtonRight.clicks == 3 || ButtonRight.clicks == -3)
		{
			//Serial.println("Sent POWER");
			irsend.sendRC5(0x1000C, 12);
		}
	}

	/*
	last_button_state_left = ButtonLeft.clicks;
	last_pressed_state_left = ButtonLeft.depressed;
	*/
}

void sleep() {

	/*
	  GIMSK |= _BV(PCIE);                     // Enable Pin Change Interrupts
	  PCMSK |= _BV(PCINT3);                   // Use PB3 as interrupt pin
	  //ADCSRA &= ~_BV(ADEN);                   // ADC off
	  set_sleep_mode(SLEEP_MODE_PWR_DOWN);    // replaces above statement

	  sleep_enable();                         // Sets the Sleep Enable bit in the MCUCR Register (SE BIT)
	  sei();                                  // Enable interrupts
	  sleep_cpu();                            // sleep

	  cli();                                  // Disable interrupts
	  PCMSK &= ~_BV(PCINT3);                  // Turn off PB3 as interrupt pin
	  sleep_disable();                        // Clear SE bit
	  //ADCSRA |= _BV(ADEN);                    // ADC on

	  sei();    */                              // Enable interrupts
} // sleep

ISR(PCINT0_vect) {/*
  wake_up_time = millis();*/

}

void clickPrintLeft()
{
	if (ButtonLeft.clicks > 0)
	{
		/*String output;
		output = (String)(ButtonLeft.clicks);
		output += " clicks";*/
		//Serial.println(output);
	}
}

void holdPrintLeft()
{
	if (ButtonLeft.clicks < 0)
	{
		/*String output;
		output = (String)(ButtonLeft.clicks);
		output += " hold";*/
		//Serial.println(output);
	}
}

void pressPrintLeft()
{
	if (ButtonLeft.depressed != last_pressed_state_left)
	{
		if (ButtonLeft.depressed)
		{
			//Serial.println("Pressed");
		}

		else
		{
			// Serial.println("Depressed");
		}
	}
}

void holdTimeLeft()
{
	if (ButtonLeft.depressed == 1 && ButtonLeft.clicks < 0)
	{
		hold_start_left = millis();
	}

	if (ButtonLeft.depressed == 0 && hold_start_left > 0)
	{
		hold_stop_left = millis();
	}

	if (hold_stop_left - hold_start_left > 0)
	{
		/*String output;
		output += "Hold time: ";
		output += (String)((int)(hold_stop_left - hold_start_left));
		output += " ms";
		//Serial.println(output);*/
		hold_start_left = 0;
		hold_stop_left = 0;
	}
}