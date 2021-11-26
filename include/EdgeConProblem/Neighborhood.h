/**
 * @file Neighborhood.h
 * @author Dimitri Didier / Alexis Deroo
 * @brief  Structure to store neighborhood of target in graph.
 * @version 2
 * @date 2018-11-18, 2019-07-23, 2019-08-02, 2020-06-24
 * 
 * @copyright Creative Commons.
 * 
 */
#include "Graph.h"


/** 
 * @brief: the Neighborhood type. The first field is neighborhood 
 * list and two others are size components.
 */
typedef struct neighborhood_s *Neighborhood;

/**
 * @brief Set all fields of neghborhood to -1.
 * 
 * @param n A Neighborhood structure.
 * @param size An integer, size of neighborhood .
 */
void initNeighborhood(Neighborhood n);

/**
 * @brief Create new neighborhood.
 * 
 * @param size An integer, size of neighborhood.
 */
Neighborhood createNeighborhood(int size);

/**
 * @brief Delete given neighborhood.
 * 
 * @param n An instance of neighborhood.
 */
void deleteNeighborhood(Neighborhood n);

/**
 * @brief Get neighborhood of given source.
 * 
 * @param graph An instance of the graph
 * @param source An integer.
 * @param n An instance of neighborhood.
 */
void getNeighborhood(Graph graph, int source, Neighborhood n);

/**
 * @brief get @p n member at @p index.
 * 
 * @param n An instance oof neighborhood.
 * @param index Index of neighborhood member.
 * 
 * @return A neighborhood member.
 */
int getNeighborhoodMember(Neighborhood n, int index);

/**
 * @brief Get size  of given neighborhood.
 * 
 * @param n An instance of neighborhood.
 */
int getNeighborhoodSize(Neighborhood n);

/**
 * @brief Check if @p target is in neighborhood @p n.
 * 
 * @param n A neighborhood.
 * @param target An integer.
 * 
 * @return true if @p target is in neighborhood.
 */
bool isInNeighborhood(Neighborhood n, int target);

/**
 * @brief Test if member in @p n is colored in white.
 * 
 * @param n An instance of neighborhood.
 * @param color A list of vertices color.
 * 
 * @return true is @p n has white member.
 */
bool gotWhiteNeighborhood(Neighborhood n, int * color);

/**
 * @brief Test if member in @p n is colored in black.
 * 
 * @param n An instance of neighborhood.
 * @param color A list of vertices color.
 * 
 * @return true is @p n has black member.
 */
bool gotBlackNeighborhood(Neighborhood n, int * color);

/**
 * @brief Set grey member in neighborhood in white, if grey member already have black
 * member in his neighborhood, his color does'nt change.
 * 
 * @param graph An instance of graph.
 * @param target The neighborhood to check.
 * @param source The member to not change.
 * @param color Color of member.
 */
void setGreyNeighborhoodInWhite(Graph graph, int target, int source, int * color);

/**
 * @brief Print @p neighborhood
 * 
 * @param neighborhood A Neighborhood
 */
void printNeighborhood(Neighborhood n);
