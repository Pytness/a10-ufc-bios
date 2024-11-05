#define DCSBIOS_IRQ_SERIAL

#include <DcsBios.h>
#include <Wire.h>

#include "hardware_handler.cpp"

void setup() {
	reset_button_buffer();
	pinMode(ERROR_LED, OUTPUT);
	pinMode(TEST_INPUT, INPUT_PULLUP);

	// bool enter_test_mode = digitalRead(TEST_INPUT) == HIGH;

	DcsBios::setup();
}

int last_button = -1;

void loop() {
	DcsBios::loop();

	const int button = read_buttons();

	// TODO: explain this / refactor
	if (button != last_button) {

		int button_id = button == -1 ? last_button : button;

		const Input *input = get_input(button_id);

		const char *value = button == -1 ? input->default_value : input->value;

		sendDcsBiosMessage(input->identifier, value);

		last_button = button;
	}
}
