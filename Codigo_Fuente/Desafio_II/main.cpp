#include <iostream>
#include "udeabook.h"

using namespace std;

int main()
{

    cout << "------UdeABook------" << endl;

    UdeABook red;
    Metricas metricas;

    red.inicializarMatrizAmistades(5);

    red.establecerAmistad(0, 1);
    red.establecerAmistad(0, 2);
    red.establecerAmistad(3, 1);
    red.establecerAmistad(3, 2);

    int comunes = red.contarAmigosComunes(0, 3, metricas);

    unsigned long long memoriaSistema = red.getMemoriaSistema();

    metricas.registrarMemoriaFinal(memoriaSistema);

    cout << "Amigos comunes: " << comunes << endl;
    cout << "Memoria del objeto: " << sizeof(red) << " bytes" << endl;
    cout << "Memoria de la matriz: " << red.getBytesMatrizAmistades() << " bytes" << endl;

    metricas.mostrarResultados();
}