/**
 * The movietraveler program finds the smallest way to connect all of the
 * actors through their movies. It uses an algorithm called Kruskal's
 * algorithm in order to find the connected graph with the least weight.
 * Author: Christian Kouris
 * Email: ckouris@ucsd.edu
 * Sources: 
 */

#include "ActorGraph.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

/* 
 * Parameter: argc - the number of arguments passed into the command line
 * Parameter: argv - a list of all of the arguments passed in
 */
int main( int argc, char* argv[] ) {

    if( argc < 3 ) {
        return 0;
    }
    
    //build the graph using the first argument passed as the file
    ActorGraph actorGraph = ActorGraph();
    actorGraph.loadFromFile( argv[1] );
    //open outfile and put in the header
    ofstream outfile(argv[2]);
    outfile << "(actor)<--[movie#@year]-->(actor)" << endl;

    //get the vector of strings from ActorGraph
    vector<string> output = actorGraph.findSmallestTree();
    for( unsigned int i = 0; i < output.size(); i++ ) {

        outfile << output[i] << endl;

    }

    outfile.close();
    return 0;

}
