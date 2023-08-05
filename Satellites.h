/*
 * Satellites.h
 *
 *  Created on: 2022. dec. 20.
 *      Author: BT
 */

#ifndef SATELLITES_H_
#define SATELLITES_H_

#include <Arduino.h>


struct sat {
	/**
	 * Satellite PRN number
	 */
	uint8_t prn;

	/*
	 * SNR (C/No) 00-99 dB, null when not tracking
	 */
	uint8_t snr;

	/**
	 * utolsó frissítés
	 */
	long tStamp;

	struct sat *next;
};
//extern struct sat *pSatsHead;

/**
 *
 */
 typedef enum  {
	BY_PRN, BY_SNR
} SortType_t;


/**
 *
 */
class Satellites {

private:
	struct sat *pSatsHead = NULL;
	SortType_t sortType = BY_PRN;

public:
	uint8_t countSats();
	struct sat* searchSat(byte prnNo);
	void deleteSat(struct sat *p);
	void freeSatellites();
	void insertSatellite(uint8_t prnNo, uint8_t snrVal);
	void sortSatellites();

	SortType_t getSortType() {return sortType;}
	void setSortType(SortType_t type) {sortType = type;}

	struct sat *getStatsHead() {return pSatsHead; }


};



#endif /* SATELLITES_H_ */
