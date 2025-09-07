#ifndef ENTRADA_H
#define ENTRADA_H

#include <string>
#include <random>

class Entrada {
public:
    Entrada(const std::string& arquivo_config);

    double getVazaoAtual();
    double getBitola();
    double getPressao();
    bool isRandom();
    
private:
    void lerArquivoConfig(const std::string& arquivo_config);
    double gerarVazaoAleatoria(); 

    double vazao_minuto;
    double vazao_maxima;
    double bitola;
    double pressao;
    bool vazao_eh_randomica;

    std::mt19937 gerador;
    std::uniform_real_distribution<double> distribuicao;
};

#endif