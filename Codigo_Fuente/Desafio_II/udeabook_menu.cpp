// Metodos de UdeABook que interactuan con el usuario (menus y lectura del teclado).
// La logica de cada funcionalidad esta en udeabook.cpp; aqui solo se piden datos,
// se llama a esa logica y se muestran los resultados y las metricas.
//
// Regla de metricas usada en todo el programa:
//  - Iteraciones: una por cada vuelta de cualquier ciclo de la funcionalidad, incluidos
//    los ciclos de las funciones auxiliares que llama. No se cuentan la lectura del
//    teclado ni el calculo de las propias metricas (getMemoriaSistema).
//  - Memoria total = memoria en el momento exacto en que termina la funcionalidad:
//    estructuras permanentes del sistema (getMemoriaSistema) + variables locales,
//    buffers y parametros por valor que siguen activos en ese momento.
//  - Componentes externos: se reporta cuantas veces se usa cada libreria externa
//    (<random>, <chrono>, <fstream>, <filesystem>). La entrada y salida por consola
//    (cin y cout) es la interfaz del programa y no se reporta.

#include "udeabook.h"
#include "usuario.h"
#include <iostream>

using namespace std;

namespace
{
    const int TAM_ENTRADA = 600;
}

// ----------------LECTURA DEL TECLADO----------------

// Leer una linea completa. Si es mas larga que el buffer se descarta el resto
void UdeABook::leerLinea(const char* mensaje, char* destino, int tamanio)
{
    cout << mensaje;

    if (!cin.getline(destino, tamanio))
    {
        if (cin.eof())
        {
            destino[0] = '\0';
            return;
        }

        cin.clear();
        cin.ignore(10000, '\n');
    }
}

// Convertir texto a entero positivo. Retorna -1 si no es un numero valido
int UdeABook::convertirEntero(const char* texto)
{
    if (texto == nullptr || texto[0] == '\0')
    {
        return -1;
    }

    int valor = 0;

    for (int i = 0; texto[i] != '\0'; i++)
    {
        if (texto[i] < '0' || texto[i] > '9' || valor > 100000000)
        {
            return -1;
        }

        valor = valor * 10 + (texto[i] - '0');
    }

    return valor;
}

int UdeABook::leerEntero(const char* mensaje)
{
    char linea[32];
    leerLinea(mensaje, linea, 32);
    return convertirEntero(linea);
}

// Leer una fecha con formato dd/mm/aaaa. Retorna una fecha invalida (0/0/0) si el formato no sirve
Fecha UdeABook::leerFecha(const char* mensaje)
{
    char linea[32];
    leerLinea(mensaje, linea, 32);

    int partes[3] = {0, 0, 0};
    int parte = 0;
    bool hayDigito = false;

    for (int i = 0; linea[i] != '\0'; i++)
    {
        if (linea[i] == '/')
        {
            if (!hayDigito || parte == 2)
            {
                return Fecha();
            }

            parte++;
            hayDigito = false;
        }
        else if (linea[i] >= '0' && linea[i] <= '9' && partes[parte] < 100000)
        {
            partes[parte] = partes[parte] * 10 + (linea[i] - '0');
            hayDigito = true;
        }
        else
        {
            return Fecha();
        }
    }

    if (parte != 2 || !hayDigito)
    {
        return Fecha();
    }

    return Fecha(partes[0], partes[1], partes[2]);
}

// ----------------APOYO DE LOS MENUS----------------

// Mostrar las metricas al terminar una funcionalidad
void UdeABook::mostrarMetricas(Metricas& metricas, const char* funcionalidad) const
{
    metricas.registrarMemoriaFinal(getMemoriaSistema());

    cout << endl << "[" << funcionalidad << "]" << endl;
    metricas.mostrarResultados();
}

