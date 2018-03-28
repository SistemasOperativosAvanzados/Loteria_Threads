# Loteria_Threads

EL proposito de este proyecto es crear y controlar threads a nivel de codigo de usuario (sin intervencion del kernel).

El programa recibe los siguienes parametros:

- Modo de operacion (expropiativo o no expropiativo)
- Numero de threads (minimo 5, maximo sin defini aun)
- Cantidad de boletos para cada thread (numero entero)
- Tamanio del quantum en milisegundos (para la version expropiativa) o porcentaje del trabajo a realizar antes de ceder voluntariamente el procesador ( para la version no expropiativa).


# Instalacion

1. sudo apt-get install build-essential 
   Para actualizar e instalar las librerias correspondientes a C.

2. sudo apt-get install libgtk-3-dev
	Instalar gtk.

3. sudo apt-get install glade
	Instala glade.


# How to run?

	Run make
	Sino se requiere del comando 

	gcc -Wall -g -o main main.c finalLottery.c `pkg-config --cflags --libs gtk+-3.0` -w -lm

# Ejemplos

	Los archivos con nombre threadFile son los que se utilizan para cargar en la aplicacion y visualizar el progreso de cada thread.

# Autores

a. Sleyter Angulo Chavarria.
b. Kathy Brenes Guerrero.
c. Samuel Yoo.
