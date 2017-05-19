/*
    Program to identify patterns and statistics about prime numbers

    Based on the exercises in the book:
    Parallel programming in C with MPI and OpenMP
    By Michael J. Quinn

    Gilberto Echeverria
    gilecheverria@yahoo.com
    23/04/2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <OpenMP.h>

#define LIMIT 1000000

void testPrimes(int limit);
int isPrime(int number);

int main(int argc, char * argv[])
{
    // Set the default limit
    int limit = LIMIT;

    printf("\n=== PRIME NUMBER FINDER ===\n\n");

    // Use a different limit if passed as an argument
    if (argc == 2)
    {
        limit = atoi(argv[1]);
    }

    testPrimes(limit);

    return 0;
}

void testPrimes(int limit)
{
    int i;
    int consecutives = 0;

    #pragma opm parallel default(none) private(i) shared(limit) reduction(+:consecutives){

      for (i=2; i<limit; i++)
      {
          if (isPrime(i))
          {
              // If the gap is of size two, they are consecutive odd numbers
              if (i>3 && isPrime(i-2))
              {
                  consecutives++;
              }

              //printf("Number %7d is prime | consecutives = %4d\n", i, consecutives);
          }
      }
    }

    printf("Total number of consecutives: %d\n", consecutives);
}

// Check if the input number is prime
// Returns 1 if it is, 0 otherwise
int isPrime(int number)
{
    int divisor;

    for (divisor=2; divisor<=number/2; divisor++)
    {
        if (number % divisor == 0)
        {
            return 0;
        }
    }
    return 1;
}
