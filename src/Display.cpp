#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Display.h"
#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include "stb_image.h"
#include "stb_image_write.h"

// Funções de desenho (mantidas do seu código original)
void desenhar_retangulo(unsigned char* dados, int largura, int altura, int x1, int y1, int x2, int y2, const unsigned char cor[]) {
    for (int y = y1; y <= y2; ++y) {
        for (int x = x1; x <= x2; ++x) {
            if (x >= 0 && x < largura && y >= 0 && y < altura) {
                int idx = (y * largura + x) * 3;
                dados[idx] = cor[0];
                dados[idx + 1] = cor[1];
                dados[idx + 2] = cor[2];
            }
        }
    }
}

void desenhar_digito(unsigned char* dados, int largura, int altura, int x, int y, char c, const unsigned char cor[]) {
    int seg_largura = 5;  
    int seg_altura = 25;  
    int seg_horiz = 15;  
    std::vector<bool> s(7,false);
    
    // ... Lógica dos dígitos 7-segmentos ... 
    if (c == '0') s = {1,1,1,1,1,1,0};
    else if (c == '1') s = {0,1,1,0,0,0,0};
    else if (c == '2') s = {1,1,0,1,1,0,1};
    else if (c == '3') s = {1,1,1,1,0,0,1};
    else if (c == '4') s = {0,1,1,0,0,1,1};
    else if (c == '5') s = {1,0,1,1,0,1,1};
    else if (c == '6') s = {1,0,1,1,1,1,1};
    else if (c == '7') s = {1,1,1,0,0,0,0};
    else if (c == '8') s = {1,1,1,1,1,1,1};
    else if (c == '9') s = {1,1,1,1,0,1,1};
    else if (c == '.') {
        desenhar_retangulo(dados, largura, altura, x + seg_horiz / 2, y + seg_altura * 2 + 4, x + seg_horiz / 2 + seg_largura, y + seg_altura * 2 + 4 + seg_largura, cor);
        return;
    }
    
    if (s[0]) desenhar_retangulo(dados, largura, altura, x + seg_largura, y, x + seg_largura + seg_horiz, y + seg_largura, cor);
    if (s[1]) desenhar_retangulo(dados, largura, altura, x + seg_largura + seg_horiz, y + seg_largura, x + seg_largura + seg_horiz + seg_largura, y + seg_largura + seg_altura, cor);
    if (s[2]) desenhar_retangulo(dados, largura, altura, x + seg_largura + seg_horiz, y + 2*seg_largura + seg_altura, x + seg_largura + seg_horiz + seg_largura, y + 2*seg_largura + 2*seg_altura, cor);
    if (s[3]) desenhar_retangulo(dados, largura, altura, x + seg_largura, y + 2*seg_largura + 2*seg_altura, x + seg_largura + seg_horiz, y + 2*seg_largura + 2*seg_altura + seg_largura, cor);
    if (s[4]) desenhar_retangulo(dados, largura, altura, x, y + 2*seg_largura + seg_altura, x + seg_largura, y + 2*seg_largura + 2*seg_altura, cor);
    if (s[5]) desenhar_retangulo(dados, largura, altura, x, y + seg_largura, x + seg_largura, y + seg_largura + seg_altura, cor);
    if (s[6]) desenhar_retangulo(dados, largura, altura, x + seg_largura, y + seg_largura + seg_altura, x + seg_largura + seg_horiz, y + seg_largura + seg_altura + seg_largura, cor);
}

Display::Display() {}

// NOVO: Implementação para definir a pasta
void Display::setPastaSaida(const std::string& pasta) {
    pasta_saida = pasta;
}

void Display::mostrarMedicao(double volume_m3, double volume_litros, double pressao_kpa) {
    int largura, altura, canais;
    unsigned char* img = stbi_load("images/hidrometro.png", &largura, &altura, &canais, 3);
    
    if (!img) {
        std::cerr << "Erro ao carregar images/hidrometro.png\n";
        return;
    }
    
    int valor = static_cast<int>(volume_m3 * 100);
    char texto[20];
    sprintf(texto, "%06d", valor);
    
    int x = 320;        
    int y = 330;        
    int espacamento = 65;
    
    unsigned char preto[3] = {0,0,0};
    unsigned char vermelho[3] = {255,0,0};
    
    int len = strlen(texto);
    for (int i = 0; i < len; ++i) {
        const unsigned char* cor = preto;
        int dx = 0;
        if (i >= len - 2) {
            cor = vermelho;
            dx = 8;
        }
        desenhar_digito(img, largura, altura, x + i * espacamento + dx, y, texto[i], cor); 
    }
    
    char nome[100];
    // ALTERADO: Usa a pasta_saida definida dinamicamente
    sprintf(nome, "%s/hidrometrofinal_%.2f.png", pasta_saida.c_str(), volume_m3);
    
    if (stbi_write_png(nome, largura, altura, 3, img, largura * 3)) {
        std::cout << ">>> Imagem PNG gerada em " << nome << " <<<\n";
    } else {
        std::cerr << "Erro ao salvar PNG\n";
    }
    
    stbi_image_free(img);
}