/**
 * @file SensorTemperatura.h
 * @brief Implementación específica para sensores de temperatura
 * @author Barbie
 * @date 2025
 */

#ifndef SENSOR_TEMPERATURA_H
#define SENSOR_TEMPERATURA_H

#include "SensorBase.h"
#include "ListaSensor.h"
#include <cstdlib>

/**
 * @class SensorTemperatura
 * @brief Sensor especializado para medir temperatura
 * 
 * Hereda de SensorBase e implementa funcionalidades específicas
 * para el manejo de datos de temperatura, incluyendo filtrado
 * de valores extremos y cálculo de promedios.
 */
class SensorTemperatura : public SensorBase {
private:
    ListaSensor<float> historial; ///< Historial de lecturas de temperatura

public:
    /**
     * @brief Constructor del sensor de temperatura
     * @param nom Identificador único del sensor
     */
    SensorTemperatura(const char* nom) : SensorBase(nom) {}

    /**
     * @brief Destructor virtual
     */
    virtual ~SensorTemperatura() {
        // El historial se destruye automáticamente
    }

    /**
     * @brief Agrega una lectura de temperatura desde texto
     * @param valorTxt Valor de temperatura en formato texto
     * 
     * Convierte el texto a float y lo almacena en el historial.
     */
    void agregarLecturaDesdeTexto(const char* valorTxt) override {
        float v = static_cast<float>(atof(valorTxt));
        historial.insertarFinal(v);
        std::cout << "[Log] Insertando lectura de temperatura en " << nombre << ": " << v << "°C\n";
    }

    /**
     * @brief Procesa las lecturas de temperatura
     * 
     * Elimina la temperatura más baja (posible error de lectura)
     * y calcula el promedio de las lecturas restantes.
     */
    void procesarLectura() override {
        std::cout << "-> Procesando Sensor " << nombre << " (Temperatura)\n";
        if (historial.estaVacia()) {
            std::cout << "   No hay lecturas disponibles.\n";
            return;
        }
        
        historial.eliminarMenor(); // Filtrar posibles lecturas erróneas
        float prom = historial.promedio();
        std::cout << "   Temperatura promedio (sin valor mínimo): " << prom << "°C\n";
    }

    /**
     * @brief Imprime información del sensor
     */
    void imprimirInfo() const override {
        std::cout << "[SensorTemperatura] ID=" << nombre << "\n";
    }
};

#endif
