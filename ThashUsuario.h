//
// Created by Manuel Villegas Aceituno on 19/11/24.
//

#ifndef THASHUSUARIO_H
#define THASHUSUARIO_H
#include <vector>
#include "Reanelcar.h"


class Entrada {
public:
    char _estado = 'v'; // v (vacía), o (ocupada), d (disponibles)
    long _clave = 0;
    Usuario *_dato = nullptr;

    Entrada(long aClave, Usuario *aDato, char est): _clave(aClave), _dato(aDato), _estado(est) {
    }

    Entrada() = default;

    Entrada(const Entrada &otra): _estado(otra._estado), _clave(otra._clave), _dato(otra._dato) {
    }
};


class ThashUsuario {
private:
    std::vector<Entrada> tabla;
    float _lambda = 0.65;
    float numeroElementos = 0;
    int _numMaxColisiones = 0;
    int _numMax10 = 0;
    float _promedioColisiones = 0;
    float _numColisiones = 0;
    int inserciones = 0;

    long fDisp(int clave, int i) {
        // Funcion de dispersion
        // Cuadrática: h(x) = (x + i^2) % t (size)
        // Dispersión doble1 : h1(x) = x * q - 1
        //                   h2(x) = q + 2q
        //                   h(x) = (h1(x) + i * h2(x)) % t
        // Dispersión doble2 : h1(x) = (x * q)^2 - x
        //                   h2(x) = 31 + x q(
        //                   h(x) = (h1(x) + i * h2(x)) % t
        int primo = 0;
        while (!es_primo(primo)) {
            primo++;
        }
        return (clave + i*i) % tabla.size();
    }


public:
    bool es_primo(int n) {
        if (n <= 1) {
            return false;
        }

        if (n <= 3) {
            return true;
        }

        if (n % 2 == 0 ||  n % 3 == 0) {
            return false;
        }

        for (int i = 5; i * i <= n; i += 6) {
            if (n % i == 0  || n % (i + 2) == 0) {
                return false;
            }
        }
        return true;
    }

    ThashUsuario() = default;

    ThashUsuario(int numelementos, float lambda = 0.65) {
        int maxElementos = numelementos / lambda;
        _lambda = lambda;
        while (!es_primo(maxElementos)) {
            maxElementos++;
        }
        tabla = std::vector<Entrada>(maxElementos, Entrada());
    }

    ThashUsuario(const ThashUsuario &otro) : _lambda(otro._lambda) {
        for (int i = 0; i < tabla.size(); i++) {
            this->tabla[i] = *new Entrada(otro.tabla[i]);
        }
    }

    bool insertar(unsigned long clave, std::string clave_string, Usuario *dato) {
        int intentos = 0;
        if (!buscar(clave, clave_string, intentos)) {
            tabla[fDisp(clave, intentos)]._clave=clave;
            tabla[fDisp(clave, intentos)]._dato=dato;
            tabla[fDisp(clave, intentos)]._estado='o';
        } else {
            return 0;
        };
        if (intentos > 10) {
            ++_numMax10;
        }
        if (intentos > _numMaxColisiones) {
            _numMaxColisiones = intentos;
        }
        ++numeroElementos;

        _numColisiones+= intentos;
        ++inserciones;
        return true;

    }

    bool insertarYMostrar(unsigned long clave, std::string clave_string, Usuario *dato) {
        int intentos = 0;
        if (!buscar(clave, clave_string, intentos)) {
            tabla[fDisp(clave, intentos)]._clave=clave;
            tabla[fDisp(clave, intentos)]._dato=dato;
            tabla[fDisp(clave, intentos)]._estado='o';
        } else {
            return 0;
        };
        if (intentos > 10) {
            ++_numMax10;
        }
        if (intentos > _numMaxColisiones) {
            _numMaxColisiones = intentos;
        }
        ++numeroElementos;

        _numColisiones+= intentos;
        ++inserciones;
        std::cout << "El numero de colisiones ha sido: " << intentos << std::endl;
        return true;
    }

    ~ThashUsuario() = default;

    Usuario *buscar(unsigned long clave, std::string &clave_string, int &intentos) {
        bool validar = 0;
        while (!validar) {
            unsigned long hash = fDisp(clave, intentos);
            if (tabla[hash]._estado == 'v') {
                validar = 1;
                return nullptr;
            }

            if (tabla[hash]._estado == 'd') {
                ++intentos;
            }

            if (tabla[hash]._estado == 'o') {
                if (tabla[hash]._dato->getNif() == clave_string) {
                    validar = 1;
                    return tabla[hash]._dato;
                } else {
                    ++intentos;
                }
            }
        }
    }

    bool borrar(unsigned long clave, std::string clave_string) {
        int intentos = 0;
        if (!buscar(clave, clave_string, intentos)) {
            return 0;
        } else {
            tabla[fDisp(clave, intentos)]._clave=0;
            tabla[fDisp(clave, intentos)]._dato->eliminarTrayectos();
            tabla[fDisp(clave, intentos)]._dato->setCoche(nullptr);
            tabla[fDisp(clave, intentos)]._dato=nullptr;
            numeroElementos--;
            tabla[fDisp(clave, intentos)]._estado='d';
            return 1;
        }

    }

    unsigned int numElementos() {
        return numeroElementos;
    }


    ThashUsuario &operator=(const ThashUsuario &otro) {
        for (int i = 0; i < tabla.size(); i++) {
            this->tabla[i] = *new Entrada(otro.tabla[i]);
        }
        this->_lambda = otro._lambda;
        return *this;
    }



    unsigned int maxColisiones() {
        return _numMaxColisiones;
    }

    unsigned int numMax10() {
        return _numMax10;
    }

    float promedioColisiones() {
        return _numColisiones / inserciones;
    }

    float factorCarga() {
        return _lambda;
    }

    unsigned int tamTabla() {
        return tabla.size();
    }

    void refrescaLambda() {
        _lambda = numElementos()*1.0/tamTabla();
    }
};

#endif //THASHUSUARIO_H