// Pedir un nombre por teclado y buscar el usuario. Retorna su indice o -1
int UdeABook::pedirUsuario(const char* mensaje, Metricas& metricas) const
{
    char nombre[TAM_ENTRADA];
    leerLinea(mensaje, nombre, TAM_ENTRADA);

    int indice = buscarUsuario(nombre, metricas);

    if (indice == -1)
    {
        cout << "No existe un usuario con ese nombre." << endl;
    }

    return indice;
}

// Explicar el codigo de retorno de enviarSolicitud
void UdeABook::mostrarResultadoSolicitud(int codigo)
{
    switch (codigo)
    {
    case 0: cout << "Solicitud enviada." << endl; break;
    case 2: cout << "No puede enviarse una solicitud a si mismo." << endl; break;
    case 3: cout << "Ya son amigos." << endl; break;
    case 4: cout << "Ya le habia enviado una solicitud a este usuario." << endl; break;
    case 5: cout << "Este usuario ya le envio una solicitud. Aceptela desde sus solicitudes." << endl; break;
    default: cout << "No se pudo enviar la solicitud." << endl; break;
    }
}

// ----------------EJECUCION DEL PROGRAMA----------------

// I. Cargar los datos de la carpeta indicada
bool UdeABook::iniciar(const char* carpeta)
{
    if (carpeta == nullptr || carpetaDatos != nullptr)
    {
        return false;
    }

    // Guardar una copia de la carpeta para usarla al guardar
    int longitud = 0;

    while (carpeta[longitud] != '\0')
    {
        longitud++;
    }

    carpetaDatos = new char[longitud + 1];

    for (int i = 0; i <= longitud; i++)
    {
        carpetaDatos[i] = carpeta[i];
    }

    Metricas metricas;

    if (!cargarDatos(carpetaDatos, metricas))
    {
        cout << "No se pudieron cargar los datos de " << carpetaDatos << endl;
        return false;
    }

    cout << "Datos cargados: " << cantidadUsuarios << " usuarios, " << cantidadMensajes << " mensajes." << endl;
    mostrarMetricas(metricas, "Carga de datos");

    return true;
}

// Bucle de inicio de sesion. Al cerrar cada sesion se guardan los datos
void UdeABook::ejecutarMenu()
{
    if (usuarios == nullptr)
    {
        return;
    }

    char nombre[TAM_ENTRADA];
    char clave[TAM_ENTRADA];

    while (!cin.eof())
    {
        cout << endl << "===== Ude@Book =====" << endl;
        leerLinea("Usuario (o 'salir' para terminar): ", nombre, TAM_ENTRADA);

        if (cin.eof() || (nombre[0] == 's' && nombre[1] == 'a' && nombre[2] == 'l' && nombre[3] == 'i' && nombre[4] == 'r' && nombre[5] == '\0'))
        {
            break;
        }

        leerLinea("Contrasena: ", clave, TAM_ENTRADA);

        // II. Inicio de sesion
        Metricas metricas;
        metricas.sumarMemoriaTemporal(sizeof(nombre) + sizeof(clave) + sizeof(int));

        int usuario = iniciarSesion(nombre, clave, metricas);

        if (usuario == -1)
        {
            cout << "Usuario o contrasena incorrectos." << endl;
        }

        mostrarMetricas(metricas, "Inicio de sesion");

        if (usuario == -1)
        {
            continue;
        }

        menuPrincipal(usuario);

        // I. Actualizacion del almacenamiento permanente al cerrar sesion
        Metricas metricasGuardado;

        if (guardarDatos(carpetaDatos, metricasGuardado))
        {
            cout << "Sesion cerrada. Datos guardados." << endl;
        }

        mostrarMetricas(metricasGuardado, "Guardar datos");
    }

    cout << "Hasta pronto." << endl;
}

// ----------------MENU PRINCIPAL----------------

