/**
 * @file SensorPresion.h
 * @brief Implementación específica para sensores de presión
 * @author Barbie
 * @date 2025
 */

#ifndef SENSOR_PRESION_H
#define SENSOR_PRESION_H

#include "SensorBase.h"
#include "ListaSensor.h"
#include <cstdlib>

/**
 * @class SensorPresion
 * @brief Sensor especializado para medir presión atmosférica
 * 
 * Hereda de SensorBase e implementa funcionalidades específicas
 * para el manejo de datos de presión atmosférica.
 */
class SensorPresion : public SensorBase {
private:
    ListaSensor<int> historial; ///< Historial de lecturas de presión

public:
    /**
     * @brief Constructor del sensor de presión
     * @param nom Identificador único del sensor
     */
    SensorPresion(const char* nom) : SensorBase(nom) {}

    /**
     * @brief Destructor virtual
     */
    virtual ~SensorPresion() {
        // El historial se destruye automáticamente
    }

    /**
     * @brief Agrega una lectura de presión desde texto
     * @param valorTxt Valor de presión en formato texto
     * 
     * Convierte el texto a entero y lo almacena en el historial.
     */
    void agregarLecturaDesdeTexto(const char* valorTxt) override {
        int v = atoi(valorTxt);
        historial.insertarFinal(v);
        std::cout << "[Log] Insertando lectura de presión en " << nombre << ": " << v << " hPa\n";
    }

    /**
     * @brief Procesa las lecturas de presión
     * 
     * Calcula el promedio de todas las lecturas de presión
     * almacenadas en el historial.
     */
    void procesarLectura() override {
        std::cout << "-> Procesando Sensor " << nombre << " (Presión)\n";
        if (historial.estaVacia()) {
            std::cout << "   No hay lecturas disponibles.\n";
            return;
        }
        int prom = historial.promedio();
        std::cout << "   Presión promedio: " << prom << " hPa\n";
    }

    /**
     * @brief Imprime información del sensor
     */
    void imprimirInfo() const override {
        std::cout << "[SensorPresion] ID=" << nombre << "\n";
    }
};

#endif
