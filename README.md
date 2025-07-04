# Proyecto X  
**Analizador Portátil de Bajo Costo para Detección de Adulteración en Sueros**


## Descripción  
En marzo de 2025 se detectó en Perú un lote adulterado de suero fisiológico al 0.9 % (NaCl) que causó múltiples efectos adversos, incluidos casos de hipernatremia y fallecimientos. Este incidente reveló la necesidad de una herramienta rápida y accesible para verificar la calidad de soluciones intravenosas **in situ**, sin dependencia de laboratorios centralizados.


## Problema  
- **Falta de chequeos rápidos** en puntos de atención primaria y zonas rurales.  
- Equipos comerciales de validación son **costosos**, voluminosos y requieren personal especializado.  
- **Riesgo clínico** elevado si se administran sueros con concentración de NaCl fuera de rango.


## Solución propuesta  
Un conductímetro portátil basado en **Arduino Nano**, que integra:  
1. **Sensor TDS (Gravity V1.0)** con curva polinómica cúbica para estimar concentración de NaCl (ppm).  
2. **Sensor de temperatura DS18B20** con compensación térmica en tiempo real.  
3. **Pantalla OLED 1.3″ SSD1306** para visualización instantánea.  
4. **Alertas multisensoriales** (visual en OLED + sonora con buzzer) y flujo guiado de operación.


## Diagrama de conexiones  
![Diagrama de conexiones completo](diagrama_conexiones.png)
*Figura: Conexiones entre batería LiPo, módulos auxiliares, Arduino Nano, sensores y pantalla OLED.*


## Modelos de componentes

| Componente                      | Modelo / SKU                 |
|---------------------------------|------------------------------|
| Batería LiPo 3.7 V 2000 mAh      | E585460‑4121                 |
| Módulo de carga                 | TP4056                       |
| Elevador de voltaje             | MT3608 (LM2596S)             |
| Microcontrolador                | Arduino Nano (CH340)         |
| Pantalla OLED 1.3″              | SSD1306 I²C, 128 × 64         |
| Sensor TDS analógico            | Gravity TDS V1.0 (SEN0244)   |
| Sensor de temperatura           | DS18B20 (Digital 1‑Wire)     |
| Pulsador 12 × 12 mm               | Generic Push‑Button          |
| Switch ON/OFF                   | Generic Mini Toggle          |
| Buzzer                          | Generic Active Buzzer        |


## Compensación de temperatura  
La conductividad eléctrica varía con la temperatura del medio. Para corregirlo, se aplica en el código.
