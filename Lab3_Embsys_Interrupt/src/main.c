/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include <stdio.h>
#include <ioport.h>
#include <board.h>


int lamp1 = 0;
int door = 0;
static char strbuf[201];

static void kedip(void){
	if(lamp1==0) {
		gfx_mono_draw_string("LED1 ON ", 0, 8, &sysfont);
		gpio_set_pin_low(LED1_GPIO);
		lamp1 = 1;
	} else {
		gfx_mono_draw_string("LED1 OFF", 0, 8, &sysfont);
		gpio_set_pin_high(LED1_GPIO);
		lamp1 = 0;
	}
}

static void tulis(void) {
	door = door + 1;
	snprintf(strbuf, sizeof(strbuf), "Counter: %d times  ", door);
	gfx_mono_draw_string(strbuf, 0, 0, &sysfont);
}


void setup_timer0(void){
	
	tc_enable(&TCD0); // enable timer counter interrupt D0
	tc_set_overflow_interrupt_callback(&TCD0,kedip); // fungsi yang dipanggil saat interrupt terjadi
	tc_set_wgm(&TCD0, TC_WG_NORMAL);
	tc_write_period(&TCD0, 8000); // 1/(2jt/1024)*8000 berarti sekitar 4 detik
	tc_set_overflow_interrupt_level(&TCD0, TC_INT_LVL_LO);
	tc_write_clock_source(&TCD0, TC_CLKSEL_DIV1024_gc); // 1024 prescaler, berarti freq baru = 2 juta Hz dibagi 1024
	
	cpu_irq_enable(); // enable interrupts
}

void setup_timer1(void){
	
	tc_enable(&TCE1);
	tc_set_overflow_interrupt_callback(&TCE1,tulis);
	tc_set_wgm(&TCE1, TC_WG_NORMAL);
	tc_write_period(&TCE1, 2000);
	tc_set_overflow_interrupt_level(&TCE1, TC_INT_LVL_LO);
	tc_write_clock_source(&TCE1, TC_CLKSEL_DIV1024_gc);
	
	cpu_irq_enable();
}
int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	//init board
	board_init();
	
	sysclk_init();
	pmic_init();
		
	//init lcd
	gfx_mono_init();
	
	sysclk_enable_module(SYSCLK_PORT_GEN, SYSCLK_RTC);
	while (RTC32.SYNCCTRL & RTC32_SYNCBUSY_bm) {
		// Wait for RTC32 sysclk to become stable
	}
		
			
	//set background lcd on
	gpio_set_pin_high(LCD_BACKLIGHT_ENABLE_PIN);
	
	setup_timer0();
	setup_timer1();
	
	while(1){
	
	}

	/* Insert application code here, after the board has been initialized. */
}