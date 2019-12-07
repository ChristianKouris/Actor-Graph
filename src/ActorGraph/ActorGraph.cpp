/* This file contains the method implementation for the ActorGraph class. 
 * This class is used to build a graph that has actors as vertices and 
 * movies as edges. This graph is used to find connections between actors
 * and to find future links that may happen.
 * Author: Christian Kouris
 * Email: ckouris@ucsd.edu
 * Sources: cplusplus documentation for: unordered_map, queue, string, vector,
 *          to_string, priority_queue, 
 *          stepik Dijkstra's AlgorithmKruskal's algorithm
 */

#include "ActorGraph.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <utility>
#include <algorithm>

using namespace std;

/**
 * Constructor of the Actor graph
 */ 
ActorGraph::ActorGraph(void) {
    actorMap = std::unordered_map<string, ActorNode*>();
    movieMap = std::unordered_map<string, MovieNode*>();
}

/** 
 * This function takes in a formated file which contains a line seperated
 * list of an actor, a movie they played in, and the year of the movie and
 * builds either a weighted or unweighted graph based on the info. The
 * weight of the movie is how old the movie is which is 2020-year.
 * Parameter: in_filename - the name of the file containing the info
 * Parameter: use_weighted_edges - whether the graph is weighted or not
 */
bool ActorGraph::loadFromFile(const char* in_filename) {

    // Initialize the file stream
    ifstream infile(in_filename);

    bool have_header = false;

    // keep reading lines until the end of file is reached
    while (infile) {
        string s;

        // get the next line
        if (!getline(infile, s)) break;

        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }

        istringstream ss(s);
        vector<string> record;

        while (ss) {
            string str;

            // get the next string before hitting a tab character and put it in 'str'
            if (!getline(ss, str, '\t')) break;
            record.push_back(str);
        }

        if (record.size() != 3) {
            // we should have exactly 3 columns
            continue;
        }

        string actor(record[0]);
        string movie_title(record[1]);
        int year = stoi(record[2]);
        
        //find the actor and the movie in the map or create them
        ActorNode* actorNode;
        if( actorMap.find( actor ) == actorMap.end() ) {
            actorNode = new ActorNode( actor );
            actorMap.insert( std::pair<string,ActorNode*>(actor, actorNode) );
        } else {
            actorNode = actorMap.find( actor )->second;
        }
        
        MovieNode* movieNode;
        string movieString = movie_title + '\t' + to_string(year);
        if( movieMap.find( movieString) == movieMap.end() ) {
            movieNode = new MovieNode( movie_title, year );
            movieMap.insert( 
                std::pair<string,MovieNode*>(movieString, movieNode) );

        } else {
            movieNode = movieMap.find(movieString)->second;
        }

        //connect the movie with the actor
        actorNode->movies.push_back( movieNode );
        movieNode->actors.push_back( actorNode );

    }
    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();

    return true;
}

/**
 * This method uses a breadth first search in order to find the 
 * shortest path between two actors. The return value is a formatted
 * string detailing the path from actorStart to actorEnd. The path
 * will have information about the connected movies and actors.
 * Parameter: actorStart - the actor that will be the start of the search
 * Parameter: actorEnd - the actor that will be found in the search
 */
