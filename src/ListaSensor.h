/**
 * @file ListaSensor.h
 * @brief Template de lista enlazada para almacenar lecturas de sensores
 * @author Barbie
 * @date 2025
 */

#ifndef LISTA_SENSOR_H
#define LISTA_SENSOR_H

#include <iostream>

/**
 * @struct NodoLS
 * @brief Nodo para la lista enlazada de lecturas
 * @tparam T Tipo de dato que almacena el nodo
 */
template <typename T>
struct NodoLS {
    T dato;           ///< Valor almacenado en el nodo
    NodoLS<T>* sig;   ///< Puntero al siguiente nodo
    
    /**
     * @brief Constructor del nodo
     * @param d Dato a almacenar
     */
    NodoLS(const T& d) : dato(d), sig(nullptr) {}
};

/**
 * @class ListaSensor
 * @brief Lista enlazada template para almacenar historial de lecturas
 * @tparam T Tipo de dato de las lecturas (int, float, etc.)
 * 
 * Implementa una lista enlazada con operaciones específicas para
 * el manejo de datos de sensores como cálculo de promedios y
 * eliminación de valores extremos.
 */
template <typename T>
class ListaSensor {
private:
    NodoLS<T>* cabeza; ///< Puntero al primer nodo de la lista

public:
    /**
     * @brief Constructor por defecto
     */
    ListaSensor() : cabeza(nullptr) {}

    /**
     * @brief Destructor - libera toda la memoria
     */
    ~ListaSensor() {
        limpiar();
    }

    /**
     * @brief Constructor de copia
     * @param other Lista a copiar
     */
    ListaSensor(const ListaSensor& other) : cabeza(nullptr) {
        NodoLS<T>* aux = other.cabeza;
        while (aux) {
            insertarFinal(aux->dato);
            aux = aux->sig;
        }
    }

    /**
     * @brief Operador de asignación
     * @param other Lista a asignar
     * @return Referencia a esta lista
     */
    ListaSensor& operator=(const ListaSensor& other) {
        if (this != &other) {
            limpiar();
            NodoLS<T>* aux = other.cabeza;
            while (aux) {
                insertarFinal(aux->dato);
                aux = aux->sig;
            }
        }
        return *this;
    }

    /**
     * @brief Inserta un valor al final de la lista
     * @param valor Valor a insertar
     */
    void insertarFinal(const T& valor) {
        NodoLS<T>* nuevo = new NodoLS<T>(valor);
        if (!cabeza) {
            cabeza = nuevo;
            return;
        }
        NodoLS<T>* tmp = cabeza;
        while (tmp->sig) tmp = tmp->sig;
        tmp->sig = nuevo;
    }

    /**
     * @brief Verifica si la lista está vacía
     * @return true si está vacía, false en caso contrario
     */
    bool estaVacia() const {
        return cabeza == nullptr;
    }

    /**
     * @brief Cuenta el número de elementos en la lista
     * @return Número de elementos
     */
    int contar() const {
        int c = 0;
        NodoLS<T>* tmp = cabeza;
        while (tmp) {
            c++;
            tmp = tmp->sig;
        }
        return c;
    }

    /**
     * @brief Calcula el promedio de todos los valores
     * @return Promedio de los valores almacenados
     */
    T promedio() const {
        T suma = 0;
        int c = 0;
        NodoLS<T>* tmp = cabeza;
        while (tmp) {
            suma += tmp->dato;
            c++;
            tmp = tmp->sig;
        }
        if (c == 0) return 0;
        return suma / c;
    }

    /**
     * @brief Elimina el valor menor de la lista
     * 
     * Útil para filtrar lecturas erróneas o valores extremos
     * que puedan afectar el análisis de datos.
     */
    void eliminarMenor() {
        if (!cabeza || !cabeza->sig) {
            return; // 0 o 1 elemento, no hay nada que eliminar
        }
        
        NodoLS<T>* menor = cabeza;
        NodoLS<T>* antMenor = nullptr;

        NodoLS<T>* ant = nullptr;
        NodoLS<T>* cur = cabeza;
        while (cur) {
            if (cur->dato < menor->dato) {
                menor = cur;
                antMenor = ant;
            }
            ant = cur;
            cur = cur->sig;
        }

        if (antMenor == nullptr) {
            cabeza = cabeza->sig;
            delete menor;
        } else {
            antMenor->sig = menor->sig;
            delete menor;
        }
    }

    /**
     * @brief Limpia toda la lista liberando memoria
     */
    void limpiar() {
        NodoLS<T>* tmp = cabeza;
        while (tmp) {
            NodoLS<T>* borr = tmp;
            tmp = tmp->sig;
            delete borr;
        }
        cabeza = nullptr;
    }

    /**
     * @brief Imprime todos los valores de la lista
     */
    void imprimir() const {
        NodoLS<T>* tmp = cabeza;
        while (tmp) {
            std::cout << tmp->dato << " -> ";
            tmp = tmp->sig;
        }
        std::cout << "NULL\n";
    }
};

#endif
