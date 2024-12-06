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


int uno=2;
    uno*=3;
    std::cout <<uno;
    return 0;
}
