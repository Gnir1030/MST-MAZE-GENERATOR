#include <iostream>
#include <fstream>
#include <getopt.h>
#include <set>
#include "grid.h"
#include "util.h"

std::string usage();

#define OPTS "i:o:p:h"

template<typename Iter, typename Cmp>
void edgeSort(Iter beg, Iter end, Cmp cmp = {}){
    for(auto ceil = end; ceil != beg; --ceil){
        for(auto iter = beg, next = beg + 1; next != ceil; ++iter, ++next){
            if(cmp(*next, *iter))
                std::swap(*iter,*next);
        }
    }
}

struct Container{
    Point x, y;
    Grid::weight_t edge;
};
std::ostream& operator<<(std::ostream &out, Container const &c) {
    return out << c.x << c.edge << c.y;
}


int main(int const argc, char *argv[]) {
    exec::execname(argv[0]);
    std::string infile = "-";
    //std::string datType = ".gen.dat";
    std::string outfile = "-";
    std::string imgfile;
    std::ifstream input;
    std::vector<Container> edges;
    Grid g(0,0);
    Grid::weight_t a,b,edge,c,d;
    std::vector<std::set<Point>> PointSet;


    // Getopts
    int opt;
    while ((opt = getopt(argc, argv, OPTS)) != -1) {
        switch (opt) {
            case 'i': infile = optarg; break;
            case 'o': outfile = optarg; break;
            case 'p': imgfile = optarg; break;
            case 'h': std::cout << usage(); return 0;
            default: std::cerr << usage(); return 1;
        }
    }
    
    //input command
    
    if(infile == "-"){
        std::string line;
        getline(std::cin, line);
        sscanf(line.c_str(), "%u %u\n", &a,&b);
        Grid sub(b, a);
        g = sub;
        do{
            getline(std::cin, line);
            sscanf(line.c_str(), "(%u,%u)%u(%u,%u)\n", &a, &b, &edge, &c, &d);
            edges.push_back(Container{{a,b},{c,d},edge});
        }while(!line.empty());
    }
    else{
        std::string line;
        input.open(infile);
        if(input.fail()){
            std::cerr << "invalid file\n";
            exit(100); 
        }
        getline(input, line);
        sscanf(line.c_str(), "%u %u\n", &a,&b);
        Grid sub(b, a);
        g = sub;
        while(!input.eof() && line != ""){
            getline(input, line);
            sscanf(line.c_str(), "(%u,%u)%u(%u,%u)\n", &a, &b, &edge, &c, &d);
            edges.push_back(Container{{a,b},{c,d},edge});
        }
    }

        
    //sort string vector
        
    edgeSort(edges.begin(), edges.end(), [](auto const &a, auto const &b){
        return a.edge < b.edge;
    });

    auto i = edges.begin();
    while(i != edges.end()){
        if(i == edges.begin()){
            std::set<Point> a;
            a.insert((*i).x);
            a.insert((*i).y);
            PointSet.push_back(a);
            i++;
        }
        else{
            bool x,y;
            auto j = PointSet.begin();
            while(j != PointSet.end()){
                x = (*j).insert((*i).x).second;
                y = (*j).insert((*i).y).second;
                if(!x && !y){
                    j = PointSet.end();
                    edges.erase(i); //delete pairs set => next pairs set
                }
                else if(!x || !y){
                    auto IterCurrent = j;
                    j++; //from pairs set
                    while(j != PointSet.end()){
                        if((*j).find((*i).y) != (*j).end() || (*j).find((*i).x) != (*j).end()){
                            (*IterCurrent).insert((*j).begin(), (*j).end());
                            PointSet.erase(j);   
                            j = PointSet.end();
                        }
                        else{
                            j++;
                        }
                    }
                    i++;
                }
                else{
                    (*j).erase((*i).y);
                    (*j).erase((*i).x);
                    j++;
                    if(j == PointSet.end()){
                        std::set<Point> b;
                        b.insert((*i).x);
                        b.insert((*i).y);
                        PointSet.push_back(b);
                        j = PointSet.end();
                        i++;
                    }
                }
            }
        }
        /*
        for(auto k : PointSet){
                for(auto j : k){
                    std::cout << j << " ";
                }
                std::cout << "\n";
        }
        std::cout << "\n";
        */ //set progress debug
    }

    for(auto i : edges){
        g.setEdge(i.x, i.y, i.edge);
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
        + "     Given a randomized \"grid-graph\", generate a maze by creating a\n"
        + " minimum spanning tree\n"
        + "\n"
        + "USAGE\n"
        + "     " + exec::execname() + " [-" + OPTS + "]\n"
        + "\n"
        + "OPTIONS\n"
        + "     -i infile           input file with serialized grid data [default: stdin]\n"
        + "     -o outfile          maze output in serialized form [default: stdout]\n"
        + "     -p img.ppm          generate ppm image of maze\n"
        + "     -h                  Print out helpful information\n"
        ;
}
