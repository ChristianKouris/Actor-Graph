/*
 * Author: Christian Kouris
 * Email: ckouris@ucsd.edu
 * Sources: cplusplus documentation for: unordered_map, queue, string, vector,
 *          to_string
 */

#include "ActorGraph.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <stack>
#include <utility>

using namespace std;

/**
 * Constructor of the Actor graph
 */ 
ActorGraph::ActorGraph(void) {

    actorMap = std::unordered_map<string, ActorNode*>();
    movieMap = std::unordered_map<string, MovieNode*>();

}

/** You can modify this method definition as you wish
 *
 * Load the graph from a tab-delimited file of actor->movie relationships.
 *
 * in_filename - input filename
 * use_weighted_edges - if true, compute edge weights as 1 + (2019 - movie_year),
 *                      otherwise all edge weights will be 1
 *
 * return true if file was loaded sucessfully, false otherwise
 */
bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges) {

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
     
/* This method uses a breadth first search in order to find the 
 * shortest path between two actors. The return value is a formatted
 * string detailing the path from actorStart to actorEnd. The path
 * will have information about the connected movies and actors.
 * Parameter: actorStart - the actor that will be the start of the search
 * Parameter: actorEnd - the actor that will be found in the search
 */
string ActorGraph::findClosestActors(string actorStart, string actorEnd) {

    if( actorMap.find(actorStart) == actorMap.end() ) { return ""; }

    //create a queue and add the starting actor to it
    ActorNode* curActor = 0;
    queue<ActorNode*> actorQueue = std::queue<ActorNode*>();
    actorQueue.push( actorMap.find(actorStart)->second );

    //create a list to clean up the movies and the actors
    vector<ActorNode*> actorCleanup = std::vector<ActorNode*>();
    vector<MovieNode*> movieCleanup = std::vector<MovieNode*>();

    //Do A BFS to find the end actor
    while( !actorQueue.empty() ) {

        //pop the curActor from the queue, set it as visited
        curActor = actorQueue.front(); actorQueue.pop();
        curActor->checked = true;
        if( curActor->name == actorEnd ) { break; }

        //go through each movie and add the actors to the queue
        for( unsigned int i = 0; i < curActor->movies.size(); i++ ) {

            MovieNode* curMov = curActor->movies.at(i);
            if( curMov->checked == true ) { continue; }
            
            //add the actors in the movies to the queue
            for( unsigned int j = 0; j < curMov->actors.size(); i++ ) {
                if( curMov->actors.at(j)->checked == true ) { continue; }
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
