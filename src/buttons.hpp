#pragma once
#include <stdint.h>

#define UFC_HID_REPORT_ID 0x11

#define CREATE_INPUT(_id, _name, _identifier, _value, _def) \
	{ .id = _id, .name = _name, .identifier = _identifier, .value = _value, .def = _def }

struct Input {
	uint8_t id;
	char name[16];
	char identifier[32];
	int8_t value;
	int8_t def;
};

const Input UNKNOWN_INPUT = {.id = 255, .name = "UNKNOWN"};

/*
"MASTER_CAUTION"
"UFC_MASTER_CAUTION"

"UFC_STEER"

"UFC_1"
"UFC_2"
"UFC_3"
"UFC_4"
"UFC_5"
"UFC_6"
"UFC_7"
"UFC_8"
"UFC_9"
"UFC_10"
"UFC_HACK"

"UFC_FUNC"
"UFC_LTR"
"UFC_CLR"

"UFC_ENT"
"UFC_MK"
"UFC_ALT_ALRT"

"UFC_DATA"
"UFC_DEPR"
"UFC_INTEN"

"UFC_SEL"
"UFC_SPC"

"UFC_NA1"
"UFC_NA2"
"UFC_NA3"
"UFC_NA4"
"UFC_NA5"
"UFC_NA6"
*/
// clang-format off
const Input UFC_INPUTS[] = {
	CREATE_INPUT(0,  "STEER UP",   "UFC_STEER",	0x02, 0x01),
	CREATE_INPUT(1,  "STEER DOWN", "UFC_STEER",	0x00, 0x01),
	CREATE_INPUT(2,  "1",          "UFC_1",		0x01, 0x00),
	CREATE_INPUT(3,  "2",          "UFC_2", 	0x01, 0x00),
	CREATE_INPUT(4,  "3",          "UFC_3", 	0x01, 0x00),
	CREATE_INPUT(5,  "4",          "UFC_4", 	0x01, 0x00),
	CREATE_INPUT(6,  "5",          "UFC_5", 	0x01, 0x00),
	CREATE_INPUT(7,  "6",          "UFC_6", 	0x01, 0x00),
	CREATE_INPUT(8,  "7",          "UFC_7", 	0x01, 0x00),
	CREATE_INPUT(9,  "8",          "UFC_8", 	0x01, 0x00),
	CREATE_INPUT(10, "9",          "UFC_9",		0x01, 0x00),
	CREATE_INPUT(11, "HACK",       "UFC_HACK",	0x01, 0x00),
	CREATE_INPUT(12, "0",          "UFC_10",	0x01, 0x00),
	CREATE_INPUT(13, "SPC",        "UFC_SPC",	0x01, 0x00),
	CREATE_INPUT(14, "FUNC",       "UFC_FUNC",	0x01, 0x00),
	CREATE_INPUT(15, "LTR",        "UFC_LTR",	0x01, 0x00),
	CREATE_INPUT(16, "CLR",        "UFC_CLR",	0x01, 0x00),
	CREATE_INPUT(17, "ENT",        "UFC_ENT",	0x01, 0x00),
	CREATE_INPUT(18, "MK",         "UFC_MK",	0x01, 0x00),
	CREATE_INPUT(19, "ALT_ALTR",   "UFC_ALT_ALTR",	0x01, 0x00),

	CREATE_INPUT(20, "COM 1",   "UFC_NA1",	0x01, 0x00),
	CREATE_INPUT(21, "COM SEC", "UFC_NA2",	0x01, 0x00),
	CREATE_INPUT(22, "IFF",     "UFC_NA3",	0x01, 0x00),
	CREATE_INPUT(23, "COM 2",   "UFC_NA4",	0x01, 0x00),
	CREATE_INPUT(24, "ECCM",    "UFC_NA5",	0x01, 0x00),
	CREATE_INPUT(25, "IDM",     "UFC_NA6",	0x01, 0x00),

	CREATE_INPUT(26, "DATA/PAGE UP",   "UFC_DATA",	0x02, 0x01),
	CREATE_INPUT(27, "DATA/PAGE DOWN", "UFC_DATA",	0x00, 0x01),

	CREATE_INPUT(28, "SEL UP",   "UFC_SEL",		0x02, 0x01),
	CREATE_INPUT(29, "SEL DOWN", "UFC_SEL",		0x00, 0x01),

	CREATE_INPUT(30, "INTEN LEFT",  "UFC_INTEN",	0x00, 0x01),
	CREATE_INPUT(31, "INTEN RIGHT", "UFC_INTEN",	0x02, 0x01),

	CREATE_INPUT(31, "DEPR UP",   "UFC_DEPR",	0x02, 0x01),
	CREATE_INPUT(33, "DEPR DOWN", "UFC_DEPR",	0x00, 0x01),

	CREATE_INPUT(34, "MASTER CAUTION", "UFC_MASTER_CAUTION", 0x01, 0x00),
};
// clang-format on

Input get_input(uint8_t id);
