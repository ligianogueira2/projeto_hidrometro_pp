#include "Controladora.h"
#include <iostream>
#include <windows.h> // Mantenha se o seu ambiente for Windows
#include <chrono>
#include <iomanip>
#include <csignal>
#include <atomic> 
#include <sstream> 
#include <filesystem> // Necessário para criar/remover o diretório (C++17+)

namespace fs = std::filesystem;

// Nome base da pasta conforme solicitado
const std::string BASE_FOLDER_NAME = "Medicoes_202311250003";

Controladora::Controladora(int id_simulador, const std::string& arquivo_config)
    : entrada(arquivo_config), ultimo_m3_inteiro(-1), id_simulador(id_simulador),
      contador_iteracoes(0) 
{
    std::stringstream ss_pasta;
    // Cria o nome da pasta no formato "Medicoes_202311250003_N"
    ss_pasta << BASE_FOLDER_NAME << "_" << id_simulador;
    std::string nome_pasta = ss_pasta.str();

    // 1. Tenta apagar a pasta e todo o seu conteúdo (Limpeza)
    try {
        if (fs::exists(nome_pasta)) {
            size_t count = fs::remove_all(nome_pasta); 
            std::cout << "[Simulador " << id_simulador << "] Limpando: Removidos " << count 
                      << " itens do diretorio anterior: " << nome_pasta << std::endl;
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Erro ao tentar apagar o diretorio para o Simulador " << id_simulador << ": " << e.what() << std::endl;
    }

    // 2. Cria o diretório (Recria a pasta vazia)
    try {
        if (fs::create_directories(nome_pasta)) {
            std::cout << "[Simulador " << id_simulador << "] Diretorio de saida criado: " << nome_pasta << std::endl;
        } else {
            std::cout << "[Simulador " << id_simulador << "] Diretorio de saida pronto: " << nome_pasta << std::endl;
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Erro ao criar diretorio para o Simulador " << id_simulador << ": " << e.what() << std::endl;
    }
    
    // Passa o nome da pasta exclusiva para o Display desta Controladora
    display.setPastaSaida(nome_pasta);
}


void Controladora::executarSimulacao(std::atomic<bool>& flag_execucao_global) {
    std::cout << "[Simulador " << id_simulador << "] Iniciando simulacao ininterrupta do hidrometro..." << std::endl;
    
    double current_volume_m3 = 0.0;
    double last_litros = 0.0;
    double last_pressao = 0.0;
    
    // --- NOVO: LÓGICA DE GERAÇÃO DA IMAGEM INICIAL ---
    
    // 1. Executa a primeira medição antes de entrar no loop de Sleep(1000)
    double vazao_minuto = entrada.getVazaoAtual();
    last_pressao = entrada.getPressao();
    
    // O hidrometro mede o volume que passou em 1/60 de minuto (1 segundo)
    hidrometro.medir(vazao_minuto, 1.0 / 60.0); 
    
    current_volume_m3 = hidrometro.getVolumeTotalM3();
    last_litros = hidrometro.getVolumeTotalLitros();
    
    int current_m3_inteiro = static_cast<int>(current_volume_m3);

    std::cout << "\n--------------------------------------------------------------" << std::endl;
    std::cout << "[Simulador " << id_simulador << " - GERANDO IMAGEM INICIAL: Leitura - "
              << std::fixed << std::setprecision(2) << current_volume_m3 << " m^3]" << std::endl;
    std::cout << "--------------------------------------------------------------\n" << std::endl;
    
    display.mostrarMedicao(current_volume_m3, last_litros, last_pressao);
    
    // 2. Atualiza o rastreador de volume e zera o contador.
    // Isso garante que a próxima imagem só será gerada ao atingir o próximo m3 ou o tempo limite.
    ultimo_m3_inteiro = current_m3_inteiro;
    contador_iteracoes = 0; 
    
    // --- FIM DA LÓGICA DE IMAGEM INICIAL ---
    
    // O loop começa a partir do segundo 1, com o volume e o estado já atualizados
    while (flag_execucao_global.load()) {
        
        // Aguarda 1 segundo
        Sleep(1000); 

        if (!flag_execucao_global.load()) {
            break; 
        }
        
        // Medição e Cálculo ocorrem após a espera
        vazao_minuto = entrada.getVazaoAtual();
        last_pressao = entrada.getPressao();
        
        // O hidrometro mede o volume que passou em 1/60 de minuto (1 segundo)
        hidrometro.medir(vazao_minuto, 1.0 / 60.0); 
        
        current_volume_m3 = hidrometro.getVolumeTotalM3();
        last_litros = hidrometro.getVolumeTotalLitros();
        
        current_m3_inteiro = static_cast<int>(current_volume_m3);
        
        // Contador de iterações para geração de imagem por tempo
        contador_iteracoes++; 

        // CONDIÇÕES PARA GERAR IMAGEM: A cada 1m³ OU a cada 5 minutos
        bool condicao_volume = (current_m3_inteiro != ultimo_m3_inteiro);
        bool condicao_tempo = (contador_iteracoes >= INTERVALO_IMAGEM_SEGUNDOS);

        if (condicao_volume || condicao_tempo) {
            std::cout << "\n--------------------------------------------------------------" << std::endl;
            
            // Mensagem de log
            if (condicao_volume) {
                 std::cout << "[Simulador " << id_simulador << " - GERANDO IMAGEM: Leitura - "
                           << std::fixed << std::setprecision(2) << current_volume_m3 << " m^3] (Por Volume)" << std::endl;
            } else {
                 std::cout << "[Simulador " << id_simulador << " - GERANDO IMAGEM: Leitura - "
                           << std::fixed << std::setprecision(2) << current_volume_m3 << " m^3] (Por Tempo)" << std::endl;
            }
            std::cout << "--------------------------------------------------------------\n" << std::endl;
            
            display.mostrarMedicao(current_volume_m3, last_litros, last_pressao);
            
            // ATUALIZAÇÃO DO ESTADO CORRIGIDA:
            // O valor de m3 (inteiro) deve ser atualizado sempre que a imagem é gerada,
            // independentemente da condição (volume ou tempo) que a disparou.
            ultimo_m3_inteiro = current_m3_inteiro;
            
            // O contador de tempo SEMPRE ZERA se a imagem foi gerada por qualquer condição
            contador_iteracoes = 0; 
        }
    }
    
    // Lógica de finalização
    std::cout << "\n[Simulador " << id_simulador << "] Gerando imagem final com a leitura real..." << std::endl;
    display.mostrarMedicao(current_volume_m3, last_litros, last_pressao);

    std::cout << "\n[Simulador " << id_simulador << "] Simulacao finalizada." << std::endl;
    std::cout << "--------------------------------------------------------------" << std::endl;
    std::cout << "[Simulador " << id_simulador << "] Leitura Final: "
              << std::fixed << std::setprecision(2)
              << current_volume_m3 << " m^3" << std::endl;
    std::cout << "--------------------------------------------------------------" << std::endl;
}