#include "Hidrometro.h"

Hidrometro::Hidrometro() : volume_total_litros(0.0) {}

void Hidrometro::medir(double vazao, double tempo_segundos) {
    double vazao_por_segundo = vazao / 60.0;
    
    double volume_passou = vazao_por_segundo * tempo_segundos;
    
    volume_total_litros += volume_passou;
}

double Hidrometro::getVolumeTotalLitros() {
    return volume_total_litros;
}

double Hidrometro::getVolumeTotalM3() {
    return volume_total_litros / 1000.0;
}