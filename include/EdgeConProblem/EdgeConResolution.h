/**
 * @file EdgeConResolution.h
 * @author Vincent Penelle (vincent.penelle@u-bordeaux.fr)
 * @brief  Algorithms to solve directly the EdgeCon problem
 * @version 1
 * @date 2021-10-18
 * 
 * @copyright Creative Commons.
 * 
 */

#ifndef COCA_EDGECONRESOLUTION_H
#define COCA_EDGECONRESOLUTION_H

#include "EdgeConGraph.h"

/**
 * @brief Get minimal value and white.
 * 
 * @param list List of value.
 * @param color List of color.
 * @param size Size of both list.
 * 
 * @return index of value. 
 */
int getMinAndWhite(int * list, int * color, int size);

/**
 * @brief Dijkstra' algorithm found minimal cost that  @p source and
 * @p target communicate.
 * 
 * @param graph An instance of problem.
 * @param source An integer -> u.
 * @param target An integer -> v.
 * 
 * @return the minimal cost that two nodes communicate.
 */
int dijkstra(EdgeConGraph graph, int source, int target);

/**
 * @brief Brute Force Algorithm. If there is a result, the solution 
 * will be stored in @param graph, and its homogeneous components 
 * updated. If no solution, graph won't be modified. Returns the maximal 
 * cost of communication for any choice of translators.
 * 
 * @param graph An instance of the problem.
 * @return the maximal cost that two nodes communicate with 
 * for any possible set of transducers of minimal size. Returns -1
 * if there is no solution (i.e., the graph is not connex).
 * 
 * @pre graph must be valid.
 */
int BruteForceEdgeCon(EdgeConGraph graph);

#endif