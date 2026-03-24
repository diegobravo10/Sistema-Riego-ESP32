# Sistema de Riego Inteligente con ESP32

[![ESP32](https://img.shields.io/badge/Board-ESP32-blue.svg)](https://www.espressif.com/en/products/socs/esp32)
[![C++](https://img.shields.io/badge/Language-C++-00599C.svg)](https://cplusplus.com/)
[![React](https://img.shields.io/badge/Frontend-React-61DAFB.svg)](https://reactjs.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

## Descripción del Proyecto

Este proyecto es una solución IoT para el riego automatizado y remoto basado en un microcontrolador ESP32. El sistema permite controlar una electroválvula de riego a través de una aplicación web moderna construida con React. El usuario puede encender o apagar el riego de forma manual, activar un temporizador, o dejar que el sistema opere en modo automático utilizando las lecturas físicas de un sensor de humedad del suelo.

**🌍 Acceso Global y Arquitectura:**  
La interfaz web frontend está alojada en un servidor local que ha sido expuesto a internet de forma segura a través de un túnel. Esto permite acceder fácilmente al panel de control desde cualquier parte del mundo (y en cualquier red) medianta la URL:  

👉 **[https://riego.bravoserver.fun/](https://riego.bravoserver.fun/)**

Una vez que el usuario interactúa con la interfaz web, el servidor local redirige y canaliza de forma interna (vía red local WiFi) las distintas peticiones HTTP JSON hacia la API del microcontrolador ESP32.

---

## 📑 Tabla de Contenido
- [✨ Características](#-características)
- [🛠️ Tecnologías Utilizadas](#️-tecnologías-utilizadas)
- [⚙️ Arquitectura y Diagrama de Funcionamiento](#️-arquitectura-y-diagrama-de-funcionamiento)
- [📂 Estructura del Proyecto](#-estructura-del-proyecto)
- [🚀 Instrucciones de Instalación](#-instrucciones-de-instalación)
- [💻 Ejemplo de Uso](#-ejemplo-de-uso)
- [🔮 Mejoras Futuras](#-mejoras-futuras)
- [👨‍💻 Autor](#-autor)

---

## ✨ Características

- 🌐 **Despliegue Público Activo:** Plataforma web en línea para la gestión, sin necesidad de conectarse a la misma red de la casa.
- 🎛️ **Control Manual Remoto:** Enciende y apaga la válvula de riego al instante desde la URL pública.
- ⏱️ **Modo Temporizador:** Configura el riego por una cantidad de segundos determinados y el sistema cerrará la válvula automáticamente al finalizar el conteo.
- 🤖 **Modo Automático:** Utiliza las lecturas del sensor de humedad (analógico) para activar el riego solo cuando la tierra lo requiera (se activa cuando la humedad está por debajo del 30%).
- 📊 **Monitoreo en Tiempo Real:** Visualiza la humedad actual del suelo, el estado de la válvula y los modos activos a través de la aplicación web.
- ⚡ **Servidor Asíncrono:** Respuestas rápidas, estables y sin bloqueos del microcontrolador gracias a la robusta librería `ESPAsyncWebServer`.

---

## 🛠️ Tecnologías Utilizadas

- **Hardware:**
  - Placa Microcontrolador **ESP32**
  - Módulo de Relé (Control de etapa de potencia de la válvula en pin 26)
  - Sensor Analógico de Humedad del Suelo (Lectura en pin 34)
  - Electroválvula de Riego (12V/24V)

- **Software / Firmware:**
  - IDE de Arduino
  - C++ (Arduino Core para ESP32)
  - `WiFi.h` (Conexión de red)
  - `ESPAsyncWebServer` & `AsyncTCP` (Configuración del servidor local en el ESP32)

- **Servidor y Frontend:**
  - Túnel web seguro para el dominio y SSL (`bravoserver.fun`)
  - Node.js / Servidor Local
  - React.js
  - HTML5 & CSS3
  - API Fetch nativa

---

## ⚙️ Arquitectura y Diagrama de Funcionamiento

```text
 +--------------------------+        Acceso Seguro (Túnel)       +----------------------------+
 |                          |  https://riego.bravoserver.fun/    |                            |
 | 🌍 Usuario Remoto        | <================================> | 🖥️ Servidor Local         |
 |    (Redes Móviles/WiFi)  |                                    |    (Frontend React Host)   |
 |                          |                                    +----------------------------+
 +--------------------------+                                         |                 ^
                                                                      | Red Local (WiFi)|
                                                Peticiones locales    v         JSON    |
                                                                 +----------------------------+
                                                                 | 📟 ESP32 Microcontrolador  |
                                                                 |    (ESPAsyncWebServer)     |
                                                                 +----------------------------+
                                                                      |                 |
                                                                      v                 v  (Señal Analógica)
                                                          +------------------+  +-------------------+
                                                          | 💧 Módulo Relé   |  | 🌱 Sensor de      |
                                                          |    & Válvula     |  |    Humedad (P34)  |
                                                          +------------------+  +-------------------+
```

---

## 📂 Estructura del Proyecto

El repositorio está dividido principalmente en dos dominios de desarrollo:

```text
Sistema-Riego-ESP32/
├── appriego/               # Aplicación Web Frontend (React) que corre en el Servidor Local
│   ├── src/                # Componentes y código principal
│   ├── public/             # Recursos estáticos
│   └── package.json        # Dependencias de npm e información del proyecto
│
└── riego/                  # Código Firmware (ESP32)
    └── riego.ino           # Script de arduino en C++ conteniendo las lógicas de red y lectura
```

---

## 🚀 Instrucciones de Instalación

### 1. Configuración del ESP32 (Firmware)
1. Abre el archivo [riego/riego.ino](cci:7://file:///c:/Users/user/OneDrive/Documentos/PROYECTOSDIEGO/Sistema-Riego-ESP32/riego/riego.ino:0:0-0:0) en **Arduino IDE** (o VSCode con PlatformIO).
2. Asegúrate de tener instaladas las librerías `ESPAsyncWebServer` y `AsyncTCP` (puedes bajarlas desde GitHub e importarlas como archivo .ZIP).
3. Modifica las credenciales de tu red WiFi en el código fuente:
   ```cpp
   const char* ssid = "TU_RED_WIFI";
   const char* password = "TU_PASSWORD";
   ```
4. Conecta tu ESP32 al PC, selecciona la placa y el puerto en tu IDE y sube el código.
5. Abre el Monitor Serie de Arduino configurado a **115200 baudios** para visualizar la conexión exitosa y copiar la **dirección IP real o local** asignada a la placa en tu red.

### 2. Configuración del Servidor Web (React)
1. Abre una terminal en el servidor donde correrá la interfaz y navega a la carpeta de la misma:
   ```bash
   cd appriego
   ```
2. Instala las dependencias y paquetes de Node.js:
   ```bash
   npm install
   ```
3. *(En el código de envío de solicitudes o fetch de React, asegúrate de utilizar o apuntar a la dirección IP local del ESP32 obtenida en el paso previo).*
4. Inicia la aplicación para el pase a producción y configúrala a través del túnel seleccionado (Cloudflare Tunnel, Ngrok, proxy inverso Nginx, etc.).

---

## 💻 Ejemplo de Uso

1. **Accede a la Interfaz Pública:** Desde cualquier conexión a internet (incluso si no estás en casa), visita **[https://riego.bravoserver.fun/](https://riego.bravoserver.fun/)**.
2. **Revisa la Humedad:** Constata de forma visual el porcentaje de hidratación actual del terreno para decidir cuáles son los requerimientos del cultivo.
3. **Control Manual:** Haz clic en el botón de estado manual pasando a **"ON"**. A través del túnel web, tu servidor local retransmitirá ese comando la IP del ESP32. El módulo de relé conmutará en ese preciso instante y el riego se abrirá. 
4. **Prueba el Modo Auto:** Activa la condición de trabajo automático en el Panel Web. Si el nivel de agua resulta óptimo y tu porcentaje de lectura supera el 30%, la válvula se mantendrá cortada. De lo contrario, activará el flujo automáticamente.

---

## 🔮 Mejoras Futuras

- [ ] ☁️ **Sincronización de Base de Datos Remota:** Implementar persistencia de estados en Firebase / MongoDB por si el servidor físico se reinicia.
- [ ] 📈 **Dashboards y Analítica:** Implementar métricas usando gráficos a través del tiempo sobre parámetros cruzados de humedad de tierra versus tiempo de riego.
- [ ] 🌦️ **Regulación Climática Avanzada:** Consumo de APIs (Ej: OpenWeatherMap) para denegar el riego automático si se pronostica fuerte lluvia por la tarde en la misma locación geográfica.
- [ ] 🔔 **Alertas mediante Notificaciones:** Implementación de Push Notifications (PWAs) por Telegram/WhatsApp informando cada vez que la bomba/válvula se activan mas allá de las programaciones establecidas.

---

## 👨‍💻 Autor

Diseñado, desarrollado e implementado por **Diego Bravo** - [GitHub @diegobravo10](https://github.com/diegobravo10) | Aplicación Web: **[riego.bravoserver.fun](https://riego.bravoserver.fun/)**

