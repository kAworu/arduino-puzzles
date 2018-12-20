/*
 * "Simple" led / push button based Arduino puzzle.
 */
#include <Arduino.h>

#include "Led.hh"
#include "Button.hh"


/* Internal Built-in LED, lit during setup() */
Led internal(13);

/* Our puzzle LEDs (output) */
Led lamp[] = { Led(3), Led(4), Led(5), Led(7), Led(8) };
const int nlamp = sizeof(lamp) / sizeof(*lamp);

/* Our puzzle push buttons (input) */
Button button[] = { Button(10), Button(11), Button(12) };
const int nbutton = sizeof(button) / sizeof(*button);

/* Our game state */
struct {
	/*
	 * We are either waiting on some button to be pressed so that we can
	 * start recording which are being pressed, or waiting on full release
	 * so we can take action based on what we recorded.
	 */
	enum {
		ANY_PRESSED,
		ALL_RELEASED,
	} on;
	/* used to keep track of timings */
	unsigned long since;
} waiting;


/*
 * Puzzle logic, toggle LEDs based on the push buttons actions.
 */
void
light_up(const int b0, const int b1, const int b2)
{
	/* toggle 0, 1, 2 if button 0, 1, 2 is pressed (respectively) */
	if (b0)
		lamp[0].toggle();
	if (b1)
		lamp[1].toggle();
	if (b2)
		lamp[2].toggle();

	/* toggle 3 if any of the button is pressed */
	if (b0 | b1 | b2)
		lamp[3].toggle();

	/* toggle 4 if more than one button is pressed */
	if (b0 + b1 + b2 > 1)
		lamp[4].toggle();
}


void setup()
{
	internal.turn_on();
	waiting.on = waiting.ANY_PRESSED;
	waiting.since = millis();
	internal.turn_off();
}


void loop()
{
	/* gather the "current time" */
	const unsigned long now = millis();
	/* probe our inputs */
	const Button::State s0 = button[0].probe(now);
	const Button::State s1 = button[1].probe(now);
	const Button::State s2 = button[2].probe(now);
	/* some cosmetic helpers */
	const int any_pressed  = (s0 | s1 | s2) & Button::PRESSED;
	const int none_pressed = !any_pressed;

	switch (waiting.on) {
	case waiting.ANY_PRESSED:
		/*
		 * We are waiting on any button to be presssed. In other words,
		 * last time we checked all three buttons were in a released
		 * state.
		 */
		if (any_pressed) {
			waiting.on = waiting.ALL_RELEASED;
			waiting.since = now;
		}
		break;
	case waiting.ALL_RELEASED:
		/*
		 * We are waiting on all buttons to be released. In other words,
		 * last time we checked at least one button was in a pressed
		 * state.
		 */
		if (none_pressed) {
			/*
			 * Check which buttons were pressed during the time we
			 * were waiting for all of them to be released.
			 */
			const int b0 = button[0].last_released() > waiting.since;
			const int b1 = button[1].last_released() > waiting.since;
			const int b2 = button[2].last_released() > waiting.since;
			light_up(b0, b1, b2);
			waiting.on = waiting.ANY_PRESSED;
			waiting.since = now;
		}
		break;
	}
}
