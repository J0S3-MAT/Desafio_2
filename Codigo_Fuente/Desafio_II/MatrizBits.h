#ifndef MATRIZBITS_H
#define MATRIZBITS_H

int calcularIndice(int fila, int columna, int columnas);

unsigned char* crearMatrizBits(int filas, int columnas);

void destruirMatrizBits(unsigned char*& matriz);

void encenderBit(unsigned char* matriz, int fila, int columna, int columnas);

void apagarBit(unsigned char* matriz, int fila, int columna, int columnas);

bool leerBit(const unsigned char* matriz, int fila, int columna, int columnas);

void limpiarMatrizBits(unsigned char* matriz, int filas, int columnas);


// ---------- Matriz triangular (para relaciones simetricas, como la amistad) ----------

int calcularIndiceTriangular(int fila, int columna);

int calcularBytesTriangular(int n);

unsigned char* crearMatrizTriangular(int n);

void encenderBitTriangular(unsigned char* matriz, int fila, int columna);

bool leerBitTriangular(const unsigned char* matriz, int fila, int columna);

#endif // MATRIZBITS_H