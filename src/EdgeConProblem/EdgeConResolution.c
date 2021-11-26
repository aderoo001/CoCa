#include "EdgeConResolution.h"
#include "Graph.h"
#include "Stack.h"
#include "Neighborhood.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define TERM_SIZE 15;
#define KRED  "\x1B[31m"
#define KYEL  "\x1B[33m"
#define KWHT  "\x1B[37m"

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

unsigned int getMinCost(EdgeConGraph graph, int source, int target) {
    unsigned int counter = 0, k = getNumComponents(graph);
    int graphOrder = orderG(getGraph(graph));

    Stack s = createStack(graphOrder);

    //< vertice's neighborhood.
    Neighborhood neighborhood = createNeighborhood(graphOrder + 1);
    getNeighborhood(getGraph(graph), source, neighborhood);
    //< The vertice's color - 0->white 1->grey 2->black
    int * color = (int*) malloc(sizeof(int)*graphOrder);
    for (int i = 0; i < graphOrder ; i++) color[i] = 0;
    push(s, source);
    color[source] = 2;

    while (!isEmpty(s))
    {
        pop(s);
        //sleep(1);
        system("clear");
        printf("Source -> %d, target -> %d, k = %d, compteur = %d\n", source, target, k, counter);
        printStack(s);
        isEmpty(s);
        printNeighborhood(neighborhood);
        printColoredVertices(color, graphOrder);

        if (((source == target) && (counter < k))) {
            k = counter;
            if (k == 1) break;
        } 

        if (!gotWhiteNeighborhood(neighborhood, color) 
            || (source == target))
        {
            color[source] = 1;
            int tmp = pop(s);
            setGreyNeighborhoodInWhite(getGraph(graph),tmp, source, color);
            if (isEdgeHeterogeneous(graph, source, tmp)) {
                counter --;
                removeTranslator(graph, tmp, source);
            }
            source = tmp;
        } 
        else
        { 
            int nSize = getNeighborhoodSize(neighborhood);
            for (int i = 0; i < nSize; i++)
            {     
                int v = getNeighborhoodMember(neighborhood, i);
                if (color[v] == 0)
                {
                    if (counter > k) {
                        color[source] = 1;
                        int tmp = pop(s);
                        setGreyNeighborhoodInWhite(getGraph(graph),tmp, source, color);
                        if (isEdgeHeterogeneous(graph, source, tmp)) {
                            counter --;
                            removeTranslator(graph, tmp, source);
                        }
                        source = tmp;
                        break;
                    } else {
                        if (isEdgeHeterogeneous(graph, source, v)) {
                            counter ++;
                            addTranslator(graph, source, v);
                        } 
                        push(s, source);
                        source = v;
                        color[source] = 2;
                        break;
                    }
                }
            }
        }

        getNeighborhood(getGraph(graph), source, neighborhood);
        if (source != -1) {
            push(s, source);
        }
    }

    freeStack(s);
    free(color);
    color = NULL;
    deleteNeighborhood(neighborhood);

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
                tmp = (int) getMinCost(tmpGraph, u, v);
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
