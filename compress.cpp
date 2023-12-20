/*
 * Name: Ojeen Gammah
 * Email: ogammah@ucsd.edu
 
 */

#include<fstream>
#include <iostream>
#include "HCTree.hpp"
#include <queue>

int main (int argc, char** argv){
    const int num_args = 3; 
    const int uniquebyte = 256;
    const int secondargs = 2;
    if (argc != num_args){
        std::cout << "must have three arguments" << std::endl;
    }
    else {
       FancyInputStream in (argv[1]);
        unsigned char readIn;
        if (in.good()){
            vector <int> counts (uniquebyte,0);
            while (in.good()){
                readIn = in.read<unsigned char>();
                counts[readIn]++;
            }
            HCTree tree;
            tree.build(counts);
            FancyOutputStream fos(argv[secondargs]);
            tree.writeHeader(fos, in.filesize());
            in.reset();
            long x = 0;
            while (in.good() && x < in.filesize()) {
                readIn = in.read<unsigned char>();
                tree.encode(readIn, fos);
                x++;
            }
        }
        else {
            // some kinda error message
            std::cout << "File doesn't exist" << std::endl;
        }
    }
}
