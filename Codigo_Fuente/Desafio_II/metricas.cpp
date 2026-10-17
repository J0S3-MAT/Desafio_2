#include "metricas.h"
#include <iostream>

using namespace std;

Metricas::Metricas()
{
    iteraciones = 0;
    memoriaTemporalBytes = 0;
    memoriaTotalBytes = 0;

    for (int i = 0; i < CANTIDAD_EXTERNOS; i++)
    {
        usosExternos[i] = 0;
    }
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

    for (int i = 0; i < CANTIDAD_EXTERNOS; i++)
    {
        usosExternos[i] = 0;
    }
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
    // Memoria en el momento exacto en que termina la funcionalidad:
    // estructuras permanentes del sistema + variables locales y parametros por valor aun activos
    memoriaTotalBytes = memoriaSistema + memoriaTemporalBytes;
}

unsigned long long Metricas::getMemoriaTotal() const
{
    return memoriaTotalBytes;
}

void Metricas::registrarExterno(int tipo, unsigned long long cantidad)
{
    if (tipo >= 0 && tipo < CANTIDAD_EXTERNOS)
    {
        usosExternos[tipo] += cantidad;
    }
}

unsigned long long Metricas::getUsosExterno(int tipo) const
{
    if (tipo < 0 || tipo >= CANTIDAD_EXTERNOS)
    {
        return 0;
    }

    return usosExternos[tipo];
}

void Metricas::mostrarResultados() const
{
    const char* nombres[CANTIDAD_EXTERNOS] = {"generador aleatorio <random>", "reloj del sistema <chrono>", "archivos abiertos <fstream>", "operaciones de archivo <filesystem>"};

    cout << "------METRICAS DE RECURSOS------" << endl;
    cout << "Iteraciones: " << iteraciones << endl;
    cout << "Memoria total: " << memoriaTotalBytes << " bytes" << endl;
    cout << "Componentes externos: ";

    bool alguno = false;

    for (int i = 0; i < CANTIDAD_EXTERNOS; i++)
    {
        if (usosExternos[i] > 0)
        {
            cout << (alguno ? ", " : "") << nombres[i] << " x" << usosExternos[i];
            alguno = true;
        }
    }

    cout << (alguno ? "" : "ninguno") << endl;
}
