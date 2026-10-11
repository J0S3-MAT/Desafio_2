#include <iostream>
#include "metricas.h"

using namespace std;

int main()
{
    cout << "==========================" << endl;
    cout << "       UdeABook           " << endl;
    cout << "==========================" << endl;

    cout << "Proyecto iniciado correctamente" << endl;

    Metricas prueba;

    cout << "Iteraciones iniciales: "
         << prueba.obtenerIteraciones() << endl;

    prueba.sumarIteraciones(5);
    prueba.sumarIteraciones(3);

    cout << "Iteraciones finales: "
         << prueba.obtenerIteraciones() << endl;
    return 0;
}