/* 
 * File:   Dateien.h
 * Author: debian
 *
 * Created on 24. November 2016, 20:50
 */

#ifndef DATEIEN_H
#define	DATEIEN_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

#include "Uebertragung.h"



void init();
void release();

struct sendeAnfrageReturn {
    long naechstesPaket;
    int verbindungsid;
};

struct ladeAnfrageReturn {
    long dateigroese;
    int verbindungsid;
    long naechstesPaket;
};

struct paketSendenReturn {
    long naechstesPaket;
    signed char* paket;
};

sendeAnfrageReturn initialeSendeAnfrage(string dateiname, string peerip, long dateigroese);
ladeAnfrageReturn initialeLadeAnfrage(string dateiname, string peerip);
int paketGroeseFestlegen(int verbindungsid, int paketgroese);
long paketEmpfangen(int verbindungsid, const signed char* paket, long neuedateiposition);
paketSendenReturn paketSenden(int verbindungsid, long dateiposition);
void ladeUebertragungen();
void speicherUebertragungen();
int searchForNextId();
int bereitsVorhanden(string dateiname, string peerip, long dateigroese = -1);
#endif	/* DATEIEN_H */

