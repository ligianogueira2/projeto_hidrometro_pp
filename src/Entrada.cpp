#include "Entrada.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <random>
#include <chrono>

Entrada::Entrada(const std::string& arquivo_config) :
    gerador(std::chrono::high_resolution_clock::now().time_since_epoch().count()),
    pressao(0.0) {
    lerArquivoConfig(arquivo_config);
    if (vazao_eh_randomica) {
        distribuicao = std::uniform_real_distribution<double>(0.0, 100.0);
    }
}

void Entrada::lerArquivoConfig(const std::string& arquivo_config) {
    std::ifstream arquivo(arquivo_config);
    std::string linha;

    if (!arquivo.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo de configuracao." << std::endl;
        return;
    }

    while (std::getline(arquivo, linha)) {
        if (linha.empty() || linha[0] == '#') {
            continue;
        }

        std::istringstream iss(linha);
        std::string chave;
        double valor;

        if (std::getline(iss, chave, ':') && (iss >> valor)) {
            chave.erase(0, chave.find_first_not_of(" \t"));
            chave.erase(chave.find_last_not_of(" \t") + 1);
            
            if (chave == "vazao") {
                if (valor == -1) {
                    vazao_eh_randomica = true;
                } else {
                    vazao_eh_randomica = false;
                    vazao_minuto = valor;
                }
            } else if (chave == "bitola") {
                bitola = valor;
            } else if (chave == "pressao") {
                pressao = valor;
            }
        }
    }
    arquivo.close();
}

double Entrada::getVazaoAtual() {
    if (vazao_eh_randomica) {
        return distribuicao(gerador);
    }
    return vazao_minuto;
}

double Entrada::getBitola() {
    return bitola;
}

double Entrada::getPressao() {
    return pressao;
}

bool Entrada::isRandom() {
    return vazao_eh_randomica;
}