// II. Menu principal con las fotos de perfil de 3 amigos (se pueden cambiar una sola vez)
void UdeABook::menuPrincipal(int usuario)
{
    int fotos[3] = {-1, -1, -1};
    int cantidadFotos = 0;
    bool fotosActualizadas = false;
    char opcion[8];

    // Seleccion inicial de fotos
    Metricas metricas;
    metricas.sumarMemoriaTemporal(sizeof(fotos) + sizeof(cantidadFotos) + sizeof(usuario));

    cantidadFotos = seleccionarFotosAmigos(usuario, fotos, nullptr, 0, metricas);

    cout << endl;
    mostrarFotosAmigos(fotos, cantidadFotos, metricas);
    mostrarMetricas(metricas, "Fotos de perfil de amigos");

    do
    {
        cout << endl << "Bienvenido a UdeaBook " << usuarios[usuario].getNombreUsuario() << endl << endl;
        cout << "Menu de opciones:" << endl;
        cout << "1. Administrar amistades" << endl;
        cout << "2. Visitar perfil de un amigo" << endl;
        cout << "3. Ver sugerencias de amistad" << endl;
        cout << "4. Administrar mensajes" << endl << endl;

        if (!fotosActualizadas && cantidadFotos > 0)
        {
            cout << "Ingrese 'N' o 'n' para alternar el grupo de fotos de perfil de amigos." << endl;
        }

        cout << "Ingrese 'S' o 's' para cerrar su sesion." << endl;
        leerLinea("Su opcion: ", opcion, 8);

        switch (opcion[0])
        {
        case '1': menuAmistades(usuario); break;
        case '2': visitarPerfil(usuario); break;
        case '3': verSugerencias(usuario); break;
        case '4': menuMensajes(usuario); break;
        case 'N':
        case 'n':
            if (fotosActualizadas || cantidadFotos == 0)
            {
                cout << "Las fotos de perfil ya se actualizaron en esta sesion." << endl;
            }
            else
            {
                // Nuevo grupo evitando repetir el anterior (solo las posiciones realmente obtenidas)
                int anteriores[3] = {-1, -1, -1};

                for (int i = 0; i < cantidadFotos; i++)
                {
                    anteriores[i] = fotos[i];
                }

                Metricas metricasFotos;
                metricasFotos.sumarMemoriaTemporal(sizeof(fotos) + sizeof(anteriores) + sizeof(cantidadFotos) + sizeof(usuario));

                cantidadFotos = seleccionarFotosAmigos(usuario, fotos, anteriores, cantidadFotos, metricasFotos);
                fotosActualizadas = true;

                cout << endl;
                mostrarFotosAmigos(fotos, cantidadFotos, metricasFotos);
                mostrarMetricas(metricasFotos, "Actualizar fotos de perfil");
            }
            break;
        case 'S':
        case 's':
            break;
        default:
            cout << "Opcion invalida." << endl;
        }
    }
    while (opcion[0] != 'S' && opcion[0] != 's' && !cin.eof());
}

// ----------------III. ADMINISTRAR AMISTADES----------------

void UdeABook::menuAmistades(int usuario)
{
    char opcion[8];

    do
    {
        cout << endl << "--- Administrar amistades ---" << endl;
        cout << "1. Enviar solicitud de amistad" << endl;
        cout << "2. Ver y aceptar solicitudes recibidas" << endl;
        cout << "0. Volver" << endl;
        leerLinea("Su opcion: ", opcion, 8);

        if (opcion[0] == '1')
        {
            enviarSolicitudPorNombre(usuario);
        }
        else if (opcion[0] == '2')
        {
            administrarSolicitudesRecibidas(usuario);
        }
    }
    while (opcion[0] != '0' && !cin.eof());
}

// III.a Enviar solicitud de amistad dado un nombre de usuario
void UdeABook::enviarSolicitudPorNombre(int usuario)
{
    Metricas metricas;
    metricas.sumarMemoriaTemporal(TAM_ENTRADA + sizeof(usuario) + sizeof(int));

    int receptor = pedirUsuario("Nombre del usuario: ", metricas);

    if (receptor != -1)
    {
        mostrarResultadoSolicitud(enviarSolicitud(usuario, receptor, metricas));
    }

    mostrarMetricas(metricas, "Enviar solicitud de amistad");
}

