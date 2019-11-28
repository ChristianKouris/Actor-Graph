/**
 * TODO
 * Author: Christian Kouris
 * Email: ckouris@ucsd.edu
 * Sources: cplusplus doc for: std::unordered_map, std::string, 
 *          foward declaration
 */
#ifndef ACTORNODE_HPP
#define ACTORNODE_HPP

#include <string>
#include <vector>

using namespace std;

class MovieNode;

class ActorNode {

  public:
    
    //name of the actor
    string name; 
    //map of each connected actor and the movie that connects them
    vector<MovieNode*> movies;
    //previous MovieNode* in the BFS
    MovieNode* previous;
    //has been checked in the BFS already
    bool checked;

    /* Constructor that initializes the ActorNode */
    ActorNode(string name) : name(name) {
        movies = std::vector<MovieNode*>();
        previous = 0;
        checked = false;
    }

};


#endif  // ACTORNODE_HPP
