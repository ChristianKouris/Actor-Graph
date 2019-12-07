/**
 * Author: Christian Kouris
 * Email: ckouris@ucsd.edu
 * Sources: cplusplus doc for: std::unordered_map, std::string, 
 *          foward declaration
 */
#ifndef UNIONFIND_HPP
#define UNIONFIND_HPP

#include "ActorNode.hpp"

using namespace std;

/* TODO
 */
class UnionFind {

  public:
    
    /* Constructor that initializes the UnionFind */
    UnionFind() {}
    
    ActorNode* find( ActorNode* node ){

        //finds the root
        ActorNode* root = node;
        while( root->parent != 0 ) {
            root = root->parent;
        }
        //make all the parent nodes of node point to root
        ActorNode* curNode = node;
        while( curNode != root ) {
            ActorNode* tmp = curNode->parent;
            curNode->parent = root;
            curNode = tmp;
        }
        return root;

    }

    ActorNode* unite( ActorNode* a1, ActorNode* a2 ) {

        ActorNode* root1 = find( a1 );
        ActorNode* root2 = find( a2 );
        //check to see if they are in the same tree
        if( root1 == root2 ) { return root1; }

        //else set one root to be the other's root
        if( root1->size < root2->size ) {
            root1->parent = root2;
            root2->size += root1->size;
            return root2;
        } else {
            root2->parent = a1;
            root1->size += root2->size;
            return root1;
        }

    }

};
#endif  // UNIONFIND_HPP
