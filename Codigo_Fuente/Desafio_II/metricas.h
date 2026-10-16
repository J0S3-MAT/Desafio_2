#ifndef METRICAS_H
#define METRICAS_H

class Metricas
{
private:
    unsigned long long iteraciones;
    unsigned long long memoriaTemporalBytes;
    unsigned long long memoriaTotalBytes;
    unsigned long long picoMemoriaTemporalBytes;

public:
    Metricas();

    void reiniciar();

    void sumarIteraciones(unsigned long long cantidad);
    unsigned long long getIteraciones() const;

    void sumarMemoriaTemporal(unsigned long long bytes);
    unsigned long long getMemoriaTemporal() const;

    void restarMemoriaTemporal(unsigned long long bytes);

    void registrarMemoriaFinal(unsigned long long memoriaSistema);
    unsigned long long getMemoriaTotal() const;

    void mostrarResultados() const;
    unsigned long long getPicoMemoriaTemporal() const;
};

#endif