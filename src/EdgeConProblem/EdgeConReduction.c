
#include "EdgeConReduction.h"
#include "Z3Tools.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

Z3_ast getVariableIsIthTranslator(Z3_context ctx, int node1, int node2, int number)
{
    char name[40];
    if (node1 < node2)
        snprintf(name, 40, "[(%d,%d),%d]", node1, node2, number);
    else
        snprintf(name, 40, "[(%d,%d),%d]", node2, node1, number);
    return mk_bool_var(ctx, name);
}

Z3_ast getVariableParent(Z3_context ctx, int child, int parent)
{
    char name[40];
    snprintf(name, 40, "p_[%d,%d]", child, parent);
    return mk_bool_var(ctx, name);
}

Z3_ast getVariableLevelInSpanningTree(Z3_context ctx, int level, int component)
{
    char name[40];
    snprintf(name, 40, "l_[%d,%d]", component, level);
    return mk_bool_var(ctx, name);
}

Z3_ast EdgeConReduction(Z3_context ctx, EdgeConGraph edgeGraph, int cost)
{
    Graph graph = getGraph(edgeGraph);
    int size_edge = sizeG(graph);
    int size_node = orderG(graph);
    int N = getNumHeteregeneousEdges(edgeGraph);

    //Z3_ast x = mk_bool_var(ctx, "x");

    //Z3_ast p = mk_bool_var(ctx, "p");

    //Z3_ast l = mk_bool_var(ctx, "l");


    Z3_ast x[size_edge][N];
    Z3_ast p[size_edge][N];
    Z3_ast l[size_edge][N];


    //Z3_ast negx = Z3_mk_not(ctx, x);

    //Z3_ast negp = Z3_mk_not(ctx, p);

    //Z3_ast negl = Z3_mk_not(ctx, l);


    Z3_ast tab_phi_1[size_edge];
    for(int e = 0; e<size_edge; e++){

        Z3_ast tab_and[N*N];

        for(int i = 0; i < N; i++){
            for (int j = 0; j < N; j++)
            {
                if(i!=j){
                    Z3_ast negx_1 = Z3_mk_not(ctx, x[e][i]);
                    Z3_ast negx_2 = Z3_mk_not(ctx, x[e][j]);
                    Z3_ast tab_or[2] = {negx_1, negx_2};
                    tab_and[(i*N)+j] = Z3_mk_or(ctx, 2, tab_or);

                }
            }
        }        

        Z3_ast tab_or[N];

        for (int i = 0; i < N; i++)
        {
            tab_or[i] = x[e][i];
        }

        Z3_ast and = Z3_mk_and(ctx, N*N, tab_and);
        Z3_ast or = Z3_mk_or(ctx, N, tab_or);
        Z3_ast tab_and_2[2] = {and, or};

        tab_phi_1[e] = Z3_mk_and(ctx, 2, tab_and_2);
    }
    Z3_ast phi_1 = Z3_mk_and(ctx, size_edge, tab_phi_1);

    Z3_ast tab_phi_2[N-1];
    for (int i = 1; i < N; i++) {

        Z3_ast tab_and[(N-1)*(N-1)];

        for (int j = 1; j < N; j++) {
            for (int k = 1; k < N; k++) {

                if ((i != j) && (i != k) && (j != k)) {
                    Z3_ast negp_1 = Z3_mk_not(ctx, p[i][j]);
                    Z3_ast negp_2 = Z3_mk_not(ctx, p[i][k]);
                    Z3_ast tab_or[2] = {negp_1, negp_2};
                    tab_and[((j-1)*N)+(k-1)] = Z3_mk_or(ctx, 2, tab_or);
                }
            }
        }

        Z3_ast tab_or[N-1];

        for (int j = 1; j < N; i++)
            if (i != j) tab_or[i-1] = p[i][j];

        Z3_ast and = Z3_mk_and(ctx, N*N, tab_and);
        Z3_ast or = Z3_mk_or(ctx, N, tab_or);
        Z3_ast tab_and_2[2] = {and, or};

        tab_phi_2[i-1] = Z3_mk_and(ctx, 2, tab_and_2);
    }
    Z3_ast phi_2 = Z3_mk_and(ctx, N, tab_phi_2);

    Z3_ast tab_phi_3[N];
    for (int j = 0; j < N; j++) {

        Z3_ast tab_and[N*N];

        for (int h = 0; h < N; h++) {
            for (int k = 0; k < N; k++) {

                if (h != k) {
                    Z3_ast negl_1 = Z3_mk_not(ctx, l[j][h]);
                    Z3_ast negl_2 = Z3_mk_not(ctx, l[j][k]);
                    Z3_ast tab_or[2] = {negl_1, negl_2};
                    tab_and[(h*N)+k] = Z3_mk_or(ctx, 2, tab_or);
                }
            }
        }

        Z3_ast tab_or[N];

        for (int h = 0; h < N; h++)
            tab_or[h] = l[j][h];

        Z3_ast and = Z3_mk_and(ctx, N*N, tab_and);
        Z3_ast or = Z3_mk_or(ctx, N, tab_or);
        Z3_ast tab_and_2[2] = {and, or};

        tab_phi_3[j] = Z3_mk_and(ctx, 2, tab_and_2);
    }
    Z3_ast phi_3 = Z3_mk_and(ctx, N, tab_phi_3);

    Z3_ast tab_phi_4[N];
    for (int j = 0; j < N; j++) {
        Z3_ast tab_or[N-cost];
        for (int h = cost; h < N; ++h) {
            tab_or[h-cost] = l[j][h];
        }

        tab_phi_4[j] = Z3_mk_or(ctx, N-cost, tab_or);
    }
    Z3_ast phi_4 = Z3_mk_or(ctx, N, tab_phi_4);

    Z3_ast tab_phi_7[N-1];
    for (int j = 1; j < N; j++) {
        Z3_ast tab_or[N-1];
        for (int k = 1; k < N; k++) {
            Z3_ast tab_and[2];
            tab_and[0] = p[j][k];

            Z3_ast tab_or_2[N]
            int cmpt = 0;
            for (int u = 0; u < size_node; u++) {
                for (int v = 0; v < size_node; v++) {
                    if (isEdge(graph, u, v)
                        && !areInSameComponent(edgeGraph, u, v)) {

                        Z3_ast tab_or_3[N - 1];
                        for (int i = 1; i < N; ++i) {
                            tab_or_3[i - 1] = x[cmpt][i]
                        }
                        Z3_ast or_3 = Z3_mk_or(ctx, N-1, tab_or_3);

                        tab_or_2[cmpt] = or_3;
                        cmpt++;
                    }
                }
            }

            Z3_ast or_2 = Z3_mk_or(ctx, N, tab_or_2);

            Z3_ast tab_or_3[N-1];
            for (int h = 1; h < N; h++) {
                Z3_ast tab_and_2[2] = {l[j][h], l[k][h-1]}
                tab_or_3[h-1] = Z3_mk_and(ctx, 2, tab_and_2);
            }
            Z3_ast or_3 = Z3_mk_or(ctx, N-1, tab_or_3);

            Z3_ast tab_and_2[2] = {or_2, or_3};
            Z3_ast and_2 = Z3_mk_and(ctx, 2, tab_and_2);

            tab_and[1] = and_2;
            tab_or[k-1] = Z3_mk_and(cts, 2, tab_and);
        }
        tab_phi_7[j-1] = Z3_mk_or(cts, N-1, tab_or);
    }
    Z3_ast phi_7 = Z3_mk_and(ctx, N-1, tab_phi_7);

    Z3_ast tab_phi[5] = {phi_1, phi_2, phi_3, phi_4, phi_7};
    Z3_ast phi = Z3_mk_and(ctx, 5, tab_phi);

    return phi;
}

void getTranslatorSetFromModel(Z3_context ctx, Z3_model model, EdgeConGraph graph)
{
    return;
}
