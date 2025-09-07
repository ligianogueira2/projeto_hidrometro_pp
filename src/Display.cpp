#include "Display.h"
#include <iomanip>
#include <cmath>

void Display::mostrarMedicao(double volume_m3, double volume_litros, double pressao) {
    int metros_cubicos_inteiro = static_cast<int>(volume_m3);
    
    double decimais = (volume_m3 - metros_cubicos_inteiro);
    int centenas_e_dezenas_litros = static_cast<int>(std::round(decimais * 100.0));

    if (centenas_e_dezenas_litros == 100) {
        metros_cubicos_inteiro++;
        centenas_e_dezenas_litros = 0;
    }
    
    std::cout << "Display: ";
    
    std::cout << "\x1b[0m"; // Resetar cor para preto/branco (terminal padrao)
    std::cout << std::setw(4) << std::setfill('0') << metros_cubicos_inteiro << ".";

    std::cout << "\x1b[31m"; // Definir cor para vermelho
    std::cout << std::setw(2) << std::setfill('0') << centenas_e_dezenas_litros;
    std::cout << "\x1b[0m"; 

    std::cout << " m^3 | Pressao: " << pressao << " kPa" << std::endl;
}

void Display::mostrarAnomalia(const std::string& mensagem) {
    std::cout << "Anomalia: " << mensagem << std::endl;
}