/**
 * @file ListaGestion.h
 * @brief Lista para gestionar sensores de diferentes tipos
 * @author Barbie
 * @date 2025
 */

#ifndef LISTA_GESTION_H
#define LISTA_GESTION_H

#include "SensorBase.h"
#include <iostream>
#include <cstring>

/**
 * @struct NodoGestion
 * @brief Nodo para la lista de gestión de sensores
 */
struct NodoGestion {
    SensorBase* sensor; ///< Puntero al sensor
    NodoGestion* sig;   ///< Puntero al siguiente nodo
    
    /**
     * @brief Constructor del nodo
     * @param s Puntero al sensor
     */
    NodoGestion(SensorBase* s) : sensor(s), sig(nullptr) {}
};

/**
 * @class ListaGestion
 * @brief Lista para administrar todos los sensores del sistema
 * 
 * Permite agregar, buscar y procesar sensores de diferentes tipos
 * de manera polimórfica. Maneja la memoria automáticamente.
 */
class ListaGestion {
private:
    NodoGestion* cabeza; ///< Primer nodo de la lista

public:
    /**
     * @brief Constructor por defecto
     */
    ListaGestion() : cabeza(nullptr) {}

    /**
     * @brief Destructor - libera todos los sensores
     * 
     * Utiliza polimorfismo para destruir correctamente cada sensor
     * según su tipo específico.
     */
    ~ListaGestion() {
        NodoGestion* tmp = cabeza;
        while (tmp) {
            NodoGestion* borr = tmp;
            tmp = tmp->sig;
            if (borr->sensor) {
                std::cout << "[Destructor] Liberando sensor: " << borr->sensor->getNombre() << "\n";
                delete borr->sensor; // Llama al destructor virtual correcto
            }
            delete borr;
        }
        cabeza = nullptr;
    }

    /**
     * @brief Inserta un nuevo sensor en la lista
     * @param s Puntero al sensor a insertar
     */
    void insertar(SensorBase* s) {
        NodoGestion* nuevo = new NodoGestion(s);
        if (!cabeza) {
            cabeza = nuevo;
            return;
        }
        NodoGestion* tmp = cabeza;
        while (tmp->sig) tmp = tmp->sig;
        tmp->sig = nuevo;
    }

    /**
     * @brief Busca un sensor por su nombre
     * @param nom Nombre del sensor a buscar
     * @return Puntero al sensor encontrado o nullptr si no existe
     */
    SensorBase* buscarPorNombre(const char* nom) {
        NodoGestion* tmp = cabeza;
        while (tmp) {
            if (std::strcmp(tmp->sensor->getNombre(), nom) == 0) {
                return tmp->sensor;
            }
            tmp = tmp->sig;
        }
        return nullptr;
    }

    /**
     * @brief Ejecuta el procesamiento polimórfico de todos los sensores
     * 
     * Recorre la lista y llama al método procesarLectura() de cada sensor,
     * ejecutando la implementación específica según el tipo de sensor.
     */
    void procesarTodos() {
        std::cout << "--- Ejecutando Procesamiento Polimórfico ---\n";
        NodoGestion* tmp = cabeza;
        while (tmp) {
            tmp->sensor->procesarLectura();
            tmp = tmp->sig;
        }
    }

    /**
     * @brief Imprime información de todos los sensores
     */
    void imprimir() const {
        std::cout << "[Lista de Sensores Registrados]\n";
        NodoGestion* tmp = cabeza;
        while (tmp) {
            tmp->sensor->imprimirInfo();
            tmp = tmp->sig;
        }
    }
};

#endif
