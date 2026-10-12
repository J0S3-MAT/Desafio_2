#include "udeabook.h"
#include "matrizbits.h"

UdeABook::UdeABook()
{
    matrizAmistades = nullptr;
    cantidadUsuarios = 0;
}

UdeABook::~UdeABook()
{
    destruirMatrizBits(matrizAmistades);
}

bool UdeABook::inicializarMatrizAmistades(int cantidad)
{
    if (cantidad <= 0)
    {
        return false;
    }

    unsigned char* nuevaMatriz = crearMatrizBits(cantidad, cantidad);

    if (nuevaMatriz == nullptr)
    {
        return false;
    }

    destruirMatrizBits(matrizAmistades);

    matrizAmistades = nuevaMatriz;
    cantidadUsuarios = cantidad;

    return true;
}

bool UdeABook::sonAmigos(int usuario1, int usuario2) const
{
    if (matrizAmistades == nullptr)
    {
        return false;
    }

    if (usuario1 < 0 || usuario1 >= cantidadUsuarios || usuario2 < 0 || usuario2 >= cantidadUsuarios)
    {
        return false;
    }

    if (usuario1 == usuario2)
    {
        return false;
    }

    return leerBit(matrizAmistades, usuario1, usuario2, cantidadUsuarios);
}

bool UdeABook::establecerAmistad(int usuario1, int usuario2)
{
    if (matrizAmistades == nullptr)
    {
        return false;
    }

    if (usuario1 < 0 || usuario1 >= cantidadUsuarios || usuario2 < 0 || usuario2 >= cantidadUsuarios)
    {
        return false;
    }

    if (usuario1 == usuario2)
    {
        return false;
    }

    if (sonAmigos(usuario1, usuario2))
    {
        return false;
    }

    encenderBit(matrizAmistades, usuario1, usuario2, cantidadUsuarios);
    encenderBit(matrizAmistades, usuario2, usuario1, cantidadUsuarios);

    return true;
}

int UdeABook::contarAmigosComunes(int usuario1, int usuario2, Metricas& metricas) const
{
    if (matrizAmistades == nullptr)
    {
        return 0;
    }

    if (usuario1 < 0 || usuario1 >= cantidadUsuarios || usuario2 < 0 || usuario2 >= cantidadUsuarios)
    {
        return 0;
    }

    if (usuario1 == usuario2)
    {
        return 0;
    }

    int comunes = 0;

    for (int i = 0; i < cantidadUsuarios; i++)
    {
        metricas.sumarIteraciones(1);

        if (sonAmigos(usuario1, i) && sonAmigos(usuario2, i))
        {
            comunes++;
        }
    }

    return comunes;
}

unsigned long long UdeABook::getBytesMatrizAmistades() const
{
    if (matrizAmistades == nullptr)
    {
        return 0;
    }

    unsigned long long totalBits = static_cast<unsigned long long>(cantidadUsuarios) * cantidadUsuarios;

    unsigned long long bytes = totalBits / 8;

    if (totalBits % 8 != 0)
    {
        bytes++;
    }

    return bytes;
}

unsigned long long UdeABook::getMemoriaSistema() const
{
    return sizeof(UdeABook) + getBytesMatrizAmistades();
}