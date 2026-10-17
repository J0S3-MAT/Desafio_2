#include "udeabook.h"
#include "MatrizBits.h"
#include "mensaje.h"
#include "usuario.h"
#include <iostream>
#include <random>
#include <fstream>
#include <filesystem>

using namespace std;

UdeABook::UdeABook()
{
    matrizAmistades = nullptr;
    matrizSolicitudes = nullptr;
    cantidadUsuarios = 0;
    cantidadAmistades = 0;
    cantidadSolicitudes = 0;

    usuarios = nullptr;

    primerBloqueMensajes = nullptr;
    ultimoBloqueMensajes = nullptr;
    cantidadMensajes = 0;
    bytesTextosMensajes = 0;

    carpetaDatos = nullptr;
}

UdeABook::~UdeABook()
{
    destruirMatrizBits(matrizAmistades);
    destruirMatrizBits(matrizSolicitudes);

    delete[] usuarios;
    delete[] carpetaDatos;

    // Liberar los bloques de mensajes uno por uno
    Mensaje* bloque = primerBloqueMensajes;

    while (bloque != nullptr)
    {
        Mensaje* siguiente = bloque[0].getSiguienteBloque();
        delete[] bloque;
        bloque = siguiente;
    }
}

bool UdeABook::inicializarMatrizAmistades(int cantidad)
{
    if (cantidad <= 0 || usuarios != nullptr)
    {
        return false;
    }

    // Dos matrices de bits N x N: amistades y solicitudes pendientes
    unsigned char* nuevaMatriz = crearMatrizBits(cantidad, cantidad);
    unsigned char* nuevasSolicitudes = crearMatrizBits(cantidad, cantidad);

    if (nuevaMatriz == nullptr || nuevasSolicitudes == nullptr)
    {
        destruirMatrizBits(nuevaMatriz);
        destruirMatrizBits(nuevasSolicitudes);
        return false;
    }

    destruirMatrizBits(matrizAmistades);
    destruirMatrizBits(matrizSolicitudes);

    matrizAmistades = nuevaMatriz;
    matrizSolicitudes = nuevasSolicitudes;
    cantidadUsuarios = cantidad;
    cantidadAmistades = 0;
    cantidadSolicitudes = 0;

    return true;
}

bool UdeABook::sonAmigos(int usuario1, int usuario2) const
{
    if (matrizAmistades == nullptr)
    {
        return false;
    }

    if (usuario1 < 0 || usuario1 >= cantidadUsuarios || usuario2 < 0 || usuario2 >= cantidadUsuarios)
    {
        return false;
    }

    if (usuario1 == usuario2)
    {
        return false;
    }

    return leerBit(matrizAmistades, usuario1, usuario2, cantidadUsuarios);
}

bool UdeABook::establecerAmistad(int usuario1, int usuario2)
{
    if (matrizAmistades == nullptr)
    {
        return false;
    }

    if (usuario1 < 0 || usuario1 >= cantidadUsuarios || usuario2 < 0 || usuario2 >= cantidadUsuarios)
    {
        return false;
    }

    if (usuario1 == usuario2)
    {
        return false;
    }

    if (sonAmigos(usuario1, usuario2))
    {
        return false;
    }

    encenderBit(matrizAmistades, usuario1, usuario2, cantidadUsuarios);
    encenderBit(matrizAmistades, usuario2, usuario1, cantidadUsuarios);

    cantidadAmistades++;

    return true;
}

int UdeABook::contarAmigosComunes(int usuario1, int usuario2, Metricas& metricas) const
{


    if (matrizAmistades == nullptr)
    {
        return 0;
    }

    if (usuario1 < 0 || usuario1 >= cantidadUsuarios || usuario2 < 0 || usuario2 >= cantidadUsuarios)
    {
        return 0;
    }

    if (usuario1 == usuario2)
    {
        return 0;
    }

    int comunes = 0;

    for (int i = 0; i < cantidadUsuarios; i++)
    {
        metricas.sumarIteraciones(1);

        if (sonAmigos(usuario1, i) && sonAmigos(usuario2, i))
        {
            comunes++;
        }
    }



    return comunes;
}

unsigned long long UdeABook::getBytesMatrizAmistades() const
{
    if (matrizAmistades == nullptr)
    {
        return 0;
    }

    unsigned long long totalBits = static_cast<unsigned long long>(cantidadUsuarios) * cantidadUsuarios;

    unsigned long long bytes = totalBits / 8;

    if (totalBits % 8 != 0)
    {
        bytes++;
    }

    return bytes;
}



// La matriz de solicitudes tiene el mismo tamano que la de amistades
unsigned long long UdeABook::getBytesMatrizSolicitudes() const
{
    if (matrizSolicitudes == nullptr)
    {
        return 0;
    }

    return getBytesMatrizAmistades();
}

unsigned long long UdeABook::getMemoriaSistema() const
{
    unsigned long long memoria = sizeof(UdeABook) + getBytesMatrizAmistades() + getBytesMatrizSolicitudes();

    if (usuarios != nullptr)
    {
        memoria += static_cast<unsigned long long>(cantidadUsuarios) * sizeof(Usuario);

        for (int i = 0; i < cantidadUsuarios; i++)
        {
            memoria += usuarios[i].getBytesDinamicos();
        }
    }

    // Bloques de mensajes reservados y textos de los mensajes
    unsigned long long bloques = (static_cast<unsigned long long>(cantidadMensajes) + TAM_BLOQUE_MENSAJES - 1) / TAM_BLOQUE_MENSAJES;

    memoria += bloques * TAM_BLOQUE_MENSAJES * sizeof(Mensaje);
    memoria += bytesTextosMensajes;

    // Ruta de la carpeta de datos
    if (carpetaDatos != nullptr)
    {
        int longitud = 0;

        while (carpetaDatos[longitud] != '\0')
        {
            longitud++;
        }

        memoria += static_cast<unsigned long long>(longitud) + 1;
    }

    return memoria;
}


int UdeABook::contarAmigos(int usuario, Metricas& metricas) const
{
    if (matrizAmistades == nullptr || usuario < 0 || usuario >= cantidadUsuarios)
    {
        return 0;
    }

    int cantidad = 0;

    for (int i = 0; i < cantidadUsuarios; i++)
    {
        metricas.sumarIteraciones(1);

        if (sonAmigos(usuario, i))
        {
            cantidad++;
        }
    }

    return cantidad;
}


