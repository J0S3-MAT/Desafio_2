#include "fecha.h"

Fecha::Fecha()
    : dia(0), mes(0), anio(0)
{
}

// Constructor con parametros
Fecha::Fecha(int dia, int mes, int anio)
    : dia(0), mes(0), anio(0)
{
    if (anio >= 1 && anio <= 32767 &&
        mes >= 1 && mes <= 12 &&
        dia >= 1 && dia <= 31)
    {
        this->dia = dia;
        this->mes = mes;
        this->anio = anio;

        if (!esValida())
        {
            this->dia = 0;
            this->mes = 0;
            this->anio = 0;
        }
    }
}

// Constructor de copia
Fecha::Fecha(const Fecha& otra)
    : dia(otra.dia), mes(otra.mes), anio(otra.anio)
{
}

// Verificar si el año es bisiesto
bool Fecha::esBisiesto(int anio) const
{
    return (anio % 400 == 0) ||
           (anio % 4 == 0 && anio % 100 != 0);
}

// Validar una fecha
bool Fecha::esValida() const
{
    if (anio < 1 || mes < 1 || mes > 12 ||
        dia < 1)
    {
        return false;
    }

    int diasMes;

    if (mes == 2)
    {
        diasMes = esBisiesto(anio) ? 29 : 28;
    }
    else if (mes == 4 || mes == 6 ||
             mes == 9 || mes == 11)
    {
        diasMes = 30;
    }
    else
    {
        diasMes = 31;
    }

    return dia <= diasMes;
}

// Operador menor que
bool Fecha::operator<(const Fecha& otra) const
{
    if (anio != otra.anio)
        return anio < otra.anio;

    if (mes != otra.mes)
        return mes < otra.mes;

    return dia < otra.dia;
}

// Operador de igualdad
bool Fecha::operator==(const Fecha& otra) const
{
    return dia == otra.dia &&
           mes == otra.mes &&
           anio == otra.anio;
}

// Operador menor o igual
bool Fecha::operator<=(const Fecha& otra) const
{
    return (*this < otra) || (*this == otra);
}

// Obtener dia
int Fecha::obtenerDia() const
{
    return dia;
}

// Obtener mes
int Fecha::obtenerMes() const
{
    return mes;
}

// Obtener año
int Fecha::obtenerAnio() const
{
    return anio;
}

//comentario para prueba de ramas