#include "MatrizBits.h"
#include <climits>

int calcularIndice(int fila, int columna, int columnas)
{
    return fila * columnas + columna;
}

unsigned char* crearMatrizBits(int filas, int columnas)
{
    if (filas <= 0 || columnas <= 0 || filas > INT_MAX / columnas)
    {
        return nullptr;
    }

    int totalBits = filas * columnas;
    int bytes = totalBits / 8;

    if (totalBits % 8 != 0)
    {
        bytes++;
    }

    return new unsigned char[bytes]();
}

void destruirMatrizBits(unsigned char*& matriz)
{
    delete[] matriz;
    matriz = nullptr;
}

void encenderBit(unsigned char* matriz, int fila, int columna, int columnas)
{
    int indice = calcularIndice(fila, columna, columnas);

    int byte = indice / 8;
    int bit = indice % 8;

    matriz[byte] |= (1 << bit);
}

bool leerBit(const unsigned char* matriz, int fila, int columna, int columnas)
{
    int indice = calcularIndice(fila, columna, columnas);

    int byte = indice / 8;
    int bit = indice % 8;

    return (matriz[byte] & (1 << bit)) != 0;
}

void limpiarMatrizBits(unsigned char* matriz, int filas, int columnas)
{
    int totalBits = filas * columnas;
    int bytes = totalBits / 8;

    if (totalBits % 8 != 0)
    {
        bytes++;
    }

    for (int i = 0; i < bytes; i++)
    {
        matriz[i] = 0;
    }
}