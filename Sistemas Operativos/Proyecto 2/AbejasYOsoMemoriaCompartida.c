#include<stdlib.h>

/*Algoritmo	
    Mutex Bear = 0;
    Semaphore Bees = 0;
    Semaphore Empty = M;
    Mutex Mutex = 1;

    Bear{
        wait(&Bear)
        while(!Empty.Capacity() == M){ 
            signal(&Empty) //Empty the Honey Jar
        }    
        signal(&Mutex) //Let Bees return to work
    }
    Bee{
        signal(&Bees)
        wait(&Mutex)
            wait(&Empty)
        wait(&Bees)
        try_wait(&Bees) //"Am I the last bee to exit?"
            try_wait(&Empty) //"Is the Honey Jar full?
                signal(&Bear) //Honey jar is full, Bear can eat, DON'T FREE THE MUTEX!
            else
                signal(&Mutex) //Honey jar wasn't empty, free Mutex
        else{
            signal(&Bees)
            signal(&Mutex) //I wasn't the last Bee, give back the Mutex and correct error at Count
        }
*/