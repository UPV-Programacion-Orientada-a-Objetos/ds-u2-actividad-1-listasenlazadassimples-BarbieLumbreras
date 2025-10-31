#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <cstring>

#include "ListaGestion.h"
#include "SensorTemperatura.h"
#include "SensorPresion.h"

using namespace std;

// ===================== SERIAL ========================
int configurarSerial(const char* puerto) {
    int serial = open(puerto, O_RDWR | O_NOCTTY | O_NDELAY);
    if (serial < 0) {
        perror("No se pudo abrir el puerto serial");
        return -1;
    }

    fcntl(serial, F_SETFL, 0); // bloqueante

    struct termios opciones;
    tcgetattr(serial, &opciones);

    cfsetispeed(&opciones, B115200);
    cfsetospeed(&opciones, B115200);

    opciones.c_cflag |= (CLOCAL | CREAD);
    opciones.c_cflag &= ~PARENB;
    opciones.c_cflag &= ~CSTOPB;
    opciones.c_cflag &= ~CSIZE;
    opciones.c_cflag |= CS8;

    opciones.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // modo raw
    opciones.c_iflag &= ~(IXON | IXOFF | IXANY);
    opciones.c_oflag &= ~OPOST;

    tcsetattr(serial, TCSANOW, &opciones);

    return serial;
}

// Lee una línea del puerto serial hasta '\n'
bool leerLinea(int fd, char* buffer, size_t maxLen) {
    size_t idx = 0;
    while (idx < maxLen - 1) {
        char c;
        int n = read(fd, &c, 1);
        if (n > 0) {
            if (c == '\n') {
                buffer[idx] = '\0';
                return true;
            } else if (c != '\r') {
                buffer[idx++] = c;
            }
        } else {
            // no hay datos, dormir un poco
            usleep(10000); // 10ms
            // si no quieres bloquear tanto, puedes salir con false
        }
    }
    buffer[maxLen-1] = '\0';
    return true;
}

// Separa "T;T-001;25.6"
void parsearLinea(const char* linea, char* tipo, char* id, char* valor) {
    char copia[128];
    std::strncpy(copia, linea, sizeof(copia));
    copia[sizeof(copia)-1] = '\0';

    char* token = std::strtok(copia, ";");
    if (token) {
        *tipo = token[0];
    } else {
        *tipo = 'X';
        return;
    }

    token = std::strtok(nullptr, ";");
    if (token) {
        std::strncpy(id, token, 50);
        id[49] = '\0';
    } else {
        id[0] = '\0';
    }

    token = std::strtok(nullptr, ";");
    if (token) {
        std::strncpy(valor, token, 50);
        valor[49] = '\0';
    } else {
        valor[0] = '\0';
    }
}
// =====================================================

// Crea sensor según tipo y lo inserta
SensorBase* crearSensorPorTipo(char tipo, const char* id, ListaGestion& lista) {
    SensorBase* s = nullptr;
    if (tipo == 'T') {
        s = new SensorTemperatura(id);
        lista.insertar(s);
        cout << "Sensor de Temperatura '" << id << "' creado e insertado.\n";
    } else if (tipo == 'P') {
        s = new SensorPresion(id);
        lista.insertar(s);
        cout << "Sensor de Presion '" << id << "' creado e insertado.\n";
    } else {
        cout << "Tipo no valido.\n";
    }
    return s;
}

