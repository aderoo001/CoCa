#include "EdgeConResolution.h"
#include "Graph.h"
#include "Stack.h"
#include "Neighborhood.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define TERM_SIZE 25;
#define KRED  "\x1B[31m"
#define KYEL  "\x1B[33m"
#define KWHT  "\x1B[37m"

int getMinAndWhite(int * list, int * color, int size) {
    int min = -1;
    int tst = size;
    for (int i = 0; i < size; i++)
    {
        if ((color[i] != 2) && list[i] < tst)
        {
            tst = list[i];
            min = i;
        }
    }
    return min;
}

/**
 * @brief for debbug.
 */
void printWeight(int * weight, int size) {
    printf("***Print weight vertices***\n");
    int x = TERM_SIZE;
    int n = size/TERM_SIZE;

    for (int j = 0; j <= n; j++)
    {
        for (int i = 0; i < x; i++) {
            if (((j*x)+i) >= size) break;
            if (((j*x)+i)/10 == 0) printf("   %d   ", (j*x)+i);
            else printf("  %d   ", (j*x)+i);
        }
        printf("\n");
        for (int i = 0; i < x; i++)
        {
            int a = weight[(j*x)+i];
            if (((j*x)+i) >= size) break;
            if (a/10 == 0) printf("   %d   ", a);
            else printf("  %d   ", a);
        }
        printf(KWHT "\n");
    }
    printf("***end***\n");
}

/**
 * @brief for debbug.
 */
void printColoredVertices(int* color, int size) {
    printf("***Print colored vertices***\n");
    int x = TERM_SIZE;
    int n = size/TERM_SIZE;

    for (int j = 0; j <= n; j++)
    {
        for (int i = 0; i < x; i++) {
            if (((j*x)+i) >= size) break;
            if (((j*x)+i)/10 == 0) printf("   %d   ", (j*x)+i);
            else printf("  %d   ", (j*x)+i);
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

int dijkstra (EdgeConGraph graph, int source, int target) {
    unsigned int k = 0;
    int graphOrder = orderG(getGraph(graph)), root = source;

    //< vertice's neighborhood.
    Neighborhood neighborhood = createNeighborhood(graphOrder + 1);
    getNeighborhood(getGraph(graph), source, neighborhood);
    //< The vertice's color - 0->white 1->grey 2->black
    int * color = (int*) malloc(sizeof(int)*graphOrder+1);
    int * weight = (int*) malloc(sizeof(int)*graphOrder+1);
    for (int i = 0; i < graphOrder ; i++) color[i] = 0;
    for (int i = 0; i < graphOrder ; i++) weight[i] = getNumHeteregeneousEdges(graph);

    color[source] = 1;
    weight[source] = 0;

    while (source != target)
    {
        int nSize = getNeighborhoodSize(neighborhood);
        for (int i = 0; i < nSize; i++)
        {
            k = weight[source];
            int v = getNeighborhoodMember(neighborhood, i);
            if (color[v] != 2)
            {
                if (isEdgeHeterogeneous(graph, source, v)) k++;
                if (weight[v] >= k) weight[v] = k;
            }
            
        }
        color[source] = 2;
        int tmp = getMinAndWhite(weight, color, graphOrder);
        addTranslator(graph, source, tmp);
        source = tmp;
        color[source] = 1;
        getNeighborhood(getGraph(graph), source, neighborhood);
    }

    k = weight[target];

    free(weight);
    weight = NULL;
    free(color);
    color = NULL;
    deleteNeighborhood(neighborhood);

    return k;
}

/**
 * @brief uncorrect algorithm
 */
int getMinCost(EdgeConGraph graph, int source, int target) {
    unsigned int counter = 0;
    int graphOrder = orderG(getGraph(graph));

    Stack s = createStack(graphOrder);

    //< vertice's neighborhood.
    Neighborhood neighborhood = createNeighborhood(graphOrder + 1);
    getNeighborhood(getGraph(graph), source, neighborhood);
    //< The vertice's color - 0->white 1->grey 2->black
    int * color = (int*) malloc(sizeof(int)*graphOrder);
    int * d = (int*) malloc(sizeof(int)*graphOrder);
    for (int i = 0; i < graphOrder ; i++) color[i] = 0;
    for (int i = 0; i < graphOrder ; i++) d[i] = graphOrder;
    push(s, source);
    color[source] = 1;
    d[source] = 0;

    while (source != target)
    {
        source = pop(s);
        color[source] = 2;
        getNeighborhood(getGraph(graph), source, neighborhood);
        sleep(1);
        system("clear");
        printf("Source -> %d, target -> %d, compteur = %d\n", source, target, counter);
        printStack(s);
        isEmpty(s);
        printNeighborhood(neighborhood);
        printColoredVertices(color, graphOrder);

        int nSize = getNeighborhoodSize(neighborhood);
        for (int i = 0; i < nSize ; ++i) {
            int v = getNeighborhoodMember(neighborhood, i);
            if (color[v] != 1 || color[v] !=2) {
                color[v] = 1;
                d[v] = d[source]++;
                push(s, v);
            }
        }
    }

    freeStack(s);
    free(color);
    color = NULL;
    deleteNeighborhood(neighborhood);
    return d[target];
}

int BruteForceEdgeCon(EdgeConGraph graph)
{
    int k = 0;
    int graphOrder = orderG(getGraph(graph));
    EdgeConGraph tmpGraph = initializeGraph(getGraph(graph));

    for (int u = 0; u < graphOrder; u++)
    {
        for (int v = 0; v < graphOrder; v++)
        {
            if (u != v)
            {
                int tmp = k;
                tmp = (int) dijkstra(tmpGraph, u, v);
                int lul = getMinCost(graph, u, v);
                printf("%d ", lul);
                if (tmp > k) {
                    k = tmp;
                    copyTranslator(tmpGraph, graph);
                }
                resetTranslator(tmpGraph);
            }
        }
    }

    printTranslator(graph);

    return k;
}
