/**
 * This file defines a class that represents a ActorNode. The ActorNode 
 * represents a vertex in the Actor graph where the movies are the edges.
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

/*
 * The Actor Node class represents a vertex in the ActorGraph class. The class
 * holds an actor's name along with all of the movies they were in. The other 
 * members are used to help the path finding in the graph.
 */
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
