#ifndef ALBUM_H
#define ALBUM_H

#include "foto.h"

class Album
{
private:
    char* nombre;
    char* ruta;
    int privacidad;
    Foto* fotos;
    int cantidadFotos;

public:
    // Constructores
    Album();

    Album(const char* nombre,
          const char* ruta,
          int privacidad,
          const Foto* fotos,
          int cantidadFotos);

    // Constructor de copia
    Album(const Album& otro);

    // Destructor
    ~Album();

    // Operador de asignacion
    Album& operator=(const Album& otro);

    // Privacidad
    bool permiteAcceso(bool esPropietario,
                       bool esAmigo) const;

    // Consultar fotografias
    int obtenerCantidadFotos() const;
    Foto* obtenerFoto(int posicion);

    // Getters
    const char* obtenerNombre() const;
    const char* obtenerRuta() const;
};

#endif // ALBUM_H
