//
// Created by Danidel on 10/10/2024.
//

#include "Reanelcar.h"

void Usuario::setCoche(Coche *c) {
    rent = c;
}

const std::string Usuario::getNif() const {
    return _nif;
}

const std::string &Usuario::getClave() const {
    return _clave;
}

const std::string &Usuario::getNombre() const {
    return _nombre;
}

const std::string &Usuario::getDireccion() const {
    return _direccion;
}

Coche *Usuario::getRent() const {
    return rent;
}


Usuario::Usuario(const std::string &nif, const std::string &clave, const std::string &nombre,
                 const std::string &direccion) : _nif(nif), _clave(clave), _nombre(nombre), _direccion(direccion) {
}

Trayecto *Usuario::crearTrayecto(PuntoRecarga *orig, PuntoRecarga *dest, Fecha &fInicio, Fecha &fFin) {
    Trayecto *nuevoTrayecto = new Trayecto(*orig, *dest, fInicio, fFin);
    nuevoTrayecto->setCoche(rent);
    _route.insert(std::make_pair(nuevoTrayecto->getFechaInicio(), *nuevoTrayecto));
    return nuevoTrayecto;
}

Coche *Usuario::iniciaTrayecto(int idPuntoInicio, int idPuntoFinal, Fecha &fInicio, Fecha &fFin) {
    Coche *aDev = usrReanel->alquilar(this, idPuntoInicio, idPuntoFinal);
    PuntoRecarga *prInicio = usrReanel->buscarPunto(idPuntoInicio); //Cambiado
    PuntoRecarga *prDestino = usrReanel->buscarPunto(idPuntoFinal);
    crearTrayecto(prInicio, prDestino, fInicio, fFin);
    return aDev;
}

void Usuario::aparcaCoche(Coche *c, PuntoRecarga *pr, int retraso) {
    this->usrReanel->colocarCochePR(c, pr);
    rent = nullptr;
    _puntos = decrementaPuntos(retraso);
}

int Usuario::decrementaPuntos(int retraso) {
    return _puntos - 2 * retraso;
}

void Usuario::eliminarTrayectos() {
    _route.clear();
}


std::multimap<Fecha, Trayecto> &Usuario::getTrayectosFecha(const Fecha &f) {
    std::multimap<Fecha, Trayecto> *aDev = new std::multimap<Fecha, Trayecto>;
    std::multimap<Fecha, Trayecto>::iterator it = _route.begin();
    while (it != _route.end()) {
        if (it->first.mismoDia(f)) {
            aDev->insert(std::pair(it->first, it->second));
        }
        it++;
    }
    return *aDev;
}


Trayecto *Usuario::getTrayecto(Fecha &fIni) {
    std::multimap<Fecha, Trayecto>::iterator it;
    it = _route.find(fIni);
    return &it->second;
}

void Usuario::mostarDatos() {
    std::cout << "Nombre: " << _nombre;
    std::cout << " | DNI: " << _nif;
    std::cout << " | Direccion: " << _direccion;
    std::cout << " | Clave: " << _clave << std::endl;
    if (_route.empty()) {
        std::cout << "No tiene trayectos" << std::endl;
    } else {
        auto it = _route.begin();
        while (it != _route.end()) {
            std::cout << " | Id Tryecto: " << it->second.getIdTrayecto();
            std::cout << " | Fecha de inicio: " << it->second.getFechaInicio();
            std::cout << " | Fecha de fin: " << it->second.getFechaFinal();
            std::cout << " | Id PR origen del trayecto: " << it->second.getOrigin()->getID();
            std::cout << " | Id PR destino del trayecto: " << it->second.getDestination()->getID();
            it++;
        }
    }
}
