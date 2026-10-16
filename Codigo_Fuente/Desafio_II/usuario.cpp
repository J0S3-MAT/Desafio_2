#include "usuario.h"
#include <climits>

// -------------METODOS AUXILIARES------------

// Crear una copia independiente de una cadena
char* Usuario::copiarCadena(const char* texto)
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


// Liberar toda la memoria dinamica propiedad del usuario
void Usuario::liberarMemoria()
{
    delete[] nombreUsuario;
    delete[] correoElectronico;
    delete[] rutaFotoPerfil;
    delete[] contrasena;
    delete[] ciudad;
    delete[] pais;

    delete[] albumes;

    delete[] mensajesEnviados;
    delete[] mensajesRecibidos;

    delete[] solicitudesRecibidas;


    nombreUsuario = nullptr;
    correoElectronico = nullptr;
    rutaFotoPerfil = nullptr;
    contrasena = nullptr;
    ciudad = nullptr;
    pais = nullptr;

    albumes = nullptr;

    mensajesEnviados = nullptr;
    mensajesRecibidos = nullptr;

    solicitudesRecibidas = nullptr;
}


// Intercambiar datos entre dos usuarios
void Usuario::intercambiar(Usuario& otro)
{
    char* textoTemporal;
    int* arregloTemporal;
    Album* albumTemporal;
    int enteroTemporal;
    Fecha fechaTemporal;


    // Nombre
    textoTemporal = nombreUsuario;
    nombreUsuario = otro.nombreUsuario;
    otro.nombreUsuario = textoTemporal;


    // Fecha de nacimiento
    fechaTemporal = fechaNacimiento;
    fechaNacimiento = otro.fechaNacimiento;
    otro.fechaNacimiento = fechaTemporal;


    // Correo
    textoTemporal = correoElectronico;
    correoElectronico = otro.correoElectronico;
    otro.correoElectronico = textoTemporal;


    // Foto de perfil
    textoTemporal = rutaFotoPerfil;
    rutaFotoPerfil = otro.rutaFotoPerfil;
    otro.rutaFotoPerfil = textoTemporal;


    // Contrasena
    textoTemporal = contrasena;
    contrasena = otro.contrasena;
    otro.contrasena = textoTemporal;


    // Ciudad
    textoTemporal = ciudad;
    ciudad = otro.ciudad;
    otro.ciudad = textoTemporal;


    // Pais
    textoTemporal = pais;
    pais = otro.pais;
    otro.pais = textoTemporal;


    // Fecha de inscripcion
    fechaTemporal = fechaInscripcion;
    fechaInscripcion = otro.fechaInscripcion;
    otro.fechaInscripcion = fechaTemporal;


    // Albumes
    albumTemporal = albumes;
    albumes = otro.albumes;
    otro.albumes = albumTemporal;

    enteroTemporal = cantidadAlbumes;
    cantidadAlbumes = otro.cantidadAlbumes;
    otro.cantidadAlbumes = enteroTemporal;


    // Mensajes enviados
    arregloTemporal = mensajesEnviados;
    mensajesEnviados = otro.mensajesEnviados;
    otro.mensajesEnviados = arregloTemporal;

    enteroTemporal = cantidadMensajesEnviados;
    cantidadMensajesEnviados = otro.cantidadMensajesEnviados;
    otro.cantidadMensajesEnviados = enteroTemporal;

    enteroTemporal = capacidadMensajesEnviados;
    capacidadMensajesEnviados = otro.capacidadMensajesEnviados;
    otro.capacidadMensajesEnviados = enteroTemporal;


    // Mensajes recibidos
    arregloTemporal = mensajesRecibidos;
    mensajesRecibidos = otro.mensajesRecibidos;
    otro.mensajesRecibidos = arregloTemporal;

    enteroTemporal = cantidadMensajesRecibidos;
    cantidadMensajesRecibidos = otro.cantidadMensajesRecibidos;
    otro.cantidadMensajesRecibidos = enteroTemporal;

    enteroTemporal = capacidadMensajesRecibidos;
    capacidadMensajesRecibidos = otro.capacidadMensajesRecibidos;
    otro.capacidadMensajesRecibidos = enteroTemporal;


    // Solicitudes
    arregloTemporal = solicitudesRecibidas;
    solicitudesRecibidas = otro.solicitudesRecibidas;
    otro.solicitudesRecibidas = arregloTemporal;

    enteroTemporal = cantidadSolicitudesRecibidas;
    cantidadSolicitudesRecibidas = otro.cantidadSolicitudesRecibidas;
    otro.cantidadSolicitudesRecibidas = enteroTemporal;

    enteroTemporal = capacidadSolicitudesRecibidas;
    capacidadSolicitudesRecibidas = otro.capacidadSolicitudesRecibidas;
    otro.capacidadSolicitudesRecibidas = enteroTemporal;
}


