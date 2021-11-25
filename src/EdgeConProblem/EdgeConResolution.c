#include "EdgeConResolution.h"
#include "Graph.h"
#include "Stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define TERM_SIZE 25;
#define KRED  "\x1B[31m"
#define KYEL  "\x1B[33m"
#define KWHT  "\x1B[37m"

void initNeighborhood(int * neighborHood, int length) {
    for (int i = 0; i < length; i++)
    {
        neighborHood[i] = -1;
    }
}

int getNeighborhood(Graph graph, int source, int *list) {
    int v = 0, size = 0;
    initNeighborhood(list, orderG(graph) + 1);

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

bool isInNeighborhood(int * neighborhood, int target) {
    int i = 0;
    while (neighborhood[i] != -1)
    {
        if (neighborhood[i] == target) return true;
        i++;
    }
    return false;
}

void setGreyNeighborhoodInWhith(int * oldNeighborhood, int * newNeighborhood, int * color) {
    int i = 0;
    while (oldNeighborhood[i] != -1)
    {
        int v = oldNeighborhood[i++];
        if ((color[v] == 1 && !isInNeighborhood(newNeighborhood, v)))
        {
            color[v] = 0;
        }
    }
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
    int x = TERM_SIZE;
    int n = size/TERM_SIZE;

    for (int j = 0; j < n+1; j++)
    {
        for (int i = 0; i < x; i++) {
            if (((j*x)+i) >= size) break;
            
            if (((j*x)+i)/10 == 0)
            {
                printf("   %d   ", (j*x)+i);
            } else {
                printf("  %d   ", (j*x)+i);
            }
        }
        printf("\n");
        for (int i = 0; i < x; i++)
        {
            if (((j*x)+i) >= size) break;
            switch (color[(j*x)+i])
            {
            case 0:
                printf(KWHT " WHITE ");
                break;
            case 1:
                printf(KYEL " GREY* ");
                break;
            case 2:
                printf(KRED " BLACK ");
                break;
            default:
                break;
            }
        }
        printf(KWHT "\n");
    }
    printf("***end***\n");
}

unsigned int getMinCost(EdgeConGraph graph, int source, int target) {
    unsigned int counter = 0, k = getNumComponents(graph);
    int graphOrder = orderG(getGraph(graph));
    int root = source;

    Stack s = createStack(graphOrder);

    //< vertice's neighborhood.
    int * neighborhood = (int *) malloc(sizeof(int)*graphOrder+1);
    int neighborhoodNumber = getNeighborhood(getGraph(graph), source, neighborhood);
    //< The vertice's color - 0->white 1->grey 2->black
    int * color = (int*) malloc(sizeof(int)*graphOrder);
    for (int i = 0; i < graphOrder ; i++)
    {
        color[i] = 0;
    }
    push(s, source);
    color[source] = 2;
    int a = 0;

    while (!isEmpty(s))
    {
        a++;
        printf("%d\n", a);
        pop(s);
        sleep(1);
        system("clear");
        printf("Source -> %d, k = %d\n", source, k);
        printStack(s);
        isEmpty(s);
        printNeighborhood(neighborhood);
        printColoredVertices(color, graphOrder);

        if (((source == target) && (counter < k))) {
            k = counter;
            if (k == 1) break;
        } 

        if (!gotWhiteNeighborhood(neighborhood, color) 
            || (source == target ))
        {
            color[source] = 1;
            int tmp = pop(s);
            int * nTmp = (int *) malloc(sizeof(int)*graphOrder);
            getNeighborhood(getGraph(graph), tmp, nTmp);
            setGreyNeighborhoodInWhith(neighborhood, nTmp, color);
            free(nTmp);
            nTmp = NULL;
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
                    if (isEdgeHeterogeneous(graph, source, v)) counter ++;
                    if (counter > k) {
                        color[source] = 1;
                        source = pop(s);
                        int * nTmp = (int *) malloc(sizeof(int)*graphOrder);
                        getNeighborhood(getGraph(graph), source, nTmp);
                        setGreyNeighborhoodInWhith(neighborhood, nTmp, color);
                        free(nTmp);
                        nTmp = NULL;
                        counter--;
                    } else {
                        push(s, source);
                        source = v;
                        color[source] = 2;
                        break;
                    }
                }
            }
        }

        neighborhoodNumber = getNeighborhood(getGraph(graph), source, neighborhood);
        if (!((root == source) && !gotWhiteNeighborhood(neighborhood, color))) {
            push(s, source);
        }
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
                sleep(3);
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