int UdeABook::obtenerSugerencias(int usuario, int k, int* resultados, Metricas& metricas) const
{
    if (matrizAmistades == nullptr || usuarios == nullptr || resultados == nullptr || usuario < 0 || usuario >= cantidadUsuarios || k < 1 || k > 20)
    {
        return 0;
    }

    // Reservar y registrar el arreglo de puntuaciones
    unsigned long long bytesPuntuaciones = static_cast<unsigned long long>(cantidadUsuarios) * sizeof(int);

    int* puntuaciones = new int[cantidadUsuarios]();

    metricas.sumarMemoriaTemporal(bytesPuntuaciones);

    // Registrar parametros por valor y variables locales principales
    unsigned long long bytesLocales = sizeof(usuario) + sizeof(k) + sizeof(resultados) + sizeof(bytesPuntuaciones) + sizeof(puntuaciones) + sizeof(bytesLocales);

    metricas.sumarMemoriaTemporal(bytesLocales);

    // Buscar amigos de amigos y calcular puntuaciones
    metricas.sumarMemoriaTemporal(sizeof(int));

    for (int amigo = 0; amigo < cantidadUsuarios; amigo++)
    {
        metricas.sumarIteraciones(1);

        if (!sonAmigos(usuario, amigo))
        {
            continue;
        }

        metricas.sumarMemoriaTemporal(sizeof(int));

        for (int candidato = 0; candidato < cantidadUsuarios; candidato++)
        {
            metricas.sumarIteraciones(1);

            if (candidato != usuario && !sonAmigos(usuario, candidato) && sonAmigos(amigo, candidato))
            {
                puntuaciones[candidato]++;
            }
        }

        metricas.restarMemoriaTemporal(sizeof(int));
    }

    metricas.restarMemoriaTemporal(sizeof(int));

    // Excluir candidatos con una solicitud pendiente en cualquier sentido
    metricas.sumarMemoriaTemporal(sizeof(int));

    for (int candidato = 0; candidato < cantidadUsuarios; candidato++)
    {
        metricas.sumarIteraciones(1);

        if (puntuaciones[candidato] > 0 && (tieneSolicitud(usuario, candidato) || tieneSolicitud(candidato, usuario)))
        {
            puntuaciones[candidato] = 0;
        }
    }

    metricas.restarMemoriaTemporal(sizeof(int));

    // Seleccionar hasta K candidatos con mas amigos comunes
    int cantidad = 0;

    metricas.sumarMemoriaTemporal(sizeof(cantidad));
    metricas.sumarMemoriaTemporal(sizeof(int));

    for (int i = 0; i < k; i++)
    {
        int mejor = -1;

        metricas.sumarMemoriaTemporal(sizeof(mejor));
        metricas.sumarMemoriaTemporal(sizeof(int));

        for (int candidato = 0; candidato < cantidadUsuarios; candidato++)
        {
            metricas.sumarIteraciones(1);

            if (puntuaciones[candidato] > 0 && (mejor == -1 || puntuaciones[candidato] > puntuaciones[mejor]))
            {
                mejor = candidato;
            }
        }

        metricas.restarMemoriaTemporal(sizeof(int));

        if (mejor == -1)
        {
            metricas.restarMemoriaTemporal(sizeof(mejor));
            break;
        }

        resultados[cantidad] = mejor;
        cantidad++;

        puntuaciones[mejor] = -1;

        metricas.restarMemoriaTemporal(sizeof(mejor));
    }

    metricas.restarMemoriaTemporal(sizeof(int));

    // Liberar y descontar la memoria temporal
    delete[] puntuaciones;

    metricas.restarMemoriaTemporal(bytesPuntuaciones);
    metricas.restarMemoriaTemporal(bytesLocales);
    metricas.restarMemoriaTemporal(sizeof(cantidad));

    return cantidad;
}

bool UdeABook::inicializarUsuarios()
{
    if (matrizAmistades == nullptr || cantidadUsuarios <= 0)
    {
        return false;
    }

    if (usuarios != nullptr)
    {
        return false;
    }

    usuarios = new Usuario[cantidadUsuarios];

    return true;
}

Usuario* UdeABook::getUsuario(int indice)
{
    if (usuarios == nullptr || indice < 0 || indice >= cantidadUsuarios)
    {
        return nullptr;
    }

    return &usuarios[indice];
}


int UdeABook::buscarUsuario(const char* nombre, Metricas& metricas) const
{
    if (usuarios == nullptr || nombre == nullptr)
    {
        return -1;
    }

    for (int i = 0; i < cantidadUsuarios; i++)
    {
        metricas.sumarIteraciones(1);

        if (usuarios[i].coincideNombre(nombre, metricas))
        {
            return i;
        }
    }

    return -1;
}

int UdeABook::iniciarSesion(const char* nombre, const char* clave, Metricas& metricas) const
{
    if (clave == nullptr)
    {
        return -1;
    }

    int indice = buscarUsuario(nombre, metricas);

    if (indice == -1)
    {
        return -1;
    }

    if (!usuarios[indice].validarContrasena(clave, metricas))
    {
        return -1;
    }

    return indice;
}


// ---------------SOLICITUDES DE AMISTAD----------------
// Las solicitudes pendientes se guardan en una matriz de bits N x N:
// el bit (emisor, receptor) en 1 indica que emisor le envio una solicitud a receptor.

// Indica si emisor tiene una solicitud pendiente enviada a receptor: O(1)
bool UdeABook::tieneSolicitud(int emisor, int receptor) const
{
    if (matrizSolicitudes == nullptr || emisor < 0 || emisor >= cantidadUsuarios || receptor < 0 || receptor >= cantidadUsuarios)
    {
        return false;
    }

    return leerBit(matrizSolicitudes, emisor, receptor, cantidadUsuarios);
}

// Enviar solicitud de emisor a receptor: O(1)
// Retorna: 0 enviada, 1 usuario invalido, 2 a si mismo, 3 ya son amigos,
//          4 ya se habia enviado, 5 el receptor ya le habia enviado una al emisor
int UdeABook::enviarSolicitud(int emisor, int receptor, Metricas& metricas)
{
    if (matrizSolicitudes == nullptr || emisor < 0 || emisor >= cantidadUsuarios || receptor < 0 || receptor >= cantidadUsuarios)
    {
        return 1;
    }

    metricas.sumarIteraciones(1);

    if (emisor == receptor)
    {
        return 2;
    }

    if (sonAmigos(emisor, receptor))
    {
        return 3;
    }

    if (tieneSolicitud(emisor, receptor))
    {
        return 4;
    }

    if (tieneSolicitud(receptor, emisor))
    {
        return 5;
    }

    encenderBit(matrizSolicitudes, emisor, receptor, cantidadUsuarios);
    cantidadSolicitudes++;

    return 0;
}

// Aceptar la solicitud que emisor le envio a usuario: O(1).
// Primero se confirma que exista, luego se crea la amistad y solo entonces se borra la solicitud
bool UdeABook::aceptarSolicitud(int usuario, int emisor, Metricas& metricas)
{
    metricas.sumarIteraciones(1);

    if (!tieneSolicitud(emisor, usuario))
    {
        return false;
    }

    if (!establecerAmistad(usuario, emisor))
    {
        return false;
    }

    apagarBit(matrizSolicitudes, emisor, usuario, cantidadUsuarios);
    cantidadSolicitudes--;

    return true;
}

