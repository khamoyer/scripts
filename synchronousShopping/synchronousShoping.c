#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define D(x) x
//test 12: 1268

typedef struct
{
    int *array;
    int nextPosition;
    int length;
} Vector;

typedef struct
{
    Vector *elements;
    int *positions;
    int (*comparer)(int, int);
} Heap;

int heapCompare( int child, int parent )
{
    return child > parent ? 1 : 0;
}

Vector *initVector( Vector *vector )
{
    vector->length = 2;
    vector->nextPosition = 0;
    vector->array = malloc(sizeof(int)*vector->length);
    return vector;
}

Vector *createVector()
{
    Vector *vector = malloc(sizeof(Vector));
    return initVector( vector );
}

void deleteVector(Vector *vector)
{
    free(vector->array);
    free(vector);
}

Heap *createHeap( int max )
{
    Heap *result = malloc(sizeof(Heap));
    result->elements = createVector();
    result->positions = malloc(sizeof(int)*max);
    result->comparer = heapCompare;
    return result;
}

void deleteHeap( Heap *heap )
{
    deleteVector( heap->elements );
    free( heap->positions );
    free(heap);
}

void addToVector(Vector *vector, int value)
{
    if(vector->length==0)
    {
        initVector( vector );
    }
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

void swap( Heap *heap, int firstIndex, int secondIndex)
{
    int temp = heap->elements->array[firstIndex];
    heap->elements->array[firstIndex] = heap->elements->array[secondIndex]; 
    heap->elements->array[secondIndex] = temp;
    heap->positions[heap->elements->array[firstIndex]] = secondIndex;
    heap->positions[heap->elements->array[secondIndex]] = firstIndex;
}

void upHeap( Heap *heap, int value )
{
    int position = heap->positions[value] + 1;
    while( position > 1 
        && (*heap->comparer)( heap->elements->array[position-1], heap->elements->array[position/2-1] ) )
    {
        swap(heap, position/2 - 1, position - 1);
        position /= 2;
    }
}

void addToHeap( Heap *heap, int value )
{
    heap->positions[value] = heap->elements->nextPosition;    
    addToVector(heap->elements, value);
    upHeap( heap, value );
}

int getNumberOfChildrenHeap( Heap *heap, int position )
{
    int result = (heap->elements->nextPosition + 1) - (position<<1);
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

void downHeap( Heap *heap )
{
    int position = 1;
    while( 1 )
    {
        int childPosition = position<<1;
        int childrenCount = getNumberOfChildrenHeap(heap, position);
        if( childrenCount == 0 )
        {
            break;
        }
        else if( childrenCount == 2)
        {
            childPosition += (*heap->comparer)( heap->elements->array[childPosition], heap->elements->array[childPosition-1] );
        }

        if(!(*heap->comparer)(heap->elements->array[childPosition-1], heap->elements->array[position-1]))
        {
            break;
        }
        swap( heap, childPosition-1, position-1);
        position = childPosition;
    }
}

int removeRootHeap( Heap *heap )
{
    int result = -1;
    if(heap->elements->nextPosition > 0)
    {
        result = heap->elements->array[0];
        heap->elements->nextPosition--;
        heap->elements->array[0] = heap->elements->array[heap->elements->nextPosition];
        downHeap( heap );
    }
    return result; 
}

#define FullLoad ((1<<FishCount) - 1)
#define NodesCount CitiesCount*(FullLoad+1)
Vector *FishOccurrence;
Vector *Distances;
Vector *Neighbourhood;
int CitiesCount;
int RoadsCount;
int FishCount;

//dijkstra

int *DijkstraDistances;
int compareForClosest( int child, int parent )
{
    return DijkstraDistances[child] < DijkstraDistances[parent] ? 1 : 0;
}

void calculateShortestPaths( int startNode )
{
    DijkstraDistances = malloc(sizeof(int)*(NodesCount+1));
    int *added = malloc(sizeof(int)*(NodesCount+1));
    for(int i = 0; i <= NodesCount; i++)
    {
        DijkstraDistances[i] = 100000000;
        added[i] = 0;
    }
    Heap *heap = createHeap(NodesCount+1);
    heap->comparer = compareForClosest;
    addToHeap( heap, startNode );
    DijkstraDistances[startNode] = 0;
    added[startNode]=1;
    while( heap->elements->nextPosition > 0 )
    {      
        int current = removeRootHeap( heap );
        if(current > 0 )
        {
            for(int i=0; i<Neighbourhood[current].nextPosition; i++ )
            {
                int neighbour = Neighbourhood[current].array[i];
                if( DijkstraDistances[neighbour] > DijkstraDistances[current] + Distances[current].array[i] )
                {
                    DijkstraDistances[neighbour] = DijkstraDistances[current] + Distances[current].array[i];
                    upHeap( heap, neighbour );
                }
                if( added[neighbour] == 0 )
                {
                    addToHeap( heap, neighbour );
                    added[neighbour] = 1;
                }
            }
        }
    }
    free( added );
    deleteHeap( heap );
}

void readInput()
{
    scanf("%d %d %d", &CitiesCount, &RoadsCount, &FishCount);
    FishOccurrence = createVector();
    Distances = malloc(sizeof(Vector)*(NodesCount+1));
    Neighbourhood = malloc(sizeof(Vector)*(NodesCount+1));
    memset( Distances, 0, sizeof(Vector)*(NodesCount+1));
    memset( Neighbourhood, 0, sizeof(Vector)*(NodesCount+1));

    addToVector( FishOccurrence, 0 );
    for(int city = 0; city < CitiesCount; city++)
    {
        int fishes;
        scanf("%d",&fishes);
        int fishLoad = 0;
        for(int i = 0; i < fishes; i++)
        {
            int fish;
            scanf("%d",&fish);
            fish--;
            fishLoad |= 1<<fish;
        }
        addToVector( FishOccurrence, fishLoad );
    }
    for(int i = 0; i < RoadsCount; i++)
    {
        int city_1, city_2, time;
        scanf("%d %d %d",&city_1,&city_2,&time);
        addToVector( &Neighbourhood[city_1], city_2);
        addToVector( &Neighbourhood[city_2], city_1);
        addToVector( &Distances[city_1], time);
        addToVector( &Distances[city_2], time);        
    }
}

void copyOriginalRoads( int city )
{
    for( int current=1; current<=CitiesCount; current++, city++ )
    {
        for( int i = 0; i<Neighbourhood[current].nextPosition; i++ )
        {
            int respectiveNeighbour = city - (current - Neighbourhood[current].array[i]);
            addToVector( &Neighbourhood[city], respectiveNeighbour);
            addToVector( &Distances[city], Distances[current].array[i]);
        }
    }
}

void addVerticalConnections( int flag, int node )
{
    for( int i = node - CitiesCount, iFlag = flag+1
        ; i > 0
        ; i-=CitiesCount, iFlag++ )
    {
        int fishLoad = FishOccurrence->array[i];
        if( (iFlag & (~fishLoad)) == flag )
        {
            addToVector( &Neighbourhood[node], i);
            addToVector( &Neighbourhood[i], node);
            addToVector( &Distances[node], 0);
            addToVector( &Distances[i], 0);
        }
    }
}

void generateLevel( int flag )
{
    int node = NodesCount - ( flag * CitiesCount ) - CitiesCount + 1;
    for(int i = node; i<node+CitiesCount; i++)
    {
        int ancestor = ((i-1) % CitiesCount) + 1;
        int fishLoad = FishOccurrence->array[ancestor];
        addToVector( FishOccurrence, fishLoad & flag );
        addVerticalConnections( flag, i );
    }
}

int getMax( int first, int second )
{
    return first > second ? first : second;
}

int findBestResult()
{
    int result = DijkstraDistances[NodesCount];
    int halfLevel = (FullLoad/2)+1;
    for( int i=1; i<halfLevel; i++)
    {
        int city = i*CitiesCount;
        int max = getMax( DijkstraDistances[city+CitiesCount], DijkstraDistances[NodesCount - city] );
        if( max < result )
        {
            result = max;
        }
    }
    return result;
}

int main() 
{
    freopen("sample12.txt", "r", stdin);
    readInput();
    for(int i=FullLoad - 1; i>=0; i--)
    {
        int node = NodesCount - ( i * CitiesCount ) - CitiesCount + 1;
        copyOriginalRoads( node );
    }
    for(int i=FullLoad - 1; i>=0; i--)
    {
        generateLevel( i );
    }
    calculateShortestPaths( 1 );
    printf("%d", findBestResult());
    return 0;
}
