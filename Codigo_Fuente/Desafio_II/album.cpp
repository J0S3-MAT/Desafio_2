#include "album.h"

// Constructor predeterminado
Album::Album()
    : nombre(nullptr),
    ruta(nullptr),
    privacidad(2),
    fotos(nullptr),
    cantidadFotos(0)
{
}

// Constructor con parametros
Album::Album(const char* nombre,
             const char* ruta,
             int privacidad,
             const Foto* fotos,
             int cantidadFotos)
    : nombre(nullptr),
    ruta(nullptr),
    privacidad(2),
    fotos(nullptr),
    cantidadFotos(0)
{
    try
    {
        // Copiar nombre
        if (nombre != nullptr)
        {
            int longitud = 0;

            while (nombre[longitud] != '\0')
            {
                longitud++;
            }

            this->nombre = new char[longitud + 1];

            for (int i = 0; i <= longitud; i++)
            {
                this->nombre[i] = nombre[i];
            }
        }

        // Copiar ruta
        if (ruta != nullptr)
        {
            int longitud = 0;

            while (ruta[longitud] != '\0')
            {
                longitud++;
            }

            this->ruta = new char[longitud + 1];

            for (int i = 0; i <= longitud; i++)
            {
                this->ruta[i] = ruta[i];
            }
        }

        // Validar privacidad
        if (privacidad >= 0 && privacidad <= 2)
        {
            this->privacidad = privacidad;
        }

        // Copiar fotografias
        if (fotos != nullptr && cantidadFotos > 0)
        {
            this->fotos = new Foto[cantidadFotos];

            for (int i = 0; i < cantidadFotos; i++)
            {
                this->fotos[i] = fotos[i];
            }

            this->cantidadFotos = cantidadFotos;
        }
    }
    catch (...)
    {
        delete[] this->nombre;
        delete[] this->ruta;
        delete[] this->fotos;
        throw;
    }
}

// Constructor de copia
Album::Album(const Album& otro)
    : Album(otro.nombre,
            otro.ruta,
            otro.privacidad,
            otro.fotos,
            otro.cantidadFotos)
{
}

// Destructor
Album::~Album()
{
    delete[] nombre;
    delete[] ruta;
    delete[] fotos;
}

// Operador de asignacion
Album& Album::operator=(const Album& otro)
{
    if (this != &otro)
    {
        // Crear una copia profunda temporal
        Album copia(otro);

        // Intercambiar nombres
        char* nombreTemporal = nombre;
        nombre = copia.nombre;
        copia.nombre = nombreTemporal;

        // Intercambiar rutas
        char* rutaTemporal = ruta;
        ruta = copia.ruta;
        copia.ruta = rutaTemporal;

        // Intercambiar privacidad
        int privacidadTemporal = privacidad;
        privacidad = copia.privacidad;
        copia.privacidad = privacidadTemporal;

        // Intercambiar arreglos de fotos
        Foto* fotosTemporales = fotos;
        fotos = copia.fotos;
        copia.fotos = fotosTemporales;

        // Intercambiar cantidades
        int cantidadTemporal = cantidadFotos;
        cantidadFotos = copia.cantidadFotos;
        copia.cantidadFotos = cantidadTemporal;
    }

    return *this;
}

// Comprobar acceso segun privacidad
bool Album::permiteAcceso(bool esPropietario,
                          bool esAmigo) const
{
    // El propietario siempre puede acceder
    if (esPropietario)
    {
        return true;
    }

    // Album publico
    if (privacidad == 0)
    {
        return true;
    }

    // Album solo para amigos
    if (privacidad == 1 && esAmigo)
    {
        return true;
    }

    // Album privado
    return false;
}

// Obtener cantidad de fotografias
int Album::obtenerCantidadFotos() const
{
    return cantidadFotos;
}

// Obtener fotografia por posicion
Foto* Album::obtenerFoto(int posicion)
{
    if (posicion < 0 || posicion >= cantidadFotos)
    {
        return nullptr;
    }

    return &fotos[posicion];
}

// Obtener nombre
const char* Album::obtenerNombre() const
{
    return nombre;
}

// Obtener ruta
const char* Album::obtenerRuta() const
{
    return ruta;
}


