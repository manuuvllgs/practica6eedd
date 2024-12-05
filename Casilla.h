//
// Created by Manuel Villegas Aceituno on 3/12/24.
//

#ifndef CASILLA_H
#define CASILLA_H
#include <list>
#include "MallaRegular.h"

template<typename T>
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


#endif //CASILLA_H