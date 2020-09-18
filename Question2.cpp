#include "mpi.h"
#include <stdio.h>
#include<unistd.h>
#include <iostream>
using namespace std;

int main (int argc, char *argv[])
{


  int myid, numprocs, counter, procCounter, dataCounter;
  MPI_Request request;

  MPI_Status status;

  MPI_Init (&argc, &argv);
  MPI_Comm_size (MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank (MPI_COMM_WORLD, &myid);

  int inputDataSize = (numprocs - 1) * 10;
  int localDataSize = 10;	//data/numprocs-1
  int globaldata[inputDataSize];
  int localdata[localDataSize];

  int keyToSearch = 17;//change it to check for a different key
  int isRecieved=0;

  //process 0 prepares the data
  if (myid == 0)
    {
      for (counter = 0; counter < inputDataSize; counter++)
	{
	  globaldata[counter] = 2 * counter + 1;
	}
      //it is better to write a print function here
      printf ("Process %d has input data: ", myid);
      for (counter = 0; counter < inputDataSize; counter++)
	{
	  printf ("%d ", globaldata[counter]);

	}
      printf ("\n");

      int dataToSend[localDataSize];
      int dataStart = 0;
      for (procCounter = 1; procCounter < numprocs; procCounter++)
	{
	  //preparing data to be sent to each process
	  for (dataCounter = 0; dataCounter < localDataSize; dataCounter++)
	    {
	      dataToSend[dataCounter] = globaldata[dataStart + dataCounter];
	    }
	  MPI_Send (dataToSend, localDataSize, MPI_INT, procCounter, 000,
		    MPI_COMM_WORLD);
	  dataStart += localDataSize;
	}
      //The process with rank 0, waits the response from other processes 
      //If the response if found i.e. 'F' then asks all other processes to stop
      //In case it is not found the missionComplete var remains 0 at the end it
      //represents the that given key was not presesnt.
      int counter = 1;
      int missionComplete = 0;
      while (counter < numprocs)
	{
	  char msgFromSlave[100];
	  MPI_Recv (msgFromSlave, sizeof (msgFromSlave), MPI_CHAR,
		    MPI_ANY_SOURCE, 000, MPI_COMM_WORLD, &status);
	  if (msgFromSlave[0] == 'F')
	    {
	      cout << "Master got result from: "<<status.MPI_SOURCE <<" Sending Stop Msg..." << endl;
	      for (int i = 1; i < numprocs; i++)
		{
		  char terminate[] = "Stop";
		  MPI_Isend (terminate, sizeof (terminate), MPI_CHAR, i, 000,
			     MPI_COMM_WORLD, &request);
		}
	      missionComplete = 1;
	      break;
	    }
	counter++;
	}

     if (missionComplete == 0)
	{
		cout<<"Nobody found the given key to search."<<endl;
	}
    }

  else
    {				//other processes receive data
      MPI_Recv (localdata, localDataSize, MPI_INT, 0, 000, MPI_COMM_WORLD,
		MPI_STATUS_IGNORE);
      printf ("Process %d has local data: ", myid);
      cout<<endl;

      //all processes other than process with id 0, get their chunk of array and 
     // call MPI_Irecv the MPI_Test which is associated with flad isRecieved 
      //turns its value to 1 once it finds the key. At this time, it sends the
      // message 'F' i.e. found to the process zero which in turn asks other    	//processes to terminate
      char stopMsg[] = "aDummyStr";
      MPI_Irecv (stopMsg, sizeof (stopMsg), MPI_CHAR, 0, 000, MPI_COMM_WORLD,
		 &request);


      int successfulSlave = 0;
      for (counter = 0; counter < localDataSize; counter++)
	{
          MPI_Test (&request, &isRecieved, &status);
	  printf ("%d ", localdata[counter]);
	  if (localdata[counter] == keyToSearch)
	    {
	      char msg[] = "F";
	      MPI_Send (msg, sizeof (msg), MPI_CHAR, 0, 000, MPI_COMM_WORLD);
	      successfulSlave = 1;
	      break;
	    }

	  if (isRecieved)
	    {
	      cout <<"Process: "<< myid << " got stopping msg at " << counter << endl;
	      break;
	    }

	}
	cout<<endl;

	if (successfulSlave == 0){
		char msg[] = "N";
	      MPI_Send (msg, sizeof (msg), MPI_CHAR, 0, 000, MPI_COMM_WORLD);
	     

	}



    }
  MPI_Finalize ();
  return 0;
}









