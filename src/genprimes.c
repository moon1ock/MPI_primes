///////////////////////////////////////////////////////////////////
//                  BY ANDRII LUNIN                              //
//                                                               //
//      This is my implementation of the parallel Sieve of       //
//  Eratosthenes Algorithm. To ensure the best speed I give      //
//  up on the ideal parallelism and generate a Sieve for         //
//  each process of the appropriate for the process size.        //
//  This lets me achieve the fastest speed possible. While       //
//  with this algo I will not get a 2x speedup, the question     //
//  is why would I care if my program runs 100x times faster     //
//  than anyone who would check numbers manually against all     //
//  divisors from 2,3,4..(n-1)/2.                                //
//                                                               //
//  Either way, the program ensures the maximum possible load    //
//  balancing and the fastest performance.                       //
//                                                               //
//  Thank you for your time, and the lab was actually super fun! //
///////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>



int * findprimes(int rank, int numtasks, int n)
{
    // ARGS: rank of the process, the number of processes, and the N
    // OUT: pointer to an array of values where -1 stands for not prime
    //      and p stands for prime

    // First, find the size of the array to be initialized
    int array_size, split;
    array_size = ceil((float) (n)/numtasks);
    split = array_size; // split is used to store how many numbers are split per core

    // Case 1: array_size * (rank+1) <= N
    // then the array is good as it is

    // Case 2: array_size * (rank+1) > N
    // so we will only deal with the remaining numbers
    if (array_size*(rank+1) > n)
        array_size = n % array_size;


    // REMARK: as noted in the lab, inputs will be much bigger than
    // the amount of processors, so we will not be dealing with the case
    // like 4 elements and 3 processors, where one processor will not get any
    // elements

    int starting_number;

    starting_number = rank*split  + 1;

    //printf("ARRAY SIZE: %d\n", array_size);
    //printf("RANK: %d\n", rank);
    //printf("NUM OF TASKS: %d\n", numtasks);
    //printf("START: %d\n", starting_number);
    //fflush(stdin)
if (starting_number > n){
        int* numbers = (int*) malloc((1)*sizeof(int));
        numbers[0] = 0;
        return numbers;
    }
    int* numbers = (int*) malloc((array_size+1)*sizeof(int));

    for (int i = 0; i<array_size; i++){
        numbers[i] = starting_number + i;
    }
    numbers[array_size] = 0; // We will use it as a terminator for the array to know when it ends after a pointer to it is returned
    // if rank == 0, immediately cross out 1 as it's not prime
    if (rank == 0)
        numbers[0] = -1;

    // Now the algorithm itself

    // let's find the upper bound for our specific case of subarrays
    // as in,  there will be arrays from 1-10 in the context of a 1k problem,
    // and we certainly don't need to check all numbers up to 500 hehe
    int upper_bound;
    upper_bound = ceil(sqrt(numbers[array_size-1])); // trial divison algorithm


    // let's generate a Sieve of eratosphenes which we will use to check against our part of the array

    // Generate the sieve

    int* divisors = (int*) malloc((upper_bound+1)*sizeof(int));

	for(int i = 0; i <= upper_bound; i++)
        divisors[i] = i;

    divisors[0] = -1;
    divisors[1] = -1; // 0 and 1 are not prime so mark them as not prime immediately



	for(int i = 2; i*i <= upper_bound; i++)
	{
		if(divisors[i] != -1)
		{
			//Mark all the multiples of i as -1.
			for(int j = 2*i; j <=upper_bound ; j += i){
				divisors[j] = -1;
            }
		}
	}

    // Now, once we have all of the divisors, let's check the numbers against them
    for (int i =0; i<=upper_bound; i++){
        // check if the divisor is valid
        if (divisors[i] != -1){
            // find the first number divisible by the divisor
            for (int j =0; j<array_size; j++){

                if (numbers[j] != -1 && numbers[j] != divisors[i] && numbers[j] % divisors[i] == 0){
                    for (int k = j; k<array_size; k+=divisors[i]){
                        numbers[k] = -1;
                    }
                    break; // break the loop once we are done so that our complexity is at most O(n*n)
                }
            }
        }
    }

    free(divisors); // Manually collecting garbage

    return numbers;
}




int main(int argc, char** argv)
{

    // Nessesary MPI setup
    int rank, numtasks;

    // Use this to time our program
    double start = MPI_Wtime();
    // Let's init MPI with args, so that we don't have to Broadcast anything,
    // thus -- save time!
    MPI_Init(&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    MPI_Comm_size (MPI_COMM_WORLD, &numtasks);

    int n = atoi(argv[1]);

    int maxrecv = ceil((float) (n)/numtasks); // this is the maximum amount of primes we can ever recv from MPI

    int *numbers; // pointer to the numbers array that will be returned
    numbers = findprimes(rank, numtasks, n);


    // Since we have at least 1 prime, let's send it to root
    // Get the array that we will return to the root
    int cnt = 0;
    for (int i=0; numbers[i]!= 0; i++){
        if (numbers[i] != -1){
            cnt++ ;
        }
    }
    int* primes = (int*) malloc((cnt+1)*sizeof(int));

    int j = 0;
    for (int i=0; numbers[i]!= 0; i++){
        if (numbers[i] != -1){
            primes[j] = numbers[i];
            j++;
        }
    }
    // add this as a flag for root for when to stop counting. This way we will not need to tell root the size of the array
    primes[j] = 0;

    cnt = j;

    //int recieved[maxrecv];

    // if we are the master process, we first want to append the primes that we had to the text file, and then recieve the
    // primes from all other processes
    if(rank ==0){
            // writing out the primes that we have
            // open the file
            FILE *prime_file;
            char name[50];
            snprintf(name, 50, "%d.txt", n); // concatenate the name as n and .txt
            prime_file = fopen(name,"w");
            for (int i = 0; primes[i]!=0; i++)
                fprintf(prime_file, "%d ", primes[i]);

            // now let's receive our sweet primes from everyone else
            for (int i = 1; i<numtasks; i++){
                int* recieved = (int*) calloc(maxrecv,sizeof(int));

     //           memset(&recieved, 0, maxrecv);
                MPI_Recv(recieved, maxrecv, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                // write the recieved nums to a file
                for (int i = 0; recieved[i]!=0; i++){
                    fprintf(prime_file, "%d ", recieved[i]);
		        }
              free(recieved);
            } fclose(prime_file);
	double end = MPI_Wtime();
	printf("TIME TAKEN: %f\n", end-start);
    }
    else{
        MPI_Send(primes, cnt, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }


    free(primes);
    free(numbers);
    MPI_Finalize ();

    return 0;
}
