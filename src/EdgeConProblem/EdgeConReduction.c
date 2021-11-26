
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

    int size_Node = orderG(getGraph(edgeGraph));
    int N = getNumHeteregeneousEdges(edgeGraph);

    //Z3_ast x = mk_bool_var(ctx, "x");

    Z3_ast p = mk_bool_var(ctx, "p");

    Z3_ast l = mk_bool_var(ctx, "l");


    Z3_ast x[size_Node][N];
    Z3_ast x[size_Node][N];


    //Z3_ast negx = Z3_mk_not(ctx, x);

    Z3_ast negp = Z3_mk_not(ctx, p);

    Z3_ast negl = Z3_mk_not(ctx, l);


    Z3_ast tab_phi_1[size_Node];
    for(int e = 0; e<size_Node; e++){

        Z3_ast tab_and[N*N];

        for(int i = 0; i < N; i++){
            for (int j = 0; j < N; j++)
            {
                if(i!=j){
                    Z3_ast negx_1 = Z3_mk_not(ctx, x[e][i]);
                    Z3_ast negx_2 = Z3_mk_not(ctx, x[e][j]);
                    Z3_ast tab_or[2] = {negx_1, negx_2};
                    tab_and[i*j] = Z3_mk_or(ctx, 2, tab_or);

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
    Z3_ast phi_1 = Z3_mk_and(ctx, size_Node, tab_phi_1);

    

    return Z3_mk_false(ctx);
}

void getTranslatorSetFromModel(Z3_context ctx, Z3_model model, EdgeConGraph graph)
{
    return;
}
