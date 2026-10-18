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
Album::Album(const char* nombre,const char* ruta,int privacidad,const Foto* fotos,int cantidadFotos)
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
    : Album(otro.nombre,otro.ruta,otro.privacidad,otro.fotos,otro.cantidadFotos)
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

        Album copia(otro);


        char* nombreTemporal = nombre;
        nombre = copia.nombre;
        copia.nombre = nombreTemporal;


        char* rutaTemporal = ruta;
        ruta = copia.ruta;
        copia.ruta = rutaTemporal;

        int privacidadTemporal = privacidad;
        privacidad = copia.privacidad;
        copia.privacidad = privacidadTemporal;


        Foto* fotosTemporales = fotos;
        fotos = copia.fotos;
        copia.fotos = fotosTemporales;

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

    if (esPropietario)
    {
        return true;
    }

    if (privacidad == 0)
    {
        return true;
    }


    if (privacidad == 1 && esAmigo)
    {
        return true;
    }


    return false;
}

int Album::getCantidadFotos() const
{
    return cantidadFotos;
}

Foto* Album::getFoto(int posicion)
{
    if (posicion < 0 || posicion >= cantidadFotos)
    {
        return nullptr;
    }

    return &fotos[posicion];
}



const char* Album::getNombre() const
{
    return nombre;
}

const char* Album::getRuta() const
{
    return ruta;
}


unsigned long long Album::getBytesDinamicos() const
{
    unsigned long long memoria = 0;


    if (nombre != nullptr)
    {
        int longitud = 0;

        while (nombre[longitud] != '\0')
        {
            longitud++;
        }

        memoria += longitud + 1;
    }

    if (ruta != nullptr)
    {
        int longitud = 0;

        while (ruta[longitud] != '\0')
        {
            longitud++;
        }

        memoria += longitud + 1;
    }


    if (fotos != nullptr)
    {
        memoria += cantidadFotos * sizeof(Foto);

        for (int i = 0; i < cantidadFotos; i++)
        {
            memoria += fotos[i].getBytesRuta();
        }
    }

    return memoria;
}


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