// Rechazar la solicitud: solo se borra: O(1)
bool UdeABook::rechazarSolicitud(int usuario, int emisor, Metricas& metricas)
{
    metricas.sumarIteraciones(1);

    if (!tieneSolicitud(emisor, usuario))
    {
        return false;
    }

    apagarBit(matrizSolicitudes, emisor, usuario, cantidadUsuarios);
    cantidadSolicitudes--;

    return true;
}

// Contar las solicitudes recibidas por un usuario: recorre su columna, O(N)
int UdeABook::contarSolicitudesRecibidas(int usuario, Metricas& metricas) const
{
    if (matrizSolicitudes == nullptr || usuario < 0 || usuario >= cantidadUsuarios)
    {
        return 0;
    }

    int cantidad = 0;

    for (int emisor = 0; emisor < cantidadUsuarios; emisor++)
    {
        metricas.sumarIteraciones(1);

        if (leerBit(matrizSolicitudes, emisor, usuario, cantidadUsuarios))
        {
            cantidad++;
        }
    }

    return cantidad;
}

// Obtener quien envio la solicitud numero 'posicion' (desde 0) de las recibidas por usuario: O(N).
// Retorna -1 si no existe
int UdeABook::getSolicitante(int usuario, int posicion, Metricas& metricas) const
{
    if (matrizSolicitudes == nullptr || usuario < 0 || usuario >= cantidadUsuarios || posicion < 0)
    {
        return -1;
    }

    int encontradas = 0;

    for (int emisor = 0; emisor < cantidadUsuarios; emisor++)
    {
        metricas.sumarIteraciones(1);

        if (leerBit(matrizSolicitudes, emisor, usuario, cantidadUsuarios))
        {
            if (encontradas == posicion)
            {
                return emisor;
            }

            encontradas++;
        }
    }

    return -1;
}

// Mostrar las solicitudes recibidas numeradas desde 1 (recorre la columna del usuario). Retorna la cantidad
int UdeABook::mostrarSolicitudes(int usuario, Metricas& metricas) const
{
    if (usuarios == nullptr || matrizSolicitudes == nullptr || usuario < 0 || usuario >= cantidadUsuarios)
    {
        return 0;
    }

    int cantidad = 0;

    for (int emisor = 0; emisor < cantidadUsuarios; emisor++)
    {
        metricas.sumarIteraciones(1);

        if (leerBit(matrizSolicitudes, emisor, usuario, cantidadUsuarios))
        {
            cantidad++;
            cout << cantidad << ". " << usuarios[emisor].getNombreUsuario() << endl;
        }
    }

    if (cantidad == 0)
    {
        cout << "No tiene solicitudes de amistad pendientes." << endl;
    }

    return cantidad;
}

// ---------------MENSAJES----------------

// Enviar un mensaje entre amigos. Retorna: 0 enviado, 1 usuario invalido, 2 no son amigos, 3 texto vacio o mayor a 512, 4 fecha invalida
int UdeABook::enviarMensaje(int emisor, int receptor, const char* texto, const Fecha& fecha, Metricas& metricas)
{
    if (usuarios == nullptr || emisor < 0 || emisor >= cantidadUsuarios || receptor < 0 || receptor >= cantidadUsuarios)
    {
        return 1;
    }

    if (!sonAmigos(emisor, receptor))
    {
        return 2;
    }

    return agregarMensaje(emisor, receptor, texto, fecha, false, metricas);
}

// Guardar un mensaje en el almacen central. Mismos codigos de retorno que enviarMensaje
int UdeABook::agregarMensaje(int emisor, int receptor, const char* texto, const Fecha& fecha, bool leido, Metricas& metricas)
{
    if (usuarios == nullptr || emisor < 0 || emisor >= cantidadUsuarios || receptor < 0 || receptor >= cantidadUsuarios || emisor == receptor)
    {
        return 1;
    }

    // Validar el texto antes de ocupar espacio
    if (texto == nullptr)
    {
        return 3;
    }

    int longitud = 0;

    while (longitud <= 512 && texto[longitud] != '\0')
    {
        metricas.sumarIteraciones(1);
        longitud++;
    }

    if (longitud == 0 || longitud > 512)
    {
        return 3;
    }

    if (!fecha.esValida())
    {
        return 4;
    }

    // Si el ultimo bloque esta lleno (o no hay bloques) se enlaza uno nuevo
    if (cantidadMensajes % TAM_BLOQUE_MENSAJES == 0)
    {
        Mensaje* nuevo = new Mensaje[TAM_BLOQUE_MENSAJES];

        if (primerBloqueMensajes == nullptr)
        {
            primerBloqueMensajes = nuevo;
        }
        else
        {
            ultimoBloqueMensajes[0].setSiguienteBloque(nuevo);
        }

        ultimoBloqueMensajes = nuevo;
    }

    // Llenar el mensaje directamente en su posicion del bloque (no se copia)
    int indice = cantidadMensajes;
    Mensaje& mensaje = ultimoBloqueMensajes[indice % TAM_BLOQUE_MENSAJES];

    mensaje.setIndiceEmisor(emisor);
    mensaje.setIndiceReceptor(receptor);
    mensaje.setTexto(texto);
    mensaje.setFecha(fecha);

    if (leido)
    {
        mensaje.marcarComoLeido();
    }

    cantidadMensajes++;
    bytesTextosMensajes += mensaje.getBytesTexto();

    // Enlazar el mismo mensaje al final de la lista del emisor y de la del receptor
    usuarios[emisor].registrarMensajeEnviado(&mensaje);
    usuarios[receptor].registrarMensajeRecibido(&mensaje);

    return 0;
}

// Reunir los mensajes recibidos por el usuario que cumplan la condicion y ordenarlos por fecha.
// soloNoLeidos = true: solo los no leidos. soloNoLeidos = false: solo los del rango [inicio, fin].
// Se recorre la lista enlazada de recibidos del usuario y los mensajes elegidos se guardan en un
// arreglo temporal de punteros (no se copian mensajes), que se ordena por insercion. La insercion es
// estable: los mensajes del mismo dia conservan su orden de llegada.
// Retorna la cantidad; 'seleccionados' queda reservado y debe liberarse con delete[]
int UdeABook::seleccionarMensajesOrdenados(int usuario, bool soloNoLeidos, const Fecha& inicio, const Fecha& fin, Mensaje**& seleccionados, Metricas& metricas) const
{
    int recibidos = usuarios[usuario].getCantidadMensajesRecibidos();

    seleccionados = new Mensaje*[recibidos > 0 ? recibidos : 1];

    // 1. Seleccionar recorriendo la lista de recibidos
    int cantidad = 0;
    Mensaje* actual = usuarios[usuario].getPrimerMensajeRecibido();

    while (actual != nullptr)
    {
        metricas.sumarIteraciones(1);

        bool cumple = soloNoLeidos ? !actual->getLeido() : actual->estaEnIntervalo(inicio, fin);

        if (cumple)
        {
            seleccionados[cantidad] = actual;
            cantidad++;
        }

        actual = actual->getSiguienteRecibido();
    }

    // 2. Ordenar por fecha (insercion)
    for (int i = 1; i < cantidad; i++)
    {
        Mensaje* mensajeActual = seleccionados[i];
        int j = i - 1;

        while (j >= 0 && mensajeActual->getFecha() < seleccionados[j]->getFecha())
        {
            metricas.sumarIteraciones(1);

            seleccionados[j + 1] = seleccionados[j];
            j--;
        }

        metricas.sumarIteraciones(1);

        seleccionados[j + 1] = mensajeActual;
    }

    return cantidad;
}

