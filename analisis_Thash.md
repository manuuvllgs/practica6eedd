# Análisis de tablas de dispersión

* *Daniel del Salto Abril*
* *Manuel Villegas Aceituno*

## Tamaño de tabla: 14713
| función            | máximo de colisiones |  factor de carga  |    promedio de colisiones    | Tiempo de Búsqueda |
|--------------------|:--------------------:|:-----------------:|:----------------------------:|--------------------|
| Cuadrática         |          16          |       0.679739       |           0.745625           | 0                  |
| Dispersión Doble 1 |          39          |       0.679739       |           1.08509            | 0.001              |
| Dispersion Doble 2 |         14         |       0.679739      |            0.688431            | 0.001              |

## Tamaño de tabla: 15391
| función                        | máximo de colisiones |  factor de carga  |    promedio de colisiones     | Tiempo de Búsqueda |
|--------------------------------|:--------------------:|:-----------------:|:-----------------------------:|--------------------|
| Cuadrática                     |          19          |       0.649795       |           0.721228            | 0                  |
| Dispersión Doble 1             |          52          |       0.649795      |             0.943106            | 0                  |
| Dispersion Doble 2             |         16           |       0.649795      |             0.627437             | 0                  |



## Justificación de la configuración elegida
Las funcines son las siguientes:
Cuadrática: h(x) = (x + i^2) % t

Dispersión doble1 : h1(x) = x * q - 1
h2(x) = q + 2q
h(x) = (h1(x) + i * h2(x)) % t

Dispersión doble2 : h1(x) = (x * q)^2 - x
h2(x) = 31 + x + q/2
/h(x) = (h1(x) + i * h2(x)) % t

Hemos optado por coger la funcion de dispersion doble 2  con el lambda = 0,65 ya que es la que menos promedio de 
colisiones nos provoca.

Como podemos comprobar, las búsquedas son casi instantáneas, por lo que las funciones hash parecen bien elegidas.


