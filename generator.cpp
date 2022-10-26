#include <ctime>
#include <iostream>
#include <fstream>
#include <getopt.h>
#include <set>

//#include <string>
//#include <sstream>

#include "grid.h"
#include "util.h"

std::string usage();

#define OPTS "s:o:p:x:y:h"



int main(int const argc, char *argv[]) {
    exec::execname(argv[0]);

    // Options
    std::string outfile = "-";
    std::string imgfile;
    unsigned seed = time(nullptr);
    unsigned height = 25;
    unsigned width = 25;

    // Getopts
    int opt;
    while ((opt = getopt(argc, argv, OPTS)) != -1) {
        switch (opt) {
            case 's': seed = std::stol(optarg); break;
            case 'o': outfile = optarg; break;
            case 'p': imgfile = optarg; break;
            case 'x': width = std::stol(optarg); break;
            case 'y': height = std::stol(optarg); break;
            case 'h': std::cout << usage(); return 0;
            default: std::cerr << usage(); return 1;
        }
    }

    // Prep Generation -- We're reserving some weight values
    // 0            : reflexive edge
    // 1            : path edge
    // Grid::NOEDGE : There is no edge
    srandom(seed);
    Grid g {height, width};
    std::set<Grid::weight_t> used_weights {0, Grid::PATH, Grid::NOEDGE};

    // Generate unique edges
    for (unsigned y = 0; y < g.getHeight(); ++y) {
        for (unsigned x = 0; x < g.getWidth(); ++x) {
            // Set Down Edge
            if (y + 1 != g.getHeight()) {
                Grid::weight_t roll;
                do {
                    roll = random();
                } while (used_weights.contains(roll));
                g.setEdge({x,y}, {x, y+1}, roll);
                used_weights.insert(roll);
            }

            // Set Right Edge
            if (x + 1 != g.getWidth()) {
                Grid::weight_t roll;
                do {
                    roll = random();
                } while (used_weights.contains(roll));
                g.setEdge({x,y}, {x+1, y}, roll);
                used_weights.insert(roll);
            }
        }
    }

    // Serialize
    if (outfile == "-") {
        g.serialize(std::cout);
    } else {
        std::ofstream out {outfile};
        g.serialize(out);
    }

    // Output ppm
    if (imgfile == "-") {
        g.draw(std::cout);
    } else if (imgfile.length() > 0){
        std::ofstream out {imgfile};
        g.draw(out);
    }

    /*
    std::ifstream fin("0_5x10.dat", std::ios_base::in);
    read(fin).serialize(std::cout);
    fin.close();
    */

    // Fin
    return exec::status();
}

std::string usage() {
    return std::string{}
        + "SYNOPSIS\n"
        + "     Generate randomized \"grid-graph\" and output to file\n"
        + "\n"
        + "USAGE\n"
        + "     " + exec::execname() + " [-" + OPTS + "]\n"
        + "\n"
        + "OPTIONS\n"
        + "     -s seed             Provide seed for PRNG\n"
        + "     -o output_file      Dump information to reproduce grid to file [default: stdout]\n"
        + "     -p img.ppm          Generate ppm image of grid\n"
        + "     -x width            Set width  [default: 25]\n"
        + "     -y height           Set height [default: 25]\n"
        + "     -h                  Print out helpful information\n"
        ;
}

