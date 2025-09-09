#include "Controladora.h"
#include <iostream>
#include <windows.h>
#include <chrono>
#include <iomanip>
#include <csignal> 
#include <atomic>  

static std::atomic<bool> g_executar_simulacao{true};

void sinal_handler(int sinal) {
    if (sinal == SIGINT) {
        std::cout << "\nSinal de interrupcao (Ctrl + C) recebido. Finalizando..." << std::endl;
        g_executar_simulacao = false;
    }
}

Controladora::Controladora(const std::string& arquivo_config) : entrada(arquivo_config), ultimo_m3_inteiro(-1) {
}

void Controladora::executarSimulacao() {
    std::cout << "Iniciando simulacao ininterrupta do hidrometro..." << std::endl;
    std::cout << "Pressione Ctrl + C para finalizar." << std::endl;

    // Registra o tratador de sinal para capturar o Ctrl + C
    signal(SIGINT, sinal_handler);

    while (g_executar_simulacao) {
        double vazao_minuto = entrada.getVazaoAtual();
        double pressao_atual = entrada.getPressao();
        
        hidrometro.medir(vazao_minuto, 1.0 / 60.0);
        
        double current_volume_m3 = hidrometro.getVolumeTotalM3();
        int current_m3_inteiro = static_cast<int>(current_volume_m3);

        if (current_m3_inteiro != ultimo_m3_inteiro) {
            std::cout << "\n--------------------------------------------------------------" << std::endl;
            std::cout << "[SIMULANDO GERACAO DE IMAGEM: Leitura do Hidrometro - "
                      << std::fixed << std::setprecision(2) << current_volume_m3 << " m^3]" << std::endl;
            std::cout << "--------------------------------------------------------------\n" << std::endl;
            
            display.mostrarMedicao(current_volume_m3, hidrometro.getVolumeTotalLitros(), pressao_atual);
            
            ultimo_m3_inteiro = current_m3_inteiro;
        }

        Sleep(1000);
    }
    
    std::cout << "\nSimulacao finalizada." << std::endl;
    std::cout << "--------------------------------------------------------------" << std::endl;
    std::cout << "Leitura Final do Hidrometro: " << std::fixed << std::setprecision(2) 
              << hidrometro.getVolumeTotalM3() << " m^3" << std::endl;
    std::cout << "--------------------------------------------------------------" << std::endl;
}