/*
 * dcfSignalBuilder.h
 *
 *  Created on: 19 Jan 2020
 *      Author: X1_AV
 */

#ifndef APPLICATION_USER_DCFSIGNALBUILDER_H_
#define APPLICATION_USER_DCFSIGNALBUILDER_H_

#include <stdint.h>

#define DCF_SIGNAL_ARR_SIZE 59

extern uint8_t DCFSignal[DCF_SIGNAL_ARR_SIZE];
void initDCFSignal(uint16_t year, uint8_t month, uint8_t day, uint8_t dow, uint8_t hours, uint8_t minutes);


#endif /* APPLICATION_USER_DCFSIGNALBUILDER_H_ */
