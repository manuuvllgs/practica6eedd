//
// Created by Manuel Villegas Aceituno on 4/11/24.
//

#include "Coche.h"

int Trayecto::numTrayectos = 0;


Trayecto::Trayecto(PuntoRecarga &orig, PuntoRecarga &dest,Fecha &fInicio,Fecha &fFin)
                     : _origin(&orig), _destination(&dest),_idTrayecto(numTrayectos++),
                     _fechaInicio(fInicio),_fechaFinal(fFin){}

void Trayecto::setCoche(Coche *c) {
    _inthecar = c;

}

int Trayecto::getIdTrayecto() const {
    return _idTrayecto;
}

void Trayecto::setIdTrayecto(int idTrayecto) {
    _idTrayecto = idTrayecto;
}

const Fecha &Trayecto::getFechaInicio() const {
    return _fechaInicio;
}

void Trayecto::setFechaInicio(const Fecha &fechaInicio) {
    _fechaInicio = fechaInicio;
}

const Fecha &Trayecto::getFechaFinal() const {
    return _fechaFinal;
}

void Trayecto::setFechaFinal(const Fecha &fechaFinal) {
    _fechaFinal = fechaFinal;
}

PuntoRecarga *Trayecto::getOrigin() const {
    return _origin;
}

void Trayecto::setOrigin(PuntoRecarga *origin) {
    _origin = origin;
}

PuntoRecarga *Trayecto::getDestination() const {
    return _destination;
}

void Trayecto::setDestination(PuntoRecarga *destination) {
    _destination = destination;
}

Coche *Trayecto::getInthecar() const {
    return _inthecar;
}

void Trayecto::setInthecar(Coche *inthecar) {
    _inthecar = inthecar;
}