// Agregar un indice a un arreglo dinamico
bool Usuario::agregarIndice(int*& arreglo, int& cantidad, int& capacidad,int valor, Metricas& metricas)
{
    if (valor < 0)
    {
        return false;
    }

    // Si no hay espacio se redimensiona (¡pendiente cambiar este redimencionar y copiar!
    if (cantidad == capacidad)
    {
        int nuevaCapacidad;

        if (capacidad == 0)
        {
            nuevaCapacidad = 1;
        }
        else
        {
            if (capacidad > INT_MAX / 2)
            {
                return false;
            }

            nuevaCapacidad = capacidad * 2;
        }

        int* nuevoArreglo = new int[nuevaCapacidad];

        for (int i = 0; i < cantidad; i++)
        {
            metricas.sumarIteraciones(1);

            nuevoArreglo[i] = arreglo[i];
        }

        delete[] arreglo;

        arreglo = nuevoArreglo;
        capacidad = nuevaCapacidad;
    }

    arreglo[cantidad] = valor;
    cantidad++;

    return true;
}


// ------------CONSTRUCTORES Y DESTRUCTOR--------------

Usuario::Usuario()
    : nombreUsuario(nullptr),
    fechaNacimiento(),
    correoElectronico(nullptr),
    rutaFotoPerfil(nullptr),
    contrasena(nullptr),
    ciudad(nullptr),
    pais(nullptr),
    fechaInscripcion(),
    albumes(nullptr),
    cantidadAlbumes(0),
    mensajesEnviados(nullptr),
    cantidadMensajesEnviados(0),
    capacidadMensajesEnviados(0),
    mensajesRecibidos(nullptr),
    cantidadMensajesRecibidos(0),
    capacidadMensajesRecibidos(0),
    solicitudesRecibidas(nullptr),
    cantidadSolicitudesRecibidas(0),
    capacidadSolicitudesRecibidas(0)
{
}


// Constructor de copia
Usuario::Usuario(const Usuario& otro)
    : Usuario()
{
    try
    {
        // Copiar datos personales
        nombreUsuario = copiarCadena(otro.nombreUsuario);

        fechaNacimiento = otro.fechaNacimiento;

        correoElectronico = copiarCadena(otro.correoElectronico);

        rutaFotoPerfil = copiarCadena(otro.rutaFotoPerfil);

        contrasena = copiarCadena(otro.contrasena);

        ciudad = copiarCadena(otro.ciudad);

        pais = copiarCadena(otro.pais);

        fechaInscripcion = otro.fechaInscripcion;


        // Copiar albumes
        if (otro.albumes != nullptr && otro.cantidadAlbumes > 0)
        {
            albumes = new Album[otro.cantidadAlbumes];

            for (int i = 0; i < otro.cantidadAlbumes; i++)
            {
                albumes[i] = otro.albumes[i];
            }

            cantidadAlbumes = otro.cantidadAlbumes;
        }


        // Copiar mensajes enviados
        if (otro.capacidadMensajesEnviados > 0)
        {
            mensajesEnviados = new int[otro.capacidadMensajesEnviados];

            for (int i = 0; i < otro.cantidadMensajesEnviados; i++)
            {
                mensajesEnviados[i] = otro.mensajesEnviados[i];
            }

            cantidadMensajesEnviados = otro.cantidadMensajesEnviados;

            capacidadMensajesEnviados = otro.capacidadMensajesEnviados;
        }


        // Copiar mensajes recibidos
        if (otro.capacidadMensajesRecibidos > 0)
        {
            mensajesRecibidos = new int[otro.capacidadMensajesRecibidos];

            for (int i = 0; i < otro.cantidadMensajesRecibidos; i++)
            {
                mensajesRecibidos[i] = otro.mensajesRecibidos[i];
            }

            cantidadMensajesRecibidos = otro.cantidadMensajesRecibidos;

            capacidadMensajesRecibidos = otro.capacidadMensajesRecibidos;
        }


        // Copiar solicitudes
        if (otro.capacidadSolicitudesRecibidas > 0)
        {
            solicitudesRecibidas = new int[otro.capacidadSolicitudesRecibidas];

            for (int i = 0; i < otro.cantidadSolicitudesRecibidas; i++)
            {
                solicitudesRecibidas[i] = otro.solicitudesRecibidas[i];
            }

            cantidadSolicitudesRecibidas = otro.cantidadSolicitudesRecibidas;

            capacidadSolicitudesRecibidas = otro.capacidadSolicitudesRecibidas;
        }
    }
    catch (...)
    {
        liberarMemoria();
        throw;
    }
}


