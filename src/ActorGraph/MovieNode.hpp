/**
 * TODO
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

    /* Constructor that initializes the ActorNode */
    MovieNode(string name, unsigned int year) : name(name), year(year) {
        actors = std::vector<ActorNode*>();
        previous = 0;
        checked = false;
    }

};


#endif  // MOVIENODE_HPP
