#include "Neighborhood.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

struct neighborhood_s {
    int * neighborhood;     //< neighborhood list
    int maxSize;            //< maximum size of neighborhood
    int size;               //< size of neighborhood
};

void initNeighborhood(Neighborhood n) {
    for (int i = 0; i < n->maxSize; i++)
    {
        n->neighborhood[i] = -1;
    }
    n->size = 0;
}

Neighborhood createNeighborhood(int size) {
    Neighborhood n = malloc(sizeof(Neighborhood));
    n->neighborhood = (int *) malloc(sizeof(int) * size);
    n->maxSize = size;
    n->size = 0;
    initNeighborhood(n);
    return n;
}

void deleteNeighborhood(Neighborhood n){
    free(n->neighborhood);
    n->neighborhood = NULL;
    free(n);
    n = NULL;
}

void getNeighborhood(Graph graph, int source, Neighborhood n) {
    int v = 0;
    initNeighborhood(n);

    while (v < n->maxSize)
    {
        if (isEdge(graph, source, v))
        {
            n->neighborhood[n->size] = v;
            n->size++;
        }
        v++;
    }
}

int getNeighborhoodMember(Neighborhood n, int index) {
    return n->neighborhood[index];
}

int getNeighborhoodSize(Neighborhood n) {
    return n->size;
}

bool isInNeighborhood(Neighborhood n, int target) {
    int i = 0;
    int v = n->neighborhood[i];
    while (v != -1)
    {
        if (v == target) return true;
        i++;
        v = n->neighborhood[i];
    }
    return false;
}

bool gotWhiteNeighborhood(Neighborhood n, int* color) {
    bool tst = false;
    int i = 0;
    int v = n->neighborhood[i];
    while (v != -1)
    {
        if (color[v] == 0) tst = true;
        i++;
        v = n->neighborhood[i];
    }
    return tst;
}

bool gotBlackNeighborhood(Neighborhood n, int * color) {
    int i = 0;
    int v = n->neighborhood[i];

    while (v != -1)
    {
        if (color[v] == 2) return true;
        i++;
        v = n->neighborhood[i];
    }

    return false;
}

void setGreyNeighborhoodInWhite(Graph graph, int target, int source, int * color) {
    int nSize = orderG(graph) + 1;
    Neighborhood n = createNeighborhood(nSize);
    Neighborhood nTmp = createNeighborhood(nSize);
    getNeighborhood(graph, target, n);
            
    int i = 0;
    int v = n->neighborhood[i];
    while (v != -1)
    {
        getNeighborhood(graph, v, nTmp);
        if ((color[v] == 1 && !gotBlackNeighborhood(nTmp, color)))
            color[v] = 0;
        i++;
        v = n->neighborhood[i];
    }

deleteNeighborhood(n);
deleteNeighborhood(nTmp);
}

void printNeighborhood(Neighborhood n) {
    printf("***Print neighborhood list***\n");
    
    int i = 0;
    int v = n->neighborhood[i];
    while (v != -1)
    {
        printf(" %d ", v);
        i++;
        v = n->neighborhood[i];
    }
    printf("\n");
    printf("***End***\n");
}