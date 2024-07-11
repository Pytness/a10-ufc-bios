#pragma once
#include <stdint.h>

#define UFC_HID_REPORT_ID 0x11
#define VALUE_LEN 1

#define CREATE_INPUT(_id, _name, _identifier, _value, _default_value) \
	{ .id = _id, .name = _name, .identifier = _identifier, .value = _value, .default_value = _default_value }

struct Input {
	uint8_t id;
	char name[16];
	char identifier[32];
	char value[VALUE_LEN + 1];
	char default_value[VALUE_LEN + 1];
};

const Input UNKNOWN_INPUT = {.id = 255, .name = "UNKNOWN"};

/*
"MASTER_CAUTION"      // LED
"UFC_MASTER_CAUTION"  // Button
*/
// clang-format off
const Input UFC_INPUTS[] = {
	CREATE_INPUT(0,  "STEER UP",   "UFC_STEER",	"2", "1"),
	CREATE_INPUT(1,  "STEER DOWN", "UFC_STEER",	"0", "1"),
	CREATE_INPUT(2,  "1",          "UFC_1",		"1", "0"),
	CREATE_INPUT(3,  "2",          "UFC_2", 	"1", "0"),
	CREATE_INPUT(4,  "3",          "UFC_3", 	"1", "0"),
	CREATE_INPUT(5,  "4",          "UFC_4", 	"1", "0"),
	CREATE_INPUT(6,  "5",          "UFC_5", 	"1", "0"),
	CREATE_INPUT(7,  "6",          "UFC_6", 	"1", "0"),
	CREATE_INPUT(8,  "7",          "UFC_7", 	"1", "0"),
	CREATE_INPUT(9,  "8",          "UFC_8", 	"1", "0"),
	CREATE_INPUT(10, "9",          "UFC_9",		"1", "0"),
	CREATE_INPUT(11, "HACK",       "UFC_HACK",	"1", "0"),
	CREATE_INPUT(12, "0",          "UFC_10",	"1", "0"),
	CREATE_INPUT(13, "SPC",        "UFC_SPC",	"1", "0"),
	CREATE_INPUT(14, "FUNC",       "UFC_FUNC",	"1", "0"),
	CREATE_INPUT(15, "LTR",        "UFC_LTR",	"1", "0"),
	CREATE_INPUT(16, "CLR",        "UFC_CLR",	"1", "0"),
	CREATE_INPUT(17, "ENT",        "UFC_ENT",	"1", "0"),
	CREATE_INPUT(18, "MK",         "UFC_MK",	"1", "0"),
	CREATE_INPUT(19, "ALT_ALTR",   "UFC_ALT_ALTR",	"1", "0"),

	CREATE_INPUT(20, "COM 1",   "UFC_NA1",	"1", "0"),
	CREATE_INPUT(21, "COM SEC", "UFC_NA2",	"1", "0"),
	CREATE_INPUT(22, "IFF",     "UFC_NA3",	"1", "0"),
	CREATE_INPUT(23, "COM 2",   "UFC_NA4",	"1", "0"),
	CREATE_INPUT(24, "ECCM",    "UFC_NA5",	"1", "0"),
	CREATE_INPUT(25, "IDM",     "UFC_NA6",	"1", "0"),

	CREATE_INPUT(26, "DATA/PAGE UP",   "UFC_DATA",	"2", "1"),
	CREATE_INPUT(27, "DATA/PAGE DOWN", "UFC_DATA",	"0", "1"),

	CREATE_INPUT(28, "SEL UP",   "UFC_SEL",		"2", "1"),
	CREATE_INPUT(29, "SEL DOWN", "UFC_SEL",		"0", "1"),

	CREATE_INPUT(30, "INTEN LEFT",  "UFC_INTEN",	"0", "1"),
	CREATE_INPUT(31, "INTEN RIGHT", "UFC_INTEN",	"2", "1"),

	CREATE_INPUT(31, "DEPR UP",   "UFC_DEPR",	"2", "1"),
	CREATE_INPUT(33, "DEPR DOWN", "UFC_DEPR",	"0", "1"),

	CREATE_INPUT(34, "MASTER CAUTION", "UFC_MASTER_CAUTION", "1", "0"),
};
// clang-format on

const Input *get_input(uint8_t id);