// Destructor
Usuario::~Usuario()
{
    liberarMemoria();
}


// Operador de asignacion
Usuario& Usuario::operator=(const Usuario& otro)
{
    if (this != &otro)
    {
        Usuario copia(otro);

        intercambiar(copia);
    }

    return *this;
}

// -------------SETTERS----------


void Usuario::setNombreUsuario(const char* nombre)
{
    char* copia = copiarCadena(nombre);

    delete[] nombreUsuario;

    nombreUsuario = copia;
}


void Usuario::setFechaNacimiento(const Fecha& fecha)
{
    fechaNacimiento = fecha;
}


void Usuario::setCorreoElectronico(const char* correo)
{
    char* copia = copiarCadena(correo);

    delete[] correoElectronico;

    correoElectronico = copia;
}


void Usuario::setRutaFotoPerfil(const char* ruta)
{
    char* copia = copiarCadena(ruta);

    delete[] rutaFotoPerfil;

    rutaFotoPerfil = copia;
}


void Usuario::setContrasena(const char* clave)
{
    char* copia = copiarCadena(clave);

    delete[] contrasena;

    contrasena = copia;
}


void Usuario::setCiudad(const char* nuevaCiudad)
{
    char* copia = copiarCadena(nuevaCiudad);

    delete[] ciudad;

    ciudad = copia;
}


void Usuario::setPais(const char* nuevoPais)
{
    char* copia = copiarCadena(nuevoPais);

    delete[] pais;

    pais = copia;
}


void Usuario::setFechaInscripcion(const Fecha& fecha)
{
    fechaInscripcion = fecha;
}


bool Usuario::setAlbumes(const Album* nuevosAlbumes,int cantidad)
{
    if (cantidad < 0)
    {
        return false;
    }

    if (cantidad > 0 &&
        nuevosAlbumes == nullptr)
    {
        return false;
    }

    Album* copia = nullptr;

    if (cantidad > 0)
    {
        copia = new Album[cantidad];

        try
        {
            for (int i = 0; i < cantidad; i++)
            {
                copia[i] = nuevosAlbumes[i];
            }
        }
        catch (...)
        {
            delete[] copia;
            throw;
        }
    }

    delete[] albumes;

    albumes = copia;
    cantidadAlbumes = cantidad;

    return true;
}

// ------------GETTERS--------------


const char* Usuario::getNombreUsuario() const
{
    return nombreUsuario;
}


const Fecha& Usuario::getFechaNacimiento() const
{
    return fechaNacimiento;
}


const char* Usuario::getCorreoElectronico() const
{
    return correoElectronico;
}


const char* Usuario::getRutaFotoPerfil() const
{
    return rutaFotoPerfil;
}


const char* Usuario::getCiudad() const
{
    return ciudad;
}


const char* Usuario::getPais() const
{
    return pais;
}


const Fecha& Usuario::getFechaInscripcion() const
{
    return fechaInscripcion;
}

// ----------------VALIDACIONES-----------------

