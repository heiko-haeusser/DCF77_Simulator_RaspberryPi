/* blink.c
 *
 * Raspberry Pi GPIO example using sysfs interface.
 * Guillermo A. Amaral B. <g@maral.me>
 *
 * This file blinks GPIO 4 (P1-07) while reading GPIO 24 (P1_18).
 */

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "dcfSignalBuilder.h"
#include "piGpioSysfs.h"
#include <stdint.h>

#include <time.h>
#include <sys/time.h>

#define DELAY_ONE 200
#define DELAY_ZERO 100

#define MAX_ONE 225
#define MIN_ONE 145
#define MAX_ZERO 125
#define MIN_ZERO 45

#define PIN_OUTPUT_LED 23  /* P1-07 */

void msleep(int ms) {
	usleep(ms * 1000);
}

void procDCFSignalWithJitter(int cnt) {
	uint16_t jitter = 0;
	if (DCFSignal[cnt] == 1) {
		jitter = rand() % (MAX_ONE - MIN_ONE);
		printf("\tone:  jitter: %02d \t", jitter);
		printf("\tone:  delay: %03d\r\n", MIN_ONE + jitter);
		if (-1 == GPIOWrite(PIN_OUTPUT_LED, 0))
			puts("Write Error");
		msleep(MIN_ONE + jitter);
		if (-1 == GPIOWrite(PIN_OUTPUT_LED, 1))
			puts("Write Error");
		msleep(1000 - (MIN_ONE + jitter));
		//printf("1");
	} else {
		jitter = rand() % (MAX_ZERO - MIN_ZERO);
		printf("\tzero: jitter: %02d\t", jitter);
		printf("\tzero: delay: %03d\r\n", MIN_ZERO + jitter);

		if (-1 == GPIOWrite(PIN_OUTPUT_LED, 0))
			puts("Write Error");
		msleep(MIN_ZERO + jitter);
		if (-1 == GPIOWrite(PIN_OUTPUT_LED, 1))
			puts("Write Error");
		msleep(1000 - (MIN_ZERO + jitter));
		//printf("0");

	}
}

void procDCFSignal(int cnt) {
	if (DCFSignal[cnt] == 1) {
		if (-1 == GPIOWrite(PIN_OUTPUT_LED, 0))
			puts("Write Error");
		msleep(DELAY_ONE);
		if (-1 == GPIOWrite(PIN_OUTPUT_LED, 1))
			puts("Write Error");
		msleep(1000 - DELAY_ONE);

		//printf("1");
	} else {
		if (-1 == GPIOWrite(PIN_OUTPUT_LED, 0))
			puts("Write Error");
		msleep(DELAY_ZERO);
		if (-1 == GPIOWrite(PIN_OUTPUT_LED, 1))
			puts("Write Error");
		msleep(1000 - DELAY_ZERO);
	}
}

void runDCFSignalWithJitter() {
	printf("\n\rRun DCF signal on pin GPIO23 without jitter.\n\r");

	for (uint8_t i = 0; i < DCF_SIGNAL_ARR_SIZE; i++) {

		printf("i: %d\tsignal: %d", i, DCFSignal[i]);
		procDCFSignalWithJitter(i);
	}
}

void runDCFSignalWithoutJitter() {
	printf("\n\rRun DCF signal on pin GPIO23 without jitter.\n\r");

	for (uint8_t i = 0; i < DCF_SIGNAL_ARR_SIZE; i++) {

		printf("i: %d\tsignal: %d\r\n", i, DCFSignal[i]);

		procDCFSignal(i);
	}
}

int main(int argc, char *argv[]) {

	struct timeval curTime;
	struct tm *dt;
	gettimeofday(&curTime, NULL);
	dt = localtime(&curTime.tv_sec);

	uint16_t year = 1900 + dt->tm_year - 100;
	uint8_t month = dt->tm_mon + 1;
	uint8_t day = dt->tm_mday;
	uint8_t dow = dt->tm_wday;
	uint8_t hours = dt->tm_hour;
	uint8_t minutes = dt->tm_min;

	printf("\r\nyear set to: %d\r\n", year);
	printf("month set to: %d\r\n", month);
	printf("day set to: %d\r\n", day);
	printf("dow set to: %d\r\n", dow);
	printf("hours set to: %d\r\n", hours);
	printf("minutes set to: %d\r\n", minutes);

	//initDCFSignal(2002, 10, 14, 1, 20, 48);
	initDCFSignal(year, month, day, dow, hours, minutes);

	/*
	 * Enable GPIO pins
	 */
	if (-1 == GPIOExport(PIN_OUTPUT_LED))
		puts("GPIOExport Error");

	/*
	 * Set GPIO directions
	 */
	if (-1 == GPIODirection(PIN_OUTPUT_LED, OUT))
		puts("GPIODirection Error");

	if (-1 == GPIOWrite(PIN_OUTPUT_LED, 1))
		puts("Write Error");
	msleep(1000 - DELAY_ZERO);
	if (-1 == GPIOWrite(PIN_OUTPUT_LED, 1))
		puts("Write Error");
	sleep(2);

	runDCFSignalWithJitter();
	sleep(1);

	runDCFSignalWithJitter();

	if (-1 == GPIOUnexport(PIN_OUTPUT_LED))
		return (4);

	return (0);
}
