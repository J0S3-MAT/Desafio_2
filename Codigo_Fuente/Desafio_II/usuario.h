#ifndef USUARIO_H
#define USUARIO_H

#include "fecha.h"
#include "album.h"
#include "metricas.h"

class Mensaje;

class Usuario
{
private:
    // Datos personales
    char* nombreUsuario;
    Fecha fechaNacimiento;
    char* correoElectronico;
    char* rutaFotoPerfil;
    char* contrasena;
    char* ciudad;
    char* pais;
    Fecha fechaInscripcion;

    // Albumes
    Album* albumes;
    int cantidadAlbumes;


    Mensaje* primerEnviado;
    Mensaje* ultimoEnviado;
    int cantidadEnviados;

    Mensaje* primerRecibido;
    Mensaje* ultimoRecibido;
    int cantidadRecibidos;


    // Metodos auxiliares internos
    static char* copiarCadena(const char* texto);

    void liberarMemoria();

    void intercambiar(Usuario& otro);

public:
    // Constructores
    Usuario();

    Usuario(const Usuario& otro);

    // Destructor
    ~Usuario();

    // Operador de asignacion
    Usuario& operator=(const Usuario& otro);


    // Setters de datos personales
    void setNombreUsuario(const char* nombre);
    void setFechaNacimiento(const Fecha& fecha);
    void setCorreoElectronico(const char* correo);
    void setRutaFotoPerfil(const char* ruta);
    void setContrasena(const char* clave);
    void setCiudad(const char* ciudad);
    void setPais(const char* pais);
    void setFechaInscripcion(const Fecha& fecha);

    bool setAlbumes(const Album* nuevosAlbumes, int cantidad);

    // Reservar cantidad albumes vacios que luego se llenan en su lugar (se usa al cargar, sin copias)
    bool reservarAlbumes(int cantidad);


    // Getters de datos personales
    const char* getNombreUsuario() const;
    const Fecha& getFechaNacimiento() const;
    const char* getCorreoElectronico() const;
    const char* getRutaFotoPerfil() const;
    const char* getCiudad() const;
    const char* getPais() const;
    const Fecha& getFechaInscripcion() const;


    // Validaciones
    bool coincideNombre(const char* nombre, Metricas& metricas) const;

    bool validarContrasena(const char* clave, Metricas& metricas) const;


    // Albumes
    int getCantidadAlbumes() const;

    Album* getAlbum(int posicion);


    // Mensajes enviados (agregar al final de la lista: O(1))
    void registrarMensajeEnviado(Mensaje* mensaje);
    Mensaje* getPrimerMensajeEnviado() const;
    int getCantidadMensajesEnviados() const;


    // Mensajes recibidos (agregar al final de la lista: O(1))
    void registrarMensajeRecibido(Mensaje* mensaje);
    Mensaje* getPrimerMensajeRecibido() const;
    int getCantidadMensajesRecibidos() const;


    // Memoria
    unsigned long long getBytesDatosPersonales() const;

    unsigned long long getBytesDinamicos() const;
};

#endif // USUARIO_H
