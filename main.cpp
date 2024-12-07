// Trabajo realizado por Daniel del Salto Abril y Manuel Villegas Aceituno

#include <fstream>
#include <iostream>
#include <sstream>
#include "Reanelcar.h"
#include "Coche.h"
#include "stdexcept"

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



    return 0;
}
