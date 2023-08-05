/*
 * Satellites.cpp
 *
 *  Created on: 2022. dec. 20.
 *      Author: BT
 */

#include "Satellites.h"


#define SAT_MAX_AGE_MSEC (20 * 1000) /* Xmp után töröljük a sat-ot ha nem frissült */


/**
 *
 */
uint8_t Satellites::countSats() {
	struct sat *p = pSatsHead;
	uint8_t cnt = 0;

	while (p != NULL) {
		cnt++;
		p = p->next;
	}

	return cnt;
}


/**
 * Sat keresése
 */
struct sat* Satellites::searchSat(byte prnNo) {
	struct sat *p = pSatsHead;
	while (p != NULL) {
		if (p->prn == prnNo) {
			return p;
		}
		p = p->next;
	}
	return NULL;
}


/**
 *
 */
void Satellites::deleteSat(struct sat *p) {
	if (pSatsHead == NULL) {
		return;
	}

	if (p == pSatsHead) {
		pSatsHead = p->next;
		delete p;
		return;
	}
	struct sat *curr = pSatsHead;
	struct sat *prev = pSatsHead;
	while (curr != NULL) {
		if (curr == p) {
			prev->next = curr->next;
			delete p;
			return;
		}
		prev = curr;
		curr = curr->next;
	}
}


/**
 *
 */
void Satellites::freeSatellites() {

	struct sat *p = pSatsHead;
	while (p != NULL) {
		struct sat *next = p->next;
		if ((millis() - p->tStamp) > SAT_MAX_AGE_MSEC) {
			deleteSat(p);
		}
		p = next;
	}
}

/**
 *
 */
void Satellites::insertSatellite(uint8_t prnNo, uint8_t snrVal) {

	//Frissítés lesz csak?
	struct sat *p = searchSat(prnNo);
	if (p != NULL) {

		if (snrVal == 0) {
			deleteSat(p);
		} else {
			//Friss/új SNR adat
			p->tStamp = millis();
			p->snr = snrVal;
		}

		//p->tStamp = millis();
		//p->snr = snrVal;
		return;
	}

	//Nincs még ilyen node  -> Új node lesz
	p = new sat;
	p->prn = prnNo;
	p->snr = snrVal;
	p->tStamp = millis();
	p->next = NULL;

	if (pSatsHead == NULL) {
		pSatsHead = p;
		return;
	}
	struct sat *curr = pSatsHead;
	while (curr != NULL) {
		if (curr->next == NULL) {
			curr->next = p;
			break;
		}
		curr = curr->next;
	}
}


/**
 *
 */
void Satellites::sortSatellites() {
	if (pSatsHead == NULL) {
		return;
	}

	struct sat *curr = pSatsHead;
	struct sat *index = NULL;
	while (curr != NULL) {
		index = curr->next;
		while (index != NULL) {

			boolean doSwap = false;
			if (sortType == BY_PRN) {
				doSwap = curr->prn > index->prn;
			} else if (sortType == BY_SNR) {
				doSwap = curr->snr < index->snr;  //SNR csökkenő sorrendben
			}

			if (doSwap) {
				byte tmpPrn = curr->prn;
				byte tmpSnr = curr->snr;

				curr->prn = index->prn;
				curr->snr = index->snr;

				index->prn = tmpPrn;
				index->snr = tmpSnr;
			}
			index = index->next;
		}
		curr = curr->next;
	}
}
