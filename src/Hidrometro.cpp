#include "Hidrometro.h"

Hidrometro::Hidrometro() {
    volume_total_litros = 0.0;
}

void Hidrometro::medir(double vazao_litros_por_minuto, double tempo_minutos) {
    double volume_adicionado = vazao_litros_por_minuto * tempo_minutos;
    volume_total_litros += volume_adicionado;
}

double Hidrometro::getVolumeTotalLitros() {
    return volume_total_litros;
}

double Hidrometro::getVolumeTotalM3() {
    return volume_total_litros / 1000.0;
}