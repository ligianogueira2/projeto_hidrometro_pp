#include "Controladora.h"
#include <iostream>
#include <windows.h>
#include <chrono>

Controladora::Controladora(const std::string& arquivo_config) : entrada(arquivo_config), ultimo_m3_inteiro(-1) {
}

void Controladora::executarSimulacao(int duracao_segundos) {
    std::cout << "Iniciando simulacao do hidrometro..." << std::endl;
    std::cout << "Vazao inicial: " << entrada.getVazaoAtual() << " L/min" << std::endl;

    for (int i = 0; i < duracao_segundos; ++i) {
        double vazao_minuto = entrada.getVazaoAtual();
        double pressao_atual = entrada.getPressao();
        
        if (vazao_minuto == 0) {
            double vazao_ar = 10.0;
            hidrometro.medir(vazao_ar, 1.0 / 60.0);
            display.mostrarAnomalia("Vazao de agua e' 0. Registrando passagem de ar no cano.");
        } else {
            hidrometro.medir(vazao_minuto, 1.0 / 60.0);
        }

        double current_volume_m3 = hidrometro.getVolumeTotalM3();
        int current_m3_inteiro = static_cast<int>(current_volume_m3);

        // Se o valor inteiro de m3 mudou
        if (current_m3_inteiro != ultimo_m3_inteiro) {
            // Simula a "geracao de imagem"
            std::cout << "\n--------------------------------------------------------------" << std::endl;
            std::cout << "[SIMULANDO GERACAO DE IMAGEM: Leitura do Hidrometro - " 
                      << std::fixed << std::setprecision(2) << current_volume_m3 << " m^3]" << std::endl;
            std::cout << "--------------------------------------------------------------\n" << std::endl;
            ultimo_m3_inteiro = current_m3_inteiro; // Atualiza o ultimo valor
        }
        
        display.mostrarMedicao(current_volume_m3, hidrometro.getVolumeTotalLitros(), pressao_atual);

        Sleep(1000); 
    }

    std::cout << "Simulacao finalizada." << std::endl;
}