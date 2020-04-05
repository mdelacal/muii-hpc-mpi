# Compilar
mpicc -o <ejecutable> <fichero.c>
Ejemplo: mpicc -o example0 example0.c


# Ejecutar
mpirun -np <nº de procesos> <-nooversubscribe> <ejecutable>
-nooversubscribe: si ponemos un np>4 procesadores que tiene mi portátil, entonces fallará

El proceso 0 es el proceso MAESTRO (su rank es igual a 0), recibe las salidas estándar (están redirigidas) del resto de procesos hijos, mientras que su salida estándar es NULL.
Es el único que puede tener entrada estándar (STDIN)
Es posible definir grupos de procesos de comunicación, para indicar quién puede recibir mensajes a nivel de organización lógica, creando grupos y subgrupos.


# MPI_Send (blocking sends)
MPI_Send(buffer,count,type,dest,tag,comm)

buffer: mensaje
count: nº de elementos a enviar que tiene el buffer
type: tipo de elementos, etiquetas
dest: destinatario, es el taskID o el rank del proceso al cual tengo que enviar
tag: etiqueta, información asociada al mensaje que puede usar el receptor
comm: comunicador, por defecto utilizaremos el comunicador por defecto MPI_COMM_WORLD

# Ejercicios propuestos
# PI
El esclavo calcula PI y enviará (send) al maestro (receive) el nº de elementos que ha caído dentro. Cambiará la parte del algoritmo de PI.
Tanto destino como etiqueta tienen que ser el mismo