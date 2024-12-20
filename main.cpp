// Trabajo realizado por Daniel del Salto Abril y Manuel Villegas Aceituno

#include <fstream>
#include <iostream>
#include <sstream>
#include "Reanelcar.h"
#include "Coche.h"
#include "stdexcept"
#include "img.h"
#include <cmath>

#include <list>
#include <vector>
#include <map>
#include "ThashUsuario.h"

//Cambiado la funcon djb2 a una con string, cambiado insertar y borrar. Me sigue da el error de ilegal instruction que solucionamos pero que como no me respondes no se como lo arreglamos


unsigned long djb2(std::string str) {
    unsigned long hash = 5381;
    int c;
    int n = str.length();
    for (int i = 0; i < n; i++) {
        c = str[i];
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

float haversine3(UTM utm1, UTM utm2) {
    float radio_tierra = 6378;
    float incrLat = (utm2.lat() - utm1.lat()) * (M_PI / 180);
    float incrLon = (utm2.lon() - utm1.lon()) * (M_PI / 180);
    float a = sin(incrLat / 2) * sin(incrLat / 2) + cos(utm1.lat() * (M_PI / 180)) * cos(utm2.lat() * (M_PI / 180))
              * sin(incrLon / 2) * sin(incrLon / 2);
    float c = 2 * atan2(sqrt(a), sqrt(1 - a));
    float d = radio_tierra * c;
    return d;
}


int main() {
    // ---------------Ejercicio 1---------------------

    Reanelcar app;
    ThashUsuario usersNifs(10001);
    app.creaTablaHash(&usersNifs);

    std::ifstream is;
    std::stringstream columnas;
    std::string fila;
    int contador = 0;
    int i = 0;
    std::string id_matricula = "";
    std::string marca = "";
    std::string modelo = "";


    is.open(
        "../coches_v2.csv");
    //carpeta de proyecto

    if (is.good()) {
        clock_t t_ini = clock();

        std::getline(is, fila);

        while (getline(is, fila)) {
            //¿Se ha leído una nueva fila?
            if (fila != "") {
                columnas.str(fila);

                //formato de fila: id_matricula;marca;modelo;

                getline(columnas, id_matricula, ','); //leemos caracteres hasta encontrar y omitir ';'
                getline(columnas, marca, ',');
                getline(columnas, modelo, ',');

                Coche dato(id_matricula, marca, modelo);
                app.getCars().insert(std::make_pair(id_matricula, dato));


                fila = "";
                columnas.clear();

                i++;
            }
        }

        is.close();

        std::cout << "Tiempo lectura: " << ((clock() - t_ini) / (float) CLOCKS_PER_SEC) << " segs." << std::endl;
    } else {
        std::cout << "Error de apertura en archivo" << std::endl;
    }


    // Lectura de usuarios y guardado en listaUsuarios


    std::ifstream is2;
    std::stringstream columnas2;
    std::string fila2;
    contador = 0;
    int i2 = 0;
    std::string nif = "";
    std::string nombre = "";
    std::string clave = "";
    std::string direccion = "";


    is.open(
        "../usuarios1.csv");
    //carpeta de proyecto

    if (is.good()) {
        clock_t t_ini = clock();

        while (getline(is, fila)) {
            //¿Se ha leído una nueva fila?
            if (fila != "") {
                columnas.str(fila);

                //formato de fila: id_matricula;marca;modelo;

                getline(columnas, nif, ',');
                getline(columnas, clave, ',');
                getline(columnas, nombre, ',');
                getline(columnas, direccion, ',');
                unsigned long hash = djb2(nif);
                Usuario *dato = new Usuario(nif, clave, nombre, direccion);
                app.insertaUsr(dato);
                auto it = app.getUsers().end();
                --it;
                app.getUserNIF()->insertar(hash, nif, &it.operator*());


                fila = "";
                columnas.clear();

                i++;
            }
        }

        is.close();

        std::cout << "Tiempo lectura: " << ((clock() - t_ini) / (float) CLOCKS_PER_SEC) << " segs." << std::endl;
    } else {
        std::cout << "Error de apertura en archivo" << std::endl;
    }


    // Lectura de puntos de recarga y guardado en sites


    std::ifstream is3;
    std::stringstream columnas3;
    std::string fila3;
    contador = 0;
    int i3 = 0;
    std::string id = "";
    std::string latitud = "";
    std::string longitud = "";
    std::string max = "";
    int capTotal = 0;


    is.open(
        "../puntos_recarga.csv");
    //carpeta de proyecto

    if (is.good()) {
        clock_t t_ini = clock();

        while (getline(is, fila)) {
            //¿Se ha leído una nueva fila?
            if (fila != "") {
                columnas.str(fila);

                //formato de fila: id_matricula;marca;modelo;

                getline(columnas, id, ','); //leemos caracteres hasta encontrar y omitir ';'
                getline(columnas, latitud, ',');
                getline(columnas, longitud, ',');
                getline(columnas, max, ',');


                PuntoRecarga dato(std::stoi(id), std::stof(latitud), std::stof(longitud), std::stoi(max) * 2);
                app.insertarSite(dato);

                capTotal += dato.max();


                fila = "";
                columnas.clear();

                i++;
            }
        }

        is.close();

        std::cout << "Tiempo lectura: " << ((clock() - t_ini) / (float) CLOCKS_PER_SEC) << " segs." << std::endl;
    } else {
        std::cout << "Error de apertura en archivo" << std::endl;
    }

    //-------------------Ejercicio 2------------------------
    // Ejercicio hecho en los .h

    //-------------------Ejercicio 3------------------------

    contador = 0;
    std::string nom = "";
    std::string lat = "";
    std::string lon = "";
    std::vector<std::pair<std::string, UTM> > destinos;
    float latMin = 10000000000, latMax = 0, lonMin = 10000000000, lonMax = -100000000;


    is.open(
        "../destino.csv");
    //carpeta de proyecto

    if (is.good()) {
        clock_t t_ini = clock();

        while (getline(is, fila)) {
            //¿Se ha leído una nueva fila?
            if (fila != "") {
                columnas.str(fila);


                getline(columnas, nom, ','); //leemos caracteres hasta encontrar y omitir ';'
                getline(columnas, lat, ',');
                getline(columnas, lon, ',');


                UTM dato(std::stof(lat), std::stof(lon));

                destinos.push_back(std::pair(nom, dato));
                if (latMin > dato.lat()) {
                    latMin = floor(dato.lat());
                }
                if (lonMin > dato.lon()) {
                    lonMin = floor(dato.lon());
                }
                if (latMax < dato.lat()) {
                    latMax = ceil(dato.lat());
                }
                if (lonMax < dato.lon()) {
                    lonMax = ceil(dato.lon());
                }


                fila = "";
                columnas.clear();

                i++;
            }
        }

        is.close();

        std::cout << "Tiempo lectura: " << ((clock() - t_ini) / (float) CLOCKS_PER_SEC) << " segs." << std::endl;
    } else {
        std::cout << "Error de apertura en archivo" << std::endl;
    }


    //Ahora calculamos las divisiones sabiendo que queremos que tengan entre 10 y 15 coches por division
    float nDiv = 1489 / (float) 12;
    std::cout << nDiv;
    //-------------------Ejercicio 4-------------------------


    //Metemos los coches en los puntos de recarga secuencialmente
    int cont = 0;


    std::list<int> puntosDisponibles;
    for (int i = 0; i < app.sites()->size(); ++i) {
        puntosDisponibles.push_back(i);
    }

    auto itC = app.getCars().begin();


    if (app.sites()->empty() || app.getCars().empty()) {
        std::cerr << "Vector de puntos de recarga o map de coches están vacíos.";
    } else {
        while (cont < capTotal && !puntosDisponibles.empty() && itC != app.getCars().end()) {
            auto itPunto = puntosDisponibles.begin();


            while (itPunto != puntosDisponibles.end() && itC != app.getCars().end()) {
                try {
                    int idx = *itPunto;
                    app.sites()->operator[](idx).addCoche(&itC->second);

                    ++cont;
                    ++itC;


                    if (app.sites()->operator[](idx).getNumCoches() >= app.sites()->operator[](idx).max()) {
                        itPunto = puntosDisponibles.erase(itPunto);
                    } else {
                        ++itPunto;
                    }
                } catch (const std::out_of_range &e) {
                    std::cerr << e.what() << std::endl;
                    itPunto = puntosDisponibles.erase(itPunto);
                }
            }
        }
    }
    //-------------------Ejercicio 5-------------------------

    std::vector<PuntoRecarga>::iterator itS;
    Fecha fDada(12, 11, 2024);
    Fecha hoy;
    std::list<Usuario>::iterator itU;
    itU = app.getUsers().begin();
    itS = app.sites()->begin();

    std::list<Usuario *> usrW = app.buscarUsrNomb("A");
    bool encontrado = false;
    auto itUsrW = usrW.begin();
    cont = 0;
    auto auxItUsrw = usrW.begin();
    for (i = 0; i < usrW.size(); i++) {
        hoy = fDada;
        if (itUsrW.operator*()->getNombre()[0] == 'A') {
            if (itS == app.sites()->end()) {
                cont = 0;
                itS = app.sites()->begin();
            }
            if (itS->getNumCoches() != 0) {
                if (encontrado == false) {
                    encontrado = true;
                    auxItUsrw = itUsrW;
                }
                hoy.anadirDias(rand() % 3);
                itUsrW.operator*()->iniciaTrayecto(cont, (cont + 1) % 50, fDada, hoy);
                cont++;
                itUsrW++;
                itS++;
            } else {
                cont++;
                itS++;
            }
        }
    }
    itUsrW = auxItUsrw;
    for (int i = 0; i < 10; i++) {
        Usuario *aux = itUsrW.operator*();
        Trayecto *tray = aux->getTrayecto(fDada);
        Coche *coch = tray->getInthecar();
        std::cout << "Usuario " << i << std::endl
                << "El nombre del usuario es:" << aux->getNombre() << std::endl
                << "El NIF es:" << aux->getNif() << std::endl
                << "La clave es:" << aux->getClave() << std::endl
                << "La direccion es:" << aux->getDireccion() << std::endl;
        std::cout << "La fecha de inicio de trayecto es:" << tray->getFechaInicio().cadena() << std::endl
                << "La fecha de fin de trayecto es:" << tray->getFechaFinal().cadena() << std::endl
                << "El Id del punto de recarga de origen es:" << tray->getOrigin()->getID() << std::endl
                << "El Id del punto de recarga de destino es:" << tray->getDestination()->getID() << std::endl
                << "El Id del trayecto es:" << tray->getIdTrayecto() << std::endl;
        std::cout << "La matricula del coche es:" << coch->getIdMatricula() << std::endl
                << "La marca del coche es:" << coch->getMarca() << std::endl
                << "EL modelo del coche es:" << coch->getModelo() << std::endl
                << "El nivel de bateria del coche es:" << coch->getNivelBateria() << std::endl;
        itUsrW++;
    }

    //-------------------Ejercicio 6-------------------------


    itU = app.getUsers().begin();
    itS = app.sites()->begin();

    std::list<Usuario *> usrB = app.buscarUsrNomb("B");
    encontrado = false;
    auto itUsrB = usrB.begin();
    cont = 0;
    auto auxItUsrB = usrB.begin();
    for (i = 0; i < usrB.size(); i++) {
        hoy = fDada;

        if (itS == app.sites()->end()) {
            cont = 0;
            itS = app.sites()->begin();
        }
        if (itS->getNumCoches() != 0) {
            if (encontrado == false) {
                encontrado = true;
                auxItUsrB = itUsrB;
            }
            hoy.anadirDias(rand() % 3);
            itUsrB.operator*()->iniciaTrayecto(cont, (cont + 1) % 50, fDada, hoy);
            cont++;
            itUsrB++;
            itS++;
        } else {
            cont++;
            itS++;
        }
    }
    itUsrB = auxItUsrB;
    for (int i = 0; i < 10; i++) {
        Usuario *aux = itUsrB.operator*();
        Trayecto *tray = aux->getTrayecto(fDada);
        Coche *coch = tray->getInthecar();
        std::cout << "Usuario " << i << std::endl
                << "El nombre del usuario es:" << aux->getNombre() << std::endl
                << "El NIF es:" << aux->getNif() << std::endl
                << "La clave es:" << aux->getClave() << std::endl
                << "La direccion es:" << aux->getDireccion() << std::endl;
        std::cout << "La fecha de inicio de trayecto es:" << tray->getFechaInicio().cadena() << std::endl
                << "La fecha de fin de trayecto es:" << tray->getFechaFinal().cadena() << std::endl
                << "El Id del punto de recarga de origen es:" << tray->getOrigin()->getID() << std::endl
                << "El Id del punto de recarga de destino es:" << tray->getDestination()->getID() << std::endl
                << "El Id del trayecto es:" << tray->getIdTrayecto() << std::endl;
        std::cout << "La matricula del coche es:" << coch->getIdMatricula() << std::endl
                << "La marca del coche es:" << coch->getMarca() << std::endl
                << "EL modelo del coche es:" << coch->getModelo() << std::endl
                << "El nivel de bateria del coche es:" << coch->getNivelBateria() << std::endl;
        itUsrB++;
    }

    //--------------------Ejercicio 7--------------------------
    // Creamos la malla
    app.crearMalla(latMin, lonMin, latMax, lonMax, nDiv);

    std::cout << std::endl << std::endl << std::endl << "Ejercicio 4 " << std::endl;
    itUsrB = usrB.begin();
    while (itUsrB != usrB.end()) {
        app.ejVoluntario.push_back(itUsrB.operator*()->getRent());
        itUsrB.operator*()->aparcaCoche(itUsrB.operator*()->getRent(),
                                        itUsrB.operator*()->getTrayecto(fDada)->getDestination());

        itUsrB++;
    }

    itUsrB = usrB.begin();
    for (int i = 0; i < 10; ++i) {
        std::cout << " | Usaurio " << i + 1 << ": " << itUsrB.operator*()->getNombre() << std::endl;
        std::cout << " | Puntos del usuario: " << itUsrB.operator*()->getPuntos();
        std::cout << " | Origen: " << itUsrB.operator*()->getTrayecto(fDada)->getOrigin()->getID();
        std::cout << " | Destino: " << itUsrB.operator*()->getTrayecto(fDada)->getDestination()->getID();
        std::cout << " | Coche alquilado: " << itUsrB.operator*()->getTrayecto(fDada)->getInthecar()->
                getIdMatricula();
        std::cout << " | Fecha: " << itUsrB.operator*()->getTrayecto(fDada)->getFechaFinal();
        std::cout << std::endl;
        ++itUsrB;
    }


    //--------------------Ejercicio 8--------------------------

    itUsrW = usrW.begin();
    while (itUsrW != usrW.end()) {
        bool encontrado = false;
        for (int i = 0; i < usrW.size() && encontrado == false; ++i) {
            if (destinos[i].first == itUsrW.operator*()->getNombre()) {
                itUsrW.operator*()->aparcaCoche(itUsrW.operator*()->getRent(),
                                                destinos[i].second);
                encontrado = 1;
            }
        }

        itUsrW++;
    }

    itUsrW = usrW.begin();
    for (int i = 0; i < 10; ++i) {
        std::cout << " | Usaurio " << i + 1 << ": " << itUsrW.operator*()->getNombre() << std::endl;
        std::cout << " | Puntos del usuario: " << itUsrW.operator*()->getPuntos();
        std::cout << " | Origen: " << itUsrW.operator*()->getTrayecto(fDada)->getOrigin()->getID();
        std::cout << " | Destino: " << itUsrW.operator*()->getTrayecto(fDada)->getDestination()->getID();
        std::cout << " | Coche alquilado: " << itUsrW.operator*()->getTrayecto(fDada)->getInthecar()->
                getIdMatricula();
        std::cout << " | Fecha: " << itUsrW.operator*()->getTrayecto(fDada)->getFechaFinal();
        std::cout << std::endl;
        ++itUsrW;
    }

    //------------------Ejercicio 9-------------------------
    std::vector<Coche *> cercanos = app.locate1()->buscarRadio(37.7692200, -3.7902800, 10);
    std::cout << "Los coches mas cercanos a jaen son: ";
    for (int i = 0; i < cercanos.size(); ++i) {
        if (i % 20 == 0) {
            std::cout << std::endl;
        }
        std::cout << cercanos.operator[](i)->getIdMatricula() << "|";
    }

    std::cout << std::endl;

    std::string matricula = app.buscarCocheMasCercano(UTM(37.7692200, -3.7902800)).operator[](0)->getIdMatricula();
    std::cout << "La matricula del coche mas cercano es: " << matricula << std::endl;

    //------------------Ejercicio 10----------------------

    matricula = app.buscarCocheMasCercano(app.sites()->operator[](42).lugar()).operator[](0)->getIdMatricula();
    std::cout << "La matricula del coche mas cercano al PR 43 es: " << matricula << std::endl;

    //------------------Ejercicio 11------------------------
    PuntoRecarga *prMenorCochesJaen;
    int menor = 1000;
    for (int i = 0; i < app.sites()->size(); ++i) {
        float distancia;
        distancia = haversine3(app.sites()->operator[](i).lugar(), UTM(37.7692200, -3.7902800));
        if (distancia < 15) {
            if (app.sites()->operator[](i).getNumCoches() < menor) {
                menor = app.sites()->operator[](i).getNumCoches();
                prMenorCochesJaen = &app.sites()->operator[](i);
            }
        }
    }
    std::cout << "El punto de recarga con mayor numero de coches en un radio a 15kms de Jaén es el punto con ID " <<
            prMenorCochesJaen->getID() << " con " << menor << " coches" << std::endl;
    std::cout << "La matricula de los coches en un radio de 25km alrededor del punto de recarga con mas coches cerca de Jaen son:" ;
    UTM posPR(prMenorCochesJaen->lugar());
    cercanos = app.locate1()->buscarRadio(posPR.lat(), posPR.lon(), 25);
    for (int i = 0; i < cercanos.size(); ++i) {
        if (i % 18 == 0) {
            std::cout << std::endl;
        }
        std::cout << cercanos.operator[](i)->getIdMatricula() << "|";

    }

    //----------------------------Ej Voluntario--------------------------

    app.ejercicioVol("mapaResultado.ppm");
    return 0;
}
