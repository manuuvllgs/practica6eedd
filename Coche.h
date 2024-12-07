//
// Created by Danidel on 25/09/2024.
//

#ifndef PRACTICA01_COCHE_H
#define PRACTICA01_COCHE_H
#include <iostream>
#include <map>
#include <queue>

#include "fecha.h"
#include "UTM.h"


class PuntoRecarga;


class Coche {
private:
    std::string _id_matricula;
    std::string _marca;
    std::string _modelo;
    float nivelBateria = std::rand() % (100 - 20 + 1) + 20;;
    PuntoRecarga *_charging = nullptr;
    UTM _posicion;

public:
    Coche(): _id_matricula(), _marca(), _modelo(), _posicion(0, 0) {
    }

    float getY() {
        return _posicion.lon();
    }

    float getX() {
        return _posicion.lat();
    }

    Coche(std::string id_matricula, std::string marca, std::string modelo);

    Coche(const Coche &otro);

    void setCharging(PuntoRecarga *charging);

    Coche &operator=(const Coche &otro);

    bool operator>(const Coche &otro) const;

    bool operator<(const Coche &otro) const;

    bool operator==(const Coche &otro) const;

    const std::string &getIdMatricula() const;

    const std::string &getMarca() const;

    const std::string &getModelo() const;

    bool mismoModelo(const std::string &otro);

    float getNivelBateria() const;

    PuntoRecarga *getCharging() const;

    void circular();

    bool aparcar(PuntoRecarga *pr);

    void mostrarCoche();
};


class PuntoRecarga {
public:
    friend class Coche;

private:
    int _id;
    UTM _lugar;
    int _max = 0;
    int _cochAlm = 0;
    std::multimap<int, Coche*> _almacenados;

public:
    UTM lugar() const {
        return _lugar;
    }

    PuntoRecarga(int id, float latitud, float longitud, int max);

    Coche *getMaxBateria();

    bool addCoche(Coche *c);

    int getNumCoches();

    int getID();

    int max() const {
        return _max;
    }

    void mostrarDatosCochesAlmacenados();
};


class Trayecto {
private:
    static int numTrayectos;
    int _idTrayecto = 0;
    Fecha _fechaInicio;
    Fecha _fechaFinal;
    PuntoRecarga *_origin = nullptr;
    PuntoRecarga *_destination = nullptr;
    Coche *_inthecar = nullptr;

public:
    Trayecto(PuntoRecarga &orig, PuntoRecarga &dest, Fecha &fInicio, Fecha &fFin);

    void setCoche(Coche *c);

    int getIdTrayecto() const;

    void setIdTrayecto(int idTrayecto);

    const Fecha &getFechaInicio() const;

    void setFechaInicio(const Fecha &fechaInicio);

    const Fecha &getFechaFinal() const;

    void setFechaFinal(const Fecha &fechaFinal);

    PuntoRecarga *getOrigin() const;

    void setOrigin(PuntoRecarga *origin);

    PuntoRecarga *getDestination() const;

    void setDestination(PuntoRecarga *destination);

    Coche *getInthecar() const;

    void setInthecar(Coche *inthecar);
};


#endif //PRACTICA01_COCHE_H
