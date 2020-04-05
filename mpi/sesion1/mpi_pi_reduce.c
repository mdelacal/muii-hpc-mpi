/********************************************************************************************
 * Autor: Miguel de la Cal Bravo
 * Asignatura: Computación de Altas Prestaciones (HCP)
 * Práctica MPI #1: "Aproximar el valor de pi usando MPI con llamadas colectivas"
 *
 * Referencias:
 * https://linux.die.net/man/3/srandom
 * https://stackoverflow.com/questions/6218399/how-to-generate-a-random-number-between-0-and-1
 ********************************************************************************************/

/* Bibliotecas requeridas */
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

/* Funciones */
void srandom(unsigned int seed); /* generar aleatorios mediante un seed igual al taskid */
double throw_darts(int darts);   /* lanzar dardos para calcular pi */

/* Constantes */
#define DARTS 10000                /* número de dardos lanzados */
#define PLAYS 100                  /* número de iteraciones-partidas de lanzar dardos */
#define MASTER 0                   /* ID de tarea del maestro */
#define PI_VALUE 3.141592653589793 /* valor real de pi con 15 decimales */

/* Main */
int main(int argc, char *argv[])
{
   /* Variables */
   double current_pi;   /* valor de pi calculado por la tarea actual */
   double pi_total;     /* suma de los valores de pi por tareas */
   double pi_mean;      /* media de pi después de tirar los dardos */
   double pi_final = 0; /* valor medio final de pi, lo inicializamos a 0 */
   int taskid;          /* ID de la tarea - también se usa para etiquetar la tarea actual */
   int p;               /* número de tareas */
   int rc;              /* return code */

   /* Número de tareas e ID de la tarea (taskid) */
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &p);
   MPI_Comm_rank(MPI_COMM_WORLD, &taskid);

   /* Las tareas comienzan en orden aleatorio, su taskid vale entre 0 y 3, 
      ya que lo lanzamos con un np=4 */
   printf("¡La tarea MPI #%d ha comenzado!\n", taskid);

   /* Asignamos una etiqueta (seed) para generar números aleatorios, según el ID de la tarea 
      https://linux.die.net/man/3/srandom */
   srandom(taskid);

   /* El cálculo de pi lo haremos en varias partidas o PLAYS, para obtener un resultado más exacto */
   for (int j = 0; j < PLAYS; j++)
   {
      /* Todas las tareas calculan el valor de pi lanzando los dardos */
      current_pi = throw_darts(DARTS);

      /* Hacemos uso de la primitiva MPI_Reduce, usando así llamadas MPI colectivas */
      rc = MPI_Reduce(&current_pi, &pi_total, 1, MPI_DOUBLE, MPI_SUM, MASTER, MPI_COMM_WORLD);

      /* El maestro realiza los cálculos */
      if (taskid == MASTER)
      {
         pi_mean = pi_total / p;
         pi_final = ((pi_final * j) + pi_mean) / (j + 1);
         /* Resultado calculado al final de la partida */
         printf("Valor calculado de pi = %10.15f, tras %d intentos\n", pi_final, DARTS * (j + 1));
      }
   }

   /* Resultados finales del algoritmo, el taskid se corresponde con el del maestro */
   if (taskid == MASTER)
   {
      printf("\n¡El algoritmo ha terminado!");
      /* Mostramos los resultados finales del proceso */
      printf("\n\n\tCálculo de pi con np=%d", p);
      printf("\n\tValor REAL de pi: \t\t\t\t%10.15f", PI_VALUE);
      printf("\n\tValor CALCULADO de pi: \t\t\t\t%10.15f", pi_final);
      printf("\n\tDiferencia entre el valor REAL y el CALCULADO: %10.15f\n\n", PI_VALUE - pi_final);
   }

   MPI_Finalize();
   return 0;
}

/* Función que genera los lanzamientos de dardos, para el cálculo de pi */
double throw_darts(int darts)
{
/* Función definida para generar aleatorios entre 0 y 1 (respuesta con más votos del link)
      https://stackoverflow.com/questions/6218399/how-to-generate-a-random-number-between-0-and-1 */
#define r2() (double)rand() / (double)RAND_MAX

   /* Variables */
   double random1, random2;         /* números aleatorios para generar coordenadas X e Y */
   double xcoordinate, ycoordinate; /* coordinadas X e Y, valores entre 0 y 1*/
   double pi_computed;              /* valor de pi calculado */
   int circle_count = 0;            /* número de puntos obtenido, dardos que han caído dentro del círculo */

   /* Lanzamos los dardos */
   for (int j = 1; j <= darts; j++)
   {
      /* Generamos números aleatorios para las coordenadas X e Y, con valores entre 0 y 1 */
      random1 = r2();
      xcoordinate = (2.0 * random1) - 1.0;
      random2 = r2();
      ycoordinate = (2.0 * random2) - 1.0;

      /* Si el dardo lanzado cae dentro del círculo, aumento los puntos */
      if ((xcoordinate * xcoordinate + ycoordinate * ycoordinate) <= 1.0)
         circle_count++;
   }

   /* Calculamos el valor de pi, después de haber tirado todos los dardos */
   pi_computed = 4.0 * (double)circle_count / (double)darts;
   return (pi_computed);
}
