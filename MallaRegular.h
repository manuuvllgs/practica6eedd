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

    std::vector<std::vector<Casilla> > mr;

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
        mr.insert(mr.begin(), nDiv, std::vector<Casilla>(nDiv));
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

    std::vector<T> buscarRadio(float xcentro, float ycentro, float radio) {
        std::vector<T> resultado;

        float radioCoords = radio / 111.1;
        float limiteIzq = xcentro - radioCoords, limiteDer = xcentro + radioCoords;
        float limiteInf = ycentro - radioCoords, limiteSup = ycentro + radioCoords;

        if(limiteIzq < _aXMin) limiteIzq = _aXMin;
        if(limiteDer > _aXMax) limiteDer = _aXMax;
        if(limiteInf < _aYMin) limiteInf = _aYMin;
        if(limiteSup > _aYMax) limiteSup = _aYMax;

        for (float posX = limiteIzq; posX <= limiteDer; posX += tamaCasillaX) {
            for (float posY = limiteInf; posY <= limiteSup; posY += tamaCasillaY) {
                if (posX >= _aXMin && posX <= _aXMax && posY >= _aYMin && posY <= _aYMax) {
                    Casilla* casillaActual = obtenerCasilla(posX, posY);
                    if (!casillaActual) continue; // Esto lo que hace es ignorar las casillas nulas

                    for (const auto& elemento : casillaActual->puntos) {
                        UTM puntoCentro(xcentro,ycentro);
                        UTM puntoElemento(elemento->getX(),elemento->getY());

                        float distanciaCalculada = haversine(puntoCentro, puntoElemento);

                        if (distanciaCalculada <= radio) {
                            resultado.push_back(elemento);
                        }
                    }
                }
            }
        }

        return resultado;
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
