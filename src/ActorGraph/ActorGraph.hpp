/*
 * Author: Christian Kouris
 * Email: ckouris@ucsd.edu
 * Sources: cplusplus documentation for: unordered_map, queue, string, vector,
 *          to_string
 */

#ifndef ACTORGRAPH_HPP
#define ACTORGRAPH_HPP

#include <iostream>
#include "ActorNode.hpp"
#include "MovieNode.hpp"

// Maybe include some data structures here

using namespace std;

/**
 * TODO: add class header
 */
class ActorGraph {
  protected:
    // Maybe add class data structure(s) here
    unordered_map<string, ActorNode*> actorMap;
    unordered_map<string, MovieNode*> movieMap;

  public:
    /**
     * Constuctor of the Actor graph
     */
    ActorGraph(void);

    /* Destructor for the ActorGraph */
    ~ActorGraph();
    
    // Maybe add some more methods here
  
    /** 
     * 
     */
    bool loadFromFile(const char* in_filename, bool use_weighted_edges);
    
    /* This method uses a breadth first search in order to find the 
     * shortest path between two actors. The return value is a formatted
     * string detailing the path from actorStart to actorEnd. The path
     * will have information about the connected movies and actors.
     * Parameter: actorStart - the actor that will be the start of the search
     * Parameter: actorEnd - the actor that will be found in the search
     */
    string findClosestActors(string actorStart, string actorEnd);
    
};

#endif  // ACTORGRAPH_HPP