string ActorGraph::findClosestActors(string actorStart, string actorEnd) {

    if( actorMap.find(actorStart) == actorMap.end() ) { return ""; }

    //create a queue and add the starting actor to it
    ActorNode* curActor = actorMap.find(actorStart)->second;
    queue<ActorNode*> actorQueue = std::queue<ActorNode*>();
    curActor->checked = true;
    actorQueue.push( curActor );

    //create a list to clean up the movies and the actors
    vector<ActorNode*> actorCleanup = std::vector<ActorNode*>();
    vector<MovieNode*> movieCleanup = std::vector<MovieNode*>();
    actorCleanup.push_back( curActor );

    //Do A BFS to find the end actor
    while( !actorQueue.empty() ) {

        //pop the curActor from the queue, set it as visited
        curActor = actorQueue.front(); actorQueue.pop();
        if( curActor->name == actorEnd ) { break; }

        //go through each movie and add the actors to the queue
        for( unsigned int i = 0; i < curActor->movies.size(); i++ ) {

            MovieNode* curMov = curActor->movies.at(i);
            if( curMov->checked == true ) { continue; }
            
            //add the actors in the movies to the queue
            for( unsigned int j = 0; j < curMov->actors.size(); j++ ) {
                if( curMov->actors.at(j)->checked == true ) { continue; }
                curMov->actors.at(j)->checked = true;
                curMov->actors.at(j)->previous = curMov;
                actorQueue.push( curMov->actors.at(j) );
                actorCleanup.push_back( curMov->actors.at(j) );
            }
            
            //set the previous actor and the checked to 1
            curMov->previous = curActor;
            curMov->checked = true;
            movieCleanup.push_back( curMov );

        }

    } //end while loop
    
    //check to see if we even found the node at all
    if( curActor == 0 || curActor->name != actorEnd ) {

        //clean up the actor nodes and movie nodes
        for( unsigned int i = 0; i < actorCleanup.size(); i++ ) {
            actorCleanup[i]->previous = 0;
            actorCleanup[i]->checked = false;
        }
        for( unsigned int i = 0; i < movieCleanup.size(); i++ ) {
            movieCleanup[i]->previous = 0;
            movieCleanup[i]->checked = false;
        }
        return "";
    
    }

    //backtrack to the start node and add each actor and movie to a stack
    stack<pair<ActorNode*,MovieNode*>> actorOrder = 
        std::stack<std::pair<ActorNode*,MovieNode*>>();
    while( curActor->previous != 0 ) {
    
        MovieNode* movieEdge = curActor->previous;
        //add the node and edge to a pair and to the stack
        pair<ActorNode*,MovieNode*> stackPair = 
            std::pair<ActorNode*,MovieNode*>( curActor, movieEdge );
        actorOrder.push( stackPair );
        curActor = curActor->previous->previous;

    }
    
    //start with the curNode which should be the starting actor
    string outStr = "(" + curActor->name + ")";
    //go through each pair in the stack and add it to the string
    while( !actorOrder.empty() ) {

        string actorName = actorOrder.top().first->name;
        string movieName = actorOrder.top().second->name;
        string movieYear = to_string(actorOrder.top().second->year);
        string formatStr = "--[" + movieName + "#@" + movieYear + 
                           "]-->(" + actorName + ")";
        outStr += formatStr;
        actorOrder.pop();

    }
    
    //clean up the actor nodes and movie nodes
    for( unsigned int i = 0; i < actorCleanup.size(); i++ ) {
        actorCleanup[i]->previous = 0;
        actorCleanup[i]->checked = false;
    }
    for( unsigned int i = 0; i < movieCleanup.size(); i++ ) {
        movieCleanup[i]->previous = 0;
        movieCleanup[i]->checked = false;
    }

    //return the string
    return outStr;

}

/**
 * This method uses Dijkstra's Algoritm to find the closest actor
 * and movie chain from actorStart to actorEnd. The difference between
 * This algorithm and the findClosestActors alorithm is that the graph
 * for this one is weighted by movie age, so we can't effectively use
 * a Breadth First Search, we have to use Dijkstra's.
 * Parameter: actorStart - the starting actor in the actor/movie chain
 * Parameter: actorEnd - the ending actor in the actor/movie chain
 */
string ActorGraph::findWeightedActors(string actorStart, string actorEnd) {

    if( actorMap.find(actorStart) == actorMap.end() ) { return ""; }

    //create a priority queue and add the starting actor to it
    ActorNode* curActor = 0;
    priority_queue<ActorNode*, vector<ActorNode*>, CompareDist> actorQueue =
        std::priority_queue<ActorNode*, vector<ActorNode*>, CompareDist>();

    ActorNode* tmpActor = actorMap.find(actorStart)->second;
    tmpActor->dist = 0;
    actorQueue.push( tmpActor );

    //create a list to clean up the movies and the actors
    vector<ActorNode*> actorCleanup = std::vector<ActorNode*>();
    vector<MovieNode*> movieCleanup = std::vector<MovieNode*>();
    actorCleanup.push_back( tmpActor );

    //use Dijkstra's algorithm
    while( !actorQueue.empty() ) {

        //pop the curActor from the queue, set it as visited
        curActor = actorQueue.top(); actorQueue.pop();
        if( curActor->name == actorEnd ) { break; }
        if( curActor->checked == true ) { continue; }
        curActor->checked = true;
        
        //go through each movie and add the actors to the queue
        for( unsigned int i = 0; i < curActor->movies.size(); i++ ) {

            MovieNode* curMov = curActor->movies.at(i);
            if( curMov->checked == true ) { continue; }
            
            //add the actors in the movies to the queue
            for( unsigned int j = 0; j < curMov->actors.size(); j++ ) {
                
                tmpActor= curMov->actors.at(j);
                unsigned int totalDist = curActor->dist + curMov->weight;
                if( totalDist < tmpActor->dist ) {
                    tmpActor->dist = totalDist;
                    tmpActor->previous = curMov;
                    actorQueue.push( tmpActor );
                    actorCleanup.push_back( tmpActor );
                }
            
            }
            
            //set the previous actor and the checked to 1
            curMov->previous = curActor;
            curMov->checked = true;
            movieCleanup.push_back( curMov );

        }

    }

    //check to see if we even found the node at all
    if( curActor == 0 || curActor->name != actorEnd ) {
        //clean up the actor nodes and movie nodes
        for( unsigned int i = 0; i < actorCleanup.size(); i++ ) {
            actorCleanup[i]->previous = 0;
            actorCleanup[i]->checked = false;
            actorCleanup[i]->dist = (unsigned)-1;
        }
        for( unsigned int i = 0; i < movieCleanup.size(); i++ ) {
            movieCleanup[i]->previous = 0;
            movieCleanup[i]->checked = false;
        }
        return "";
    }
    
    //backtrack to the start node and add each actor and movie to a stack
    stack<pair<ActorNode*,MovieNode*>> actorOrder = 
        std::stack<std::pair<ActorNode*,MovieNode*>>();
    while( curActor->previous != 0 ) {
    
        MovieNode* movieEdge = curActor->previous;
        //add the node and edge to a pair and to the stack
        pair<ActorNode*,MovieNode*> stackPair = 
            std::pair<ActorNode*,MovieNode*>( curActor, movieEdge );
        actorOrder.push( stackPair );
        curActor = curActor->previous->previous;

    }
    
    //start with the curNode which should be the starting actor
    string outStr = "(" + curActor->name + ")";
    //go through each pair in the stack and add it to the string
    while( !actorOrder.empty() ) {

        string actorName = actorOrder.top().first->name;
        string movieName = actorOrder.top().second->name;
        string movieYear = to_string(actorOrder.top().second->year);
        string formatStr = "--[" + movieName + "#@" + movieYear + 
                           "]-->(" + actorName + ")";
        outStr += formatStr;
        actorOrder.pop();

    }
    
    //clean up the actor nodes and movie nodes
    for( unsigned int i = 0; i < actorCleanup.size(); i++ ) {
        actorCleanup[i]->previous = 0;
        actorCleanup[i]->checked = false;
        actorCleanup[i]->dist = (unsigned)-1;
    }
    for( unsigned int i = 0; i < movieCleanup.size(); i++ ) {
        movieCleanup[i]->previous = 0;
        movieCleanup[i]->checked = false;
    }

    //return the string
    return outStr;

}

