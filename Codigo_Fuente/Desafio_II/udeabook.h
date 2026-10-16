#ifndef UDEABOOK_H
#define UDEABOOK_H
class Mensaje;
#include "metricas.h"

class UdeABook
{
private:
    unsigned char* matrizAmistades;
    int cantidadUsuarios;
    Mensaje* mensajes;
    int cantidadMensajes;
    int capacidadMensajes;

public:
    //______Temporar________
    UdeABook(const UdeABook& otro) = delete;
    UdeABook& operator=(const UdeABook& otro) = delete;

    UdeABook();
    ~UdeABook();

    bool inicializarMatrizAmistades(int cantidad);

    bool sonAmigos(int usuario1, int usuario2) const;

    bool establecerAmistad(int usuario1, int usuario2);

    int contarAmigosComunes(int usuario1, int usuario2, Metricas& metricas) const;

    unsigned long long getBytesMatrizAmistades() const;

    unsigned long long getMemoriaSistema() const;

    int contarAmigos(int usuario, Metricas& metricas) const;

    int obtenerSugerencias(int usuario, int k, int* resultados, Metricas& metricas) const;
};

#endif // UDEABOOK_H