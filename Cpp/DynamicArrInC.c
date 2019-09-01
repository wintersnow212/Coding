#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


#define VECTOR_INITIAL_CAPACITY 100

// Define a vector type
typedef struct {
  int idx;      // slots used so far
  int capacity;  // total available slots
  int *data;     // array of integers we're storing
} Vector;

void vector_init(Vector *vector)
{
    vector->data = (int*) malloc(sizeof(int) * VECTOR_INITIAL_CAPACITY);
    vector->capacity = VECTOR_INITIAL_CAPACITY;
    vector->idx = 0;
}

void vector_append(Vector *vector, int value)
{
//     if (vector->idx >= vector->capacity)
//     {
//         int* newData = (int*) malloc(sizeof(int) * vector->capacity * 2);
//         for (int i = 0; i < vector->capacity; ++i)
//         {
//             newData = vector->data[i];
//         }
        
//         int* delete = vector->data;
//         vector->data = newData;
//         free(delete);
//         vector->capacity *= 2;
//     }
    // 或者直接realloc!!!!
    if (vector->idx >= vector->capacity) 
    {
        // double vector->capacity and resize the allocated memory accordingly
        vector->capacity *= 2;
        vector->data = realloc(vector->data, sizeof(int) * vector->capacity);
    }
    vector->data[vector->idx++] = value;
}

int vector_get(Vector *vector, int index)
{
    if (index >= vector->capacity || index < 0) 
    {
        printf("Index %d out of bounds for vector of size %d\n", 
               index, vector->idx);
        exit(1);
    }
    
    return vector->data[index];
}

void vector_set(Vector *vector, int index, int value)
{
    while (index >= vector->idx) 
    {
        vector_append(vector, 0);
    }
    // if (index >= vector->capacity || index < 0) 
    // {
    //     printf("Index %d out of bounds for vector of size %d\n", 
    //            index, vector->idx);
    //     exit(1);
    // }
    
    vector->data[index] = value;
}

void vector_free(Vector *vector)
{
    free(vector->data);
}

int main() {
  // declare and initialize a new vector
  Vector vector;
  vector_init(&vector);

  // fill it up with 150 arbitrary values
  // this should expand capacity up to 200
  int i;
  for (i = 200; i > -50; i--) {
    vector_append(&vector, i);
  }

  // set a value at an arbitrary index
  // this will expand and zero-fill the vector to fit
  vector_set(&vector, 4452, 21312984);

  // print out an arbitrary value in the vector
  printf("Heres the value at 27: %d\n", vector_get(&vector, 27));
    
  printf("Heres the value at 4451: %d\n", vector_get(&vector, 4451));

  printf("Heres the value at 4452: %d\n", vector_get(&vector, 4452));

  // we're all done playing with our vector, 
  // so free its underlying data array
  vector_free(&vector);
}
