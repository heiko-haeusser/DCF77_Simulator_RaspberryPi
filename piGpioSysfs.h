/*
 * piGpioSysfs.h
 *
 *  Created on: 29.09.2021
 *      Author: Heiko Haeusser
 *      Github: heiko-haeusser
 */

#ifndef PIGPIOSYSFS_H_
#define PIGPIOSYSFS_H_

#define IN  0
#define OUT 1

#define LOW  0
#define HIGH 1
int GPIOExport(int pin);
int GPIOUnexport(int pin);
int GPIODirection(int pin, int dir);
int GPIORead(int pin);
int GPIOWrite(int pin, int value);


#endif /* PIGPIOSYSFS_H_ */
