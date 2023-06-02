# Algoritmos de Anonimización de Datos

Este repositorio provee anonimización de datos utilizando los algoritmos de agrupación K-Means y K-Member, y los algoritmos de anonimización Incognito, Datafly y Mondrian.

# Ficheros de jerarquía

Para su correcta utilización, excepto K-Means, todos los algoritmos necesitan proveer un fichero de jerarquía como el siguiente por cada atributo cuasi-identificador que se desee utilizar.

* Fichero de jerarquía del atributo **race**:
    ```
    race
    White;*
    Asian-Pac-Islander;*
    Amer-Indian-Eskimo;*
    Other;*
    Black;*
    ```

Dichos ficheros de jerarquía deben encontrarse en el directorio **hierarchies**, relativo al dataset o conjunto de datos al que pertenece. Puede encontrarse un ejemplo en el siguiente directorio: **datasets/adult/age_categorical**.

* Estructura del directorio de un conjunto de datos:
```
 |-- [data directory]
      |
      |-- csv input file
      |-- hierarchies
           |
           |-- hierarchy tables as csv files
           |-- ....
```


En el caso de K-Member y Mondrian, esto no es necesario para los atributos numéricos, aunque pueden considerarse como categóricos si se provee su fichero de jerarquía correspondiente. El conjunto de datos adult provee ejemplos de estos dos tipos de tratamientos para el fichero **age**, el cual se puede encontrar en el directorio **datasets/adult**.

# Ejemplos de Ejecución

Pueden encontrarse ejemplos de ejecución para cada algoritmo en sus respectivas carpetas.