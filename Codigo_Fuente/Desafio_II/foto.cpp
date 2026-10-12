#include "foto.h"
#include <limits>

// Constructor predeterminado
Foto::Foto()
    : fechaCaptura(),
    tamanioBytes(0),
    rutaArchivo(nullptr),
    cantidadLikes(0)
{
}

// Constructor con parametros
Foto::Foto(const Fecha& fecha,
           unsigned long long tamanio,
           const char* ruta,
           int likes)
    : fechaCaptura(fecha),
    tamanioBytes(tamanio),
    rutaArchivo(nullptr),
    cantidadLikes(likes >= 0 ? likes : 0)
{
    if (ruta != nullptr)
    {
        int longitud = 0;

        // Calcular longitud de la ruta
        while (ruta[longitud] != '\0')
        {
            longitud++;
        }

        // Reservar memoria dinamica
        rutaArchivo = new char[longitud + 1];

        // Copiar caracteres y terminador
        for (int i = 0; i <= longitud; i++)
        {
            rutaArchivo[i] = ruta[i];
        }
    }
}

// Constructor de copia
Foto::Foto(const Foto& otra)
    : fechaCaptura(otra.fechaCaptura),
    tamanioBytes(otra.tamanioBytes),
    rutaArchivo(nullptr),
    cantidadLikes(otra.cantidadLikes)
{
    if (otra.rutaArchivo != nullptr)
    {
        int longitud = 0;

        while (otra.rutaArchivo[longitud] != '\0')
        {
            longitud++;
        }

        rutaArchivo = new char[longitud + 1];

        for (int i = 0; i <= longitud; i++)
        {
            rutaArchivo[i] = otra.rutaArchivo[i];
        }
    }
}

// Destructor
Foto::~Foto()
{
    delete[] rutaArchivo;
}

// Operador de asignacion
Foto& Foto::operator=(const Foto& otra)
{
    if (this != &otra)
    {
        char* nuevaRuta = nullptr;

        // Primero copiar la nueva ruta
        if (otra.rutaArchivo != nullptr)
        {
            int longitud = 0;

            while (otra.rutaArchivo[longitud] != '\0')
            {
                longitud++;
            }

            nuevaRuta = new char[longitud + 1];

            for (int i = 0; i <= longitud; i++)
            {
                nuevaRuta[i] = otra.rutaArchivo[i];
            }
        }

        // Liberar la ruta anterior
        delete[] rutaArchivo;

        // Actualizar atributos
        rutaArchivo = nuevaRuta;
        fechaCaptura = otra.fechaCaptura;
        tamanioBytes = otra.tamanioBytes;
        cantidadLikes = otra.cantidadLikes;
    }

    return *this;
}

// Incrementar likes
void Foto::darLike()
{
    if (cantidadLikes < std::numeric_limits<int>::max())
    {
        cantidadLikes++;
    }
}

// Disminuir likes
bool Foto::quitarLike()
{
    if (cantidadLikes > 0)
    {
        cantidadLikes--;
        return true;
    }

    return false;
}

// Obtener ruta
const char* Foto::obtenerRuta() const
{
    return rutaArchivo;
}

// Obtener fecha de captura
const Fecha& Foto::obtenerFechaCaptura() const
{
    return fechaCaptura;
}

// Obtener tamaño en bytes
unsigned long long Foto::obtenerTamanioBytes() const
{
    return tamanioBytes;
}

// Obtener cantidad de likes
int Foto::obtenerCantidadLikes() const
{
    return cantidadLikes;
}

