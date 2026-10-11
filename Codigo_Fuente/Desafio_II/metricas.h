#ifndef METRICAS_H
#define METRICAS_H

class Metricas
{
private:
    unsigned long long iteraciones;
    unsigned long long memoriaTemporalBytes;
    unsigned long long memoriaTotalBytes;

public:
    Metricas();

    void sumarIteraciones(unsigned long long cantidad);
    unsigned long long obtenerIteraciones() const;
};

#endif // METRICAS_H
