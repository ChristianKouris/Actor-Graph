/**
 * This file finds the most optimal path between one actor and another
 * given the movies and actors they are connected to. This function can also
 * make it so that newer movies have a higher priority than older ones to 
 * make a better search. This file uses implementation from the ActorGraph
 * class.
 * Author: Christian Kouris
 * Email: ckouris@ucsd.edu
 * Sources: loadFromFile implementation
 */

#include "ActorGraph.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

/* This is the main driver of the path finder. It takes in the database file,
 * wether or not the movies should be weighted by year, the file of the list
 * of the connected actors, and the output file. The program calls either the
 * BFS function or the Dijkstra function from the ActorGraph class.
 * Parameter: argc - the number of arguments passed in + the function name
 * Parameter: argv - the list of arguments that were passed in.
 */
int main( int argc, char* argv[] ) {

    //check to see if there are exactly 4 arguments
    if( argc != 5 ) {
        return 0;
    }

    //will the graph be weighted or not
    bool isWeighted = false;
    if( *argv[2] == 'w' ) { isWeighted = true; }

    //create an ActorGraph and populate it with 
    ActorGraph actorGraph = ActorGraph();
    actorGraph.loadFromFile(argv[1]);
    
    //Open the pair file for reading and the outfile for writing
    ifstream pairfile(argv[3]);
    ofstream outfile(argv[4]);

    //print the header to the outfile
    outfile << "(actor)--[movie#@year]-->(actor)--..." << endl;

    //loop through each line and output the results to outfile
    bool header = true;
    while (pairfile) {
        string s;
        // get the next line
        if (!getline(pairfile, s)) break;
        if (header){ 
            header = false; 
            continue; }
        istringstream ss(s);
        vector<string> record;
        while (ss) {
            string str;
            // get the next string before hitting a tab character and put it in 'str'
            if (!getline(ss, str, '\t')) break;
            record.push_back(str);
        }
        if (record.size() != 2) {
            // we should have exactly 2 columns
            continue;
        }

        string actorStart(record[0]);
        string actorEnd(record[1]);
       
        //output the string based on if the graph is weighted or not
        if( isWeighted ) {

            outfile << 
                actorGraph.findWeightedActors(actorStart, actorEnd) << endl;

        } else {

            outfile << 
                actorGraph.findClosestActors(actorStart, actorEnd) << endl;

        }

    }

    pairfile.close();
    outfile.close();
    return 0;

}
