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
    //the number of links this node has with a specified actor
    unsigned int links;
    //distance from the starting node
    unsigned int dist;
    //the parent tree in movietraveler
    ActorNode* parent;
    //size of the UnionFind
    unsigned int size;

    /* Constructor that initializes the ActorNode */
    ActorNode(string name) : name(name) {
        movies = std::vector<MovieNode*>();
        previous = 0;
        checked = false;
        links = 0;
        dist = (unsigned)-1;
        parent = 0;
        size = 1;
    }

};

struct CompareLinks {
    bool operator()( const ActorNode* a1, const ActorNode* a2 ) {

        if( a1->links == a2->links ) {
            return a1->name < a2->name;
        }

        return a2->links < a1->links;

    }
};

struct CompareDist {
    bool operator()( const ActorNode* a1, const ActorNode* a2 ) {
        return a2->dist < a1->dist;
    }
};
#endif  // ACTORNODE_HPP
