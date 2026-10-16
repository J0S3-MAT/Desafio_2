#include "udeabook.h"
#include "matrizbits.h"
#include "mensaje.h"

UdeABook::UdeABook()
{
    matrizAmistades = nullptr;
    cantidadUsuarios = 0;

    mensajes = nullptr;
    cantidadMensajes = 0;
    capacidadMensajes = 0;
}

UdeABook::~UdeABook()
{
    destruirMatrizBits(matrizAmistades);
    delete[] mensajes;
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
    unsigned long long memoria = sizeof(UdeABook) + getBytesMatrizAmistades();

    memoria += static_cast<unsigned long long>(capacidadMensajes) * sizeof(Mensaje);

    return memoria;
}

int UdeABook::contarAmigos(int usuario, Metricas& metricas) const
{
    if (matrizAmistades == nullptr || usuario < 0 || usuario >= cantidadUsuarios)
    {
        return 0;
    }

    int cantidad = 0;

    for (int i = 0; i < cantidadUsuarios; i++)
    {
        metricas.sumarIteraciones(1);

        if (sonAmigos(usuario, i))
        {
            cantidad++;
        }
    }

    return cantidad;
}


int UdeABook::obtenerSugerencias(int usuario, int k, int* resultados, Metricas& metricas) const
{
    if (matrizAmistades == nullptr || resultados == nullptr || usuario < 0 || usuario >= cantidadUsuarios || k < 1 || k > 20)
    {
        return 0;
    }

    // Reservar y registrar el arreglo de puntuaciones
    unsigned long long bytesPuntuaciones = static_cast<unsigned long long>(cantidadUsuarios) * sizeof(int);

    int* puntuaciones = new int[cantidadUsuarios]();

    metricas.sumarMemoriaTemporal(bytesPuntuaciones);

    // Registrar parametros por valor y variables locales principales
    unsigned long long bytesLocales = sizeof(usuario) + sizeof(k) + sizeof(resultados) + sizeof(bytesPuntuaciones) + sizeof(puntuaciones) + sizeof(bytesLocales);

    metricas.sumarMemoriaTemporal(bytesLocales);

    // Buscar amigos de amigos y calcular puntuaciones
    metricas.sumarMemoriaTemporal(sizeof(int));

    for (int amigo = 0; amigo < cantidadUsuarios; amigo++)
    {
        metricas.sumarIteraciones(1);

        if (!sonAmigos(usuario, amigo))
        {
            continue;
        }

        metricas.sumarMemoriaTemporal(sizeof(int));

        for (int candidato = 0; candidato < cantidadUsuarios; candidato++)
        {
            metricas.sumarIteraciones(1);

            if (candidato != usuario && !sonAmigos(usuario, candidato) && sonAmigos(amigo, candidato))
            {
                puntuaciones[candidato]++;
            }
        }

        metricas.restarMemoriaTemporal(sizeof(int));
    }

    metricas.restarMemoriaTemporal(sizeof(int));

    // Seleccionar hasta K candidatos con mas amigos comunes
    int cantidad = 0;

    metricas.sumarMemoriaTemporal(sizeof(cantidad));
    metricas.sumarMemoriaTemporal(sizeof(int));

    for (int i = 0; i < k; i++)
    {
        int mejor = -1;

        metricas.sumarMemoriaTemporal(sizeof(mejor));
        metricas.sumarMemoriaTemporal(sizeof(int));

        for (int candidato = 0; candidato < cantidadUsuarios; candidato++)
        {
            metricas.sumarIteraciones(1);

            if (puntuaciones[candidato] > 0 && (mejor == -1 || puntuaciones[candidato] > puntuaciones[mejor]))
            {
                mejor = candidato;
            }
        }

        metricas.restarMemoriaTemporal(sizeof(int));

        if (mejor == -1)
        {
            metricas.restarMemoriaTemporal(sizeof(mejor));
            break;
        }

        resultados[cantidad] = mejor;
        cantidad++;

        puntuaciones[mejor] = -1;

        metricas.restarMemoriaTemporal(sizeof(mejor));
    }

    metricas.restarMemoriaTemporal(sizeof(int));

    // Liberar y descontar la memoria temporal
    delete[] puntuaciones;

    metricas.restarMemoriaTemporal(bytesPuntuaciones);
    metricas.restarMemoriaTemporal(bytesLocales);
    metricas.restarMemoriaTemporal(sizeof(cantidad));

    return cantidad;
}
