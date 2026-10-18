#ifndef MENSAJE_H
#define MENSAJE_H
#include "fecha.h"

class Mensaje
{
private:
    int indiceEmisor;
    int indiceReceptor;
    char* texto;
    unsigned short longitudTexto;
    bool leido;
    Fecha fecha;

    // Enlace al siguiente bloque de mensajes (solo lo usa el primer mensaje de cada bloque)
    Mensaje* siguienteBloque;


    Mensaje* siguienteEnviado;
    Mensaje* siguienteRecibido;

public:
    //__________Temporal_________
    Mensaje(const Mensaje& otro) = delete;
    Mensaje& operator=(const Mensaje& otro) = delete;

    unsigned long long getBytesTexto() const;
    Mensaje();
    ~Mensaje();


    bool setTexto(const char* nuevoTexto);

    const char* getTexto() const;
    unsigned short getLongitudTexto() const;

    bool setIndiceEmisor(int indice);
    bool setIndiceReceptor(int indice);

    int getIndiceEmisor() const;
    int getIndiceReceptor() const;

    void marcarComoLeido();
    bool getLeido() const;

    void setFecha(const Fecha& nuevaFecha);
    const Fecha& getFecha() const;

    Mensaje(int emisor, int receptor, const char* nuevoTexto, const Fecha& fechaEnvio);

    bool esValido() const;

    bool estaEnIntervalo(const Fecha& fechaInicio, const Fecha& fechaFin) const;

    // Enlace entre bloques de almacenamiento
    void setSiguienteBloque(Mensaje* bloque);
    Mensaje* getSiguienteBloque() const;

    // Enlaces de las listas de enviados y recibidos
    void setSiguienteEnviado(Mensaje* mensaje);
    Mensaje* getSiguienteEnviado() const;

    void setSiguienteRecibido(Mensaje* mensaje);
    Mensaje* getSiguienteRecibido() const;
};



#endif // MENSAJE_H