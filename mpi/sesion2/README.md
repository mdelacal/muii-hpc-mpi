# example1.c
Ejecución:
mpicc -o example1 example1.c 
mpirun -np 2 example1

Se envían mensajes y se reciben mutuamente, entre 2 procesos (los workers, es decir los no maestros, son los que envían)
El send me bloquea hasta que estoy seguro.

Bloqueante: no voy a retornar de la invocación del send o receive hasta que no sea seguro manipular la variable.
En cuanto el proceso copia los datos en el buffer del sistema, ya retorna.

MPI_Ssend: invocación síncrona
Síncrono: El que envía hasta que no está seguro de que el que recibe el mensaje haya recibido el mensaje, no retorna

Si no es síncrono no tienes la certeza de que el otro haya recibido el mensaje

Cuando es no bloqueante retorno inmediatamente, aunque lo completo con la primitiva MPI wait (hasta que no esté seguro no puedo enviar, pero puedo hacer otras cosas - el maestro puede hacer un procesamiento de datos en pipelining) (síncrono y no bloqueante no se suele utilizar)

Asíncrono: termino, hago los cálculos y lo envío (sin recibir un acknowledge)

+++ ver diapositiva 5 de la sesion 2


# Barrier
Hasta que todos los procesos no hayan llegado a un punto, no podemos continuar

# Reduction
Espera a que el resto de procesos completen la operación de reducción, y aplicar una operación sobre los datos recibidos (es una operación bastante habitual)


# mpi_bug1
El comportamiento erróneo detectado consiste en que el programa se queda colgado, a la espera de recibir los mensajes entre las tareas.
Este problema se debe a las tags utilizadas en las primitivas MPI_Send y MPI_Recv, las cuales no se diferencian en los dos envíos de mensajes que existen.
Para corregir este problema, bastaría con utilizar tags diferentes en los envíos de ambos mensajes. De esta manera, task0 sabe qué debe recibir de task1, y lo mismo con task1, sabiendo qué debe recibir de task0.

# mpi_bug2
El comportamiento erróneo detectado consiste en que el programa mezcla tipos de datos a la hora de enviar y recibir mensajes, por lo que el resultado final de los mensajes no es el deseado.
Este problema se debe al tipo de dato (int != float) utilizado para el envío y recepción del mensaje.
Para corregir este problema, bastaría con utilizar el mismo tipo de dato tanto para el envío como recepción del mensaje (valores de las variables alpha y beta). De esta manera, si task0 envía un mensaje a task1, el formato del dato será recibido correctamente por task1.

# mpi_bug3
El comportamiento erróneo detectado consiste en que el programa NO hace uso de llamadas a las funciones MPI_Init y MPI_Finalize, por lo que no es posible inicializar y terminar el programa MPI en cuestión.
Este problema se debe a que es necesario en un programa MPI hacer uso de las llamadas principales de inicialización y terminación.
Para corregir este problema, bastaría con añadir las llamadas a las funciones MPI_Init y MPI_Finalize en el lugar que corresponde, es decir, al principio y al final del método fin.

# mpi_bug4
El comportamiento erróneo detectado consiste en que el programa no devuelve el resultado correcto de la suma final.
Este problema se debe a que el proceso MASTER (task0) no hace uso de llamadas colectivas, por lo que no participa en la suma final como las otras tres tareas.
Para corregir este problema, bastaría con añadir la llamada a la función MPI_Reduce al igual que lo hacen las demás tareas. De esta manera, utilizaríamos la operación de reducción para así obtener la suma final deseada.

