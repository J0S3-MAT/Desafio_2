#ifndef METRICAS_H
#define METRICAS_H

class Metricas
{
public:
    // Componentes externos (librerias del lenguaje) cuyo uso se reporta
    static const int EXTERNO_ALEATORIO = 0;         // <random>: generador de numeros aleatorios
    static const int EXTERNO_RELOJ = 1;             // <chrono>: consulta de la fecha actual
    static const int EXTERNO_ARCHIVO = 2;           // <fstream>: apertura de archivos
    static const int EXTERNO_SISTEMA_ARCHIVOS = 3;  // <filesystem>: renombrar o borrar archivos
    static const int CANTIDAD_EXTERNOS = 4;

private:
    unsigned long long iteraciones;
    unsigned long long memoriaTemporalBytes;
    unsigned long long memoriaTotalBytes;
    unsigned long long usosExternos[CANTIDAD_EXTERNOS];

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

    // Registrar que la funcionalidad uso un componente externo (tipo = EXTERNO_...)
    void registrarExterno(int tipo, unsigned long long cantidad = 1);
    unsigned long long getUsosExterno(int tipo) const;

    void mostrarResultados() const;
};

#endif
