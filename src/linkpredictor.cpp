/**
 * This file predicts whether or not two actors will come together to create
 * a new movie based on the movies they acted in previously. This takes into
 * consideration how many movies the two actors worked on or how many actors
 * they have worked with previously. The file outputs two different lists, one
 * with actors that been in the same movies and ones that have not.
 * Author: Christian Kouris
 * Email: ckouris@ucsd.edu
 * Sources: loadFromFile implementation, std::vector docs, std::pair docs,
 *          docs for ofstream and ifstream
 */

#include "ActorGraph.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

/* This is the main driver for the linkpredictor program. This takes in a file
 * to create the graph, a file of actors to get the future links, and two 
 * output files for each list of actors. This file relies on the program 
 * getPredictedLinks from the ActorGraph class.
 * Parameter: argc - the number of arguments passed into the command line
 * Parameter: argv - a list of all of the arguments passed in
 */
int main( int argc, char* argv[] ) {

    //check to see if there are exactly 4 arguments
    if( argc != 5 ) {
        return 0;
    }
   
    //build the graph using the first argument passed as the file
    ActorGraph actorGraph = ActorGraph();
    actorGraph.loadFromFile( argv[1] );
    //open up the infile and the two outfiles
    ifstream actorfile(argv[2]);
    ofstream outfilecollab(argv[3]);
    ofstream outfilepred(argv[4]);

    //print the headers to the two outfiles
    outfilecollab << "Actor1,Actor2,Actor3,Actor4" << endl;
    outfilepred << "Actor1,Actor2,Actor3,Actor4" << endl;

    //loop through each actor and print to both output files
    bool header = true;
    while (actorfile) {
        string str;
        // get the next line
        if (!getline(actorfile, str)){ break; }
        //skip the header
        if (header){ 
            header = false; 
            continue; 
        }
        string actor = str;
       
        //use the ActorGraph method to get a list of the actors
        pair<vector<string>,vector<string>> actorList = 
            actorGraph.getPredictedLinks( actor );
        vector<string> collabList = actorList.first;
        vector<string> futureList = actorList.second;

        if( collabList.size() != 0 ) {
            string collabStr = collabList[0];
            for( unsigned int i = 1; i < collabList.size(); i++ ) {
                collabStr + '\t' + collabList[i];
            }
            outfilecollab << collabStr << endl;
        }
        if( futureList.size() != 0 ) {
            string futureStr = futureList[0];
            for( unsigned int i = 1; i < futureList.size(); i++ ) {
                futureStr + '\t' + futureList[i];
            }
            outfilepred << futureStr << endl;
        }

    }

    actorfile.close();
    outfilecollab.close();
    outfilepred.close();
    return 0;

}