// Imprimir un mensaje con su fecha y su emisor
void UdeABook::imprimirMensaje(const Mensaje& mensaje) const
{
    const Fecha& fecha = mensaje.getFecha();

    cout << "[" << fecha.getDia() << "/" << fecha.getMes() << "/" << fecha.getAnio() << "] " << usuarios[mensaje.getIndiceEmisor()].getNombreUsuario() << ": " << mensaje.getTexto() << endl;
}

// Mostrar los mensajes recibidos no leidos ordenados por fecha y marcarlos como leidos. Retorna cuantos se mostraron
int UdeABook::mostrarMensajesNoLeidos(int usuario, Metricas& metricas)
{
    if (usuarios == nullptr || usuario < 0 || usuario >= cantidadUsuarios)
    {
        return 0;
    }

    Mensaje** seleccionados = nullptr;
    int cantidad = seleccionarMensajesOrdenados(usuario, true, Fecha(), Fecha(), seleccionados, metricas);

    for (int i = 0; i < cantidad; i++)
    {
        metricas.sumarIteraciones(1);

        imprimirMensaje(*seleccionados[i]);
        seleccionados[i]->marcarComoLeido();
    }

    if (cantidad == 0)
    {
        cout << "No tiene mensajes nuevos." << endl;
    }

    delete[] seleccionados;

    return cantidad;
}

// Mostrar los mensajes recibidos entre dos fechas (ambas inclusive) ordenados por fecha.
// Retorna cuantos se mostraron, o -1 si el rango es invalido
int UdeABook::mostrarMensajesPorFecha(int usuario, const Fecha& inicio, const Fecha& fin, Metricas& metricas) const
{
    if (usuarios == nullptr || usuario < 0 || usuario >= cantidadUsuarios)
    {
        return 0;
    }

    if (!inicio.esValida() || !fin.esValida() || fin < inicio)
    {
        return -1;
    }

    Mensaje** seleccionados = nullptr;
    int cantidad = seleccionarMensajesOrdenados(usuario, false, inicio, fin, seleccionados, metricas);

    for (int i = 0; i < cantidad; i++)
    {
        metricas.sumarIteraciones(1);

        imprimirMensaje(*seleccionados[i]);
    }

    if (cantidad == 0)
    {
        cout << "No hay mensajes en ese rango de fechas." << endl;
    }

    delete[] seleccionados;

    return cantidad;
}

int UdeABook::getCantidadMensajes() const
{
    return cantidadMensajes;
}

// ---------------PERFIL DE UN AMIGO----------------

// Indica si el visitante puede ver un album del dueno segun su privacidad
bool UdeABook::puedeVerAlbum(int visitante, int dueno, int posicionAlbum) const
{
    if (usuarios == nullptr || visitante < 0 || visitante >= cantidadUsuarios || dueno < 0 || dueno >= cantidadUsuarios)
    {
        return false;
    }

    Album* album = usuarios[dueno].getAlbum(posicionAlbum);

    if (album == nullptr)
    {
        return false;
    }

    return album->permiteAcceso(visitante == dueno, sonAmigos(visitante, dueno));
}

// Listar los albumes del dueno que el visitante puede ver, numerados por su posicion + 1. Retorna cuantos son visibles
int UdeABook::mostrarAlbumesVisibles(int visitante, int dueno, Metricas& metricas) const
{
    if (usuarios == nullptr || dueno < 0 || dueno >= cantidadUsuarios)
    {
        return 0;
    }

    int cantidadAlbumes = usuarios[dueno].getCantidadAlbumes();
    int visibles = 0;

    for (int i = 0; i < cantidadAlbumes; i++)
    {
        metricas.sumarIteraciones(1);

        if (!puedeVerAlbum(visitante, dueno, i))
        {
            continue;
        }

        Album* album = usuarios[dueno].getAlbum(i);

        cout << i + 1 << ". " << album->getNombre() << " (" << album->getCantidadFotos() << " fotos)" << endl;

        visibles++;
    }

    if (visibles == 0)
    {
        cout << "Este usuario no tiene albumes visibles para usted." << endl;
    }

    return visibles;
}

// Recorrer las fotos de un album de forma circular. Opciones: siguiente, previa, dar/quitar like, salir
void UdeABook::verAlbumCarrusel(int visitante, int dueno, int posicionAlbum, Metricas& metricas)
{
    if (!puedeVerAlbum(visitante, dueno, posicionAlbum))
    {
        cout << "No tiene permiso para ver este album." << endl;
        return;
    }

    Album* album = usuarios[dueno].getAlbum(posicionAlbum);
    int total = album->getCantidadFotos();

    if (total == 0)
    {
        cout << "El album no tiene fotos." << endl;
        return;
    }

    // Recordar en que fotos dio like el visitante durante esta visita, para alternar dar/quitar
    bool* conLike = new bool[total]();
    unsigned long long bytesConLike = static_cast<unsigned long long>(total) * sizeof(bool);

    metricas.sumarMemoriaTemporal(bytesConLike);
    metricas.sumarMemoriaTemporal(sizeof(album) + sizeof(total) + sizeof(conLike) + sizeof(bytesConLike) + 16 * sizeof(char));

    int actual = 0;
    char opcion = ' ';
    char linea[16];

    while (opcion != '0')
    {
        metricas.sumarIteraciones(1);

        Foto* foto = album->getFoto(actual);

        cout << endl << "Album " << album->getNombre() << " - Foto " << actual + 1 << " de " << total << endl;
        cout << foto->getRuta() << endl;
        cout << "Likes: " << foto->getCantidadLikes() << (conLike[actual] ? " (le gusta)" : "") << endl;
        cout << "1. Siguiente   2. Previa   3. " << (conLike[actual] ? "Quitar like" : "Dar like") << "   0. Salir del album" << endl;
        cout << "Su opcion: ";

        if (!cin.getline(linea, 16))
        {
            if (cin.eof())
            {
                break;
            }

            cin.clear();
            cin.ignore(10000, '\n');
            linea[0] = ' ';
        }

        opcion = linea[0];

        if (opcion == '1')
        {
            actual = (actual + 1) % total;
        }
        else if (opcion == '2')
        {
            actual = (actual - 1 + total) % total;
        }
        else if (opcion == '3')
        {
            if (conLike[actual])
            {
                foto->quitarLike();
                conLike[actual] = false;
            }
            else
            {
                foto->darLike();
                conLike[actual] = true;
            }
        }
        else if (opcion != '0')
        {
            cout << "Opcion invalida." << endl;
        }
    }

    delete[] conLike;

    metricas.restarMemoriaTemporal(bytesConLike);
    metricas.restarMemoriaTemporal(sizeof(album) + sizeof(total) + sizeof(conLike) + sizeof(bytesConLike) + 16 * sizeof(char));
}

