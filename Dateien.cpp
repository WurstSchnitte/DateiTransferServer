#include "Dateien.h"
#include "Uebertragung.h"

const int maximaleUebertragungen{500};
vector<Uebertragung*> idvec{maximaleUebertragungen};

void init() {
    cout << "Starte Server" << endl;
    cout << "Lade unterbrochene Uebertragungen" << endl;
    for(int i= 0; i < maximaleUebertragungen; ++i){
        idvec.at(i) = nullptr;
    }
    ladeUebertragungen();
    cout << endl;
}

void release() {
    cout << "Server beenden" << endl;
    speicherUebertragungen();
    for(int i= 0; i < maximaleUebertragungen; ++i){
        delete idvec.at(i);
        idvec.at(i) = nullptr;
    }
    cout << endl;
}

sendeAnfrageReturn initialeSendeAnfrage(string dateiname, string peerip, long dateigroese) {
    long out{-1};
    cout << "Eingehende Uebertragung von " << peerip << endl;
    int id = bereitsVorhanden(dateiname, peerip, dateigroese);
    if (id == -1) {
        cout << "Die Datei '" << dateiname << "' wird neu angelegt" << endl;
        id = searchForNextId();
        idvec.at(id) = new Uebertragung(dateiname, peerip, dateigroese, id);
        out = 0;
        speicherUebertragungen();
    } else if (id == -2) {
        cout << "Die Datei wird bereits von einem anderen User uebertragen" << endl;
        out = -1;
    } else if (id == -3) {
        cout << "Eine andere Datei mit selben Namen wird bereits vom selben User uebertragen" << endl;
        out = -2;
    } else if (id >= 0) {
        cout << "Die Datei wird bereits von dem selben User uebertragen" << endl;
        out = idvec.at(id)->getDateiposition();
    }
    cout << endl;
    return sendeAnfrageReturn{out, id};
}

ladeAnfrageReturn initialeLadeAnfrage(string dateiname, string peerip) {
    long dateigroese{-1};
    long naechstesPaket{0};
    cout << "Eingehende Uebertragung von " << peerip << endl;
    int id = bereitsVorhanden(dateiname, peerip);
    if (id == -1) {
        cout << "Die Uebertragung der '" << dateiname << "' wird gestartet" << endl;
        id = searchForNextId();
        idvec.at(id) = new Uebertragung(dateiname, peerip, id);
        dateigroese = idvec.at(id)->getDateigroese();
        speicherUebertragungen();
    } else if (id == -2) {
        cout << "Die Datei wird bereits von einem anderen User uebertragen" << endl;
        dateigroese = -1;
    } else if (id == -3) {
        cout << "Eine andere Datei mit selben Namen wird bereits vom selben User uebertragen" << endl;
        dateigroese = -2;
    } else if (id >= 0) {
        cout << "Die Datei wird bereits von dem selben User uebertragen" << endl;
        dateigroese = idvec.at(id)->getDateigroese();
        naechstesPaket = idvec.at(id)->getDateiposition();
    }
    cout << endl;
    return ladeAnfrageReturn{dateigroese, id, naechstesPaket};
}

int paketGroeseFestlegen(int verbindungsid, int paketgroese) {
    cout << "Abgleich der Paketgroese von Verbindung '" << verbindungsid << "'" << endl;
    int standard = 100;
    if (paketgroese < standard) {
        standard = paketgroese;
    }
    idvec.at(verbindungsid)->setPaketgroese(standard);
    speicherUebertragungen();
    cout << endl;
    return standard;
}

long paketEmpfangen(int verbindungsid, const signed char* paket, long neuedateiposition) {
    cout << "Eingehende Uebertragung von Verbindung '" << verbindungsid << "'" << endl;
    cout << "Empfangen --> Groeße: " << (neuedateiposition - idvec.at(verbindungsid)->getDateiposition()) << "bytes" << endl;
    char* pa = (char*)paket;
    long out = idvec.at(verbindungsid)->speicherPaket(pa);
    if (out == idvec.at(verbindungsid)->getDateigroese()) {
        cout << "Datei Fertig Uebertragen" << endl;
        delete idvec.at(verbindungsid);
        idvec.at(verbindungsid) = nullptr;
        out = -1;
    }
    speicherUebertragungen();
    cout << endl;
    return out;
}

paketSendenReturn paketSenden(int verbindungsid, long dateiposition) {
    cout << "Eingehende Uebertragung von Verbindung '" << verbindungsid << "'" << endl;
    char* paket;
    int out = idvec.at(verbindungsid)->ladePaket(paket);
    cout << "Senden --> Groeße: " << (out - dateiposition) << "bytes" << endl;
    if (out == idvec.at(verbindungsid)->getDateigroese()) {
        cout << "Datei Fertig Uebertragen" << endl;
        delete idvec.at(verbindungsid);
        idvec.at(verbindungsid) = nullptr;
    }
    speicherUebertragungen();

    cout << endl;
    
    return paketSendenReturn{out, (signed char*)paket};
}

void ladeUebertragungen() {
    ifstream stream{"log"};
    string dateiname{};
    string peername{};
    string peerip{};
    long dateigroese{};
    long dateiposition{};
    int type{};
    int verbindungsid{};
    while (stream >> dateiname && stream >> peername && stream >> peerip && stream >> dateigroese && stream >> dateiposition && stream >> type && stream >> verbindungsid) {
        if (verbindungsid > maximaleUebertragungen) {
            int id = searchForNextId();
            idvec.at(id) = new Uebertragung(dateiname, peerip, dateigroese, dateiposition, type, verbindungsid);
        } else if (idvec.at(verbindungsid) == nullptr) {
            idvec.at(verbindungsid) = new Uebertragung(dateiname, peerip, dateigroese, dateiposition, type, verbindungsid);
        }
    }
}

void speicherUebertragungen() {
    ofstream stream{"log"};
    for (int i = 0; i < maximaleUebertragungen; ++i) {
        if (idvec.at(i) != nullptr) {
            stream << idvec.at(i)->getDateiname() << " " << idvec.at(i)->getPeerip() << " "
                    << idvec.at(i)->getDateigroese() << " " << idvec.at(i)->getDateiposition()
                    << " " << idvec.at(i)->getSendet() << " " << idvec.at(i)->getVerbindungsid() << endl;
        }
    }
}

int searchForNextId() {
    for (int i = 0; i < maximaleUebertragungen; ++i) {
        if (idvec.at(i) == nullptr) {
            return i;
        }
    }
    return -1;
}

int bereitsVorhanden(string dateiname, string peerip, long dateigroese) {
    for (int i = 0; i < maximaleUebertragungen; ++i) {
        if (idvec.at(i) != nullptr) {
            if (idvec.at(i)->getDateiname() == dateiname) {
                if (idvec.at(i)->getPeerip() == peerip) {
                    if (dateigroese <= 0 || idvec.at(i)->getDateigroese() == dateigroese) {
                        //Alles Passt index der Uebertragung wird zurück gegeben
                        return i;
                    }
                    //Fehler die Groese der Datei passt nicht ueberein
                    return -2;
                }
                //Datei wird schon von wem anders hochgeladen
                return -3;
            }
        }
    }
    //Keine bestehende Uebertragung der datei
    return -1;
}
