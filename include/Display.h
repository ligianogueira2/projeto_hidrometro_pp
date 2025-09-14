#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include <iostream>

class Display {
public:
    Display();
    void mostrarMedicao(double volume_m3, double volume_litros, double pressao_kpa);
};

#endif