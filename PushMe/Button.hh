#ifndef BUTTON_HH
#define BUTTON_HH
/*
 * Button.hh
 *
 * Simple push button abstraction.
 */
#include <Arduino.h>


/*
 * Basic push button class holding the button's state, pin, and last
 * pressed/released timings.
 */
class Button
{
	public:

	/*
	 * How the push button is setup. BUILTIN_PULLUP is Arduino's
	 * INPUT_PULLUP, making use of the built in 20k pullup resistors built
	 * into the Atmega chip.
	 */
	enum Mode {
		PULLUP,
		BUILTIN_PULLUP,
		PULLDOWN,
	};

	/*
	 * A push button is either released or pressed.
	 */
	enum State {
		RELEASED = 0x0,
		PRESSED  = 0x1,
	};

	/*
	 * Button constructor given its pin and its mode. PULLUP is the
	 * recommended setup.
	 */
	Button(uint8_t pin, Mode mode = PULLUP);

	/*
	 * Check the pin's state and return the button's state.
	 */
	State	probe();
	State	probe(unsigned long now);

	/*
	 * Last time this push button was probe()'d in a PRESSED state.
	 */
	unsigned long	last_pressed();

	/*
	 * Last time this push button was probe()'d in a RELEASED state.
	 */
	unsigned long	last_released();

	protected:

	/*
	 * True if this push button state is PRESSED, false otherwise.
	 */
	bool	is_pressed();

	/*
	 * True if this push button state is RELEASED, false otherwise.
	 */
	bool	is_released();

	/*
	 * Actually read the pin's state and change the button's state
	 * accordingly. Returns the button's state.
	 */
	State	read();

	/*
	 * This push button's pin to read from.
	 */
	uint8_t m_pin;

	/*
	 * This push button's chosen mode.
	 */
	Mode m_mode;

	/*
	 * Last time this push button was seen in a PRESSED state.
	 */
	unsigned long m_last_pressed;

	/*
	 * Last time this push button was seen in a RELEASED state.
	 */
	unsigned long m_last_released;
};


Button::Button(uint8_t pin, Mode mode):
	m_pin(pin),
	m_mode(mode),
	m_last_pressed(0),
	m_last_released(0)
{
	pinMode(m_pin, m_mode == BUILTIN_PULLUP ? INPUT_PULLUP : INPUT);
}


Button::State
Button::probe()
{
	const unsigned long now = millis();
	return (probe(now));
}


Button::State
Button::probe(unsigned long now)
{
	const bool pressed = (read() == PRESSED);
	const bool changed = (pressed != is_pressed());

	if (changed && pressed)
		m_last_pressed = now;
	else if (changed && !pressed)
		m_last_released = now;

	return (pressed ? PRESSED : RELEASED);
}


unsigned long
Button::last_pressed()
{
	return (m_last_pressed);
}


unsigned long
Button::last_released()
{
	return (m_last_released);
}


bool
Button::is_pressed()
{
	return (m_last_pressed > m_last_released);
}


bool
Button::is_released()
{
	return (!is_pressed());
}


Button::State
Button::read()
{
	const bool is_high = (digitalRead(m_pin) == HIGH);

	if (m_mode == PULLDOWN) {
		/*
		 * In a pull-down setup pressing the button connect power to the
		 * pin, i.e. pressed reads HIGH.
		 */
		return (is_high ? PRESSED : RELEASED);
	} else {
		/*
		 * In a pull-up setup pressing the button connect ground to the
		 * pin, i.e. pressed reads LOW.
		 */
		return (is_high ? RELEASED : PRESSED);
	}
}

#endif /* ndef BUTTON_HH */
