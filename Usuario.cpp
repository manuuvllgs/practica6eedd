//
// Created by Danidel on 10/10/2024.
//

#include "Reanelcar.h"

float haversine2(UTM utm1, UTM utm2) {
    float radio_tierra = 6378;
    float incrLat = (utm2.lat() - utm1.lat()) * (M_PI / 180);
    float incrLon = (utm2.lon() - utm1.lon()) * (M_PI / 180);
    float a = sin(incrLat / 2) * sin(incrLat / 2) + cos(utm1.lat() * (M_PI / 180)) * cos(utm2.lat() * (M_PI / 180))
              * sin(incrLon / 2) * sin(incrLon / 2);
    float c = 2 * atan2(sqrt(a), sqrt(1 - a));
    float d = radio_tierra * c;
    return d;
}

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

void Usuario::aparcaCoche(Coche *c, PuntoRecarga *pr){
    usrReanel->locate1()->insertar(pr->lugar().lat(),pr->lugar().lon(),c);
    this->usrReanel->colocarCochePR(c, pr);
    c->set_posicion(pr->lugar());
    rent = nullptr;
    int distancia = haversine2(UTM(c->getX(),c->getY()),pr->lugar());
    _puntos = decrementaPuntos(distancia);
}

void Usuario::aparcaCoche(Coche *c, UTM pos){
    usrReanel->locate1()->insertar(pos.lat(),pos.lon(),c);
    c->set_posicion(pos);
    rent = nullptr;
    int distancia = haversine2(UTM(c->getX(),c->getY()),pos);
    _puntos = decrementaPuntos(distancia);
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

