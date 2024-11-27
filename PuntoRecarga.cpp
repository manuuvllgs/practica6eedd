//
// Created by Manuel Villegas Aceituno on 22/10/24.
//

#include "Coche.h"

PuntoRecarga::PuntoRecarga(int id, float latitud, float longitud, int max): _id(id), _lugar(latitud, longitud),
                                                                            _max(max) {
}

Coche* PuntoRecarga::getMaxBateria() {
    return _almacenados.top();
}


bool PuntoRecarga::addCoche(Coche *c) {
    if (c->getCharging() || _cochAlm + 1 > _max) {
        return false;
    }
    _almacenados.push(c);
    _cochAlm++;
    c->setCharging(this);

    return true;
}

int PuntoRecarga::getNumCoches() {
    return _cochAlm;
}



int PuntoRecarga::getID() {
    return _id;
}

