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
#include <stdint.h>


#define DELAY_ONE 200
#define DELAY_ZERO 100

#define IN  0
#define OUT 1

#define LOW  0
#define HIGH 1

#define PIN_OUTPUT_LED 23  /* P1-07 */

static int GPIOExport(int pin) {
#define BUFFER_MAX 3
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/gpio/export", O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open export for writing!\n");
		return (-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return (0);
}

static int GPIOUnexport(int pin) {
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open unexport for writing!\n");
		return (-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return (0);
}

static int GPIODirection(int pin, int dir) {
	static const char s_directions_str[] = "in\0out";

#define DIRECTION_MAX 35
	char path[DIRECTION_MAX];
	int fd;

	snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio direction for writing!\n");
		return (-1);
	}

	if (-1
			== write(fd, &s_directions_str[IN == dir ? 0 : 3],
					IN == dir ? 2 : 3)) {
		fprintf(stderr, "Failed to set direction!\n");
		return (-1);
	}

	close(fd);
	return (0);
}

static int GPIORead(int pin) {
#define VALUE_MAX 30
	char path[VALUE_MAX];
	char value_str[3];
	int fd;

	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_RDONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio value for reading!\n");
		return (-1);
	}

	if (-1 == read(fd, value_str, 3)) {
		fprintf(stderr, "Failed to read value!\n");
		return (-1);
	}

	close(fd);

	return (atoi(value_str));
}

static int GPIOWrite(int pin, int value) {
	static const char s_values_str[] = "01";

	char path[VALUE_MAX];
	int fd;

	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio value for writing!\n");
		return (-1);
	}

	if (1 != write(fd, &s_values_str[LOW == value ? 0 : 1], 1)) {
		fprintf(stderr, "Failed to write value!\n");
		return (-1);
	}

	close(fd);
	return (0);
}

/*
void procDCFSignalWithJitter(int cnt) {
	uint16_t jitter = 0;
	if (DCFSignal[cnt] == 1) {
		jitter = rand() % (MAX_ONE - MIN_ONE);
		printf("one: jitter modulo (227-143 = %d): %d \t", 227 - 143, jitter);
		printf("one: delay(143+jitter = %d\n", MIN_ONE + jitter);

		//printf("1");
	} else {
		jitter = rand() % (MAX_ZERO - MIN_ZERO);
		printf("zero: jitter modulo (127-43 = %d): %d \t", 127 - 43, jitter);
		printf("zero: delay(43+jitter = %d\n", MIN_ONE + jitter);

		//printf("0");

	}
}
*/

void procDCFSignal(int cnt) {
	if (DCFSignal[cnt] == 1) {
		if (-1 == GPIOWrite(PIN_OUTPUT_LED, 0))
			puts("Write Error");
		msleep(DELAY_ONE);
		if (-1 == GPIOWrite(PIN_OUTPUT_LED, 1))
			puts("Write Error");
		msleep(1000-DELAY_ONE);

		//printf("1");
	} else {
		if (-1 == GPIOWrite(PIN_OUTPUT_LED, 0))
			puts("Write Error");
		msleep(DELAY_ZERO);
		if (-1 == GPIOWrite(PIN_OUTPUT_LED, 1))
			puts("Write Error");
		msleep(1000-DELAY_ZERO);
	}
}

int main(int argc, char *argv[]) {
	uint16_t year = 2002;
	uint8_t month = 10;
	uint8_t day = 14;
	uint8_t dow = 1;
	uint8_t hours = 21;
	uint8_t minutes = 11;
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

	for (uint8_t i = 0; i < DCF_SIGNAL_ARR_SIZE; i++) {
		/*printf("%d", DCFSignal[i]);
		 if(i==20)
		 printf("\nMinutes: %d\n", minutes);

		 if(i==28)
		 printf("\nHours: %d\n", hours);

		 if(i==35)
		 printf("\nDate:%d\t %d.%d.%d %d:%d\n", dow, day,month, year, hours, minutes);

		 */
		procDCFSignal(i);
	}

	for (uint8_t i = 0; i < DCF_SIGNAL_ARR_SIZE; i++) {
		/*printf("%d", DCFSignal[i]);
		 if(i==20)
		 printf("\nMinutes: %d\n", minutes);

		 if(i==28)
		 printf("\nHours: %d\n", hours);

		 if(i==35)
		 printf("\nDate:%d\t %d.%d.%d %d:%d\n", dow, day,month, year, hours, minutes);

		 */
		procDCFSignal(i);
	}

	if (-1 == GPIOUnexport(PIN_OUTPUT_LED))
		return (4);

	return (0);
}
