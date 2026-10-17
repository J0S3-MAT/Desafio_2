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

    static char* copiarCadena(const char* texto);

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

    // Reemplazar los datos del album y reservar cantidadFotos fotos vacias, que luego
    // se llenan en su lugar con getFoto(i)->setDatos(...). Se usa al cargar, sin copias temporales
    bool inicializar(const char* nuevoNombre, const char* nuevaRuta, int nuevaPrivacidad, int nuevaCantidadFotos);

    // Privacidad
    bool permiteAcceso(bool esPropietario,
                       bool esAmigo) const;

    // Consultar fotografias
    int getCantidadFotos() const;
    Foto* getFoto(int posicion);

    // Getters
    const char* getNombre() const;
    const char* getRuta() const;

    unsigned long long getBytesDinamicos() const;
};

#endif // ALBUM_H
