#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
  int numtasks, rank, dest, source, rc, count, tag=1;
  
  int npoints = 10000;
  int circle_count = 0;
  int p = &numtasks;

  char inmsg, outmsg='x';
  MPI_Status Stat;

  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  for (size_t i = 1; i < numtasks; i++)
  {
    if (rank == 0){
    	dest = i+1;
    	source = i;
    	outmsg='S';

    	rc = MPI_Send(&outmsg, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
    	rc = MPI_Recv(&inmsg, 1, MPI_INT, i, tag, MPI_COMM_WORLD, &Stat);  
    }else{
			
		}
		
    
  }  

  printf("Mensaje recibido: %c\n",inmsg);
  MPI_Finalize();
}
