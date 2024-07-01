#include <Adafruit_LiquidCrystal.h>
#include <Joystick.h>
#include <PCF8574.h>
#include <Wire.h>

#define ERROR_LED 11
#define TEST_INPUT 12

#define BUTTON_BUFFER_LENGTH 8
#define NO_BUTTON -1

byte ADDRESS_COUNT_LIMIT = 4;

const byte ROW_ADDRESS_START = 0x20;
const byte COLUMN_ADDRESS_START = 0x24;

int ROW_ADDRESS_COUNT = 0;
int COLUMN_ADDRESS_COUNT = 0;

int BUTTON_BUFFER[BUTTON_BUFFER_LENGTH] = {};

Adafruit_LiquidCrystal lcd(0);

void reset_button_buffer() {
	for (int i = 0; i < BUTTON_BUFFER_LENGTH; i++) {
		BUTTON_BUFFER[i] = NO_BUTTON;
	}
}

int discover_line(int from, int to) {
	int count = 0;

	for (int i = from; i <= to; i++) {
		Wire.beginTransmission(i);
		if (Wire.endTransmission() == 0) {
			count++;
			delay(1);
		} else {
			break;
		}
	}

	return count;
}

void discover_row_col_devices() {

	const int ROW_ADDRESS_END = ROW_ADDRESS_START + ADDRESS_COUNT_LIMIT;
	const int COLUMN_ADDRESS_END =
	    COLUMN_ADDRESS_START + ADDRESS_COUNT_LIMIT;

	ROW_ADDRESS_COUNT = discover_line(ROW_ADDRESS_START, ROW_ADDRESS_END);
	COLUMN_ADDRESS_COUNT =
	    discover_line(COLUMN_ADDRESS_START, COLUMN_ADDRESS_END);

	Serial.println(ROW_ADDRESS_COUNT);
	Serial.println(COLUMN_ADDRESS_COUNT);
}

bool get_bit(int n, char bit) { return (n & (1 << bit)) != 0; }

int read_buttons_at_address(int row_address, int column_address) {
	int result = -1;

	PCF8574 ROWS_PCF(row_address);
	PCF8574 COLS_PCF(column_address);

	const int row_chunk_index = row_address - ROW_ADDRESS_START;
	const int column_chunk_index = column_address - COLUMN_ADDRESS_START;

	ROWS_PCF.write8(0);

	const int cols_status = COLS_PCF.read8();

	// Reset to HIGH
	ROWS_PCF.write8(0xff);

	// No button pressed detected anywhere
	if (cols_status == 0xff) {
		return result;
	}

	// TODO: shift cols_status and & with rows_status to extract

	for (byte column = 0; column <= 7; column++) {
		// All bits are inverted, so looking for a LOW bit
		const bool is_activated = get_bit(cols_status, column) == false;

		if (!is_activated) {
			continue;
		}

		for (byte row = 0; row < 8; row++) {

			// Query the row
			ROWS_PCF.write(row, LOW);

			// Read the column value
			int col_value = COLS_PCF.read(column);

			// always return the row driver output to HIGH after
			// checking a row
			ROWS_PCF.write(row, HIGH);

			if (col_value == LOW) {
				int columns_per_row = 8 * COLUMN_ADDRESS_COUNT;
				int row_offset =
				    (columns_per_row * 8 * row_chunk_index) +
				    columns_per_row * row;
				int column_offset = 8 * column_chunk_index;

				result =
				    (row_offset + column_offset) + (column);
				break;
			}
		}

		break;
	}

	return result;
}

int read_buttons() {

	reset_button_buffer();

	const int ROW_ADDRESS_END = ROW_ADDRESS_START + ROW_ADDRESS_COUNT;
	const int COLUMN_ADDRESS_END =
	    COLUMN_ADDRESS_START + COLUMN_ADDRESS_COUNT;

	int count = 0;

	for (int row_address = ROW_ADDRESS_START; row_address < ROW_ADDRESS_END;
	     row_address++) {
		for (int column_address = COLUMN_ADDRESS_START;
		     column_address < COLUMN_ADDRESS_END; column_address++) {

			if (count == BUTTON_BUFFER_LENGTH) {
				return count;
			}

			int button = read_buttons_at_address(row_address,
							     column_address);

			if (button != -1) {
				BUTTON_BUFFER[count] = button;
				count += 1;
			}
		}
	}

	return count;
}

void test_mode() {
	byte count = 0;

	Serial.println("I2C scanner. Scanning ...");

	for (int i = 0; i <= 255; i++) {
		Wire.beginTransmission(i);
		if (Wire.endTransmission() == 0) {
			Serial.print("Found address: ");
			Serial.print(i, DEC);
			Serial.print(" (0x");
			Serial.print(i, HEX);
			Serial.println(")");
			count++;
			delay(1);
		}
	}

	Serial.println("Done.");
	Serial.print("Found ");
	Serial.print(count, DEC);
	Serial.println(" device(s).");
}

void show_error_signal() { digitalWrite(ERROR_LED, HIGH); }

void write_to_alphanum(int value) {
	lcd.clear();
	lcd.print(value);
}

void setup() {

	reset_button_buffer();
	Serial.begin(115200);

	pinMode(ERROR_LED, OUTPUT);
	pinMode(TEST_INPUT, INPUT_PULLUP);

	// Wait for serial port to be available
	while (!Serial) {
	}

	Wire.begin();

	bool enter_test_mode = digitalRead(TEST_INPUT) == HIGH;

	if (!lcd.begin(16, 2)) {
		show_error_signal();
		Serial.println("Could not init lcd. Check wiring.");
		while (1)
			;
	}

	if (enter_test_mode) {
		Serial.println("Entering test mode...");
		test_mode();
		while (1)
			;
	}

	discover_row_col_devices();
	lcd.print("hello, world!");
}

int last_button = -1;
void loop() {
	int const button_count = read_buttons();

	for (int i = 0; i < button_count; i++) {
		int button = BUTTON_BUFFER[i];
		last_button = button;

		write_to_alphanum(button);

		Serial.print("button # ");
		Serial.print(button);
		Serial.print(" is pressed.\n");
	}
}
