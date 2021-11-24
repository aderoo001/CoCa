#include "EdgeConResolution.h"
#include "Graph.h"
#include "Stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void initNeighborhood(int * neighborHood, int length) {
    for (int i = 0; i < length; i++)
    {
        neighborHood[i] = -1;
    }
}

int getNeighborhood(Graph graph, int source, int *list) {
    int v = 0, size = 0;
    initNeighborhood(list, orderG(graph));

    while (v <= orderG(graph))
    {
        if (isEdge(graph, source, v))
        {
            list[size] = v;
            size++;
        }
        v++;
    }
    
    return size;
}

bool gotWhiteNeighborhood(int * neighborhood, int* color) {
    bool tst = false;
    int i = 0;
    while (neighborhood[i] != -1)
    {
        int v = neighborhood[i];
        if (color[v] == 0)
        {
            tst = true;
        }
        i++;
    }
    return tst;
}

void printNeighborhood(int* neighborhood) {
    printf("***Print neighborhood list***\n");
    
    int i = 0;
    while (neighborhood[i] != -1)
    {
        printf(" %d ", neighborhood[i]);
        i++;
    }
    printf("\n");
    printf("***End***\n");
}

void printColoredVertices(int* color, int size) {
    printf("***Print colored vertices***\n");
    for (int i = 0; i < size; i++)
    {
        printf("Node %d ->", i);
        switch (color[i])
        {
        case 0:
            printf(" WHITE ");
            break;
        case 1:
            printf(" GREY ");
            break;
        case 2:
            printf(" BLACK ");
            break;
        default:
            break;
        }
        printf("\n");
    }
    printf("***end***\n");
}

unsigned int getMinCost(EdgeConGraph graph, int source, int target) {
    unsigned int counter = 0, k = getNumComponents(graph);
    int graphOrder = orderG(getGraph(graph));

    Stack s = createStack(graphOrder);

    //< vertice's neighborhood.
    int * neighborhood = (int *) malloc(sizeof(int)*graphOrder);
    int neighborhoodNumber = getNeighborhood(getGraph(graph), source, neighborhood);
    //< The vertice's color - 0->white 1->grey 2->black
    int * color = (int*) malloc(sizeof(int)*graphOrder);
    for (int i = 0; i < graphOrder ; i++)
    {
        color[i] = 0;
    }
    push(s, source);
    color[source] = 2;

    while (!isEmpty(s))
    {
        sleep(1);
        printf("--------------------------------\n");
        printf("Source : %d\n", source);
        //printNeighborhood(neighborhood);
        //printColoredVertices(color, graphOrder);
        printStack(s);

        if ((source == target) && (counter < k)) k = counter;

        if (!gotWhiteNeighborhood(neighborhood, color) || (source == target))
        {
            color[source] = 1;
            for (int i = 0; i < neighborhoodNumber; i++)
            {
                int v = neighborhood[i];
                if (color[v] == 1)
                {
                    color[v] = 0;
                }
            }
            int tmp = pop(s);
            if (areInSameComponent(graph, source, tmp))
            {
                counter --;
            }
            source = tmp;
        } 
        else
        { 
            for (int i = 0; i < neighborhoodNumber; i++)
            {     
                int v = neighborhood[i];
                if (color[v] == 0)
                {
                    if (isEdgeHeterogeneous(graph, source, v))
                    {
                        counter ++;
                    }
                    push(s, source);
                    source = v;
                    color[source] = 2;
                    break;
                }
            }
        }
        neighborhoodNumber = getNeighborhood(getGraph(graph), source, neighborhood);
    }

    freeStack(s);
    free(color);
    color = NULL;
    free(neighborhood);
    neighborhood = NULL;

    return k;
}

int BruteForceEdgeCon(EdgeConGraph graph)
{
    int k = 0;
    int graphOrder = orderG(getGraph(graph));
    printf("Taille du graph : %d\n", graphOrder);
    EdgeConGraph tmpGraph = initializeGraph(getGraph(graph));

    for (int u = 0; u < graphOrder; u++)
    {
        for (int v = 0; v < graphOrder; v++)
        {
            if (u != v)
            {
                int tmp = k;
                printf("From %d to %d.\n", u, v);
                tmp = (int) getMinCost(tmpGraph, u, v);
                printf("cost = %d\n", tmp);
                if (tmp > k) {
                    k = tmp;
                    copyTranslator(tmpGraph, graph);
                }
                resetTranslator(tmpGraph);
            }
        }
    }
    printf("\nResultat : %d.\n\n", k);

    return k;
}