// III.b Ver solicitudes recibidas y aceptar o rechazar una
void UdeABook::administrarSolicitudesRecibidas(int usuario)
{
    Metricas metricas;
    char opcion[8];
    metricas.sumarMemoriaTemporal(sizeof(usuario) + 3 * sizeof(int) + 32 + sizeof(opcion));

    cout << endl << "Solicitudes recibidas:" << endl;
    int cantidad = mostrarSolicitudes(usuario, metricas);

    if (cantidad > 0)
    {
        int numero = leerEntero("Numero de la solicitud (0 para volver): ");

        if (numero >= 1 && numero <= cantidad)
        {
            int emisor = getSolicitante(usuario, numero - 1, metricas);

            leerLinea("A para aceptar, R para rechazar: ", opcion, 8);

            if (opcion[0] == 'A' || opcion[0] == 'a')
            {
                if (aceptarSolicitud(usuario, emisor, metricas))
                {
                    cout << "Ahora es amigo de " << usuarios[emisor].getNombreUsuario() << "." << endl;
                }
                else
                {
                    cout << "No se pudo aceptar la solicitud." << endl;
                }
            }
            else if (opcion[0] == 'R' || opcion[0] == 'r')
            {
                rechazarSolicitud(usuario, emisor, metricas);
                cout << "Solicitud rechazada." << endl;
            }
        }
    }

    mostrarMetricas(metricas, "Solicitudes recibidas");
}

// ----------------IV. VISITAR PERFIL DE UN AMIGO----------------

void UdeABook::visitarPerfil(int usuario)
{
    Metricas metricas;
    metricas.sumarMemoriaTemporal(TAM_ENTRADA + sizeof(usuario) + 2 * sizeof(int) + 32);

    int amigo = pedirUsuario("Nombre del amigo: ", metricas);

    if (amigo != -1 && !sonAmigos(usuario, amigo))
    {
        cout << "Solo puede visitar el perfil de sus amigos." << endl;
        amigo = -1;
    }

    while (amigo != -1)
    {
        cout << endl << "Albumes de " << usuarios[amigo].getNombreUsuario() << ":" << endl;

        if (mostrarAlbumesVisibles(usuario, amigo, metricas) == 0)
        {
            break;
        }

        int numero = leerEntero("Numero del album (0 para volver): ");

        if (numero <= 0)
        {
            break;
        }

        if (!puedeVerAlbum(usuario, amigo, numero - 1))
        {
            cout << "Album no disponible." << endl;
            continue;
        }

        verAlbumCarrusel(usuario, amigo, numero - 1, metricas);
    }

    mostrarMetricas(metricas, "Visitar perfil de un amigo");
}

// ----------------V. SUGERENCIAS DE AMISTAD----------------

void UdeABook::verSugerencias(int usuario)
{
    Metricas metricas;
    int resultados[20];
    metricas.sumarMemoriaTemporal(sizeof(resultados) + sizeof(usuario) + 4 * sizeof(int) + 32);

    int k = leerEntero("Cantidad de sugerencias (1 a 20): ");

    if (k < 1 || k > 20)
    {
        cout << "Debe ser un numero entre 1 y 20." << endl;
        mostrarMetricas(metricas, "Sugerencias de amistad");
        return;
    }

    int cantidad = obtenerSugerencias(usuario, k, resultados, metricas);

    if (cantidad == 0)
    {
        cout << "No hay sugerencias de amistad por ahora." << endl;
    }
    else
    {
        cout << endl << "Sugerencias de amistad:" << endl;

        for (int i = 0; i < cantidad; i++)
        {
            metricas.sumarIteraciones(1);

            int comunes = contarAmigosComunes(usuario, resultados[i], metricas);

            cout << i + 1 << ". " << usuarios[resultados[i]].getNombreUsuario() << " (" << comunes << " amigos en comun)" << endl;
        }

        int numero = leerEntero("Numero para enviar solicitud (0 para volver): ");

        if (numero >= 1 && numero <= cantidad)
        {
            mostrarResultadoSolicitud(enviarSolicitud(usuario, resultados[numero - 1], metricas));
        }
    }

    mostrarMetricas(metricas, "Sugerencias de amistad");
}

