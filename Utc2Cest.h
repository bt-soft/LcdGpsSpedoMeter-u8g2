/*
 * Utc2Cest.h
 *
 *  Created on: 2022. dec. 29.
 *      Author: BT
 */

#ifndef UTC2CEST_H_
#define UTC2CEST_H_

#include <Arduino.h>
#include <TinyGPS++.h>

//Európában a nyári időszámítás március utolsó vasárnapján kezdődik, és október utolsó vasárnapjáig tart.
//Ennek megfelelően minden évben más dátumra esik, de az átállítás napja mindig vasárnap.
//Az óraátállítás az UTC szerint hajnali 1 órakor történik,
//vagyis Magyarországon márciusban helyi idő szerint 2 órakor előre (3-ra), októberben nyári idő szerint 3 órakor vissza (2-re) kell állítani az órákat.

class Utc2Cet {

private:

	/**
	 * Az év adott hónapjának adott napja vasárnap?
	 *
	 * see https://rosettacode.org/wiki/Find_the_last_Sunday_of_each_month#C++
	 */
	int getWeekDay(int year, uint8_t month, uint8_t day) {
		int f = year + day + 3 * month;
		month++;
		if (month < 3) {
			year--;
		} else {
			f -= int(.4 * month + 2.3);
		}
		f += int(year / 4) - int((year / 100 + 1) * 0.75);
		f %= 7;
		return f;
	}

	/**
	 * Az év adott hónapjának utolsó vasárnapjának napja
	 */
	void findLastSundayOfMonth(int year, uint8_t month, uint8_t &lastSunDay) {
		while (true) {
			if (!getWeekDay(year, month, lastSunDay))
				break;
			lastSunDay--;
		}
	}

public:
	/**
	 * CEST timeoffszet kiszámítása
	 */
	uint8_t getOffsetHour(TinyGPSDate &date, TinyGPSTime &time) {

#define MONTH_MARCH 		3
#define DLS_HOUR_MARCH  	2
#define MONTH_OCTOBER 		10
#define DLS_HOUR_OCTOBER 	3

		uint8_t lastSunDayInMarch = 31; //Március utolsó vasárnapjának napja
		findLastSundayOfMonth(date.year(), MONTH_MARCH, lastSunDayInMarch);

		uint8_t lastSunDayInOctober = 31; //Október utolsó vasárnapjának napja
		findLastSundayOfMonth(date.year(), MONTH_OCTOBER, lastSunDayInOctober);

		//Március utolsó vasárnapjának hajnal 02:00 vagy annál nagyobb idejében vagyunk?
		bool afterMarchLastSunday02Hour = date.month() >= MONTH_MARCH && date.day() >= lastSunDayInMarch && time.hour() >= DLS_HOUR_MARCH;

		//Október utolsó vasárnapjának hajnal 03:00 előtti idejében vagyunk?
		bool BeforaOctoberLastSunday03Hour = date.month() <= MONTH_OCTOBER && date.day() >= lastSunDayInMarch && time.hour() < DLS_HOUR_OCTOBER;

		return afterMarchLastSunday02Hour && BeforaOctoberLastSunday03Hour ? 2 : 1;
	}

};
#endif /* UTC2CEST_H_ */
