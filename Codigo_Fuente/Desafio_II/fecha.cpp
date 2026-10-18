#include "fecha.h"
#include <chrono>

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

// Operador de asignacion
Fecha& Fecha::operator=(const Fecha& otra)
{
    dia = otra.dia;
    mes = otra.mes;
    anio = otra.anio;

    return *this;
}


Fecha Fecha::hoy()
{

    long long segundos = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());


    long long dias = (segundos - 5 * 3600) / 86400;

    int anio = 1970;

    while (true)
    {
        bool bisiesto = (anio % 400 == 0) || (anio % 4 == 0 && anio % 100 != 0);
        int diasAnio = bisiesto ? 366 : 365;

        if (dias < diasAnio)
        {
            break;
        }

        dias -= diasAnio;
        anio++;
    }

    bool bisiesto = (anio % 400 == 0) || (anio % 4 == 0 && anio % 100 != 0);
    int diasMes[12] = {31, bisiesto ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int mes = 0;

    while (dias >= diasMes[mes])
    {
        dias -= diasMes[mes];
        mes++;
    }

    int dia = dias + 1;

    return Fecha(dia, mes + 1, anio);
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


bool Fecha::operator<=(const Fecha& otra) const
{
    return (*this < otra) || (*this == otra);
}


int Fecha::getDia() const
{
    return dia;
}


int Fecha::getMes() const
{
    return mes;
}


int Fecha::getAnio() const
{
    return anio;
}