#include <string>
#include <stdio.h>
#include <mpi.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;

int main (int argc, char **argv)
{
  int myrank, nprocs, namelen;
  char processorName[10];
  char data[8];

  MPI_Init (&argc, &argv);
  MPI_Comm_size (MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank (MPI_COMM_WORLD, &myrank);
  MPI_Get_processor_name (processorName, &namelen);


  if (myrank == 0)
    {
   
      cout << "The total processes are:" << nprocs << endl;
      //cout<<"Im process zero"<<endl;
      char proposedDate[] = "21012020";
      for (int i = 1; i < nprocs; i++)
	{
	  MPI_Send (proposedDate, 9, MPI_CHAR, i, 888, MPI_COMM_WORLD);
	}
      char approval[2];
      string responses = "";
      for (int i = 1; i < nprocs; i++)
	{
	  MPI_Recv (approval, 2, MPI_CHAR, i, 888, MPI_COMM_WORLD,
		    MPI_STATUS_IGNORE);
	  cout << "Received a " << approval[0] << " from process:" << i <<
	    endl;
	  responses.push_back (approval[0]);
	}
  
      bool cancelFlag = false;
      for (int i = 0; i < nprocs; i++)
	{
	  if (responses[i] == 'N')
	    {
              cancelFlag = true;
	      break;	
	    }
	}
     if (cancelFlag) 
	{
	 for(int i = 1; i < nprocs; i++)
		{
			char msg[]="Sorry! party has been cancelled.";
			MPI_Send (msg, sizeof(msg), MPI_CHAR, i, 888, MPI_COMM_WORLD);

		}
	}

     else{
	 for(int i = 1; i < nprocs; i++)
		{
			char msg[]="YES! we are having a party";
			MPI_Send (msg, sizeof(msg), MPI_CHAR, i, 888, MPI_COMM_WORLD);

		}

        }



    }
  else
    {
      MPI_Recv (data, 9, MPI_CHAR, 0, 888, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      srand (myrank);
      int aRandomNumber = rand () % 2;
      char choice[] = { 'C', '\0' };
   
      if (aRandomNumber)
	{
	  choice[0] = 'Y';
	}
      else
	{
	  choice[0] = 'N';

	}
      MPI_Send (choice, 2, MPI_CHAR, 0, 888, MPI_COMM_WORLD);
      char msg[100];
      MPI_Recv (msg, 100, MPI_CHAR, 0, 888, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      cout<<"I received msg from process 0: "<<msg<<endl;



    }

  MPI_Finalize ();
  return 0;
}