bool Usuario::coincideNombre(const char* nombre,Metricas& metricas) const
{
    if (nombreUsuario == nullptr || nombre == nullptr)
    {
        return false;
    }

    int i = 0;

    while (nombreUsuario[i] != '\0' && nombre[i] != '\0')
    {
        metricas.sumarIteraciones(1);

        if (nombreUsuario[i] != nombre[i])
        {
            return false;
        }

        i++;
    }

    return nombreUsuario[i] == nombre[i];
}


bool Usuario::validarContrasena(const char* clave,Metricas& metricas) const
{
    if (contrasena == nullptr || clave == nullptr)
    {
        return false;
    }

    int i = 0;

    while (contrasena[i] != '\0' && clave[i] != '\0')
    {
        metricas.sumarIteraciones(1);

        if (contrasena[i] != clave[i])
        {
            return false;
        }

        i++;
    }

    return contrasena[i] == clave[i];
}


// --------------ALBUMES---------------

int Usuario::getCantidadAlbumes() const
{
    return cantidadAlbumes;
}


Album* Usuario::getAlbum(int posicion)
{
    if (posicion < 0 || posicion >= cantidadAlbumes)
    {
        return nullptr;
    }

    return &albumes[posicion];
}


// ---------------SOLICITUDES DE AMISTAD----------------

bool Usuario::tieneSolicitudDe(int indiceEmisor,Metricas& metricas) const
{
    for (int i = 0; i < cantidadSolicitudesRecibidas; i++)
    {
        metricas.sumarIteraciones(1);

        if (solicitudesRecibidas[i] == indiceEmisor)
        {
            return true;
        }
    }

    return false;
}


bool Usuario::agregarSolicitud(int indiceEmisor,Metricas& metricas)
{
    if (indiceEmisor < 0)
    {
        return false;
    }

    if (tieneSolicitudDe(indiceEmisor,metricas))
    {
        return false;
    }

    return agregarIndice(solicitudesRecibidas,cantidadSolicitudesRecibidas,capacidadSolicitudesRecibidas,indiceEmisor,metricas);
}


bool Usuario::eliminarSolicitud(int indiceEmisor,Metricas& metricas)
{
    int posicion = -1;

    for (int i = 0; i < cantidadSolicitudesRecibidas;i++)
    {
        metricas.sumarIteraciones(1);

        if (solicitudesRecibidas[i] == indiceEmisor)
        {
            posicion = i;
            break;
        }
    }

    if (posicion == -1)
    {
        return false;
    }

    for (int i = posicion;i < cantidadSolicitudesRecibidas - 1;i++)
    {
        metricas.sumarIteraciones(1);

        solicitudesRecibidas[i] = solicitudesRecibidas[i + 1];
    }

    cantidadSolicitudesRecibidas--;

    return true;
}


int Usuario::getCantidadSolicitudes() const
{
    return cantidadSolicitudesRecibidas;
}


int Usuario::getSolicitante(int posicion) const
{
    if (posicion < 0 ||posicion >= cantidadSolicitudesRecibidas)
    {
        return -1;
    }

    return solicitudesRecibidas[posicion];
}


// --------------MENSAJES ENVIADOS-------------

bool Usuario::registrarMensajeEnviado(int indiceMensaje, Metricas& metricas)
{
    return agregarIndice(mensajesEnviados, cantidadMensajesEnviados, capacidadMensajesEnviados, indiceMensaje, metricas);
}


int Usuario::getCantidadMensajesEnviados() const
{
    return cantidadMensajesEnviados;
}


int Usuario::getIndiceMensajeEnviado(int posicion) const
{
    if (posicion < 0 ||posicion >= cantidadMensajesEnviados)
    {
        return -1;
    }

    return mensajesEnviados[posicion];
}


// -------------MENSAJES RECIBIDOS---------------

bool Usuario::registrarMensajeRecibido(int indiceMensaje, Metricas& metricas)
{
    return agregarIndice(mensajesRecibidos, cantidadMensajesRecibidos,capacidadMensajesRecibidos,indiceMensaje,metricas);
}


int Usuario::getCantidadMensajesRecibidos() const
{
    return cantidadMensajesRecibidos;
}


int Usuario::getIndiceMensajeRecibido(int posicion) const
{
    if (posicion < 0 ||posicion >= cantidadMensajesRecibidos)
    {
        return -1;
    }

    return mensajesRecibidos[posicion];
}