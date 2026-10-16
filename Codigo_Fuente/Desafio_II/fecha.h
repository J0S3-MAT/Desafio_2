#ifndef FECHA_H
#define FECHA_H

class Fecha
{
private:
    short dia;
    short mes;
    short anio;

    bool esBisiesto(int anio) const;

public:
    // Constructores
    Fecha();
    Fecha(int dia, int mes, int anio);
    Fecha(const Fecha& otra);

    // Validacion
    bool esValida() const;

    // Sobrecarga de operadores
    bool operator<(const Fecha& otra) const;
    bool operator==(const Fecha& otra) const;
    bool operator<=(const Fecha& otra) const;

    // Getters
    int getDia() const;
    int getMes() const;
    int getAnio() const;
};

#endif // FECHA_H