// ---------------FOTOS DE PERFIL DE AMIGOS----------------

// Elegir hasta 3 amigos distintos al azar. Peso segun amigos en comun:
// 10 o menos -> 2, entre 11 y 20 -> 5, mas de 20 -> 7 (equivale a 1, 2.5 y 3.5 multiplicado por 2).
// Los amigos en "excluir" (el grupo mostrado antes) solo se usan si no hay suficientes amigos nuevos.
// Retorna cuantos amigos quedaron en seleccion (arreglo de al menos 3 posiciones)
int UdeABook::seleccionarFotosAmigos(int usuario, int* seleccion, const int* excluir, int cantidadExcluir, Metricas& metricas) const
{
    if (usuarios == nullptr || seleccion == nullptr || usuario < 0 || usuario >= cantidadUsuarios)
    {
        return 0;
    }

    // Generador aleatorio de <random>: se crea una sola vez con una semilla de random_device
    static bool generadorCreado = false;
    static std::mt19937 generador(std::random_device{}());

    if (!generadorCreado)
    {
        metricas.registrarExterno(Metricas::EXTERNO_ALEATORIO);
        generadorCreado = true;
    }

    int cantidadAmigos = contarAmigos(usuario, metricas);

    if (cantidadAmigos == 0)
    {
        return 0;
    }

    // Arreglos temporales con los amigos y sus pesos
    int* amigos = new int[cantidadAmigos];
    int* pesos = new int[cantidadAmigos];
    bool* excluido = new bool[cantidadAmigos]();
    unsigned long long bytesTemporales = static_cast<unsigned long long>(cantidadAmigos) * (2 * sizeof(int) + sizeof(bool));

    metricas.sumarMemoriaTemporal(bytesTemporales);

    int posicion = 0;

    for (int i = 0; i < cantidadUsuarios; i++)
    {
        metricas.sumarIteraciones(1);

        if (!sonAmigos(usuario, i))
        {
            continue;
        }

        int comunes = contarAmigosComunes(usuario, i, metricas);

        amigos[posicion] = i;
        pesos[posicion] = comunes <= 10 ? 2 : (comunes <= 20 ? 5 : 7);

        for (int j = 0; j < cantidadExcluir; j++)
        {
            metricas.sumarIteraciones(1);

            if (excluir[j] == i)
            {
                excluido[posicion] = true;
            }
        }

        posicion++;
    }

    int elegidos = 0;

    // Ronda 0: solo amigos no excluidos. Ronda 1: si faltan, tambien los excluidos
    for (int ronda = 0; ronda < 2 && elegidos < 3; ronda++)
    {
        while (elegidos < 3)
        {
            int total = 0;

            for (int i = 0; i < cantidadAmigos; i++)
            {
                metricas.sumarIteraciones(1);

                if (ronda == 1 || !excluido[i])
                {
                    total += pesos[i];
                }
            }

            if (total == 0)
            {
                break;
            }

            // Numero al azar en [0, total) y busqueda del amigo al que le corresponde
            std::uniform_int_distribution<int> distribucion(0, total - 1);
            int azar = distribucion(generador);
            metricas.registrarExterno(Metricas::EXTERNO_ALEATORIO);

            for (int i = 0; i < cantidadAmigos; i++)
            {
                metricas.sumarIteraciones(1);

                if (ronda == 0 && excluido[i])
                {
                    continue;
                }

                if (azar < pesos[i])
                {
                    seleccion[elegidos] = amigos[i];
                    elegidos++;
                    pesos[i] = 0;   // No puede salir dos veces en el mismo grupo
                    break;
                }

                azar -= pesos[i];
            }
        }
    }

    delete[] amigos;
    delete[] pesos;
    delete[] excluido;

    metricas.restarMemoriaTemporal(bytesTemporales);

    return elegidos;
}

// Mostrar las rutas de las fotos de perfil de los amigos seleccionados
void UdeABook::mostrarFotosAmigos(const int* seleccion, int cantidad, Metricas& metricas) const
{
    if (cantidad == 0)
    {
        cout << "Aun no tiene amigos para mostrar fotos de perfil." << endl;
        return;
    }

    for (int i = 0; i < cantidad; i++)
    {
        metricas.sumarIteraciones(1);

        const char* ruta = usuarios[seleccion[i]].getRutaFotoPerfil();

        cout << "Foto perfil de amigo " << i + 1 << ": " << (ruta != nullptr ? ruta : "(sin foto)") << endl;
    }
}

int UdeABook::getCantidadUsuarios() const
{
    return cantidadUsuarios;
}

// ---------------ALMACENAMIENTO PERMANENTE----------------
// Formato: primera linea = cantidad de registros; campos separados por '\'
//   usuarios.txt    indice\nombre\dia\mes\anio(nacimiento)\correo\rutaFotoPerfil\contrasena\ciudad\pais\dia\mes\anio(inscripcion)\cantidadAlbumes
//   albumes.txt     usuario\posicion\nombre\ruta\privacidad(0 publico,1 amigos,2 privado)\cantidadFotos
//   fotos.txt       usuario\album\posicion\dia\mes\anio\tamanioBytes\ruta\likes
//   amistades.txt   usuario1\usuario2
//   solicitudes.txt emisor\receptor
//   mensajes.txt    indice\emisor\receptor\dia\mes\anio\leido(0/1)\texto
// albumes.txt y fotos.txt deben estar ordenados por usuario, album y posicion.
// Los mensajes no necesitan estar ordenados: se ordenan por fecha al mostrarlos.

