//
// Created by Danidel on 25/09/2024.
//

#include "Coche.h"
#include "iostream"
#include "stdexcept"

Coche::Coche(std::string id_matricula, std::string marca, std::string modelo): _id_matricula(id_matricula),
                                                                               _marca(marca),
                                                                               _modelo(modelo), _charging(nullptr),_posicion(0,0) {
}

Coche::Coche(const Coche &otro): _id_matricula(otro._id_matricula), _marca(otro._marca), _modelo(otro._modelo),
                                 nivelBateria(otro.nivelBateria), _charging(otro._charging), _posicion(otro._posicion.lat(),otro._posicion.lon()) {
}

Coche &Coche::operator=(const Coche &otro) {
    if (this != &otro) {
        _id_matricula = otro._id_matricula;
        _marca = otro._marca;
        _modelo = otro._modelo;
    }
    return (*this);
}

bool Coche::operator>(const Coche &otro) const {
    return nivelBateria > otro.nivelBateria;
}

bool Coche::operator<(const Coche &otro) const {
    return nivelBateria < otro.nivelBateria;
}

const std::string &Coche::getIdMatricula() const {
    return _id_matricula;
}

const std::string &Coche::getMarca() const {
    return _marca;
}

const std::string &Coche::getModelo() const {
    return _modelo;
}

bool Coche::operator==(const Coche &otro) const {
    return nivelBateria == otro.nivelBateria;
}

bool Coche::mismoModelo(const std::string &otro) {
    if (_modelo == otro) {
        return true;
    } else {
        return false;
    }
}

float Coche::getNivelBateria() const {
    return nivelBateria;
}

void Coche::setCharging(PuntoRecarga *charging) {
    _charging = charging;
}


void Coche::circular() {
    if(_charging->_almacenados.empty()) {
        throw std::invalid_argument("Coche::circular(): No hay coches almacenados en la cola de prioridad");
    }
    std::multimap<int, Coche*>::iterator it = _charging->_almacenados.end();
    it--;
    _charging->_almacenados.erase(it);
    _charging->_cochAlm--;
    _charging = nullptr;
}

bool Coche::aparcar(PuntoRecarga *pr) {
    this->setCharging(pr);
    if (_charging) {
        return true;
    } else {
        return false;
    }
}

void Coche::mostrarCoche() {
    std::cout << "Informacion del coche: "<< std::endl;
    std::cout << "Matricula: " << _id_matricula << std::endl;
    std::cout << "Marca: " << _marca << std::endl;
    std::cout << "Modelo: " << _modelo << std::endl;
    std::cout << "Cargando: ";
    if (_charging != nullptr) {
        std::cout << "Si" << std::endl;
        std::cout << "ID Punto de recarga: " << _charging->getID() << std::endl;
        std::cout << "Numero de coches cargando en el punto de recarga: " << _charging->getNumCoches() << std::endl;
    } else {
        std::cout << "No";
    }
}

PuntoRecarga *Coche::getCharging() const {
    return _charging;
}
