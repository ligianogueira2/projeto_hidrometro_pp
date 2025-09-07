#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>

class Display {
public:
    void mostrarMedicao(double volume_m3, double volume_litros, double pressao);
    void mostrarAnomalia(const std::string& mensagem);
};

#endif