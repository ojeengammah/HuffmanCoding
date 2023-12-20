/*
 * Name: Ojeen Gammah
 * Email: ogammah@ucsd.edu
 *
 */

#include<fstream>
#include <iostream>
#include "HCTree.hpp"
#include <queue>

int main (int argc, char** argv){
    const int num_args = 3;
    const int secondargs = 2;
    if (argc != num_args){
        std::cout << "must have three arguments" << std::endl;
    }
    else {
        FancyOutputStream out (argv[secondargs]);
        unsigned char readIn; 
		FancyInputStream fis(argv[1]);
        if (fis.good()){
            HCTree tree;
            tree.readHeader(fis);
            unsigned long x = 0;
            while (fis.good() && x < tree.getFileSize()) {
                readIn = tree.decode(fis);
				out.write<unsigned char>(readIn);
                x++;
            }
        }
        else {
            // some kinda error message
            std::cout << "File doesn't exist" << std::endl;
        }
    }
}
