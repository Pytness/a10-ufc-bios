#include <Adafruit_LiquidCrystal.h>

#include <Wire.h>

#ifndef PCF8574_INITIAL_VALUE
#define PCF8574_INITIAL_VALUE 0xFF
#endif

#define PCF8574_OK 0x00
#define PCF8574_PIN_ERROR 0x81
#define PCF8574_I2C_ERROR 0x82

class PCF8574 {
public:
	explicit PCF8574(const uint8_t deviceAddress = 0x20, TwoWire *wire = &Wire);

	bool begin(uint8_t value = PCF8574_INITIAL_VALUE);
	bool isConnected();

	//  note: setting the address corrupt internal buffer values
	//  a read8() / write8() call updates them.
	bool setAddress(const uint8_t deviceAddress);
	uint8_t getAddress();

	uint8_t read8();
	uint8_t read(const uint8_t pin);
	uint8_t value() const { return _dataIn; };

	void write8(const uint8_t value);
	void write(const uint8_t pin, const uint8_t value);
	uint8_t valueOut() const { return _dataOut; }

	//  added 0.1.07/08 Septillion
	uint8_t readButton8() { return PCF8574::readButton8(_buttonMask); }
	uint8_t readButton8(const uint8_t mask);
	uint8_t readButton(const uint8_t pin);
	void setButtonMask(const uint8_t mask) { _buttonMask = mask; };
	uint8_t getButtonMask() { return _buttonMask; };

	//  rotate, shift, toggle, reverse expect all lines are output
	void toggle(const uint8_t pin);
	//      default 0xFF ==> invertAll()
	void toggleMask(const uint8_t mask = 0xFF);
	void shiftRight(const uint8_t n = 1);
	void shiftLeft(const uint8_t n = 1);
	void rotateRight(const uint8_t n = 1);
	void rotateLeft(const uint8_t n = 1);
	void reverse();

	void select(const uint8_t pin);
	void selectN(const uint8_t pin);
	void selectNone() { write8(0x00); };
	void selectAll() { write8(0xFF); };

	int lastError();

private:
	uint8_t _address;
	uint8_t _dataIn;
	uint8_t _dataOut;
	uint8_t _buttonMask;
	int _error;

	TwoWire *_wire;
};

PCF8574::PCF8574(const uint8_t deviceAddress, TwoWire *wire) {
	_address = deviceAddress;
	_wire = wire;
	_dataIn = 0;
	_dataOut = 0xFF;
	_buttonMask = 0xFF;
	_error = PCF8574_OK;
}

bool PCF8574::begin(uint8_t value) {
	if (!isConnected())
		return false;
	PCF8574::write8(value);
	return true;
}

bool PCF8574::isConnected() {
	_wire->beginTransmission(_address);
	return (_wire->endTransmission() == 0);
}

bool PCF8574::setAddress(const uint8_t deviceAddress) {
	_address = deviceAddress;
	return isConnected();
}

uint8_t PCF8574::getAddress() {
	return _address;
}

//  removed _wire->beginTransmission(_address);
//  with    @100 KHz -> 265 micros()
//  without @100 KHz -> 132 micros()
//  without @400 KHz -> 52 micros()
//  TODO    @800 KHz -> ??
uint8_t PCF8574::read8() {
	if (_wire->requestFrom(_address, (uint8_t)1) != 1) {
		_error = PCF8574_I2C_ERROR;
		return _dataIn; //  last value
	}
	_dataIn = _wire->read();
	return _dataIn;
}

void PCF8574::write8(const uint8_t value) {
	_dataOut = value;
	_wire->beginTransmission(_address);
	_wire->write(_dataOut);
	_error = _wire->endTransmission();
}

uint8_t PCF8574::read(const uint8_t pin) {
	if (pin > 7) {
		_error = PCF8574_PIN_ERROR;
		return 0;
	}
	PCF8574::read8();
	return (_dataIn & (1 << pin)) > 0;
}

void PCF8574::write(const uint8_t pin, const uint8_t value) {
	if (pin > 7) {
		_error = PCF8574_PIN_ERROR;
		return;
	}
	if (value == LOW) {
		_dataOut &= ~(1 << pin);
	}
	else {
		_dataOut |= (1 << pin);
	}
	write8(_dataOut);
}

void PCF8574::toggle(const uint8_t pin) {
	if (pin > 7) {
		_error = PCF8574_PIN_ERROR;
		return;
	}
	toggleMask(1 << pin);
}

