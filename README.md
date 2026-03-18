# 🌱 Sistema de Riego (ESP32 IoT)

Este proyecto es un sistema de riego inteligente basado en el microcontrolador **ESP32**. Permite controlar una electroválvula de 12V mediante una interfaz web local, facilitando la automatización del cuidado de plantas o cultivos pequeños.

##  Características
- **Control Web Real-Time:** Interfaz minimalista para encender/apagar el riego desde cualquier dispositivo en la red WiFi.
- **Monitoreo de Humedad:** Integración con sensores capacitivos para lectura de humedad del suelo (v2.0).
- **Arquitectura Asíncrona:** Uso de `ESPAsyncWebServer` para un manejo eficiente de peticiones sin bloquear el procesador.
- **Hardware Robusto:** Aislamiento mediante relés para manejo de cargas de 12V.

##  Hardware Utilizado
| Componente | Descripción |
| :--- | :--- |
| **Microcontrolador** | ESP32 DevKit V1 (Chip Serial CH9102) |
| **Actuador** | Módulo Relé 5V (Optoacoplado) |
| **Válvula** | Electroválvula Solenoide 12V DC |
| **Sensor** | Capacitive Soil Moisture Sensor v2.0 |
| **Alimentación** | Fuente externa 12V (Válvula) + USB 5V (ESP32) |

##  Diagrama de Conexiones (Pinout)
- **Relé (IN):** GPIO 26
- **Sensor Humedad (Analog):** GPIO 34
- **VCC Relé/Sensor:** VIN (5V)
- **GND:** Unificado

##  Instalación y Configuración

1. **Librerías Necesarias:**
   Instala las siguientes dependencias en el Arduino IDE:
   - [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
   - [AsyncTCP](https://github.com/me-no-dev/ESPAsyncTCP)

2. **Configuración WiFi:**
   Edita las variables `ssid` y `password` en el archivo `.ino` con tus credenciales locales.

3. **Carga del código:**
   Selecciona la placa `DOIT ESP32 DEVKIT V1` y el puerto COM correspondiente a tu chip CH9102.

##  Interfaz Web
La interfaz permite visualizar el estado actual de la válvula y alternar su estado mediante botones dinámicos de fácil acceso.

---
Creado por Diego Bravo - 2026 🇪🇨