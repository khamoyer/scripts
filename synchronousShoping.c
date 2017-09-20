#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

typedef struct
{
    int *array;
    int nextPosition;
    int length;
    int (*comparer)(int, int);
} Vector;

int heapCompare( int child, int parent )
{
    return child > parent ? 1 : 0;
}

Vector *createVector()
{
    Vector *vector = malloc(sizeof(Vector));
    vector->length = 2;
    vector->nextPosition = 0;
    vector->array = malloc(sizeof(int)*vector->length);
    vector->comparer = heapCompare;
    return vector;
}

void deleteVector(Vector *vector)
{
    free(vector->array);
    free(vector);
}

void addToVector(Vector *vector, int value)
{
    if(vector->nextPosition >= vector->length )
    {
        int *tab = malloc(sizeof(int)*vector->length*2);
        for(int i = 0; i < vector->nextPosition; i++)
        {
            tab[i]=vector->array[i];
        }
        free( vector->array );
        vector->length *= 2;        
        vector->array = tab;
    }
    vector->array[vector->nextPosition] = value;
    vector->nextPosition++;
}

void swap( int *first, int *second)
{
    int temp = *first;
    *first = *second; 
    *second = temp;
}

void addToHeap( Vector *vector, int value )
{
    addToVector(vector, value);
    int position = vector->nextPosition;
    while( position > 1 
        && (*vector->comparer)( vector->array[position-1], vector->array[position/2-1] ) )
    {
        swap(vector->array + position/2 - 1, vector->array + position - 1);
        position /= 2;
    }
}

int getNumberOfChildrenHeap( Vector *vector, int position )
{
    int result = (vector->nextPosition + 1) - (position<<1);
    if( result > 2 )
    {
        result = 2;
    }
    else if(result < 0)
    {
        result = 0;
    }
    return result;
}

void downHeap( Vector *vector )
{
    int position = 1;
    while( 1 )
    {
        int childPosition = position<<1;
        int childrenCount = getNumberOfChildrenHeap(vector, position);
        if( childrenCount == 0 )
        {
            break;
        }
        else if( childrenCount == 2)
        {
            childPosition += (*vector->comparer)( vector->array[childPosition], vector->array[childPosition-1] );
        }

        if(!(*vector->comparer)(vector->array[childPosition-1], vector->array[position-1]))
        {
            break;
        }
        swap(&vector->array[childPosition-1], &vector->array[position-1]);
        position = childPosition;
    }
}

int removeRootHeap( Vector *vector )
{
    int result = -1;
    if(vector->nextPosition > 0)
    {
        result = vector->array[0];
        vector->nextPosition--;
        vector->array[0] = vector->array[vector->nextPosition];
        downHeap( vector );
    }
    return result; 
}

//test

int validateHeap( Vector *vector )
{
    int result = 1;
    for(int position = vector->nextPosition; position > 1; position--)
    {
        if( (*vector->comparer)( vector->array[position-1], vector->array[position/2-1] ) )
        {
            result = 0;
            printf("\nheap is not correct\n");
            break;
        }
    }
    return result;
}

void testHeap()
{
    time_t tt;
    int initValue = time(&tt);
    srand(initValue);
    int count = rand()%1000 + 10;
    Vector *vector = createVector();
    for(int i=0; i<count; i++)
    {
        addToVector(vector, rand()%1000);
    }

    Vector *heap = createVector();
    for(int city = 0; city < vector->nextPosition; city++)
    {
        addToHeap(heap, vector->array[city]);
    }
    for(int city = 0; city < vector->nextPosition; city++)
    {
        vector->array[city] = heap->array[city];
    }
    
    validateHeap(vector);
    for(int city = 0, count=vector->nextPosition-2; city < count; city++)
    {
        int removed = removeRootHeap(vector);
        validateHeap(vector);
    }
}

int main()
{
    testHeap();
    return 0;
}