/**
 * This method predicts what future links the given actor might have with
 * all of the other actors in the graph. The method seaches all of the
 * actors that have worked on the same movies as the input actor and makes
 * a sorted list of actors that have direclty collaborated and have not.
 * These two lists, directly collaborated and haven't collaborated, are
 * returned in a pair.
 * Parameter: actor - the actor which we want to find all of the links of
 */
pair<vector<string>, vector<string>> 
    ActorGraph::getPredictedLinks( string actor ) {
    
    //check to see if the actor isn't in the map, return empty arrays if so
    if( actorMap.find( actor ) == actorMap.end() ) {
        vector<string> noStr = std::vector<string>();
        return pair<vector<string>, vector<string>>(noStr, noStr);
    }
    
    vector<ActorNode*> collabActors = std::vector<ActorNode*>();
    vector<ActorNode*> futureActors = std::vector<ActorNode*>();
    vector<MovieNode*> movieCleanup = std::vector<MovieNode*>();
    //loop through all of the actors immediately connected to the given actor
    ActorNode* actorNode = actorMap.find( actor )->second;
    actorNode->checked = true;
    for( unsigned int i = 0; i < actorNode->movies.size(); i++ ) {
        MovieNode* movieNode = actorNode->movies.at(i);
        for( unsigned int j = 0; j < movieNode->actors.size(); j++ ) {
            ActorNode* tmp = movieNode->actors.at(j);
            if( tmp->links == 0 ) {
                collabActors.push_back( tmp );
            }
            tmp->checked = true;
            tmp->links++;
        }
    }

    //sort the list of actor nodes
    std::sort( collabActors.begin(), collabActors.end(), CompareLinks() );
    
    //loop through all of the first generation actors
    for( unsigned int i = 0; i < collabActors.size(); i++ ) {

        ActorNode* curActor = collabActors[i];
        //loop through each movie in curActor
        for( unsigned int j = 0; j < curActor->movies.size(); j++ ) {

            MovieNode* movieNode = curActor->movies.at(j);
            //loop through each actor for each movie for each first gen actor
            for( unsigned int k = 0; k < movieNode->actors.size(); k++ ) {
    
                ActorNode* tmp = movieNode->actors.at(k);
                if( tmp->checked ) { continue; }
                if( tmp->links == 0 ) {
                    futureActors.push_back( tmp );
                }
                //instead of adding by one, add by cur actor # links
                tmp->links += curActor->links;

            }

        }

    } // end add future actors links

    //sort the second list
    std::sort( futureActors.begin(), futureActors.end(), CompareLinks() );
    
    //add the string vectors in order to return
    vector<string> collabStr = std::vector<string>();
    vector<string> futureStr = std::vector<string>();

    //add the actor names from the list to a vector
    for( unsigned int i = 0; i < 4; i++ ) {
        if( i < collabActors.size() ) {
            collabStr.push_back( collabActors[i]->name );
        }
        if( i < futureActors.size() ) {
            futureStr.push_back( futureActors[i]->name );
        }
    }

    //reset all the values that we've changed
    actorNode->checked = false;
    for( unsigned int i = 0; i < collabActors.size(); i++ ) {
        collabActors[i]->links = 0;
        collabActors[i]->checked = false;
    }
    for( unsigned int i = 0; i < futureActors.size(); i++ ) {
        futureActors[i]->links = 0;
        futureActors[i]->checked = false;
    }

    return std::pair<vector<string>,vector<string>>( collabStr, futureStr );

}

