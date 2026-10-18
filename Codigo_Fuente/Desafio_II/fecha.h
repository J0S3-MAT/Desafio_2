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

    Fecha();
    Fecha(int dia, int mes, int anio);
    Fecha(const Fecha& otra);


    Fecha& operator=(const Fecha& otra);


    static Fecha hoy();

    // Validacion
    bool esValida() const;

    // Sobrecarga de operadores
    bool operator<(const Fecha& otra) const;
    bool operator==(const Fecha& otra) const;
    bool operator<=(const Fecha& otra) const;


    int getDia() const;
    int getMes() const;
    int getAnio() const;
};

#endif // FECHA_H