namespace
{
const int TAM_TEXTO_CORTO = 128;
const int TAM_RUTA = 512;
const int TAM_TEXTO_MENSAJE = 600;

// Leer un campo de texto hasta el separador (o fin de linea) quitando un '\r' final
bool leerCampoArchivo(std::ifstream& archivo, char* destino, int tamanio, char separador)
{
    archivo.getline(destino, tamanio, separador);

    if (!archivo)
    {
        return false;
    }

    int i = 0;

    while (destino[i] != '\0')
    {
        i++;
    }

    if (i > 0 && destino[i - 1] == '\r')
    {
        destino[i - 1] = '\0';
    }

    return true;
}

// Leer un entero seguido de su separador
bool leerEnteroArchivo(std::ifstream& archivo, int& valor)
{
    archivo >> valor;

    if (!archivo)
    {
        return false;
    }

    archivo.ignore(1);
    return true;
}
}

void UdeABook::armarRuta(char* destino, int tamanio, const char* carpeta, const char* archivo)
{
    int i = 0;

    for (int j = 0; carpeta != nullptr && carpeta[j] != '\0' && i < tamanio - 1; j++)
    {
        destino[i++] = carpeta[j];
    }

    for (int j = 0; archivo[j] != '\0' && i < tamanio - 1; j++)
    {
        destino[i++] = archivo[j];
    }

    destino[i] = '\0';
}

// Cargar todos los archivos. Solo se puede hacer con el sistema vacio
bool UdeABook::cargarDatos(const char* carpeta, Metricas& metricas)
{
    if (usuarios != nullptr || matrizAmistades != nullptr)
    {
        return false;
    }

    return cargarUsuarios(carpeta, metricas) && cargarAlbumesYFotos(carpeta, metricas) && cargarAmistades(carpeta, metricas) && cargarSolicitudes(carpeta, metricas) && cargarMensajes(carpeta, metricas);
}

// Leer los usuarios y reservar sus albumes vacios (se llenan en cargarAlbumesYFotos)
bool UdeABook::cargarUsuarios(const char* carpeta, Metricas& metricas)
{
    char ruta[TAM_RUTA];
    char nombre[TAM_TEXTO_CORTO];
    char correo[TAM_TEXTO_CORTO];
    char rutaPerfil[TAM_RUTA];
    char clave[TAM_TEXTO_CORTO];
    char ciudad[TAM_TEXTO_CORTO];
    char pais[TAM_TEXTO_CORTO];

    unsigned long long bytesLocales = sizeof(ruta) + sizeof(nombre) + sizeof(correo) + sizeof(rutaPerfil) + sizeof(clave) + sizeof(ciudad) + sizeof(pais) + 10 * sizeof(int);
    metricas.sumarMemoriaTemporal(bytesLocales);

    armarRuta(ruta, TAM_RUTA, carpeta, "usuarios.txt");

    std::ifstream archivo(ruta);
    metricas.registrarExterno(Metricas::EXTERNO_ARCHIVO);
    int cantidad = 0;
    bool correcto = true;

    if (!archivo || !(archivo >> cantidad) || cantidad <= 0 || !inicializarMatrizAmistades(cantidad) || !inicializarUsuarios())
    {
        cout << "No se pudo leer la cantidad de usuarios de " << ruta << endl;
        correcto = false;
    }

    for (int i = 0; i < cantidad && correcto; i++)
    {
        metricas.sumarIteraciones(1);

        int indice, dia, mes, anio, diaIns, mesIns, anioIns, albumes;

        correcto = leerEnteroArchivo(archivo, indice) && leerCampoArchivo(archivo, nombre, TAM_TEXTO_CORTO, '\\') && leerEnteroArchivo(archivo, dia) && leerEnteroArchivo(archivo, mes) && leerEnteroArchivo(archivo, anio) && leerCampoArchivo(archivo, correo, TAM_TEXTO_CORTO, '\\') && leerCampoArchivo(archivo, rutaPerfil, TAM_RUTA, '\\') && leerCampoArchivo(archivo, clave, TAM_TEXTO_CORTO, '\\') && leerCampoArchivo(archivo, ciudad, TAM_TEXTO_CORTO, '\\') && leerCampoArchivo(archivo, pais, TAM_TEXTO_CORTO, '\\') && leerEnteroArchivo(archivo, diaIns) && leerEnteroArchivo(archivo, mesIns) && leerEnteroArchivo(archivo, anioIns) && (archivo >> albumes);
        correcto = correcto && indice == i && albumes >= 0;

        if (!correcto)
        {
            cout << "Error en la linea " << i + 2 << " de " << ruta << endl;
            break;
        }

        Usuario& usuario = usuarios[i];

        usuario.setNombreUsuario(nombre);
        usuario.setFechaNacimiento(Fecha(dia, mes, anio));
        usuario.setCorreoElectronico(correo);
        usuario.setRutaFotoPerfil(rutaPerfil);
        usuario.setContrasena(clave);
        usuario.setCiudad(ciudad);
        usuario.setPais(pais);
        usuario.setFechaInscripcion(Fecha(diaIns, mesIns, anioIns));
        usuario.reservarAlbumes(albumes);
    }

    metricas.restarMemoriaTemporal(bytesLocales);

    return correcto;
}

