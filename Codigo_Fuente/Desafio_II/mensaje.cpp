#include "mensaje.h"
#include "fecha.h"

Mensaje::Mensaje()
{
    indiceEmisor = -1;
    indiceReceptor = -1;
    texto = nullptr;
    longitudTexto = 0;
    leido = false;
    siguienteBloque = nullptr;
    siguienteEnviado = nullptr;
    siguienteRecibido = nullptr;
}

Mensaje::~Mensaje()
{
    delete[] texto;
}

bool Mensaje::setTexto(const char* nuevoTexto)
{
    if (nuevoTexto == nullptr)
    {
        return false;
    }

    int longitud = 0;

    while (longitud <= 512 && nuevoTexto[longitud] != '\0')
    {
        longitud++;
    }

    if (longitud == 0 || longitud > 512)
    {
        return false;
    }

    char* copia = new char[longitud + 1];

    for (int i = 0; i <= longitud; i++)
    {
        copia[i] = nuevoTexto[i];
    }

    delete[] texto;

    texto = copia;
    longitudTexto = longitud;

    return true;
}

const char* Mensaje::getTexto() const
{
    return texto;
}

unsigned short Mensaje::getLongitudTexto() const
{
    return longitudTexto;
}

bool Mensaje::setIndiceEmisor(int indice)
{
    if (indice < 0)
    {
        return false;
    }

    indiceEmisor = indice;
    return true;
}

bool Mensaje::setIndiceReceptor(int indice)
{
    if (indice < 0)
    {
        return false;
    }

    indiceReceptor = indice;
    return true;
}

int Mensaje::getIndiceEmisor() const
{
    return indiceEmisor;
}

int Mensaje::getIndiceReceptor() const
{
    return indiceReceptor;
}

void Mensaje::marcarComoLeido()
{
    leido = true;
}

bool Mensaje::getLeido() const
{
    return leido;
}

void Mensaje::setFecha(const Fecha& nuevaFecha)
{
    fecha = nuevaFecha;
}

const Fecha& Mensaje::getFecha() const
{
    return fecha;
}

Mensaje::Mensaje(int emisor, int receptor, const char* nuevoTexto, const Fecha& fechaEnvio) : indiceEmisor(-1), indiceReceptor(-1), texto(nullptr), longitudTexto(0), leido(false), fecha(fechaEnvio), siguienteBloque(nullptr), siguienteEnviado(nullptr), siguienteRecibido(nullptr)
{
    setIndiceEmisor(emisor);
    setIndiceReceptor(receptor);
    setTexto(nuevoTexto);
}

unsigned long long Mensaje::getBytesTexto() const
{
    if (texto == nullptr)
    {
        return 0;
    }

    return longitudTexto + 1;
}

bool Mensaje::esValido() const
{
    if (indiceEmisor < 0 || indiceReceptor < 0)
    {
        return false;
    }

    if (indiceEmisor == indiceReceptor)
    {
        return false;
    }

    if (texto == nullptr || longitudTexto == 0 || longitudTexto > 512)
    {
        return false;
    }

    if (!fecha.esValida())
    {
        return false;
    }

    return true;
}

bool Mensaje::estaEnIntervalo(const Fecha& fechaInicio, const Fecha& fechaFin) const
{
    if (fechaFin < fechaInicio)
    {
        return false;
    }

    if (fecha < fechaInicio)
    {
        return false;
    }

    if (fechaFin < fecha)
    {
        return false;
    }

    return true;
}

void Mensaje::setSiguienteBloque(Mensaje* bloque)
{
    siguienteBloque = bloque;
}

Mensaje* Mensaje::getSiguienteBloque() const
{
    return siguienteBloque;
}

void Mensaje::setSiguienteEnviado(Mensaje* mensaje)
{
    siguienteEnviado = mensaje;
}

Mensaje* Mensaje::getSiguienteEnviado() const
{
    return siguienteEnviado;
}

void Mensaje::setSiguienteRecibido(Mensaje* mensaje)
{
    siguienteRecibido = mensaje;
}

Mensaje* Mensaje::getSiguienteRecibido() const
{
    return siguienteRecibido;
}