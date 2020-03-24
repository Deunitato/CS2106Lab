/*************************************
 * Lab 4 Exercise 1
 *************************************
 You may change this file during your own testing, 
 but note that they will be replaced with the original 
 files when we test your assignments.
 *************************************/

#include "custom_lock.h"

//initialize our struct
void initialise(custom_lock* lock);

//thread calls wait and gets blocked if necessary
void wait(custom_lock* lock);

//notifies all sleeping theads
void notify_all(custom_lock* lock);

//frees allocated memory
void cleanup(custom_lock* lock);


