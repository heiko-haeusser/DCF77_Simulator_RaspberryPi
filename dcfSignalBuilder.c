/*
 * dcfSignalBuilder.c
 *
 *  Created on: 19 Jan 2020
 *      Author: X1_AV
 */

/*
 * Bits are counted from 0 (0-59)
20	Beginn der Zeitinformation (ist immer „1“)
21	Minute = (Einer)	Bit für 1
22	Bit für 2
23	Bit für 4
24	Bit für 8
25	Bit für 10 = Minute (Zehner)
26	Bit für 20
27	Bit für 40
28	Parität Minute

29	Bit für 1 = Stunde (Einer)
30	Bit für 2
31	Bit für 4
32	Bit für 8
33	Bit für 10 = Stunde (Zehner)
34	Bit für 20
35	Parität Stunde

36	Bit für 1 = Kalendertag (Einer)
37	Bit für 2
38	Bit für 4
39	Bit für 8
40	Bit für 10 = Kalendertag (Zehner)
41	Bit für 20

42	Bit für 1 = Wochentag
43	Bit für 2
44	Bit für 4

45	Bit für 1 = Monatsnummer (Einer)
46	Bit für 2
47	Bit für 4
48	Bit für 8
49	Bit für 10 = Monatsnummer (Zehner)

50	Bit für 1 = Jahr (Einer)
51	Bit für 2
52	Bit für 4
53	Bit für 8
54	Bit für 10 = Jahr (Zehner)
55	Bit für 20
56	Bit für 40
57	Bit für 80
58	Parität Datum (Bit 29-57)
59	keine Sekundenmarke
 */

#include "dcfSignalBuilder.h"
#include <stdint.h>


uint8_t DCFBitSet(uint8_t value, uint8_t index);
void buildMinutes(uint8_t minutes);
void buildHours(uint8_t hours);
void buildDate(uint8_t dow, uint8_t day, uint8_t month, uint8_t year); //year = 0 bis 99


uint8_t DCFSignal[DCF_SIGNAL_ARR_SIZE];
uint8_t clrCnt = 0;

void initDCFSignal(uint16_t year, uint8_t month, uint8_t day, uint8_t dow, uint8_t hours, uint8_t minutes)
{
	void clearDCFSignal();
	DCFSignal[20] = 1;
	buildMinutes(minutes);
	buildHours(hours);
	buildDate(dow, day, month, year%100);
}

void clearDCFSignal() {
	for(clrCnt=0;clrCnt<DCF_SIGNAL_ARR_SIZE;clrCnt++)
		DCFSignal[clrCnt] = 0;
}

uint8_t DCFBitSet(uint8_t value, uint8_t index) {
	if(value>0) {
		DCFSignal[index] = 1;
		return 1;
	}
	else
		DCFSignal[index] = 0;
	return 0;
}

/*
 * Minute = (Einer)
21	Bit für 1
22	Bit für 2
23	Bit für 4
24	Bit für 8
25	Bit für 10 = Minute (Zehner)
26	Bit für 20
27	Bit für 40
28	Parität Minute
 */

void buildMinutes(uint8_t minutes )
{
	uint8_t tmpCnt = 21;
	uint8_t lowNibble = minutes%10;
	uint8_t highNibble = minutes/10;
	uint8_t parity = 0;

	parity += DCFBitSet(lowNibble & 0x01, tmpCnt++);
	parity += DCFBitSet(lowNibble & 0x02, tmpCnt++);
	parity += DCFBitSet(lowNibble & 0x04, tmpCnt++);
	parity += DCFBitSet(lowNibble & 0x08, tmpCnt++);

	parity += DCFBitSet(highNibble & 0x01, tmpCnt++);
	parity += DCFBitSet(highNibble & 0x02, tmpCnt++);
	parity += DCFBitSet(highNibble & 0x04, tmpCnt++);

	DCFSignal[tmpCnt] = parity % 2;
}

/*
 * Codierung Stunde
29	Bit für 1 = Stunde (Einer)
30	Bit für 2
31	Bit für 4
32	Bit für 8
33	Bit für 10 = Stunde (Zehner)
34	Bit für 20
35	Parität Stunde
 */
