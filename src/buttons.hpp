#pragma once
#include <stdint.h>

#define UFC_HID_REPORT_ID 0x11

struct Input {
	uint8_t id;
	char name[32];
};

const Input UNKNOWN_INPUT = {.id = 255, .name = "UNKNOWN"};

// clang-format off
const Input UFC_INPUTS[] = {
	{ .id = 0, .name = "STEER UP" },
	{ .id = 1, .name = "STEER DOWN" },

	{ .id = 2, .name = "1" },
	{ .id = 3, .name = "2" },
	{ .id = 4, .name = "3" },
	{ .id = 5, .name = "4" },
	{ .id = 6, .name = "5" },
	{ .id = 7, .name = "6" },
	{ .id = 8, .name = "7" },
	{ .id = 9, .name = "8" },
	{ .id = 10, .name = "9" },

	{ .id = 11, .name = "HACK" },
	{ .id = 12, .name = "0" },
	{ .id = 13, .name = "SPC" },

	{ .id = 14, .name = "FUNC" },
	{ .id = 15, .name = "LTR" },
	{ .id = 16, .name = "CLR" },

	{ .id = 17, .name = "ENT" },
	{ .id = 18, .name = "MK" },
	{ .id = 19, .name = "ALT / ALTR" },

	{ .id = 20, .name = "COM 1" },
	{ .id = 21, .name = "COM SEC" },
	{ .id = 22, .name = "IFF" },
	{ .id = 23, .name = "COM 2" },
	{ .id = 24, .name = "ECCM" },
	{ .id = 25, .name = "IDM" },

	{ .id = 26, .name = "DATA/PAGE UP" },
	{ .id = 27, .name = "DATA/PAGE DOWN" },

	{ .id = 28, .name = "SEL UP" },
	{ .id = 29, .name = "SEL DOWN" },

	{ .id = 30, .name = "INTEN LEFT" },
	{ .id = 31, .name = "INTEN RIGHT" },

	{ .id = 31, .name = "DERP UP" },
	{ .id = 33, .name = "DERP DOWN" },
};
// clang-format on

Input get_input(uint8_t id);
