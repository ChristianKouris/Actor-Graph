/* This file represents the fucntions for the ActorGraph class. This class is
 * a graph that is filled with actors as vertices and movies as nodes. The 
 * actors are connected to other actors through the movies that they've played
 * in and thus this file contains functions that build and search through all
 * of the actors and movies in this graph.
 * Author: Christian Kouris
 * Email: ckouris@ucsd.edu
 * Sources: cplusplus documentation for: unordered_map, queue, string, vector,
 *          to_string
 */

#ifndef ACTORGRAPH_HPP
#define ACTORGRAPH_HPP

#include <iostream>
#include <unordered_map>
#include "ActorNode.hpp"
#include "MovieNode.hpp"

using namespace std;

/**
 * This class contains functions that have to do with the actors and movies in
 * the graph. The most important function is loadfromFile which takes in a 
 * database of actors in movies and builds the graph out of that information.
 * The other functions are used to search through the graph to find a bunch of
 * miscellaneous information.
 */
class ActorGraph {
  protected:
    
    //The hash maps that will hold the actors and the movies of the graph
    unordered_map<string, ActorNode*> actorMap;
    unordered_map<string, MovieNode*> movieMap;

  public:
    /**
     * Constuctor of the Actor graph
     */
    ActorGraph(void);

    /* Destructor for the ActorGraph */
    ~ActorGraph();
  
    /** 
     * This function takes in a formated file which contains a line seperated
     * list of an actor, a movie they played in, and the year of the movie and
     * builds either a weighted or unweighted graph based on the info. The
     * weight of the movie is how old the movie is which is 2020-year.
     * Parameter: in_filename - the name of the file containing the info
     * Parameter: use_weighted_edges - whether the graph is weighted or not
     */
    bool loadFromFile(const char* in_filename);
    
    /**
     * This method uses a breadth first search in order to find the 
     * shortest path between two actors. The return value is a formatted
     * string detailing the path from actorStart to actorEnd. The path
     * will have information about the connected movies and actors.
     * Parameter: actorStart - the actor that will be the start of the search
     * Parameter: actorEnd - the actor that will be found in the search
     */
    string findClosestActors(string actorStart, string actorEnd);

    /**
     * This method uses Dijkstra's Algoritm to find the closest actor
     * and movie chain from actorStart to actorEnd. The difference between
     * This algorithm and the findClosestActors alorithm is that the graph
     * for this one is weighted by movie age, so we can't effectively use
     * a Breadth First Search, we have to use Dijkstra's.
     * Parameter: actorStart - the starting actor in the actor/movie chain
     * Parameter: actorEnd - the ending actor in the actor/movie chain
     */
    string findWeightedActors(string actorStart, string actorEnd);
    
    /**
     * This method predicts what future links the given actor might have with
     * all of the other actors in the graph. The method seaches all of the
     * actors that have worked on the same movies as the input actor and makes
     * a sorted list of actors that have direclty collaborated and have not.
     * These two lists, directly collaborated and haven't collaborated, are
     * returned in a pair.
     * Parameter: actor - the actor which we want to find all of the links of
     */
    pair<vector<string>, vector<string>> getPredictedLinks( string actor );
    
};

#endif  // ACTORGRAPH_HPP
