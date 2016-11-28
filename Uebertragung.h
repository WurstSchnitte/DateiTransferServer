/* 
 * File:   Uebertragung.h
 * Author: debian
 *
 * Created on 27. November 2016, 10:48
 */

#ifndef UEBERTRAGUNG_H
#define	UEBERTRAGUNG_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;


class Uebertragung {
public:
    Uebertragung() = delete;
    Uebertragung(string dateiname, string peerip, int verbindungsid);
    Uebertragung(string dateiname, string peerip, long dateigroese, int verbindungsid);
    Uebertragung(string dateiname, string peerip, long dateigroese, long dateiposition, int type, int verbindungsid);
    ~Uebertragung();

    //OBJECTMETHODEN
    long speicherPaket(char* paket);
    long ladePaket(char*& paket);


    //AUTOGENERIERT
    int getSendet() const;
    void setSendet(int sendet);

    long getDateigroese() const;
    void setDateigroese(long dateigoese);

    string getDateiname() const;
    void setDateiname(string dateiname);

    long getDateiposition() const;
    void setDateiposition(long dateiposition);

    int getPaketgroese() const;
    void setPaketgroese(int paketgroese);

    string getPeerip() const;
    void setPeerip(string peerip);

    int getVerbindungsid() const;
    void setVerbindungsid(int verbindungsid);

    
    
private:
    string dateiname;
    fstream* dateihandler;

    string peerip;
    int verbindungsid;
    
    long dateigroese;
    long dateiposition;
    int paketgroese;
    
    int sendet;
};

#endif	/* UEBERTRAGUNG_H */

