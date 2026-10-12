#include "metricas.h"
#include <iostream>

using namespace std;

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

unsigned long long Metricas::getIteraciones() const
{
    return iteraciones;
}

void Metricas::reiniciar()
{
    iteraciones = 0;
    memoriaTemporalBytes = 0;
    memoriaTotalBytes = 0;
}

void Metricas::sumarMemoriaTemporal(unsigned long long bytes)
{
    memoriaTemporalBytes += bytes;
}

unsigned long long Metricas::getMemoriaTemporal() const
{
    return memoriaTemporalBytes;
}

void Metricas::restarMemoriaTemporal(unsigned long long bytes)
{
    if (bytes <= memoriaTemporalBytes)
    {
        memoriaTemporalBytes -= bytes;
    }
}

void Metricas::registrarMemoriaFinal(unsigned long long memoriaSistema)
{
    memoriaTotalBytes = memoriaSistema + memoriaTemporalBytes;
}

unsigned long long Metricas::getMemoriaTotal() const
{
    return memoriaTotalBytes;
}

void Metricas::mostrarResultados() const
{
    cout << "------METRICAS DE RECURSOS------" << endl;
    cout << "Iteraciones: " << iteraciones << endl;
    cout << "Memoria total: " << memoriaTotalBytes << " bytes" << endl;
}