void PCF8574::toggleMask(const uint8_t mask) {
	_dataOut ^= mask;
	PCF8574::write8(_dataOut);
}

void PCF8574::shiftRight(const uint8_t n) {
	if ((n == 0) || (_dataOut == 0))
		return;
	if (n > 7)
		_dataOut = 0; //  shift 8++ clears all, valid...
	if (_dataOut != 0)
		_dataOut >>= n; //  only shift if there are bits set
	PCF8574::write8(_dataOut);
}

void PCF8574::shiftLeft(const uint8_t n) {
	if ((n == 0) || (_dataOut == 0))
		return;
	if (n > 7)
		_dataOut = 0; //  shift 8++ clears all, valid...
	if (_dataOut != 0)
		_dataOut <<= n; //  only shift if there are bits set
	PCF8574::write8(_dataOut);
}

int PCF8574::lastError() {
	int e = _error;
	_error = PCF8574_OK; //  reset error after read, is this wise?
	return e;
}

void PCF8574::rotateRight(const uint8_t n) {
	uint8_t r = n & 7;
	if (r == 0)
		return;
	_dataOut = (_dataOut >> r) | (_dataOut << (8 - r));
	PCF8574::write8(_dataOut);
}

void PCF8574::rotateLeft(const uint8_t n) {
	rotateRight(8 - (n & 7));
}

void PCF8574::reverse() //  quite fast: 4 and, 14 shifts, 3 or, 3 assignment. {
	uint8_t x = _dataOut;
	x = (((x & 0xAA) >> 1) | ((x & 0x55) << 1));
	x = (((x & 0xCC) >> 2) | ((x & 0x33) << 2));
	x = ((x >> 4) | (x << 4));
	PCF8574::write8(x);
}

//  added 0.1.07/08 Septillion
uint8_t PCF8574::readButton8(const uint8_t mask) {
	uint8_t temp = _dataOut;
	PCF8574::write8(mask | _dataOut); //  read only selected lines
	PCF8574::read8();
	PCF8574::write8(temp); //  restore
	return _dataIn;
}

//  added 0.1.07 Septillion
uint8_t PCF8574::readButton(const uint8_t pin) {
	if (pin > 7) {
		_error = PCF8574_PIN_ERROR;
		return 0;
	}

	uint8_t temp = _dataOut;
	PCF8574::write(pin, HIGH);
	uint8_t value = PCF8574::read(pin);
	PCF8574::write8(temp);
	return value;
}

void PCF8574::select(const uint8_t pin) {
	uint8_t n = 0x00;
	if (pin < 8)
		n = 1 << pin;
	write8(n);
};

void PCF8574::selectN(const uint8_t pin) {
	uint8_t n = 0xFF;
	if (pin < 8)
		n = (2 << pin) - 1;
	write8(n);
};

Adafruit_LiquidCrystal lcd(0);
PCF8574 ROWS_PCF(0x20);
PCF8574 COLS_PCF(0x21);

bool get_bit(int n, char bit) {
	return (n & (1 << bit)) != 0;
}

int read_buttons() {
	int result = -1;

	ROWS_PCF.write8(0);

	const int cols_status = COLS_PCF.read8();

	// Reset to HIGH
	ROWS_PCF.write8(255);

	if (cols_status == 255)	{
		return result;
	}

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

			// always return the row driver output to HIGH after checking a row
			ROWS_PCF.write(row, HIGH);

			if (col_value == LOW) {
				result = row * 8 + column;
				break;
			}
		}

		break;
	}

	return result;
}

void test_mode() {
	Serial.println("I2C scanner. Scanning ...");
	byte count = 0;

	Wire.begin();
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

#define ERROR_LED 11
#define TEST_INPUT 12

void show_error_signal() {
	digitalWrite(ERROR_LED, HIGH);
}

void setup() {
	Serial.begin(115200);

	pinMode(ERROR_LED, OUTPUT);
	pinMode(TEST_INPUT, INPUT_PULLUP);

	// Wait for serial port to be available
	while (!Serial) {
	}

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
	lcd.print("hello, world!");

} 

int last_button = -1;
void loop() {
	int const result = read_buttons();
	if (result != last_button) {
		last_button = result;
		Serial.print("button # ");
		Serial.print(result);
		Serial.print(" is pressed.\n");
	}
}
