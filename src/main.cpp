#include <iostream>
#include <thread> 
#include <vector> 
#include <string> 
#include <atomic> 
#include <csignal> 
#include <functional> 
#include "Controladora.h"

// Flag de execução global atômica para sincronizar a parada de todas as threads
static std::atomic<bool> g_executar_simulacao{true};

// Handler para o sinal de interrupção (Ctrl + C)
void sinal_handler(int sinal) {
    if (sinal == SIGINT) {
        std::cout << "\nSinal de interrupcao (Ctrl + C) recebido. Finalizando todas as simulacoes..." << std::endl;
        g_executar_simulacao = false; // Define a flag para parar todas as threads
    }
}

// Número de simuladores que você deseja executar
const int NUM_SIMULADORES = 5; 

int main() {
    // Registra o handler de sinal uma única vez
    signal(SIGINT, sinal_handler);

    std::vector<std::thread> threads_simulacao;
    std::vector<Controladora*> simuladores; // Para gerenciar a vida útil dos objetos

    std::cout << "Iniciando " << NUM_SIMULADORES << " simuladores em paralelo." << std::endl;
    std::cout << "Pressione Ctrl + C para finalizar todas as simulacoes." << std::endl;

    for (int i = 1; i <= NUM_SIMULADORES; ++i) {
        // Define o caminho do arquivo de configuração individual
        std::string path_config = "config/parametros_" + std::to_string(i) + ".txt";

        // Cria a Controladora (o objeto simulador)
        Controladora* c = new Controladora(i, path_config); 
        simuladores.push_back(c);

        // Cria a thread, passando o método de execução e a flag global por referência
        threads_simulacao.emplace_back(&Controladora::executarSimulacao, c, std::ref(g_executar_simulacao));
    }

    // Espera que todas as threads terminem (join)
    for (auto& t : threads_simulacao) {
        if (t.joinable()) {
            t.join();
        }
    }

    // Limpa a memória
    for (Controladora* c : simuladores) {
        delete c;
    }

    std::cout << "\nTODAS AS SIMULACOES FORAM FINALIZADAS COM SUCESSO." << std::endl;
    
    return 0;
}