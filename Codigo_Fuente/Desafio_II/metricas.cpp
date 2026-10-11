#include "metricas.h"

Metricas::Metricas()
{
    iteraciones = 0;
    memoriaTemporalBytes = 0;
    memoriaTotalBytes = 0;
}

void Metricas::sumarIteraciones(unsigned long long cantidad)
{
    iteraciones += cantidad;
}

unsigned long long Metricas::obtenerIteraciones() const
{
    return iteraciones;
}