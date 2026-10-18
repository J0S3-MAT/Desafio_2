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

// Poner en 0 un bit: la mascara ~(1 << bit) tiene todos los bits en 1 excepto el que se apaga
void apagarBit(unsigned char* matriz, int fila, int columna, int columnas)
{
    int indice = calcularIndice(fila, columna, columnas);

    int byte = indice / 8;
    int bit = indice % 8;

    matriz[byte] &= ~(1 << bit);
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



int calcularIndiceTriangular(int fila, int columna)
{
    if (fila < columna)
    {
        int temporal = fila;
        fila = columna;
        columna = temporal;
    }

    return fila * (fila - 1) / 2 + columna;
}

int calcularBytesTriangular(int n)
{
    if (n <= 1)
    {
        return 1;
    }

    int totalBits = n * (n - 1) / 2;
    int bytes = totalBits / 8;

    if (totalBits % 8 != 0)
    {
        bytes++;
    }

    return bytes;
}

unsigned char* crearMatrizTriangular(int n)
{
    // Mismo chequeo que crearMatrizBits: n * n debe caber en un int
    if (n <= 0 || n > INT_MAX / n)
    {
        return nullptr;
    }

    return new unsigned char[calcularBytesTriangular(n)]();
}

void encenderBitTriangular(unsigned char* matriz, int fila, int columna)
{
    int indice = calcularIndiceTriangular(fila, columna);

    int byte = indice / 8;
    int bit = indice % 8;

    matriz[byte] |= (1 << bit);
}

bool leerBitTriangular(const unsigned char* matriz, int fila, int columna)
{
    int indice = calcularIndiceTriangular(fila, columna);

    int byte = indice / 8;
    int bit = indice % 8;

    return (matriz[byte] & (1 << bit)) != 0;
}