// Llenar los albumes y fotos en su lugar. Los dos archivos estan ordenados por usuario y album,
// asi que se leen a la vez sin buscar nada ni crear copias temporales
bool UdeABook::cargarAlbumesYFotos(const char* carpeta, Metricas& metricas)
{
    char rutaAlbumes[TAM_RUTA];
    char rutaFotos[TAM_RUTA];
    char nombreAlbum[TAM_TEXTO_CORTO];
    char rutaAlbum[TAM_RUTA];
    char rutaFoto[TAM_RUTA];

    unsigned long long bytesLocales = sizeof(rutaAlbumes) + sizeof(rutaFotos) + sizeof(nombreAlbum) + sizeof(rutaAlbum) + sizeof(rutaFoto) + 12 * sizeof(int) + sizeof(long long);
    metricas.sumarMemoriaTemporal(bytesLocales);

    armarRuta(rutaAlbumes, TAM_RUTA, carpeta, "albumes.txt");
    armarRuta(rutaFotos, TAM_RUTA, carpeta, "fotos.txt");

    std::ifstream archivoAlbumes(rutaAlbumes);
    std::ifstream archivoFotos(rutaFotos);
    metricas.registrarExterno(Metricas::EXTERNO_ARCHIVO, 2);

    int totalAlbumes = 0;
    int totalFotos = 0;
    bool correcto = archivoAlbumes && archivoFotos && (archivoAlbumes >> totalAlbumes) && (archivoFotos >> totalFotos);

    if (!correcto)
    {
        cout << "No se pudieron abrir " << rutaAlbumes << " o " << rutaFotos << endl;
    }

    for (int u = 0; u < cantidadUsuarios && correcto; u++)
    {
        metricas.sumarIteraciones(1);

        for (int a = 0; a < usuarios[u].getCantidadAlbumes() && correcto; a++)
        {
            metricas.sumarIteraciones(1);

            int usuarioAlbum, posicion, privacidad, cantidadFotos;

            correcto = leerEnteroArchivo(archivoAlbumes, usuarioAlbum) && leerEnteroArchivo(archivoAlbumes, posicion) && leerCampoArchivo(archivoAlbumes, nombreAlbum, TAM_TEXTO_CORTO, '\\') && leerCampoArchivo(archivoAlbumes, rutaAlbum, TAM_RUTA, '\\') && leerEnteroArchivo(archivoAlbumes, privacidad) && (archivoAlbumes >> cantidadFotos);
            correcto = correcto && usuarioAlbum == u && posicion == a && cantidadFotos >= 0;

            Album* album = usuarios[u].getAlbum(a);
            correcto = correcto && album->inicializar(nombreAlbum, rutaAlbum, privacidad, cantidadFotos);

            for (int f = 0; f < cantidadFotos && correcto; f++)
            {
                metricas.sumarIteraciones(1);

                int usuarioFoto, albumFoto, posicionFoto, dia, mes, anio, likes;
                long long tamanio;

                correcto = leerEnteroArchivo(archivoFotos, usuarioFoto) && leerEnteroArchivo(archivoFotos, albumFoto) && leerEnteroArchivo(archivoFotos, posicionFoto) && leerEnteroArchivo(archivoFotos, dia) && leerEnteroArchivo(archivoFotos, mes) && leerEnteroArchivo(archivoFotos, anio);
                correcto = correcto && (archivoFotos >> tamanio);
                archivoFotos.ignore(1);
                correcto = correcto && leerCampoArchivo(archivoFotos, rutaFoto, TAM_RUTA, '\\') && (archivoFotos >> likes);
                correcto = correcto && usuarioFoto == u && albumFoto == a && posicionFoto == f && tamanio >= 0;

                if (correcto)
                {
                    album->getFoto(f)->setDatos(Fecha(dia, mes, anio), static_cast<unsigned long long>(tamanio), rutaFoto, likes);
                }
            }
        }

        if (!correcto)
        {
            cout << "Error en los albumes o fotos del usuario " << u << endl;
        }
    }

    metricas.restarMemoriaTemporal(bytesLocales);

    return correcto;
}

bool UdeABook::cargarAmistades(const char* carpeta, Metricas& metricas)
{
    char ruta[TAM_RUTA];
    unsigned long long bytesLocales = sizeof(ruta) + 4 * sizeof(int);
    metricas.sumarMemoriaTemporal(bytesLocales);

    armarRuta(ruta, TAM_RUTA, carpeta, "amistades.txt");

    std::ifstream archivo(ruta);
    metricas.registrarExterno(Metricas::EXTERNO_ARCHIVO);
    int cantidad = 0;
    bool correcto = archivo && (archivo >> cantidad);

    if (!correcto)
    {
        cout << "No se pudo abrir " << ruta << endl;
    }

    for (int i = 0; i < cantidad && correcto; i++)
    {
        metricas.sumarIteraciones(1);

        int usuario1, usuario2;

        correcto = leerEnteroArchivo(archivo, usuario1) && (archivo >> usuario2);

        if (!correcto)
        {
            cout << "Error en la linea " << i + 2 << " de " << ruta << endl;
            break;
        }

        establecerAmistad(usuario1, usuario2);
    }

    metricas.restarMemoriaTemporal(bytesLocales);

    return correcto;
}

bool UdeABook::cargarSolicitudes(const char* carpeta, Metricas& metricas)
{
    char ruta[TAM_RUTA];
    unsigned long long bytesLocales = sizeof(ruta) + 5 * sizeof(int);
    metricas.sumarMemoriaTemporal(bytesLocales);

    armarRuta(ruta, TAM_RUTA, carpeta, "solicitudes.txt");

    std::ifstream archivo(ruta);
    metricas.registrarExterno(Metricas::EXTERNO_ARCHIVO);
    int cantidad = 0;
    int ignoradas = 0;
    bool correcto = archivo && (archivo >> cantidad);

    if (!correcto)
    {
        cout << "No se pudo abrir " << ruta << endl;
    }

    for (int i = 0; i < cantidad && correcto; i++)
    {
        metricas.sumarIteraciones(1);

        int emisor, receptor;

        correcto = leerEnteroArchivo(archivo, emisor) && (archivo >> receptor);

        if (!correcto)
        {
            cout << "Error en la linea " << i + 2 << " de " << ruta << endl;
            break;
        }

        // Se aplican las mismas reglas que al enviar una solicitud
        if (enviarSolicitud(emisor, receptor, metricas) != 0)
        {
            ignoradas++;
        }
    }

    if (ignoradas > 0)
    {
        cout << "Aviso: se ignoraron " << ignoradas << " solicitudes invalidas de " << ruta << endl;
    }

    metricas.restarMemoriaTemporal(bytesLocales);

    return correcto;
}

bool UdeABook::cargarMensajes(const char* carpeta, Metricas& metricas)
{
    char ruta[TAM_RUTA];
    char texto[TAM_TEXTO_MENSAJE];
    unsigned long long bytesLocales = sizeof(ruta) + sizeof(texto) + 10 * sizeof(int);
    metricas.sumarMemoriaTemporal(bytesLocales);

    armarRuta(ruta, TAM_RUTA, carpeta, "mensajes.txt");

    std::ifstream archivo(ruta);
    metricas.registrarExterno(Metricas::EXTERNO_ARCHIVO);
    int cantidad = 0;
    int ignorados = 0;
    bool correcto = archivo && (archivo >> cantidad);

    if (!correcto)
    {
        cout << "No se pudo abrir " << ruta << endl;
    }

    for (int i = 0; i < cantidad && correcto; i++)
    {
        metricas.sumarIteraciones(1);

        int indice, emisor, receptor, dia, mes, anio, leido;

        correcto = leerEnteroArchivo(archivo, indice) && leerEnteroArchivo(archivo, emisor) && leerEnteroArchivo(archivo, receptor) && leerEnteroArchivo(archivo, dia) && leerEnteroArchivo(archivo, mes) && leerEnteroArchivo(archivo, anio) && leerEnteroArchivo(archivo, leido) && leerCampoArchivo(archivo, texto, TAM_TEXTO_MENSAJE, '\n');

        if (!correcto)
        {
            cout << "Error en la linea " << i + 2 << " de " << ruta << endl;
            break;
        }

        if (agregarMensaje(emisor, receptor, texto, Fecha(dia, mes, anio), leido == 1, metricas) != 0)
        {
            ignorados++;
        }
    }

    if (ignorados > 0)
    {
        cout << "Aviso: se ignoraron " << ignorados << " mensajes invalidos de " << ruta << endl;
    }

    metricas.restarMemoriaTemporal(bytesLocales);

    return correcto;
}

// ---------------GUARDADO----------------

