
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
    //Prueba de rendimiento
    /*std::vector<std::string> usrNifsW;
    auto it = app.getUsers().begin();
    while (it != app.getUsers().end()) {
        if(it->getNombre()[0]=='W') {
            usrNifsW.push_back(it->getNif());
        }
        it++;
    }
    clock_t t_ini = clock();
    for(int i = 0; i < usrNifsW.size(); i++) {
        unsigned long claveUsrW = djb2(usrNifsW[i]);
       app.buscarUsuNifTHash(claveUsrW, usrNifsW[i]);
    }
    std::cout << "Tiempo busqueda de los usuarios que empiezan por W: " << ((clock() - t_ini) / (float) CLOCKS_PER_SEC) << " segs." << std::endl;*/


    //Ejercicio 2
    std::cout << std::endl << std::endl << std::endl<< "Ejercicio 2 "<< std::endl ;
    app.getUserNIF()->refrescaLambda();
    app.mostrarEstadoTabla();

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

    //Ejercicio 3
    std::cout << std::endl << std::endl << std::endl<< "Ejercicio 3 "<< std::endl ;
    try {
        std::vector<PuntoRecarga>::iterator itS;
        Fecha fDada(12, 11, 2024);
        Fecha hoy;
        std::list<Usuario>::iterator itU;
        itU = app.getUsers().begin();
        itS = app.sites()->begin();

        std::list<Usuario *> usrW = app.buscarUsrNomb("W");
        bool encontrado = false;
        auto itUsrW = usrW.begin();
        cont = 0;
        auto auxItUsrw = usrW.begin();
        for (i = 0; i < usrW.size(); i++) {
            hoy = fDada;
            if (itUsrW.operator*()->getNombre()[0] == 'W') {
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

        //Ejercicio 4
        std::cout << std::endl << std::endl << std::endl<< "Ejercicio 4 " << std::endl;
        std::list<Usuario *> usrWi = app.buscarUsrNomb("Wi");
        auto itUsrWi = usrWi.begin();
        while (itUsrWi != usrWi.end()) {
            itUsrWi.operator*()->aparcaCoche(itUsrWi.operator*()->getRent(),
                                             itUsrWi.operator*()->getTrayecto(fDada)->getDestination(), 2);
            itUsrWi++;
        }
        itUsrWi = usrWi.begin();
        for (int i = 0; i < 10; ++i) {
            std::cout << " | Usaurio " << i + 1 << ": " << itUsrWi.operator*()->getNombre()<< std::endl;
            std::cout << " | Puntos del usuario: " << itUsrWi.operator*()->getPuntos();
            std::cout << " | Origen: " << itUsrWi.operator*()->getTrayecto(fDada)->getOrigin()->getID();
            std::cout << " | Destino: " << itUsrWi.operator*()->getTrayecto(fDada)->getDestination()->getID();
            std::cout << " | Coche alquilado: " << itUsrWi.operator*()->getTrayecto(fDada)->getInthecar()->
                    getIdMatricula();
            std::cout << " | Fecha: " << itUsrWi.operator*()->getTrayecto(fDada)->getFechaFinal();
            std::cout << std::endl;
            ++itUsrWi;
        }

        //Ejercicio 5
        std::cout << std::endl << std::endl << std::endl<< "Ejercicio 5 " << std::endl;
        unsigned long claveAux = djb2("84538382N");
        Usuario *aux = app.buscarUsuNifTHash(claveAux, "84538382N");

        if (aux) {
            aux->mostarDatos();
        } else {
            std::cout << "No se encontra el usuario" << std::endl;
        }

        //Ejercicio 6
        std::cout << std::endl << std::endl << std::endl<< "Ejercicio 6 " << std::endl;
        usersNifs.borrar(claveAux, aux->getNif());
        if (!app.buscarUsuNifTHash(claveAux, "84538382N")) {
            app.getUserNIF()->insertar(claveAux, "84538382N", aux);
            aux->mostarDatos();
            app.getUserNIF()->refrescaLambda();
            app.mostrarEstadoTabla();
        } else {
            std::cout << "Se ha encontrado el usuario cuando no debería" << std::endl;
        }

        // Ejercicio 7
        std::cout << std::endl << std::endl << std::endl<< "Ejercicio 7 "<< std::endl ;
        std::cout << "Tamanio de la tabla antes del borrado de Wi: " << app.getUserNIF()->numElementos() << std::endl;
        std::list<Usuario *> usrWa = app.buscarUsrNomb("Wa");
        std::vector<std::string> nifWA;
        std::list<Usuario *>::iterator itWa = usrWa.begin();
        while (itWa != usrWa.end()) {
            app.getUserNIF()->borrar(djb2(itWa.operator*()->getNif()), itWa.operator*()->getNif());
            ++itWa;
        }
        std::cout << "Tamanio de la tabla después del borrado de Wi: " << app.getUserNIF()->numElementos() << std::endl;

        //Ejercicio 8
        std::cout << std::endl << std::endl << std::endl<< "Ejercicio 8 "<< std::endl ;
        itS = app.sites()->begin();
        itU = app.getUsers().begin();
        itS = app.sites()->begin();

        encontrado = false;
        itUsrWi = usrWi.begin();
        cont = 0;
        for (i = 0; i < usrWi.size(); i++) {
            hoy = fDada;

            if (itS == app.sites()->end()) {
                cont = 0;
                itS = app.sites()->begin();
            }
            if (itS->getNumCoches() != 0) {
                if (encontrado == false) {
                    encontrado = true;
                }
                hoy.anadirDias(rand() % 3);
                itUsrWi.operator*()->iniciaTrayecto(cont, (cont + 1) % 50, fDada, hoy);
                cont++;
                itUsrWi++;
                itS++;
            } else {
                cont++;
                itS++;
            }
        }
        itUsrWi = usrWi.begin();
        for (int i = 0; i < 10; i++) {
            Usuario *aux = itUsrWi.operator*();
            Trayecto *tray = aux->getTrayecto(fDada);
            Coche *coch = tray->getInthecar();
            std::cout << "Usuario " << i + 1 << std::endl
                    << "El nombre del usuario es: " << aux->getNombre() << std::endl
                    << "El NIF es: " << aux->getNif() << std::endl
                    << "La clave es: " << aux->getClave() << std::endl
                    << "La direccion es: " << aux->getDireccion() << std::endl;
            std::cout << "La fecha de inicio de trayecto es:" << tray->getFechaInicio().cadena() << std::endl
                    << "La fecha de fin de trayecto es:" << tray->getFechaFinal().cadena() << std::endl
                    << "El Id del punto de recarga de origen es:" << tray->getOrigin()->getID() << std::endl
                    << "El Id del punto de recarga de destino es:" << tray->getDestination()->getID() << std::endl
                    << "El Id del trayecto es:" << tray->getIdTrayecto() << std::endl;
            std::cout << "La matricula del coche es:" << coch->getIdMatricula() << std::endl
                    << "La marca del coche es:" << coch->getMarca() << std::endl
                    << "EL modelo del coche es:" << coch->getModelo() << std::endl
                    << "El nivel de bateria del coche es:" << coch->getNivelBateria() << std::endl;
            itUsrWi++;
        }


        // Ejercicio 9
        std::cout << std::endl << std::endl << std::endl<< "Ejercicio 9 "<< std::endl ;
        std::list<Usuario *> usrWil = app.buscarUsrNomb("Wil");
        auto itUsrWil = usrWil.begin();
        while (itUsrWil != usrWil.end()) {
            itUsrWil.operator*()->aparcaCoche(itUsrWil.operator*()->getRent(),
                                              itUsrWil.operator*()->getTrayecto(fDada)->getDestination(), 4);
            itUsrWil++;
        }
        itUsrWil = usrWil.begin();
        for (int i = 0; i < 10; ++i) {
            std::cout << " | Usaurio " << i + 1 << ": " << itUsrWi.operator*()->getNombre()<< std::endl;
            std::cout << " | Puntos del usuario: " << itUsrWil.operator*()->getPuntos();
            std::cout << " | Origen: " << itUsrWil.operator*()->getTrayecto(fDada)->getOrigin()->getID();
            std::cout << " | Destino: " << itUsrWil.operator*()->getTrayecto(fDada)->getDestination()->getID();
            std::cout << " | Coche alquilado: " << itUsrWil.operator*()->getTrayecto(fDada)->getInthecar()->
                    getIdMatricula();
            std::cout << " | Fecha: " << itUsrWil.operator*()->getTrayecto(fDada)->getFechaFinal();
            std::cout << std::endl;
            ++itUsrWil;
        }
    } catch
    (std::invalid_argument &ia) {
        std::cerr << ia.what() << std::endl;
    } catch
    (std::out_of_range &oor) {
        std::cerr << oor.what() << std::endl;
    }


    return 0;
}
