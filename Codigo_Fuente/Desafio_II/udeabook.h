#ifndef UDEABOOK_H
#define UDEABOOK_H
#include "metricas.h"

class UdeABook
{
private:
    unsigned char* matrizAmistades;
    int cantidadUsuarios;

public:
    UdeABook();
    ~UdeABook();

    bool inicializarMatrizAmistades(int cantidad);

    bool sonAmigos(int usuario1, int usuario2) const;

    bool establecerAmistad(int usuario1, int usuario2);

    int contarAmigosComunes(int usuario1, int usuario2, Metricas& metricas) const;

    unsigned long long getBytesMatrizAmistades() const;

    unsigned long long getMemoriaSistema() const;
};

#endif // UDEABOOK_H