#include "hal.h"
#include "ch.h"
#include "liquidcrystal.h"

int main(void) {
	halInit();
	chSysInit();

	sdStart(&SD1, NULL);

	//lcd_begin();
	lcd_init_4bits(2, 16, 6, 7, 2, 3, 4, 5);
	//lcd_print("Hello, World! Oi!");
	lcd_print("Oi!");


	while (1) ;
}
