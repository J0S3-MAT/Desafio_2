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

    // Reemplazar todos los datos de la foto (se usa al cargar, sin crear copias temporales)
    void setDatos(const Fecha& fecha, unsigned long long tamanio, const char* ruta, int likes);

    // Administracion de likes
    void darLike();
    bool quitarLike();

    // Getters
    const char* getRuta() const;
    const Fecha& getFechaCaptura() const;
    unsigned long long getTamanioBytes() const;
    int getCantidadLikes() const;

    unsigned long long getBytesRuta() const;
};

#endif // FOTO_H


