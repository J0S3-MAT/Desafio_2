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

    // Operador de asignacion
    Fecha& operator=(const Fecha& otra);

    // Fecha actual en Colombia (UTC-5)
    static Fecha hoy();

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