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
int Album::getCantidadFotos() const
{
    return cantidadFotos;
}

// Obtener fotografia por posicion
Foto* Album::getFoto(int posicion)
{
    if (posicion < 0 || posicion >= cantidadFotos)
    {
        return nullptr;
    }

    return &fotos[posicion];
}


// Obtener nombre
const char* Album::getNombre() const
{
    return nombre;
}

// Obtener ruta
const char* Album::getRuta() const
{
    return ruta;
}


unsigned long long Album::getBytesDinamicos() const
{
    unsigned long long memoria = 0;

    // Memoria dinamica del nombre
    if (nombre != nullptr)
    {
        int longitud = 0;

        while (nombre[longitud] != '\0')
        {
            longitud++;
        }

        memoria += static_cast<unsigned long long>(longitud) + 1;
    }

    // Memoria dinamica de la ruta del album
    if (ruta != nullptr)
    {
        int longitud = 0;

        while (ruta[longitud] != '\0')
        {
            longitud++;
        }

        memoria += static_cast<unsigned long long>(longitud) + 1;
    }

    // Memoria dinamica de las fotografias
    if (fotos != nullptr)
    {
        memoria += static_cast<unsigned long long>(cantidadFotos) * sizeof(Foto);

        for (int i = 0; i < cantidadFotos; i++)
        {
            memoria += fotos[i].getBytesRuta();
        }
    }

    return memoria;
}

// Crear una copia independiente de una cadena
char* Album::copiarCadena(const char* texto)
{
    if (texto == nullptr)
    {
        return nullptr;
    }

    int longitud = 0;

    while (texto[longitud] != '\0')
    {
        longitud++;
    }

    char* copia = new char[longitud + 1];

    for (int i = 0; i <= longitud; i++)
    {
        copia[i] = texto[i];
    }

    return copia;
}

// Reemplazar los datos del album y reservar sus fotos vacias.
// Primero se reserva todo lo nuevo; si algo falla, el album queda como estaba
bool Album::inicializar(const char* nuevoNombre, const char* nuevaRuta, int nuevaPrivacidad, int nuevaCantidadFotos)
{
    if (nuevaCantidadFotos < 0)
    {
        return false;
    }

    char* copiaNombre = copiarCadena(nuevoNombre);
    char* copiaRuta = nullptr;
    Foto* nuevasFotos = nullptr;

    try
    {
        copiaRuta = copiarCadena(nuevaRuta);

        if (nuevaCantidadFotos > 0)
        {
            nuevasFotos = new Foto[nuevaCantidadFotos];
        }
    }
    catch (...)
    {
        delete[] copiaNombre;
        delete[] copiaRuta;
        throw;
    }

    delete[] nombre;
    delete[] ruta;
    delete[] fotos;

    nombre = copiaNombre;
    ruta = copiaRuta;
    privacidad = (nuevaPrivacidad >= 0 && nuevaPrivacidad <= 2) ? nuevaPrivacidad : 2;
    fotos = nuevasFotos;
    cantidadFotos = nuevaCantidadFotos;

    return true;
}
