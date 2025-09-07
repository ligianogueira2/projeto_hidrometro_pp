#ifndef CONTROLADORA_H
#define CONTROLADORA_H

#include "Entrada.h"
#include "Hidrometro.h"
#include "Display.h" 
#include <string>
#include <iomanip> 

class Controladora {
public:
    Controladora(const std::string& arquivo_config);
    void executarSimulacao();
    
private:
    Entrada entrada;
    Hidrometro hidrometro;
    Display display; 
    int ultimo_m3_inteiro;
};

#endif