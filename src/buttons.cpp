#include "buttons.hpp"

const Input *get_input(uint8_t id) {
	const uint8_t INPUT_COUNT = sizeof(UFC_INPUTS) / sizeof(Input);

	uint8_t left = 0;
	uint8_t right = INPUT_COUNT - 1;

	while (left <= right) {
		uint8_t mid = left + (right - left) / 2;

		if (UFC_INPUTS[mid].id == id) {
			return &UFC_INPUTS[mid];
		}

		if (UFC_INPUTS[mid].id < id) {
			left = mid + 1;
		} else {
			right = mid - 1;
		}
	}

	return &UNKNOWN_INPUT;
}
