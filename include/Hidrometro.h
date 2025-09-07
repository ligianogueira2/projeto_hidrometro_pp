#ifndef HIDROMETRO_H
#define HIDROMETRO_H

class Hidrometro {
public:
    Hidrometro();
    void medir(double vazao_litros_por_minuto, double tempo_minutos);
    double getVolumeTotalLitros();
    double getVolumeTotalM3();
    
private:
    double volume_total_litros;
};

#endif 