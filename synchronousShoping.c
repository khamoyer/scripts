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

int getNotVisited( Vector *heap, int *visited )
{
    int result = removeRootHeap( heap );
    if( result >= 0 )
    {
        if( visited[result]==1 )
        {
            result = getNotVisited( heap, visited );
        }
        else
        {
            visited[result] = 1;            
        }
    }
    return result;
}

int calculateShortestPath( int startNode, int endNode )
{
    DijkstraDistances = malloc(sizeof(int)*(CitiesCount+1));
    int *predecessors = malloc(sizeof(int)*(CitiesCount+1));
    int *visited = malloc(sizeof(int)*(CitiesCount+1));
    for(int i = 0; i <= CitiesCount; i++)
    {
        DijkstraDistances[i] = 100000000;
        predecessors[i] = 0;
        visited[i] = 0;
    }
    Vector *heap = createVector();
    heap->comparer = compareForClosest;
    addToHeap( heap, startNode );
    DijkstraDistances[startNode] = 0;
    int current = startNode;
    while( heap->nextPosition > 0 && current != endNode )
    {      
        current = getNotVisited( heap, visited );
        for(int i=0; i<Neighbourhood[current].nextPosition; i++ )
        {
            int neighbour = Neighbourhood[current].array[i];
            if( DijkstraDistances[neighbour] > DijkstraDistances[current] + Distances[current].array[i] )
            {
                DijkstraDistances[neighbour] = DijkstraDistances[current] + Distances[current].array[i];
                predecessors[neighbour] = current;
            }
            addToHeap( heap, neighbour );
        }

    }
    int result = DijkstraDistances[endNode];
    free( DijkstraDistances );
    free( predecessors );
    free( visited );
    deleteVector( heap );
    return result;
}

void readInput()
{
    scanf("%d %d %d", &CitiesCount, &RoadsCount, &FishCount);
    FishOccurrence = createVector();
    Distances = malloc(sizeof(Vector)*(NodesCount+1));
    Neighbourhood = malloc(sizeof(Vector)*(NodesCount+1));

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
            addToVector( &Neighbourhood[city], Neighbourhood[current].array[i]);
            addToVector( &Distances[city], Distances[current].array[i]);
        }
    }
}

void addVerticalConnections( int flag, int node )
{
    for( int i = node - CitiesCount; i > 0; i-=CitiesCount )
    {
        int fishLoad = FishOccurrence->array[i];
        if( (fishLoad + flag) == FullLoad )
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
    copyOriginalRoads( node );
    for(int i = node; i<node+CitiesCount; i++)
    {
        int ancestor = ((i-1) % CitiesCount) + 1;
        //printf("- %d %d\n", i, ancestor);
        int fishLoad = FishOccurrence->array[ancestor];
        addToVector( FishOccurrence, fishLoad & flag );
        //add vertical zero connections
        //for current flag + parent = full load;
        addVerticalConnections( flag, node );
    }
}

int main() 
{
    freopen("sample.txt", "r", stdin);
    readInput();
    for(int i=FullLoad - 1; i>=0; i--)
    {
        generateLevel( i );
    }

    printf("%d \n", calculateShortestPath( 1, CitiesCount));
    for(int i = 1; i <= NodesCount; i++)
    {
        printf("\ncity %d: ", i);
        for(int y = 0; y < Distances[i].nextPosition; y++)
        {
            printf("%d %d  ", Neighbourhood[i].array[y], Distances[i].array[y]);
        }
    }
    return 0;
}