// ----------------VI. ADMINISTRAR MENSAJES----------------

void UdeABook::menuMensajes(int usuario)
{
    char opcion[8];

    do
    {
        cout << endl << "--- Administrar mensajes ---" << endl;
        cout << "1. Enviar mensaje" << endl;
        cout << "2. Leer mensajes nuevos" << endl;
        cout << "3. Buscar mensajes por rango de fechas" << endl;
        cout << "0. Volver" << endl;
        leerLinea("Su opcion: ", opcion, 8);

        if (opcion[0] == '1')
        {
            enviarMensajeAmigo(usuario);
        }
        else if (opcion[0] == '2')
        {
            leerMensajesNuevos(usuario);
        }
        else if (opcion[0] == '3')
        {
            buscarMensajesPorFecha(usuario);
        }
    }
    while (opcion[0] != '0' && !cin.eof());
}

// VI.a Enviar mensaje a un amigo
void UdeABook::enviarMensajeAmigo(int usuario)
{
    Metricas metricas;
    char texto[TAM_ENTRADA];
    metricas.sumarMemoriaTemporal(TAM_ENTRADA + sizeof(texto) + sizeof(usuario) + sizeof(int) + sizeof(Fecha));

    int receptor = pedirUsuario("Nombre del amigo: ", metricas);

    if (receptor != -1)
    {
        leerLinea("Mensaje (maximo 512 caracteres): ", texto, TAM_ENTRADA);

        // La fecha del mensaje se toma del reloj del sistema (<chrono>)
        Fecha fechaEnvio = Fecha::hoy();
        metricas.registrarExterno(Metricas::EXTERNO_RELOJ);

        switch (enviarMensaje(usuario, receptor, texto, fechaEnvio, metricas))
        {
        case 0: cout << "Mensaje enviado." << endl; break;
        case 2: cout << "Solo puede enviar mensajes a sus amigos." << endl; break;
        case 3: cout << "El mensaje no puede estar vacio ni superar 512 caracteres." << endl; break;
        default: cout << "No se pudo enviar el mensaje." << endl; break;
        }
    }

    mostrarMetricas(metricas, "Enviar mensaje");
}

// VI.b Leer mensajes nuevos (ordenados por fecha)
void UdeABook::leerMensajesNuevos(int usuario)
{
    Metricas metricas;
    metricas.sumarMemoriaTemporal(sizeof(usuario));

    cout << endl << "Mensajes nuevos:" << endl;
    mostrarMensajesNoLeidos(usuario, metricas);

    mostrarMetricas(metricas, "Leer mensajes nuevos");
}

// VI.c Buscar mensajes recibidos en un rango de fechas
void UdeABook::buscarMensajesPorFecha(int usuario)
{
    Metricas metricas;
    metricas.sumarMemoriaTemporal(2 * sizeof(Fecha) + 2 * 32 + sizeof(usuario));

    Fecha inicio = leerFecha("Fecha de inicio (dd/mm/aaaa): ");
    Fecha fin = leerFecha("Fecha de fin (dd/mm/aaaa): ");

    cout << endl;

    if (mostrarMensajesPorFecha(usuario, inicio, fin, metricas) == -1)
    {
        cout << "Rango de fechas invalido." << endl;
    }

    mostrarMetricas(metricas, "Buscar mensajes por fecha");
}
