#include <iostream>
#include <fstream>
#include <getopt.h>
#include <stack>
#include <set>
#include <sstream>
#include "grid.h"
#include "util.h"

std::string usage();

#define OPTS "i:o:s:e:p:h"

int main (int const argc, char *argv[]) {
    exec::execname(argv[0]);
    std::string infile = "-";
    //std::string datType = ".gen.dat";
    std::string outfile = "-";
    std::string imgfile;
    std::ifstream input;
    Grid g(0,0);
    //Grid::weight_t m,n,l,o,p;
    unsigned x = 0;
    unsigned y = 0;
    
    unsigned a = 0;
    unsigned b = 0;

    int opt;
    while ((opt = getopt(argc, argv, OPTS)) != -1) {
        switch (opt) {
            case 'i': infile = optarg; break;
            case 'o': outfile = optarg; break;
            case 's': sscanf(optarg, "%u:%u\n", &x,&y); break;
            case 'e': sscanf(optarg, "%u:%u\n", &a,&b); break;
            case 'p': imgfile = optarg; break;
            case 'h': std::cout << usage(); return 0;
            default: std::cerr << usage(); return 1;
        }
    }
    
    if(infile == "-"){
        std::string str;
        std::stringstream in;
        do{
            getline(std::cin, str);
            in << str + "\n";
        }while(!str.empty());
        g = Grid::load(in);

        /*
        std::string line;
        Grid::weight_t c, d;
        getline(std::cin, line);
        sscanf(line.c_str(), "%u %u\n", &c,&d);
        Grid sub(d, c);
        g = sub;
        do{
            getline(std::cin, line);
            sscanf(line.c_str(), "(%u,%u)%u(%u,%u)\n", &m, &n, &l, &o, &p);
            g.setEdge({m,n},{o,p},l);
        }while(!line.empty());
        */
        
    }
    else{
        input.open(infile);
        if(input.fail()){
           std::cerr << "invalid file\n";
           exit(100); 
        }
        g = Grid::load(input);
    }
    


    //Grid g{Grid::load(input)};
    Point dest{0,0};

    if( a != 0 || b != 0){
        dest.x = a;
        dest.y = b;
    }
    else{
        dest.x = g.getWidth() - 1;
        dest.y = g.getHeight() - 1;
    }
    //std::cout <<g.getWidth() << " " << g.getHeight() <<std::endl;

    std::stack<Point> PointStack;
    std::set<Point> Set;
    PointStack.push({x,y});
    Set.insert({x,y});
    int depth = -1;

    std::stack<int> ways;
    ways.push(0);
    while(PointStack.top() != dest){
        while(((g.getEdge({x,y},{x + 1, y}) != Grid::NOEDGE) && (Set.find({x + 1, y}) == Set.end())) || ((g.getEdge({x,y},{x - 1, y}) != Grid::NOEDGE) && (Set.find({x - 1, y}) == Set.end())) 
            || ((g.getEdge({x,y},{x, y + 1}) != Grid::NOEDGE) && (Set.find({x, y + 1}) == Set.end())) || ((g.getEdge({x,y},{x, y - 1}) != Grid::NOEDGE) && (Set.find({x, y - 1}) == Set.end())) ){

            if(PointStack.top() == dest){
                break;
            }
            Point min;
            int pass = 0;
            std::vector<Point> points;
            Grid::weight_t minEdge = -1;
            points.push_back({x + 1, y});
            points.push_back({x - 1, y});
            points.push_back({x, y + 1});
            points.push_back({x, y - 1});
            for(int i = 0; i < 4; i++){
                if((g.getEdge({x,y},points[i]) != Grid::NOEDGE) && (Set.find(points[i]) == Set.end()) ){
                    pass++;
                    if(g.getEdge({x,y},points[i]) < minEdge){
                        minEdge = g.getEdge({x,y},points[i]); //min priority
                        min = points[i]; //added point
                    }             
                }
            }
            PointStack.push(min);
            Set.insert({x,y});
            depth++;
            x = PointStack.top().x;
            y = PointStack.top().y; 
            if(pass > 1)
                ways.push(depth);
            //std::cout << ways.top() << std::endl;
            //std::cout << PointStack.top() << std::endl;
            //std::cout << depth << std::endl;
        }
        depth++;
        if(PointStack.top() != dest){
            Set.insert({x,y});
            while(depth - ways.top() != 0){
                PointStack.pop();
                depth--;
            }
            ways.pop();
            x = PointStack.top().x;
            y = PointStack.top().y; 
        }
        depth--;
    }

    while(!PointStack.empty()){
        Point a = PointStack.top();
        PointStack.pop();
        if(PointStack.empty()){
            break;
        }
        Point b = PointStack.top();
        g.setEdge(a, b, Grid::PATH);
    }

    //output command
    if(outfile == "-"){
        g.serialize(std::cout);
    }
    else{
        std::ofstream out {outfile};
        g.serialize(out);
    }

    //ppm command
    if(imgfile == "-"){
        g.draw(std::cout);
    }
    else if(imgfile.length()>0){
        std::ofstream out {imgfile};
        g.draw(out);
    }

    // Fin
    return exec::status();
}

std::string usage() {
    return std::string{}
        + "SYNOPSIS\n"
        + "     Find the path between two points in a maze\n"
        + "\n"
        + "USAGE\n"
        + "     " + exec::execname() + " [-" + OPTS + "]\n"
        + "\n"
        + "OPTIONS\n"
        + "     -i input.dat        input file with serialized data [default: stdin]\n"
        + "     -o outfile.dat      output file in serialized form [default: stdout]\n"
        + "     -s x:y              starting point for finding path [default: 0:0]\n"
        + "     -e x:y              ending point for finding path [default: bottom right]\n"
        + "     -p img.ppm          generate ppm image of solved maze\n"
        + "     -h                  Print out helpful information\n"
        ;
}
