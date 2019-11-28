/**
 * TODO
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

/* TODO */
int main( int argc, char* argv[] ) {

    //check to see if there are exactly 4 arguments
    if( argc != 5 ) {
        return 0;
    }

    //will the graph be weighted or not
    bool isWeighted = false;
    if( *argv[2] == 'w' ) { isWeighted = true; }
    //name of the file containing pairs of actors
    string pairFileStr = argv[3];
    //name of the file we will print output to
    string outFileStr = argv[4];

    //create an ActorGraph and populate it with 
    ActorGraph actorGraph = ActorGraph();
    actorGraph.loadFromFile(argv[1], isWeighted);
    
    //Open the pair file for reading and the outfile for writing
    ifstream pairfile(pairFileStr);
    ofstream outfile(outFileStr);

    //loop through each line and output the results to outfile
    while (pairfile) {
        string s;
        // get the next line
        if (!getline(pairfile, s)) break;
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
       
        //method for finding two closest actors
        outfile << actorGraph.findClosestActors(actorStart, actorEnd) << endl;

    }

    pairfile.close();
    outfile.close();
    return 0;

}
