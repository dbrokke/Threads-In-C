/* *************************************************
 * Author: Danny Brokke (dannybrokke@live.com)
 * Desc: C Program that utilizes threads to calculate
 * an equation. A mutex lock is used to ensure 
 * variable integrity.
   *************************************************
*/ 
#include <pthread.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

// Creating initial variables
double number;
int iterations;
int threads;
pthread_mutex_t lock;
double total;

// Function to do the ln(x) calculation
void *calculation(void* tid)
{
    // This is the thread number (0 based), so I add 1 to make it 1 based
    // Counter variables are used to determine the number of the power & divisor
    int threadNum = *(int*) tid;
    threadNum = threadNum + 1; 
    int oddCounter = threadNum;
    int evenCounter = threadNum;

    // For loop that goes through the amount of iterations specified
	int x;
    for(x = 0; x < iterations; x++)
    {
      // If an even thread the signs are - and if an odd thread the signs are +
      if (threadNum % 2 == 0)
      {
		// Locking the total variable while I calculate the next equation
        pthread_mutex_lock(&lock);
        total = total - (double) ( (double) (pow(number - 1, evenCounter) ) / evenCounter);
        pthread_mutex_unlock(&lock);
		// Adding the amount of threads to counter so as to get to the next number N
        evenCounter = evenCounter + threads;
      }
      else
      {
		// Locking total variable and calculating 
        pthread_mutex_lock(&lock);
        total =  total + (double) ( (double) (pow(number - 1, oddCounter))/ oddCounter);
        pthread_mutex_unlock(&lock);
		// Adding the amount of threads to counter so as to get to the next number N
        oddCounter = oddCounter + threads;
      }

    }
    return NULL;
}

// Main Function
int main(int argc, char* argv[])
{
    // Creating int array for when I need to pass values into my function in pthread_create. 
    // Also setting the total to 0 since this is the start of the function
    int integers[atoi(argv[2])]; 
    total = 0;

    // Only if there are the proper amount of arguments, error message otherwise.
    if(argc == 4)
    {
		// Grabbing the number <0,2> given by the user into a double.
		// Putting the threads/iterations into integer variables. 
		// Creating an array of type pthread_t and make it the size of the amount of threads.   
		number = atof(argv[1]);
        threads = atoi(argv[2]);
        iterations = atoi(argv[3]);
        pthread_t th[threads];

		// If the number is out of range, give error.
        if (number > 2 || number < 0)
        {
            printf("NUMBER WAS OUT OF RANGE\n");
            exit(EXIT_FAILURE);
        }
        else
        {
           	// Loop through all threads
			int i;
            for(i = 0; i < threads; i++)
            {
                // Setting thread number to pass as an argument to calculation function.
                integers[i] = i; 
				// Creating the thread by passing the address of the right thread from my thread array
				// as well as giving it the function to jump to and the argument (thread number). 
                pthread_create(&(th[i]), NULL, &calculation, &integers[i]);
                
            }
            
        }

		// Joining all threads
		int j;
        for(j = 0; j < threads; j++)
        {
            pthread_join(th[j], NULL);
        }

		// Destroying the lock
        pthread_mutex_destroy(&lock);

        printf("Your calculation: %.14f\n", total);
        printf("Expected: %.14f\n", log(number));

    } 
    
    return 0;
}
