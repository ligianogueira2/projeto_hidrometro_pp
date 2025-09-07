#include <iostream>
#include "Controladora.h"

int main() {
    Controladora simulador("config/parametros.txt");

    simulador.executarSimulacao(10);
    
    return 0;
}