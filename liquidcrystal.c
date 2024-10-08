#include "liquidcrystal.h"

// Default configurations
struct Lcd lcd = {
	.rows = 2,
	.cols = 16,
	.rs = 8,
	.en = 9,
	.data = {4, 5, 6, 7},
	.cursor_row = 0,
	.cursor_col = 0
};

void lcd_init_4bits(uint8_t rows, uint8_t cols, uint8_t rs,\
		uint8_t en, uint8_t d4, uint8_t d5,\
		uint8_t d6, uint8_t d7) {

	lcd = (struct Lcd){
		.rows = rows,
		.cols = cols,
		.rs = rs,
		.en = en,
		.data = {d4, d5, d6, d7}
	};

	lcd_begin();
}

void lcd_begin(void) {
	chThdSleepMilliseconds(50);

	palSetPadMode(PORT(lcd.rs), PIN(lcd.rs), PAL_MODE_OUTPUT_PUSHPULL);
	palClearPad(PORT(lcd.rs), PIN(lcd.rs));
	palSetPadMode(PORT(lcd.en), PIN(lcd.en), PAL_MODE_OUTPUT_PUSHPULL);
	for (uint8_t i = 0; i < 4; ++i) {
		palSetPadMode(PORT(lcd.data[i]), PIN(lcd.data[i]), PAL_MODE_OUTPUT_PUSHPULL);
	}

	write_4bits(0x03);
	chThdSleepMilliseconds(5);
	write_4bits(0x03);
	chThdSleepMilliseconds(5);
	write_4bits(0x03);
	chThdSleepMilliseconds(5);
	write_4bits(0x02);

	command(0x20 | ROWS_2 | DOTS_8);
	chThdSleepMicroseconds(50);

	command(0x08 | DISPLAY_ON);
	chThdSleepMicroseconds(50);

	command(0x04 | ENTRY_INC);
	chThdSleepMicroseconds(50);

	lcd_clear();
}

void lcd_home(void) {
	command(DISPLAY_HOME);
	chThdSleepMilliseconds(2);
}

void lcd_clear(void) {
	command(DISPLAY_CLEAR);
	chThdSleepMilliseconds(2);
}

void lcd_cursor_visibility(uint8_t is_visible) {
	switch (is_visible) {
		case 1:
			command(DISPLAY_CTRL | DISPLAY_ON | CURSOR_ON);
		break;
		default:
			command(DISPLAY_CTRL | DISPLAY_ON | CURSOR_OFF);
	}

	chThdSleepMilliseconds(2);
}

void lcd_cursor_blink(uint8_t is_blinking) {
	switch (is_blinking) {
		case 1:
			command(DISPLAY_CTRL | DISPLAY_ON | CURSOR_ON | CURSOR_BLINK_ON);
		break;
		default:
			command(DISPLAY_CTRL | DISPLAY_ON | CURSOR_ON | CURSOR_BLINK_OFF);
	}

	chThdSleepMilliseconds(2);
}

void lcd_cursor_position(uint8_t row, uint8_t col) {
	lcd.cursor_row = row;
	lcd.cursor_col = col;

	command( CURSOR_CMD | (col + 0x40*row) );
	chThdSleepMicroseconds(50);
}

void lcd_shift(uint8_t n, uint8_t direction) {
	for (uint8_t i = 0; i < n; ++i) {
		command(SHIFT_CMD | DISPLAY_SHIFT | direction);
		chThdSleepMicroseconds(50);
	}
}

void command(uint8_t cmd) {
	send(cmd, 0);
}

void lcd_print(char* msg) {
	uint8_t* tmp = (uint8_t*) msg;

	for (uint8_t* s = tmp; *s; ++s, ++lcd.cursor_col) {
		send(*s, 1);
	}
}

void send(uint8_t byte, uint8_t op) {
	digital_write(lcd.rs, op);

	write_4bits(byte >> 4);
	write_4bits(byte);
}

void write_4bits(uint8_t half_byte) {
	for (uint8_t i = 0; i < 4; ++i) {
		digital_write(lcd.data[i], (half_byte & (1 << i)));
	}

	enable_pulse();
}

void digital_write(uint8_t pin, uint8_t value) {
	switch (value) {
		case 0:
			palClearPad( PORT(pin), PIN(pin) );
		break;
		default:
			palSetPad( PORT(pin), PIN(pin) );
	}
}

void enable_pulse(void) {
	palClearPad(PORT(lcd.en), PIN(lcd.en));
	chThdSleepMicroseconds(1);
	palSetPad(PORT(lcd.en), PIN(lcd.en));
	chThdSleepMicroseconds(1);
	palClearPad(PORT(lcd.en), PIN(lcd.en));
	// Maximum execution time for a instruction is 37us
	chThdSleepMicroseconds(100);
}
