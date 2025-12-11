#pragma once
#include "ArbolBPlus.h" // Asumiendo que separaste el árbol también
#include <string>

class SistemaArchivos {
private:
    ArbolBPlus* arbol;

public:
    SistemaArchivos(); // Constructor
    
    void crear_nodo(int id_nuevo, int id_padre, std::string nombre, bool es_carpeta, int tamano = 0);
    void listar_contenido(int id_directorio);
    // Agrega aquí los prototipos de las otras funciones (calcular_espacio, etc.)
};