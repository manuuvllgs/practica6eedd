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


    std::vector<T> buscarRadio(float xcentro, float ycentro, float radio) {
        std::vector<T> resultado;

        float coordenadas = radio / 111.1;

        Casilla esquinaIzqSup = obtenerCasilla(xcentro - coordenadas, ycentro + coordenadas),
                esquinaDerSup = obtenerCasilla(xcentro + coordenadas, ycentro + coordenadas),
                esquinaIzqInf = obtenerCasilla(xcentro - coordenadas, ycentro - coordenadas),
                esquinaDerInf = obtenerCasilla(xcentro + coordenadas, ycentro - coordenadas),
                centro = obtenerCasilla(xcentro, ycentro);


        auto iIzqSup = mr.find(esquinaIzqSup);
        auto iDerSup = mr.find(esquinaDerSup);
        auto iIzqInf = mr.find(esquinaIzqInf);
        auto iDerInf = mr.find(esquinaDerInf);

        for (auto i = iIzqSup; i != iDerSup; i++) {
            for (auto j = i; j != iIzqInf; j++) {
                for (auto k = j->begin(); k != j->end(); k++) {
                    if (distanciaPuntos(*k, centro) <= radio)
                        resultado.push_back(*k);
                }
            }
        }

        return resultado;
    }

    unsigned maxElementosPorCelda() {
        // Variable donde vamos a guardar el maximo que encontremos
        int numMax = 0;

        // Aqui lo que hago es definir las casillas de las cuatros esquinas para poder recorrer la malla entera
        Casilla<T> &izqSup = obtenerCasilla(_aXMin, _aYMax), izqInf = obtenerCasilla(_aXMin, _aYMin), derSup =
                obtenerCasilla(_aXMax, _aYMax), derInf = obtenerCasilla(_aXMax, _aYMin);

        // Aqui los iteradores
        auto iIzqSup = mr.find(izqSup);
        auto iDerSup = mr.find(derSup);
        auto iIzqInf = mr.find(izqInf);
        auto iDerInf = mr.find(derInf);

        // El for recorre toda la malla y guarda en una variable el numero maximo de elementos que vaya encontrando
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
        // Calculamos el numero de casillas y las guardamos en una variable
        int numCasillas = ((_aXMax - _aXMin) / tamaCasillaX) * ((_aYMax - _aYMin) / tamaCasillaY);

        // Vamos sumando todos los puntos que hay en cada casilla para al final dividirlo entre el numCasillas
        int numElementos = 0;

        // Volvemos a utilizar los for de antes para recorrer la malla entera
        // Aqui lo que hago es definir las casillas de las cuatros esquinas para poder recorrer la malla entera
        Casilla<T> &izqSup = obtenerCasilla(_aXMin, _aYMax), izqInf = obtenerCasilla(_aXMin, _aYMin), derSup =
                obtenerCasilla(_aXMax, _aYMax), derInf = obtenerCasilla(_aXMax, _aYMin);

        // Aqui los iteradores
        auto iIzqSup = mr.find(izqSup);
        auto iDerSup = mr.find(derSup);
        auto iIzqInf = mr.find(izqInf);
        auto iDerInf = mr.find(derInf);

        // El for recorre toda la malla y guarda en una variable el numero maximo de elementos que vaya encontrando
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
