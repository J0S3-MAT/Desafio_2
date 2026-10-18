#ifndef UDEABOOK_H
#define UDEABOOK_H

#include "metricas.h"
#include "fecha.h"

class Mensaje;
class Usuario;


class UdeABook
{
private:

    unsigned char* matrizAmistades;
    unsigned char* matrizSolicitudes;
    int cantidadUsuarios;
    int cantidadAmistades;
    int cantidadSolicitudes;
    Usuario* usuarios;

    // Almacen central de mensajes: bloques enlazados de TAM_BLOQUE_MENSAJES objetos
    static const int TAM_BLOQUE_MENSAJES = 64;
    Mensaje* primerBloqueMensajes;
    Mensaje* ultimoBloqueMensajes;
    int cantidadMensajes;
    unsigned long long bytesTextosMensajes;

    // Carpeta de los archivos de datos (se copia en iniciar)
    char* carpetaDatos;


    // ---------- Logica interna (udeabook.cpp) ----------


    int agregarMensaje(int emisor, int receptor, const char* texto, const Fecha& fecha, bool leido, Metricas& metricas);


    int seleccionarMensajesOrdenados(int usuario, bool soloNoLeidos, const Fecha& inicio, const Fecha& fin, Mensaje**& seleccionados, Metricas& metricas) const;
    void imprimirMensaje(const Mensaje& mensaje) const;

    // Unir carpeta y nombre de archivo en destino
    static void armarRuta(char* destino, int tamanio, const char* carpeta, const char* archivo);

    // Carga de cada archivo
    bool cargarUsuarios(const char* carpeta, Metricas& metricas);
    bool cargarAlbumesYFotos(const char* carpeta, Metricas& metricas);
    bool cargarAmistades(const char* carpeta, Metricas& metricas);
    bool cargarSolicitudes(const char* carpeta, Metricas& metricas);
    bool cargarMensajes(const char* carpeta, Metricas& metricas);

    // Escritura de cada archivo
    bool escribirAmistades(const char* ruta, Metricas& metricas) const;
    bool escribirSolicitudes(const char* ruta, Metricas& metricas) const;
    bool escribirMensajes(const char* ruta, Metricas& metricas) const;
    bool escribirFotos(const char* ruta, Metricas& metricas) const;


    // ---------- Menus (udeabook_menu.cpp) ----------

    // Lectura del teclado
    static void leerLinea(const char* mensaje, char* destino, int tamanio);
    static int convertirEntero(const char* texto);
    static int leerEntero(const char* mensaje);
    static Fecha leerFecha(const char* mensaje);

    // Apoyo de los menus
    void mostrarMetricas(Metricas& metricas, const char* funcionalidad) const;
    int pedirUsuario(const char* mensaje, Metricas& metricas) const;
    static void mostrarResultadoSolicitud(int codigo);

    // Sesion y submenus
    void menuPrincipal(int usuario);
    void menuAmistades(int usuario);
    void menuMensajes(int usuario);

    // Funcionalidades del menu
    void enviarSolicitudPorNombre(int usuario);
    void administrarSolicitudesRecibidas(int usuario);
    void visitarPerfil(int usuario);
    void verSugerencias(int usuario);
    void enviarMensajeAmigo(int usuario);
    void leerMensajesNuevos(int usuario);
    void buscarMensajesPorFecha(int usuario);

public:
    // La red no se copia: es unica durante toda la ejecucion
    UdeABook(const UdeABook& otro) = delete;
    UdeABook& operator=(const UdeABook& otro) = delete;

    UdeABook();
    ~UdeABook();

    // ---------- Ejecucion del programa (udeabook_menu.cpp) ----------
    bool iniciar(const char* carpeta);
    void ejecutarMenu();

    // ---------- Usuarios y amistades ----------
    bool inicializarMatrizAmistades(int cantidad);
    bool inicializarUsuarios();
    Usuario* getUsuario(int indice);
    int getCantidadUsuarios() const;

    bool sonAmigos(int usuario1, int usuario2) const;
    bool establecerAmistad(int usuario1, int usuario2);
    int contarAmigosComunes(int usuario1, int usuario2, Metricas& metricas) const;
    int contarAmigos(int usuario, Metricas& metricas) const;

    int buscarUsuario(const char* nombre, Metricas& metricas) const;
    int iniciarSesion(const char* nombre, const char* clave, Metricas& metricas) const;

    // ---------- Sugerencias ----------
    int obtenerSugerencias(int usuario, int k, int* resultados, Metricas& metricas) const;

    // ---------- Solicitudes de amistad ----------
    bool tieneSolicitud(int emisor, int receptor) const;
    int contarSolicitudesRecibidas(int usuario, Metricas& metricas) const;
    int getSolicitante(int usuario, int posicion, Metricas& metricas) const;
    int enviarSolicitud(int emisor, int receptor, Metricas& metricas);
    bool aceptarSolicitud(int usuario, int emisor, Metricas& metricas);
    bool rechazarSolicitud(int usuario, int emisor, Metricas& metricas);
    int mostrarSolicitudes(int usuario, Metricas& metricas) const;

    // ---------- Mensajes ----------
    int enviarMensaje(int emisor, int receptor, const char* texto, const Fecha& fecha, Metricas& metricas);
    int mostrarMensajesNoLeidos(int usuario, Metricas& metricas);
    int mostrarMensajesPorFecha(int usuario, const Fecha& inicio, const Fecha& fin, Metricas& metricas) const;
    int getCantidadMensajes() const;

    // ---------- Perfil de un amigo: albumes visibles y carrusel ----------
    bool puedeVerAlbum(int visitante, int dueno, int posicionAlbum) const;
    int mostrarAlbumesVisibles(int visitante, int dueno, Metricas& metricas) const;
    void verAlbumCarrusel(int visitante, int dueno, int posicionAlbum, Metricas& metricas);

    // ---------- Fotos de perfil de amigos al azar con prioridad por amigos en comun ----------
    int seleccionarFotosAmigos(int usuario, int* seleccion, const int* excluir, int cantidadExcluir, Metricas& metricas) const;
    void mostrarFotosAmigos(const int* seleccion, int cantidad, Metricas& metricas) const;

    // ---------- Almacenamiento permanente (carpeta terminada en '/', por ejemplo "datos/") ----------
    bool cargarDatos(const char* carpeta, Metricas& metricas);
    bool guardarDatos(const char* carpeta, Metricas& metricas) const;

    // ---------- Memoria ----------
    unsigned long long getBytesMatrizAmistades() const;
    unsigned long long getBytesMatrizSolicitudes() const;
    unsigned long long getMemoriaSistema() const;
};

#endif // UDEABOOK_H
