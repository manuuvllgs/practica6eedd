//
// Created by Danidel on 10/10/2024.
//

#ifndef PR2_VILLEGASACEITUNO_MANUEL_REANELCAR_H
#define PR2_VILLEGASACEITUNO_MANUEL_REANELCAR_H

#include <list>
#include <map>
#include <cmath>
#include "vector"
#include "Coche.h"
#include "MallaRegular.h"
#include "stdexcept"


class Usuario;

class ThashUsuario;

class Reanelcar {
private:
    std::list<Usuario> _users;
    std::map<std::string, Coche> _cars;
    std::vector<PuntoRecarga> _sites;
    ThashUsuario *_userNIF = nullptr;
    MallaRegular<Coche *> *locate;

public:
    Reanelcar(): _users(), _cars(), _sites(), _userNIF(), locate() {
    }

    ~Reanelcar() = default;

    ThashUsuario *getUserNIF() {
        return _userNIF;
    }

    void creaTablaHash(ThashUsuario *tabla) {
        _userNIF = tabla;
    }

    std::vector<PuntoRecarga> *sites() {
        return &_sites;
    }

    void insertarSite(PuntoRecarga pr) {
        _sites.push_back(pr);
    }

    void insertaUsr(Usuario *u);

    void insertaCar(Coche &c);

    std::list<Usuario> &getUsers();

    std::map<std::string, Coche> &getCars();

    //Debe de ser un puntero para que pueda devolver nulo
    Usuario *buscarUsrNif(std::string nif);

    std::list<Usuario *> &buscarUsrNomb(std::string nom);

    //Debe de ser un puntero para que pueda devolver nulo
    Coche *buscarCochMat(std::string mat);

    Coche *alquilar(Usuario *usr, int idPROrigen, int idPRDestino);

    bool colocarCochePR(Coche *c, PuntoRecarga *pr);

    std::map<std::string, Coche *> getCochesOrdenados();

    PuntoRecarga *buscarPunto(int id);

    PuntoRecarga *obtenerPRmenosCoches();

    int pruebaEj1(int &capTotal);

    std::vector<std::pair<Usuario *, Trayecto> > trayectosEnFecha(Fecha &f);

    // Práctica 5

    void mostrarEstadoTabla();

    Usuario *buscarUsuNifTHash(unsigned long clave, std::string nif);

    void borrarUsuarioTHash(unsigned long clave, std::string nif);


    // Práctica 6
    float haversine1(UTM utm1, UTM utm2) {
        float radio_tierra = 6378;
        float incrLat = (utm2.lat() - utm1.lat()) * (M_PI / 180);
        float incrLon = (utm2.lon() - utm1.lon()) * (M_PI / 180);
        float a = sin(incrLat / 2) * sin(incrLat / 2) + cos(utm1.lat() * (M_PI / 180)) * cos(utm2.lat() * (M_PI / 180))
                  * sin(incrLon / 2) * sin(incrLon / 2);
        float c = 2 * atan2(sqrt(a), sqrt(1 - a));
        float d = radio_tierra * c;
        return d;
    }





private:
    void rellenaMalla(Coche* aInsertar) {
        locate->insertar(aInsertar->getX(),aInsertar->getX(),aInsertar);
    }
public:
    // En la practica pone que tiene que ser unsigned float pero NO EXISTE UNSIGNED FLOAT ME CAGO EN DIOS
    std::vector<Coche *> buscarCochesRadio(UTM pos, float radioKm) {
        return locate->buscarRadio(pos.lat(), pos.lon(), radioKm);
    }

// Tu sabes por qué en la practica pone que tiene que devolver un vector de coches si solo tiene que devolver uno?
/* FixMe Aqui lo que intento hacer es situarme en la casilla que se me pasa como parametro, una vez ahi la guardo en
 *  una variable. En otra variable de tipo coche guardo el primer coche de la lista de esa casilla y el iterador
 *  lo situo de la misma manera al principio. La distancia la intento calcular con el teorema de pitagoras.
 *  Despues con un for y un iterador recorro toda la lista y voy calculando la distancia de cada coche y
 *  comparandola con la distancia menor que es la del primer coche de la lista.
 *
 **/
std::vector<Coche*> buscarCocheMasCercano(UTM pos) {
        MallaRegular<Coche*>::Casilla* dondeEstoy = locate->obtenerCasilla(pos.lat(),pos.lon());
        int distancia = 10;
        float comprobacion=0;
        bool encontrado = false;
        float disMenor = 100000;
        std::vector<Coche*> aux;
        while(!encontrado && distancia<99999) {
            aux = buscarCochesRadio(pos,distancia);
            if(aux.size() > 0 ) {
                encontrado = true;
            }
            else {distancia*=5;}
        }
        std::vector<Coche*> toRet;

        for (int i = 0; i<aux.size(); i++) {
            UTM posCoche(aux[i]->getX(),aux[i]->getY());
            comprobacion = haversine1(pos,posCoche);
           if (comprobacion < disMenor) {
               disMenor = comprobacion;
               toRet.clear();
               toRet.push_back(aux[i]);
           }
        }
        return toRet;

    }

void crearMalla(float aXMin, float aYMin, float aXMax, float aYMax,float ndiv) {
        MallaRegular<Coche *> *aux = new MallaRegular<Coche *>(aXMin,aYMin,aXMax,aYMax,ndiv);
        locate = aux;
    }

};

class Usuario {
private:
std::string _nif;
std::string _clave;
std::string _nombre;
std::string _direccion;
Coche *rent = nullptr;
Reanelcar *usrReanel = nullptr;
std::multimap<Fecha, Trayecto> _route;
int _puntos = 100;

public:
Usuario(const std::string &nif, const std::string &clave, const std::string &nombre, const std::string &direccion);

Usuario() = default;

//No hacemos el constructor de copia porque no puede haber dos usuarios iguales
void setCoche(Coche *c);

const std::string getNif() const;

const std::string &getClave() const;

const std::string &getNombre() const;

const std::string &getDireccion() const;

Coche *getRent() const;

Coche &cogeCoche();

Coche &linkReanel();

void set_usr_reanel(Reanelcar *usr_reanel) {
    usrReanel = usr_reanel;
}

//Practica 4
Trayecto *crearTrayecto(PuntoRecarga *orig, PuntoRecarga *dest, Fecha &fInicio, Fecha &fFin);

Coche *iniciaTrayecto(int idPuntoInicio, int idPuntoFinal, Fecha &fInicio, Fecha &fFin);

void aparcaCoche(Coche *c, PuntoRecarga *pr);

std::multimap<Fecha, Trayecto> &getTrayectosFecha(const Fecha &f);

Trayecto *getTrayecto(Fecha &fIni);

void mostarDatos();

std::multimap<Fecha, Trayecto> route() const {
    return _route;
}

//Practica 5

int decrementaPuntos(int retraso);

void eliminarTrayectos();

int getPuntos() const {
    return _puntos;
}

};


#endif //PR2_VILLEGASACEITUNO_MANUEL_REANELCAR_H


