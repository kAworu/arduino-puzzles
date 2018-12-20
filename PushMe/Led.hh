#ifndef LED_HH
#define LED_HH
/*
 * Led.hh
 *
 * Simple led abstraction.
 */
#include <Arduino.h>


/*
 * Basic Led class holding a pin number and its state.
 */
class Led
{
	public:

	/*
	 * Led constructor given its pin and initial state.
	 */
	Led(uint8_t pin, uint8_t initial_state = LOW);

	/*
	 * Returns true if the led is lit, false otherwise.
	 */
	bool	is_on();

	/*
	 * Returns false if the led is lit, true otherwise.
	 */
	bool	is_off();

	/*
	 * Turn the led on.
	 */
	void	turn_on();

	/*
	 * Turn the led off.
	 */
	void	turn_off();

	/*
	 * Turn the led on if it is off, turn it on otherwise.
	 */
	void	toggle();

	protected:

	/*
	 * Write the given value to the pin and save it in m_state.
	 */
	void	write(uint8_t state);

	/*
	 * This led's pin to write to.
	 */
	uint8_t m_pin;

	/*
	 * This led's state.
	 */
	uint8_t m_state;
};


Led::Led(uint8_t pin, uint8_t initial_state):
	m_pin(pin),
	m_state(initial_state)
{
	pinMode(m_pin, OUTPUT);
	write(m_state);
}


bool
Led::is_on()
{
	/*
	 * NOTE: LOW is defined as 0x0 and HIGH as 0x1, but any non-zero values
	 * is "interpreted" as HIGH, hence the check against LOW.
	 */
	return (m_state != LOW);
}


bool
Led::is_off()
{
	return (!is_on());
}


void
Led::turn_on()
{
	write(HIGH);
}


void
Led::turn_off()
{
	write(LOW);
}


void
Led::toggle()
{
	if (is_off())
		turn_on();
	else
		turn_off();
}


void
Led::write(uint8_t state)
{
	m_state = state;
	digitalWrite(m_pin, m_state);
}

#endif /* ndef LED_HH */
