/**
 * This file defines a class that represents a MovieNode. A MovieNode is 
 * simply an edge in the ActorGraph and connects two actors together 
 * that are in the same movie.
 * Author: Christian Kouris
 * Email: ckouris@ucsd.edu
 * Sources: cplusplus doc for: std::unordered_map, std::string
 */
#ifndef MOVIENODE_HPP
#define MOVIENODE_HPP

#include <string>
#include <unordered_map>
#include "ActorNode.hpp"

using namespace std;

/* The MovieNode class represents an edge in the graph of actors. The class
 * itself holds all of the actors that were in the movie as well as the name
 * and year of the movie. All the other members of the class are used to help 
 * with the search algorithms.
 */
class MovieNode {

  public:
    
    //name and year of the movie
    string name; 
    unsigned int year;
    //list of each actor in the movie and their pointers 
    vector<ActorNode*> actors;
    //previous ActorNode* in the BFS
    ActorNode* previous;
    //has been checked in the BFS already
    bool checked;

    /* Constructor that initializes the MovieNode */
    MovieNode(string name, unsigned int year) : name(name), year(year) {
        actors = std::vector<ActorNode*>();
        previous = 0;
        checked = false;
    }

};


#endif  // MOVIENODE_HPP
