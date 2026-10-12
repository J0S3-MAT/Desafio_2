#ifndef FOTO_H
#define FOTO_H

#include "fecha.h"

class Foto
{
private:
    Fecha fechaCaptura;
    unsigned long long tamanioBytes;
    char* rutaArchivo;
    int cantidadLikes;

public:
    // Constructores
    Foto();

    Foto(const Fecha& fecha,
         unsigned long long tamanio,
         const char* ruta,
         int likes);

    // Constructor de copia
    Foto(const Foto& otra);

    // Destructor
    ~Foto();

    // Operador de asignacion
    Foto& operator=(const Foto& otra);

    // Administracion de likes
    void darLike();
    bool quitarLike();

    // Getters
    const char* obtenerRuta() const;
    const Fecha& obtenerFechaCaptura() const;
    unsigned long long obtenerTamanioBytes() const;
    int obtenerCantidadLikes() const;
};

#endif // FOTO_H