void buildHours(uint8_t hours )
{
	uint8_t tmpCnt = 29;
	uint8_t lowNibble = hours%10;
	uint8_t highNibble = hours/10;
	uint8_t parity = 0;

	parity += DCFBitSet(lowNibble & 0x01, tmpCnt++);
	parity += DCFBitSet(lowNibble & 0x02, tmpCnt++);
	parity += DCFBitSet(lowNibble & 0x04, tmpCnt++);
	parity += DCFBitSet(lowNibble & 0x08, tmpCnt++);

	parity += DCFBitSet(highNibble & 0x01, tmpCnt++);
	parity += DCFBitSet(highNibble & 0x02, tmpCnt++);

	DCFSignal[tmpCnt] = parity % 2;

}

/*
 * Codierung date
36	Bit für 1 = Kalendertag (Einer)
37	Bit für 2
38	Bit für 4
39	Bit für 8
40	Bit für 10 = Kalendertag (Zehner)
41	Bit für 20

42	Bit für 1 = Wochentag
43	Bit für 2
44	Bit für 4

45	Bit für 1 = Monatsnummer (Einer)
46	Bit für 2
47	Bit für 4
48	Bit für 8
49	Bit für 10 = Monatsnummer (Zehner)

50	Bit für 1 = Jahr (Einer)
51	Bit für 2
52	Bit für 4
53	Bit für 8
54	Bit für 10 = Jahr (Zehner)
55	Bit für 20
56	Bit für 40
57	Bit für 80
58  parity
 */

void buildDate(uint8_t dow, uint8_t day, uint8_t month, uint8_t year) //year = 0 bis 99
{
	uint8_t tmpCnt = 36;
	uint8_t lowNibbleDay = day%10;
	uint8_t highNibbleDay = day/10;

	uint8_t lowNibbleDOW = dow%10;

	uint8_t lowNibbleMonth = month%10;
	uint8_t highNibbleMonth = month/10;

	uint8_t lowNibbleYear = year%10;
	uint8_t highNibbleYear = year/10;

	uint8_t parity = 0;

	//Day
	parity += DCFBitSet(lowNibbleDay & 0x01, tmpCnt++);
	parity += DCFBitSet(lowNibbleDay & 0x02, tmpCnt++);
	parity += DCFBitSet(lowNibbleDay & 0x04, tmpCnt++);
	parity += DCFBitSet(lowNibbleDay & 0x08, tmpCnt++);

	parity += DCFBitSet(highNibbleDay & 0x01, tmpCnt++);
	parity += DCFBitSet(highNibbleDay & 0x02, tmpCnt++);

	//Day of week
	parity += DCFBitSet(lowNibbleDOW & 0x01, tmpCnt++);
	parity += DCFBitSet(lowNibbleDOW & 0x02, tmpCnt++);
	parity += DCFBitSet(lowNibbleDOW & 0x04, tmpCnt++);

	//Month
	parity += DCFBitSet(lowNibbleMonth & 0x01, tmpCnt++);
	parity += DCFBitSet(lowNibbleMonth & 0x02, tmpCnt++);
	parity += DCFBitSet(lowNibbleMonth & 0x04, tmpCnt++);
	parity += DCFBitSet(lowNibbleMonth & 0x08, tmpCnt++);

	parity += DCFBitSet(highNibbleMonth & 0x01, tmpCnt++);

	//Year
	parity += DCFBitSet(lowNibbleYear & 0x01, tmpCnt++);
	parity += DCFBitSet(lowNibbleYear & 0x02, tmpCnt++);
	parity += DCFBitSet(lowNibbleYear & 0x04, tmpCnt++);
	parity += DCFBitSet(lowNibbleYear & 0x08, tmpCnt++);

	parity += DCFBitSet(highNibbleYear & 0x01, tmpCnt++);
	parity += DCFBitSet(highNibbleYear & 0x02, tmpCnt++);
	parity += DCFBitSet(highNibbleYear & 0x04, tmpCnt++);
	parity += DCFBitSet(highNibbleYear & 0x08, tmpCnt++);

	DCFSignal[tmpCnt] = parity % 2;

}