// Escribir las amistades: cada par una sola vez (usuario1 < usuario2)
bool UdeABook::escribirAmistades(const char* ruta, Metricas& metricas) const
{
    std::ofstream archivo(ruta);
    metricas.registrarExterno(Metricas::EXTERNO_ARCHIVO);

    archivo << cantidadAmistades << '\n';

    for (int i = 0; i < cantidadUsuarios; i++)
    {
        for (int j = i + 1; j < cantidadUsuarios; j++)
        {
            metricas.sumarIteraciones(1);

            if (sonAmigos(i, j))
            {
                archivo << i << '\\' << j << '\n';
            }
        }
    }

    archivo.close();
    return !archivo.fail();
}

// Escribir las solicitudes pendientes (bits en 1 de la matriz de solicitudes)
bool UdeABook::escribirSolicitudes(const char* ruta, Metricas& metricas) const
{
    std::ofstream archivo(ruta);
    metricas.registrarExterno(Metricas::EXTERNO_ARCHIVO);

    archivo << cantidadSolicitudes << '\n';

    for (int emisor = 0; emisor < cantidadUsuarios; emisor++)
    {
        for (int receptor = 0; receptor < cantidadUsuarios; receptor++)
        {
            metricas.sumarIteraciones(1);

            if (leerBit(matrizSolicitudes, emisor, receptor, cantidadUsuarios))
            {
                archivo << emisor << '\\' << receptor << '\n';
            }
        }
    }

    archivo.close();
    return !archivo.fail();
}

// Escribir los mensajes en orden de llegada, recorriendo los bloques una sola vez
bool UdeABook::escribirMensajes(const char* ruta, Metricas& metricas) const
{
    std::ofstream archivo(ruta);
    metricas.registrarExterno(Metricas::EXTERNO_ARCHIVO);

    archivo << cantidadMensajes << '\n';

    Mensaje* bloque = primerBloqueMensajes;

    for (int i = 0; i < cantidadMensajes; i++)
    {
        metricas.sumarIteraciones(1);

        if (i > 0 && i % TAM_BLOQUE_MENSAJES == 0)
        {
            bloque = bloque[0].getSiguienteBloque();
        }

        const Mensaje& mensaje = bloque[i % TAM_BLOQUE_MENSAJES];
        const Fecha& fecha = mensaje.getFecha();

        archivo << i << '\\' << mensaje.getIndiceEmisor() << '\\' << mensaje.getIndiceReceptor() << '\\' << fecha.getDia() << '\\' << fecha.getMes() << '\\' << fecha.getAnio() << '\\' << (mensaje.getLeido() ? 1 : 0) << '\\' << mensaje.getTexto() << '\n';
    }

    archivo.close();
    return !archivo.fail();
}

// Escribir las fotos (para conservar los likes)
bool UdeABook::escribirFotos(const char* ruta, Metricas& metricas) const
{
    std::ofstream archivo(ruta);
    metricas.registrarExterno(Metricas::EXTERNO_ARCHIVO);

    int total = 0;

    for (int u = 0; u < cantidadUsuarios; u++)
    {
        for (int a = 0; a < usuarios[u].getCantidadAlbumes(); a++)
        {
            metricas.sumarIteraciones(1);
            total += usuarios[u].getAlbum(a)->getCantidadFotos();
        }
    }

    archivo << total << '\n';

    for (int u = 0; u < cantidadUsuarios; u++)
    {
        for (int a = 0; a < usuarios[u].getCantidadAlbumes(); a++)
        {
            Album* album = usuarios[u].getAlbum(a);

            for (int f = 0; f < album->getCantidadFotos(); f++)
            {
                metricas.sumarIteraciones(1);

                const Foto* foto = album->getFoto(f);
                const Fecha& fecha = foto->getFechaCaptura();

                archivo << u << '\\' << a << '\\' << f << '\\' << fecha.getDia() << '\\' << fecha.getMes() << '\\' << fecha.getAnio() << '\\' << foto->getTamanioBytes() << '\\' << foto->getRuta() << '\\' << foto->getCantidadLikes() << '\n';
            }
        }
    }

    archivo.close();
    return !archivo.fail();
}

// Guardar lo que cambia durante la ejecucion: amistades, solicitudes, mensajes y likes.
// usuarios.txt y albumes.txt no cambian en esta version, por eso no se reescriben.
// Guardado seguro: primero se escribe todo en archivos .tmp; solo si los cuatro quedaron
// bien se reemplazan los originales. Si algo falla, los originales quedan intactos
bool UdeABook::guardarDatos(const char* carpeta, Metricas& metricas) const
{
    if (usuarios == nullptr)
    {
        return false;
    }

    const int CANTIDAD_ARCHIVOS = 4;
    const char* nombres[CANTIDAD_ARCHIVOS] = {"amistades.txt", "solicitudes.txt", "mensajes.txt", "fotos.txt"};
    char rutasFinales[CANTIDAD_ARCHIVOS][TAM_RUTA];
    char rutasTemporales[CANTIDAD_ARCHIVOS][TAM_RUTA];

    unsigned long long bytesLocales = sizeof(nombres) + sizeof(rutasFinales) + sizeof(rutasTemporales) + 2 * sizeof(int) + sizeof(bool);
    metricas.sumarMemoriaTemporal(bytesLocales);

    for (int i = 0; i < CANTIDAD_ARCHIVOS; i++)
    {
        metricas.sumarIteraciones(1);

        armarRuta(rutasFinales[i], TAM_RUTA, carpeta, nombres[i]);
        armarRuta(rutasTemporales[i], TAM_RUTA, rutasFinales[i], ".tmp");
    }

    // 1. Escribir los temporales
    bool correcto = escribirAmistades(rutasTemporales[0], metricas) && escribirSolicitudes(rutasTemporales[1], metricas) && escribirMensajes(rutasTemporales[2], metricas) && escribirFotos(rutasTemporales[3], metricas);

    // 2. Reemplazar los originales solo si todo se escribio bien
    for (int i = 0; i < CANTIDAD_ARCHIVOS; i++)
    {
        metricas.sumarIteraciones(1);

        std::error_code error;

        if (correcto)
        {
            std::filesystem::rename(rutasTemporales[i], rutasFinales[i], error);
            metricas.registrarExterno(Metricas::EXTERNO_SISTEMA_ARCHIVOS);

            if (error)
            {
                correcto = false;
            }
        }

        // Si algo fallo, se borran los temporales que queden
        if (!correcto)
        {
            std::filesystem::remove(rutasTemporales[i], error);
            metricas.registrarExterno(Metricas::EXTERNO_SISTEMA_ARCHIVOS);
        }
    }

    if (!correcto)
    {
        cout << "Error al guardar los datos en " << carpeta << ". Se conservan los archivos anteriores." << endl;
    }

    metricas.restarMemoriaTemporal(bytesLocales);

    return correcto;
}
