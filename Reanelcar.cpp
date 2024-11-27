//
// Created by Danidel on 10/10/2024.
//

#include "Reanelcar.h"

#include "ThashUsuario.h"


void Reanelcar::insertaUsr(Usuario *u) {
    u->set_usr_reanel(this);
    _users.push_back(*u);

}

void Reanelcar::insertaCar(Coche &c) {
   _cars.insert(std::make_pair(c.getIdMatricula(),c));
}

Usuario *Reanelcar::buscarUsrNif(std::string nif) {
    std::list<Usuario>::iterator iterator= this->_users.begin();
    for (int i = 0; i < _users.size(); ++i) {
        if (iterator.operator*().getNif() == nif) {
            Usuario *u1 = &iterator.operator*();
            return u1;
        }
        iterator.operator++();
    }
}

std::list<Usuario*> &Reanelcar::buscarUsrNomb(std::string nom) {
    std::list<Usuario>::iterator iterator= _users.begin();
    std::list<Usuario*> *aDevolver = new std::list<Usuario*>;
    for (int i =0; i < _users.size(); ++i) {
        if (iterator.operator*().getNombre().substr(0, nom.length()) == nom) {
            aDevolver->push_back(&iterator.operator*());
        }
        iterator.operator++();
    }

    return *aDevolver;
}


Coche *Reanelcar::buscarCochMat(std::string mat) {
    return &_cars[mat];
}

Coche* Reanelcar::alquilar(Usuario *usr, int idPROrigen, int idPRDestino){
    PuntoRecarga *PRorigen = buscarPunto(idPROrigen);
    PuntoRecarga *PRDestino = buscarPunto(idPRDestino);
    Coche *cocheAAlquilar = PRorigen->getMaxBateria();
    usr->setCoche(cocheAAlquilar);
    cocheAAlquilar->circular();
    return cocheAAlquilar;
}

std::list<Usuario> &Reanelcar::getUsers() {
    return _users;
}

std::map<std::string,Coche> &Reanelcar::getCars() {
    return _cars;
}


bool Reanelcar::colocarCochePR(Coche *c, PuntoRecarga *pr) {
    if(pr->addCoche(c) == false) {
        throw std::out_of_range("[Reanelcar::colocarCochePR](Coche,PuntoRecarga). Se ha alcanzado el maximo de coches.");
    } else {
        c->aparcar(pr);
        return true;
    }
}

PuntoRecarga* Reanelcar::buscarPunto(int id) {
    return &_sites[id];
}

PuntoRecarga *Reanelcar::obtenerPRmenosCoches() {
    int pos = 0;
    for(int i  = 0;i<_sites.size(); i++){
        if(_sites[i].getNumCoches() < _sites[pos].getNumCoches()){
            pos = i;
        }
    }
    return &_sites[pos];
}


std::vector<std::pair<Usuario *, Trayecto>> Reanelcar::trayectosEnFecha(Fecha &f) {
    std::vector<std::pair<Usuario*,Trayecto>> *toRet = new std::vector<std::pair<Usuario*,Trayecto>>;
    auto itU = _users.begin();
    while(itU != _users.end()) {
        for (int i=0; i < itU->getTrayectosFecha(f).size(); ++i) {
            
        }
    }
}

void Reanelcar::mostrarEstadoTabla() {
    std::cout << "Numero de MaxColisiones:" << _userNIF->maxColisiones()<< std::endl;
    std::cout << "Numero de numMax10:" << _userNIF->numMax10()<< std::endl;
    std::cout << "Promedio de colisiones: " << _userNIF->promedioColisiones()<< std::endl;
    std::cout << "El factor de carga es: " << _userNIF->factorCarga()<< std::endl;
    std::cout << "El tamanio de la tambla es: " << _userNIF->tamTabla()<< std::endl;
}

Usuario *Reanelcar::buscarUsuNifTHash(unsigned long clave, std::string nif) {
    int intentos = 0;
    return _userNIF->buscar(clave,nif,intentos);
}

void Reanelcar::borrarUsuarioTHash(unsigned long clave, std::string nif) {
    _userNIF->borrar(clave,nif);
}

