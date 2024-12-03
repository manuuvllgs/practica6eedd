//
// Created by Manuel Villegas Aceituno on 3/12/24.
//

#ifndef MALLREGULAR_H
#define MALLREGULAR_H
#include "Casilla.h"
#include <cmath>

#include "UTM.h"


template<typename T>
class MallaRegular {
    float _aXMin = 0;
    float _aXMax = 0;
    float _aYMin = 0;
    float _aYMax = 0;
    float tamaCasillaX, tamaCasillaY;

    std::vector<std::vector<Casilla<T> > > mr;

    Casilla<T> *obtenerCasilla(float x, float y) {
        int i = (x - _aXMin) / tamaCasillaX;
        int j = (y - _aYMin) / tamaCasillaY;
        return &mr[i][j];
    }

public:
    MallaRegular(float aXMin, float aYMin, float aXMax, float aYMax, int nDiv): _aXMin(aXMin), _aXMax(aXMax),
        _aYMin(aYMin), _aYMax(aYMax) {
        tamaCasillaX = (aXMax - aXMin) / nDiv;
        tamaCasillaY = (aYMax - aYMin) / nDiv;
        mr.insert(mr.begin(), nDiv, std::vector<Casilla<T> >(nDiv));
    };

    void insertar(float x, float y, const T &dato) {
        Casilla<T> *c = obtenerCasilla(x, y);
        c->insertar(dato);
    }

    T buscar(float x, float y, const T &dato) {
        Casilla<T> *c = obtenerCasilla(x, y);
        return *c->buscar(dato);
    }

    bool borrar(float x, float y, const T &dato) {
        Casilla<T> *c = obtenerCasilla(x, y);
        return c->borrar(dato);
    }

    float haversine(UTM utm1, UTM utm2) {
        float radio_tierra = 6378;
        float incrLat = (utm2.lat() - utm1.lat()) * (M_PI / 180);
        float incrLon = (utm2.lon() - utm1.lon()) * (M_PI / 180);
        float a = sin(incrLat / 2) * sin(incrLat / 2) + cos(utm1.lat() * (M_PI / 180)) * cos(utm2.lat() * (M_PI / 180))
                  * sin(incrLon / 2) * sin(incrLon / 2);
        float c = 2 * atan2(sqrt(a), sqrt(1 - a));
        float d = radio_tierra * c;
        return d;
    }

    std::vector<T> buscarRadio(float xCentro, float yCentro, float radio) {
        Casilla<T> *centro = obtenerCasilla(xCentro, yCentro);
        int numCasillasX = radio*2 / tamaCasillaX;
        int numCasillasY = radio*2 / tamaCasillaY;
        // for ()


    }
};


#endif //MALLREGULAR_H
