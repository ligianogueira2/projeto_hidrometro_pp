#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include <iostream>

class Display {
public:
    Display();
    // NOVO: Define a pasta de saída
    void setPastaSaida(const std::string& pasta); 
    void mostrarMedicao(double volume_m3, double volume_litros, double pressao_kpa);
    
private:
    // NOVO: Armazena o nome da pasta exclusivo para esta instância
    std::string pasta_saida; 
};

#endif