/**
  * This function uses Kruskal's algorithm along with the UnionFind
  * datastructure in order to create the smallest spanning tree given
  * a connected graph. This program will output a vector of the
  * strings expected to be printed out.
  */
vector<string> ActorGraph::findSmallestTree() {
    
    //create a forest along with an ordered set of edges
    unordered_set<ActorNode*> forest = unordered_set<ActorNode*>();
    auto actorMapIter = actorMap.begin();
    while( actorMapIter != actorMap.end() ) {
        forest.insert( actorMapIter->second );
        actorMapIter++;
    }
    vector<MovieNode*> edges = std::vector<MovieNode*>();
    auto movieMapIter = movieMap.begin();
    while( movieMapIter != movieMap.end() ) {
        edges.push_back( movieMapIter->second );
        movieMapIter++;
    }
    std::sort( edges.begin(), edges.end(), CompareWeight() );
    UnionFind unionFind = UnionFind();
    //create a set to store all of the MovieNodes that have been effected
    unordered_set<MovieNode*> treeEdge = std::unordered_set<MovieNode*>(); 
    unsigned int numEdges = 0; unsigned int numWeights = 0;

    //go trough the edges from smallest to largest weight
    while( forest.size() > 1 ) {
        
        //get the lowest weighted edge
        for( unsigned int i = 0; i < edges.size(); i++ ) {
            
            MovieNode* curEdge = edges[i];

            //get two actors from the edge and check if they work
            for( unsigned int j = 0; j < curEdge->actors.size(); j++ ) {
                ActorNode* actor1 = curEdge->actors.at(j);
                for( unsigned int k = j; k < curEdge->actors.size(); k++ ) {

                    ActorNode* actor2 = curEdge->actors.at(k);
                    if( unionFind.find(actor1) != unionFind.find(actor2) ) {

                        forest.erase( actor1 );
                        forest.erase( actor2 );
                        ActorNode* root = unionFind.unite( actor1, actor2 );
                        forest.insert( root );
                        curEdge->actorPairs.push_back( 
                            pair<ActorNode*, ActorNode*>( actor1, actor2 ));
                        if( treeEdge.find( curEdge ) == treeEdge.end() ) {
                            treeEdge.insert( curEdge );
                        }
                        numEdges++;
                        numWeights += curEdge->weight;

                    }

                    if( forest.size() <= 1 ) { break; }

                }
                
                if( forest.size() <= 1 ) { break; }
            
            } 

            if( forest.size() <= 1 ) { break; }

        }
        
        if( forest.size() <= 1 ) { break; }

    } // end while

    //now we have the tree, print out the movie nodes connected to it
    vector<string> outVector = std::vector<string>();
    auto edgeIter = treeEdge.begin();
    while( edgeIter != treeEdge.end() ) {
    
        //derefencing iter gives movie node pointer
        MovieNode* curEdge = *edgeIter;
        for( unsigned int j = 0; j < curEdge->actorPairs.size(); j++ ) {

            ActorNode* actor1 = curEdge->actorPairs.at(j).first;
            ActorNode* actor2 = curEdge->actorPairs.at(j).second;
            string outstr = "(" + actor1->name + ")<--[" + curEdge->name +
                            "#@" + to_string(curEdge->year) + "]-->(" + 
                            actor2->name + ")";
            outVector.push_back( outstr );

        }
        edgeIter++;

    }

    string nodeStr = "#NODE CONNECTED: " + to_string((*forest.begin())->size);
    outVector.push_back( nodeStr );
    string edgeStr = "#EDGE CHOSEN: " + to_string( numEdges );
    outVector.push_back( edgeStr );
    string weightStr = "TOTAL EDGE WEIGHTS: " + to_string( numWeights );
    outVector.push_back( weightStr );

    return outVector;

}

/* Destructor for the ActorGraph */
ActorGraph::~ActorGraph() {
    
    auto actorIter = actorMap.begin();
    auto movieIter = movieMap.begin();
    while( actorIter != actorMap.end() ) {
        delete actorIter->second;
        actorIter++;
    }
    while( movieIter != movieMap.end() ) {
        delete movieIter->second;
        movieIter++;
    } 

}
