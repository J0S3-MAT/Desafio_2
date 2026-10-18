#include "usuario.h"
#include "mensaje.h"

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



void Usuario::liberarMemoria()
{
    delete[] nombreUsuario;
    delete[] correoElectronico;
    delete[] rutaFotoPerfil;
    delete[] contrasena;
    delete[] ciudad;
    delete[] pais;

    delete[] albumes;

    nombreUsuario = nullptr;
    correoElectronico = nullptr;
    rutaFotoPerfil = nullptr;
    contrasena = nullptr;
    ciudad = nullptr;
    pais = nullptr;

    albumes = nullptr;
    cantidadAlbumes = 0;
}


// Intercambiar datos entre dos usuarios
void Usuario::intercambiar(Usuario& otro)
{
    char* textoTemporal;
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


    // Listas de mensajes (solo se intercambian los punteros)
    Mensaje* mensajeTemporal;

    mensajeTemporal = primerEnviado;
    primerEnviado = otro.primerEnviado;
    otro.primerEnviado = mensajeTemporal;

    mensajeTemporal = ultimoEnviado;
    ultimoEnviado = otro.ultimoEnviado;
    otro.ultimoEnviado = mensajeTemporal;

    enteroTemporal = cantidadEnviados;
    cantidadEnviados = otro.cantidadEnviados;
    otro.cantidadEnviados = enteroTemporal;

    mensajeTemporal = primerRecibido;
    primerRecibido = otro.primerRecibido;
    otro.primerRecibido = mensajeTemporal;

    mensajeTemporal = ultimoRecibido;
    ultimoRecibido = otro.ultimoRecibido;
    otro.ultimoRecibido = mensajeTemporal;

    enteroTemporal = cantidadRecibidos;
    cantidadRecibidos = otro.cantidadRecibidos;
    otro.cantidadRecibidos = enteroTemporal;
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
    primerEnviado(nullptr),
    ultimoEnviado(nullptr),
    cantidadEnviados(0),
    primerRecibido(nullptr),
    ultimoRecibido(nullptr),
    cantidadRecibidos(0)
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


        // Las listas de mensajes se comparten: los mensajes pertenecen a UdeABook,
        // asi que basta con copiar los punteros (no se duplican mensajes)
        primerEnviado = otro.primerEnviado;
        ultimoEnviado = otro.ultimoEnviado;
        cantidadEnviados = otro.cantidadEnviados;

        primerRecibido = otro.primerRecibido;
        ultimoRecibido = otro.ultimoRecibido;
        cantidadRecibidos = otro.cantidadRecibidos;
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


bool Usuario::setAlbumes(const Album* nuevosAlbumes, int cantidad)
{
    if (cantidad < 0)
    {
        return false;
    }

    if (cantidad > 0 && nuevosAlbumes == nullptr)
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

// Reservar albumes vacios. Se reemplazan los anteriores
bool Usuario::reservarAlbumes(int cantidad)
{
    if (cantidad < 0)
    {
        return false;
    }

    Album* nuevos = cantidad > 0 ? new Album[cantidad] : nullptr;

    delete[] albumes;

    albumes = nuevos;
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

bool Usuario::coincideNombre(const char* nombre, Metricas& metricas) const
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


bool Usuario::validarContrasena(const char* clave, Metricas& metricas) const
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


// --------------MENSAJES ENVIADOS-------------

// Agregar un mensaje al final de la lista de enviados
void Usuario::registrarMensajeEnviado(Mensaje* mensaje)
{
    if (mensaje == nullptr)
    {
        return;
    }

    if (primerEnviado == nullptr)
    {
        primerEnviado = mensaje;
    }
    else
    {
        ultimoEnviado->setSiguienteEnviado(mensaje);
    }

    ultimoEnviado = mensaje;
    cantidadEnviados++;
}


Mensaje* Usuario::getPrimerMensajeEnviado() const
{
    return primerEnviado;
}


int Usuario::getCantidadMensajesEnviados() const
{
    return cantidadEnviados;
}


// -------------MENSAJES RECIBIDOS---------------

// Agregar un mensaje al final de la lista de recibidos
void Usuario::registrarMensajeRecibido(Mensaje* mensaje)
{
    if (mensaje == nullptr)
    {
        return;
    }

    if (primerRecibido == nullptr)
    {
        primerRecibido = mensaje;
    }
    else
    {
        ultimoRecibido->setSiguienteRecibido(mensaje);
    }

    ultimoRecibido = mensaje;
    cantidadRecibidos++;
}


Mensaje* Usuario::getPrimerMensajeRecibido() const
{
    return primerRecibido;
}


int Usuario::getCantidadMensajesRecibidos() const
{
    return cantidadRecibidos;
}


// -------------MEMORIA---------------

unsigned long long Usuario::getBytesDatosPersonales() const
{
    const char* cadenas[6] = {nombreUsuario, correoElectronico, rutaFotoPerfil, contrasena, ciudad, pais};

    unsigned long long memoria = 0;

    for (int i = 0; i < 6; i++)
    {
        if (cadenas[i] == nullptr)
        {
            continue;
        }

        int longitud = 0;

        while (cadenas[i][longitud] != '\0')
        {
            longitud++;
        }

        memoria += longitud + 1;
    }

    return memoria;
}


unsigned long long Usuario::getBytesDinamicos() const
{
    unsigned long long memoria = getBytesDatosPersonales();

    // Memoria de los albumes
    if (albumes != nullptr)
    {
        memoria += cantidadAlbumes * sizeof(Album);

        for (int i = 0; i < cantidadAlbumes; i++)
        {
            memoria += albumes[i].getBytesDinamicos();
        }
    }

    // Los mensajes se cuentan en UdeABook (son suyos); aqui solo hay punteros, incluidos en sizeof(Usuario)

    return memoria;
}