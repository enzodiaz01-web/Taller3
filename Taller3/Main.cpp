//Enzo Diaz Parra - 21.653.611-8 / Dan Gomez - 21.744.505-1/ 

#include <iostream>
#include <string>
#include "SistemaArch.h"

using namespace std;

void limpiar() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void precargar_datos(SistemaArchivos& sys) {
    cout << "--- Cargando datos de prueba ---" << endl;
    
    sys.crear_nodo(1, 0, "home", true);         
    sys.crear_nodo(5, 0, "var", true);          
    
    sys.crear_nodo(2, 1, "user", true);         
    
    sys.crear_nodo(3, 2, "foto.jpg", false, 500); 
    sys.crear_nodo(4, 2, "tesis.doc", false, 150); 
    
    sys.crear_nodo(6, 5, "log", true);          
    
    cout << "Datos cargados correctamente.\n" << endl;
}

int main() {
    SistemaArchivos sistema;
    int opcion = -1;

    cout << "Desea precargar una estructura de archivos de prueba? (1: Si, 0: No): ";
    int cargar;
    cin >> cargar;
    if (cargar == 1) precargar_datos(sistema);

    while (opcion != 0) {
        cout << "\n=== SISTEMA DE ARCHIVOS SIMULADO ===" << endl;
        cout << "1. Crear Archivo/Carpeta" << endl;
        cout << "2. Listar contenido de una carpeta" << endl;
        cout << "3. Ver ruta completa de un nodo" << endl;
        cout << "4. Calcular peso total de una carpeta/archivo" << endl;
        cout << "0. Salir" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                int id, padre, tamano = 0;
                string nombre;
                bool es_carpeta;
                
                cout << "Ingrese ID Unico: ";
                cin >> id;
                cout << "Ingrese ID del Padre (0 es raiz): ";
                cin >> padre;
                cout << "Nombre del nodo: ";
                cin >> nombre;
                cout << "Es carpeta? (1: Si, 0: No): ";
                cin >> es_carpeta;

                if (!es_carpeta) {
                    cout << "Ingrese tamano del archivo: ";
                    cin >> tamano;
                }

                sistema.crear_nodo(id, padre, nombre, es_carpeta, tamano);
                break;
            }
            case 2: {
                int id;
                cout << "Ingrese ID de la carpeta a listar: ";
                cin >> id;
                sistema.listar_contenido(id);
                break;
            }
            case 3: {
                int id;
                cout << "Ingrese ID del nodo para ver su ruta: ";
                cin >> id;
                sistema.imprimir_rutas(id, ""); 
                break;
            }
            case 4: {
                int id;
                cout << "Ingrese ID para calcular espacio: ";
                cin >> id;
                int total = sistema.calcular_espacio(id);
                cout << "El peso total (recursivo) es: " << total << " bytes." << endl;
                break;
            }
            case 0:
                cout << "Saliendo..." << endl;
                break;
            default:
                cout << "Opcion invalida." << endl;
        }
    }

    return 0;
}