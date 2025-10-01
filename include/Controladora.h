#ifndef CONTROLADORA_H
#define CONTROLADORA_H

#include "Entrada.h"
#include "Hidrometro.h"
#include "Display.h"
#include <string>
#include <atomic> // Necessário para std::atomic
#include <iomanip>

class Controladora {
public:
    // NOVO CONSTRUTOR
    Controladora(int id_simulador, const std::string& arquivo_config);
    
    // NOVO EXECUTAR SIMULACAO (para uso com threads)
    void executarSimulacao(std::atomic<bool>& flag_execucao_global);
    
private:
    Entrada entrada;
    Hidrometro hidrometro;
    Display display;
    int ultimo_m3_inteiro;
    int id_simulador; 

    // Adicionado para geração de imagem baseada em tempo
    int contador_iteracoes; 
    const int INTERVALO_IMAGEM_SEGUNDOS = 5; // 5 minutos = 300 segundos
};

#endif