/**
 * @file SensorBase.h
 * @brief Clase base abstracta para todos los tipos de sensores
 * @author Barbie
 * @date 2025
 */

#ifndef SENSOR_BASE_H
#define SENSOR_BASE_H

#include <iostream>
#include <cstring>

/**
 * @class SensorBase
 * @brief Clase base abstracta que define la interfaz común para todos los sensores
 * 
 * Esta clase proporciona la estructura básica que deben seguir todos los sensores
 * del sistema IoT. Define métodos virtuales puros que cada sensor específico
 * debe implementar según su tipo.
 */
class SensorBase {
protected:
    char nombre[50]; ///< Identificador único del sensor (ej. "T-001", "P-105")

public:
    /**
     * @brief Constructor de la clase base
     * @param nom Nombre identificador del sensor
     */
    SensorBase(const char* nom = "SIN-NOMBRE") {
        std::strncpy(nombre, nom, sizeof(nombre));
        nombre[sizeof(nombre)-1] = '\0';
    }

    /**
     * @brief Destructor virtual para permitir polimorfismo
     */
    virtual ~SensorBase() {
        // Debe ser virtual para liberar correctamente los recursos
    }

    /**
     * @brief Obtiene el nombre del sensor
     * @return Puntero al nombre del sensor
     */
    const char* getNombre() const {
        return nombre;
    }

    /**
     * @brief Agrega una lectura al sensor desde texto
     * @param valorTxt Valor de la lectura en formato texto
     * 
     * Método virtual puro que cada sensor debe implementar para
     * convertir y almacenar lecturas desde formato texto.
     */
    virtual void agregarLecturaDesdeTexto(const char* valorTxt) = 0;

    /**
     * @brief Procesa las lecturas del sensor
     * 
     * Método virtual puro que implementa la lógica específica
     * de procesamiento para cada tipo de sensor.
     */
    virtual void procesarLectura() = 0;

    /**
     * @brief Imprime información del sensor
     * 
     * Método virtual puro para mostrar información específica
     * de cada tipo de sensor.
     */
    virtual void imprimirInfo() const = 0;
};

#endif