int main() {
    cout << "--- Sistema IoT de Monitoreo Polimórfico ---\n";

    ListaGestion lista;
    int fdSerial = -1;         // lo abriremos solo si el usuario quiere
    const char* puerto = "/dev/ttyUSB0";

    bool salir = false;
    while (!salir) {
        cout << "\n===== MENU =====\n";
        cout << "1. Crear sensor\n";
        cout << "2. Registrar lectura \n";
        cout << "3. Ejecutar procesamiento polimorfico\n";
        cout << "4. Listar sensores\n";
        cout << "5. Abrir/usar puerto serial y leer 1 linea del Arduino\n";
        cout << "6. Leer continuamente del Arduino (demo)\n";
        cout << "7. Salir\n";
        cout << "Elige opcion: ";
        int op;
        cin >> op;
        cin.ignore(1000, '\n'); // limpiar buffer

        if (op == 1) {
            // Crear sensor
            char tipo;
            char id[50];
            cout << "Tipo de sensor (T=Temperatura, P=Presion): ";
            cin >> tipo;
            cin.ignore(1000, '\n');
            cout << "ID del sensor (ej. T-001): ";
            cin.getline(id, 50);

            crearSensorPorTipo(tipo, id, lista);
        }
        else if (op == 2) {
            // Registrar lectura manual
            char id[50];
            char valor[50];
            cout << "ID del sensor: ";
            cin.getline(id, 50);
            SensorBase* s = lista.buscarPorNombre(id);
            if (!s) {
                cout << "No existe ese sensor.\n";
            } else {
                cout << "Valor de la lectura: ";
                cin.getline(valor, 50);
                s->agregarLecturaDesdeTexto(valor);
            }
        }
        else if (op == 3) {
            // Procesar todos
            lista.procesarTodos();
        }
        else if (op == 4) {
            // Listar
            lista.imprimir();
        }
        else if (op == 5) {
            // Leer UNA sola línea del Arduino
            if (fdSerial < 0) {
                fdSerial = configurarSerial(puerto);
                if (fdSerial < 0) {
                    cout << "No se pudo abrir el puerto.\n";
                    continue;
                }
                cout << "Esperando a que Arduino reinicie...\n";
                usleep(2000000);
            }

            char linea[128];
            cout << "Esperando 1 linea del Arduino...\n";
            if (leerLinea(fdSerial, linea, sizeof(linea))) {
                cout << "[RX] " << linea << "\n";
                char tipo;
                char id[50];
                char valor[50];
                parsearLinea(linea, &tipo, id, valor);

                // si no existe el sensor, lo creamos
                SensorBase* s = lista.buscarPorNombre(id);
                if (!s) {
                    cout << "Sensor " << id << " no existe, creando...\n";
                    s = crearSensorPorTipo(tipo, id, lista);
                }
                if (s) {
                    s->agregarLecturaDesdeTexto(valor);
                }
            } else {
                cout << "No se recibio linea.\n";
            }
        }
        else if (op == 6) {
            // Modo continuo: se queda leyendo del Arduino y cada 5 lecturas procesa
            if (fdSerial < 0) {
                fdSerial = configurarSerial(puerto);
                if (fdSerial < 0) {
                    cout << "No se pudo abrir el puerto.\n";
                    continue;
                }
                cout << "Esperando a que Arduino reinicie...\n";
                usleep(2000000);
            }
            cout << "Leyendo continuamente (Ctrl+C para matar el programa)...\n";
            int contador = 0;
            while (true) {
                char linea[128];
                if (leerLinea(fdSerial, linea, sizeof(linea))) {
                    if (std::strlen(linea) == 0)
                        continue;
                    cout << "[RX] " << linea << "\n";

                    char tipo;
                    char id[50];
                    char valor[50];
                    parsearLinea(linea, &tipo, id, valor);

                    SensorBase* s = lista.buscarPorNombre(id);
                    if (!s) {
                        cout << "Sensor " << id << " no existe, creando...\n";
                        s = crearSensorPorTipo(tipo, id, lista);
                    }
                    if (s) {
                        s->agregarLecturaDesdeTexto(valor);
                    }

                    contador++;
                    if (contador % 5 == 0) {
                        lista.procesarTodos();
                    }
                }
            }
        }
        else if (op == 7) {
            salir = true;
        }
        else {
            cout << "Opcion no valida.\n";
        }
    }

    if (fdSerial >= 0) {
        close(fdSerial);
    }

    cout << "Saliendo... (la lista se libera sola)\n";
    return 0;
}
