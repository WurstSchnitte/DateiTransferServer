#include "Uebertragung.h"

Uebertragung::Uebertragung(string dateinam, string peeri, int verbindung) {
    dateiname = dateinam;
    string da = "data/" + dateinam;
    dateihandler = new fstream{da, ios_base::in | ios_base::ate};
    peerip = peeri;
    dateihandler->seekg(0, ios_base::end);
    long dateigroes = dateihandler->tellg();
    dateihandler->seekg(0, ios_base::beg);
    dateigroese = dateigroes;
    dateiposition = 0;
    paketgroese = 0;
    sendet = 1;
    verbindungsid = verbindung;
}

Uebertragung::Uebertragung(string dateinam, string peeri, long dateigroes, int verbindung) {
    dateiname = dateinam;
    string da = "data/" + dateinam;
    dateihandler = new fstream{da, ios_base::out | ios_base::trunc};
    peerip = peeri;
    dateigroese = dateigroes;
    dateiposition = 0;
    paketgroese = 0;
    sendet = 0;
    verbindungsid = verbindung;
}

Uebertragung::Uebertragung(string dateinam, string peeri, long dateigroes, long dateipositio, int senden, int verbindung) {
    dateiname = dateinam;
    string da = "data/" + dateinam;
    dateihandler = new fstream{da, ios_base::in | ios_base::out | ios_base::ate};
    peerip = peeri;
    dateigroese = dateigroes;
    dateiposition = dateipositio;
    paketgroese = 0;
    sendet = senden;
    verbindungsid = verbindung;
}

Uebertragung::~Uebertragung() {
    dateihandler->close();
    delete dateihandler;
}

long Uebertragung::speicherPaket(char* paket) {
    int groese = paketgroese;
    if (dateiposition + paketgroese > dateigroese) {
        groese = dateigroese - dateiposition;
    }
    dateihandler->write(paket, groese);
    dateihandler->flush();
    dateiposition += groese;
    return dateiposition;
}

long Uebertragung::ladePaket(char*& paket) {
    int groese = paketgroese;
    if (dateiposition + paketgroese > dateigroese) {
        groese = dateigroese - dateiposition;
    }
    paket = new char[groese];
    dateihandler->read(paket, groese);
    return dateiposition + groese;
}

int Uebertragung::getSendet() const {
    return sendet;
}

void Uebertragung::setSendet(int sendet) {
    this->sendet = sendet;
}

long Uebertragung::getDateigroese() const {
    return dateigroese;
}

void Uebertragung::setDateigroese(long dateigoese) {
    this->dateigroese = dateigroese;
}

string Uebertragung::getDateiname() const {
    return dateiname;
}

void Uebertragung::setDateiname(string dateiname) {
    this->dateiname = dateiname;
}

long Uebertragung::getDateiposition() const {
    return dateiposition;
}

void Uebertragung::setDateiposition(long dateiposition) {
    this->dateiposition = dateiposition;
}

int Uebertragung::getPaketgroese() const {
    return paketgroese;
}

void Uebertragung::setPaketgroese(int paketgroese) {
    this->paketgroese = paketgroese;
}

string Uebertragung::getPeerip() const {
    return peerip;
}

void Uebertragung::setPeerip(string peerip) {
    this->peerip = peerip;
}

int Uebertragung::getVerbindungsid() const {
    return verbindungsid;
}

void Uebertragung::setVerbindungsid(int verbindungsid) {
    this->verbindungsid = verbindungsid;
}

