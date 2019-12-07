/**
 * This data structure is used primarily to support both the union and
 * the find functions for disjointed sets. The find function will take an
 * element of type ActorNode* and fins the set that it belongs to. The union
 * function will take two sets and combine them into one.
 * Author: Christian Kouris
 * Email: ckouris@ucsd.edu
 * Sources: cplusplus doc for: std::unordered_map, std::string, 
 *          foward declaration
 */
#ifndef UNIONFIND_HPP
#define UNIONFIND_HPP

#include "ActorNode.hpp"

using namespace std;

/* The UnionFind class only has the unite and the find functions. The
 * constructor does nothing and there are no member variables. The find
 * function finds a set that an element belongs to and the unite function
 * combines two sets.
 */
class UnionFind {

  public:
    
    /* Constructor that initializes the UnionFind */
    UnionFind() {}
   
    /* The find function checks to see which set an ActorNode belongs to 
     * by searching for it's root. The root defines what set the node is in.
     * At the start of the tree finding each node is its own root in which
     * case their parent is set to 0. This progrm also sets node's parents
     * parent to be root to make union more efficient.
     * Parameter: node - the ActorNode that we want to find the set of
     */
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
    
    /* unite takes in two actornodes, finds the roots and then combines
     * the sets that the two ActorNodes were in into a single set. unite 
     * relies on the find function in order to find the sets to concatinate.
     * When the program is done, it returns the root to the new set that was
     * created.
     * Parameter: The first actornode we want to combine into a set
     * Parameter: The second actornode we want to combine into a set
     */
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
