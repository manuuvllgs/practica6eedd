//
// Created by Manuel Villegas Aceituno on 3/12/24.
//

#ifndef MALLREGULAR_H
#define MALLREGULAR_H

#include <cmath>

#include "UTM.h"


template<typename T>
class MallaRegular {
public:
    class Casilla {

    public:
        std::list<T> puntos;



        Casilla(): puntos() {
        }

        void insertar(const T &dato) {
            puntos.push_back(dato);
        }

        T *buscar(const T &dato) {
            auto it = puntos.begin();
            for (; it != puntos.end(); ++it) {
                if (*it == dato) {
                    return &(*it);
                }
            }
            return 0;
        }

        bool borrar(const T &dato) {
            auto it = puntos.begin();
            for (; it != puntos.end(); ++it) {
                if (it == dato) {
                    puntos.erase(it);
                    return true;
                }
            }
            return false;
        }
    };


private:
    float _aXMin = 0;
    float _aXMax = 0;
    float _aYMin = 0;
    float _aYMax = 0;
    float tamaCasillaX, tamaCasillaY;

    std::vector<std::vector<Casilla > > mr;

public:
    Casilla *obtenerCasilla(float x, float y) {
        int i = (x - _aXMin) / tamaCasillaX;
        int j = (y - _aYMin) / tamaCasillaY;
        return &mr[i][j];
    }


    MallaRegular() = default;

    MallaRegular(float aXMin, float aYMin, float aXMax, float aYMax, int nDiv): _aXMin(aXMin), _aXMax(aXMax),
        _aYMin(aYMin), _aYMax(aYMax) {
        tamaCasillaX = (aXMax - aXMin) / nDiv;
        tamaCasillaY = (aYMax - aYMin) / nDiv;
        mr.insert(mr.begin(), nDiv, std::vector<Casilla >(nDiv));
    };

    void insertar(float x, float y, const T &dato) {
        Casilla *c = obtenerCasilla(x, y);
        c->insertar(dato);
    }

    T buscar(float x, float y, const T &dato) {
        Casilla *c = obtenerCasilla(x, y);
        return *c->buscar(dato);
    }

    bool borrar(float x, float y, const T &dato) {
        Casilla *c = obtenerCasilla(x, y);
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

    std::vector<T> buscarRadio(float xcentro, float ycentro, float radio)
    {
        std::vector<T> toRet;

        double limiteIzq, limiteDer, limiteInf, limiteSup, coordX, coordY;

        coordX = xcentro;
        coordY = ycentro;

        limiteInf = coordX - (radio / 111.1);
        limiteSup = coordX + (radio / 111.1);
        limiteIzq = coordY - (radio / 111.1);
        limiteDer = coordY + (radio / 111.1);
        double posY = limiteIzq;
        while (posY <= limiteDer) {
            double posX = limiteInf;
            while (posX <= limiteSup) {
                if (posX >= _aXMin && posX <= _aXMax && posY >= _aYMin && posY <= _aYMax) {
                    Casilla* casillaActual = obtenerCasilla(posX, posY);
                    typename std::list<T>::iterator iter = casillaActual->puntos.begin();
                    while (iter != casillaActual->puntos.begin()) {
                        UTM centro(xcentro, ycentro);
                        UTM otro((*iter)->getX(), (*iter)->getY());
                        float distanciaCalculada = haversine(centro,otro);
                        if (distanciaCalculada <= radio) {
                            toRet.push_back(*iter);
                        }
                        ++iter;
                    }
                }
                posX += tamaCasillaX;
            }
            posY += tamaCasillaY;
        }
        return toRet;
    }



    unsigned maxElementosPorCelda() {

        int numMax = 0;


        Casilla &izqSup = obtenerCasilla(_aXMin, _aYMax), izqInf = obtenerCasilla(_aXMin, _aYMin), derSup =
                obtenerCasilla(_aXMax, _aYMax), derInf = obtenerCasilla(_aXMax, _aYMin);


        auto iIzqSup = mr.find(izqSup);
        auto iDerSup = mr.find(derSup);
        auto iIzqInf = mr.find(izqInf);
        auto iDerInf = mr.find(derInf);


        for (auto i = iIzqSup; i != iDerSup; i++) {
            for (auto j = i; j != iIzqInf; j++) {
                // if que comprueba si el numero de elementos de la posicion actual es mayor que el guardado anteriormente
                if (mr[i][j].puntos.size() > numMax) {
                    numMax = mr[i][j].puntos.size();
                }
            }
        }
        return numMax;
    }

    float promedioElementosPorCelda() {

        int numCasillas = ((_aXMax - _aXMin) / tamaCasillaX) * ((_aYMax - _aYMin) / tamaCasillaY);


        int numElementos = 0;


        Casilla &izqSup = obtenerCasilla(_aXMin, _aYMax), izqInf = obtenerCasilla(_aXMin, _aYMin), derSup =
                obtenerCasilla(_aXMax, _aYMax), derInf = obtenerCasilla(_aXMax, _aYMin);


        auto iIzqSup = mr.find(izqSup);
        auto iDerSup = mr.find(derSup);
        auto iIzqInf = mr.find(izqInf);
        auto iDerInf = mr.find(derInf);

        for (auto i = iIzqSup; i != iDerSup; i++) {
            for (auto j = i; j != iIzqInf; j++) {
                // if que comprueba si el numero de elementos de la posicion actual es mayor que el guardado anteriormente
                numElementos += mr[i][j].puntos.size();
            }
        }
        return numElementos / numCasillas;
    }
};


#endif //MALLREGULAR_H
