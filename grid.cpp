#include "grid.h"

//#include <iostream>
#include <cassert>
#include <string>


// Constants ---------------------------------------------------------
Grid::weight_t const Grid::NOEDGE = -1;
Grid::weight_t const Grid::PATH = 1;
std::string const P3_NODE = "255 255 255\n";
std::string const P3_EDGE = "255 255 255\n";
std::string const P3_WALL = "0 0 0\n";
std::string const P3_PATH = "255 0 0\n";

// Definitions -------------------------------------------------------
std::ostream& operator<<(std::ostream &out, Point const &p) {
    return out << '(' << p.x << ',' << p.y << ')';
}

Grid::Node::Node() {
    weights[DOWN] = NOEDGE;
    weights[RIGHT] = NOEDGE;
}

Grid::Grid(unsigned height, unsigned width)
{
    for(weight_t i = 0; i < height; i++){
        std::vector<Node> row;
        for(weight_t j = 0; j < width; j++){
            Node n;
            row.push_back(n);
        }
        nodes_.push_back(row);
    }
}

unsigned Grid::getHeight() const {
    return nodes_.size();
}

unsigned Grid::getWidth() const {
    return nodes_.front().size();
}

Grid::weight_t Grid::getEdge(Point const &a, Point const &b) const {
    weight_t edge = 0;
    if(a == b){
        //std::cout << "a = b" << std::endl;
        return 0;
    }
    if(a.x == NOEDGE || a.y == NOEDGE || b.x == NOEDGE || b.y == NOEDGE){
        return NOEDGE;
    }
    if(a < b){
        //std::cout << "a < b" << std::endl;
        if(a.x != b.x && a.y != b.y){
            return NOEDGE;
        }
        if(b.x - a.x == 1){
            edge = nodes_.at(a.y).at(a.x).weights[Node::RIGHT];
        }
        if(b.y - a.y == 1){
            edge = nodes_.at(a.y).at(a.x).weights[Node::DOWN];
        }
    }
    if(a > b){
        //std::cout << "a > b" << std::endl;
        if(a.x != b.x && a.y != b.y){
            return NOEDGE;
        }
        if(a.x - b.x == 1){
            edge = nodes_.at(b.y).at(b.x).weights[Node::RIGHT];
        }
        if(a.y - b.y == 1){
            edge = nodes_.at(b.y).at(b.x).weights[Node::DOWN];
        }
    }

    return edge;
}

bool Grid::setEdge(Point const &a, Point const &b, weight_t weight) {
    if(a == b){
        //std::cout << "invalid edge" << std::endl;
        return 0;
    }
    if(a < b){
        if(a.x != b.x && a.y != b.y){
            //std::cout << "invalid edge" << std::endl;
            return 0;
        }
        if(b.x - a.x == 1){
            nodes_.at(a.y).at(a.x).weights[Node::RIGHT] = weight;
        }
        if(b.y - a.y == 1){
            nodes_.at(a.y).at(a.x).weights[Node::DOWN] = weight;
        }
    }
    if(a > b){
        if(a.x != b.x && a.y != b.y){
            //std::cout << "invalid edge" << std::endl;
            return 0;
        }
        if(a.x - b.x == 1){
            nodes_.at(b.y).at(b.x).weights[Node::RIGHT] = weight;
        }
        if(a.y - b.y == 1){
            nodes_.at(b.y).at(b.x).weights[Node::DOWN] = weight;
        }
    }
    return 1;
}

void Grid::draw(std::ostream &out) const {
    unsigned width = getWidth()*2+1;
    unsigned height = getHeight()*2+1;
    
    out << "P3\n" << width << " " << height << "\n" << "255\n";
    for(unsigned i = 0; i < width; i++){
        out << "0 0 0\n";
    }//draw first line boundary

    for(auto i = nodes_.begin(); i != nodes_.end(); i++ ){
        out << "0 0 0\n";
        for(auto j = (*i).begin(); j != (*i).end(); j++){
            if((*j).weights[Node::RIGHT] == PATH || (*j).weights[Node::DOWN] == PATH)
                out << "255 0 0\n";
            else
                out << "255 255 255\n";
            if((*j).weights[Node::RIGHT] == NOEDGE){
                out << "0 0 0\n";
            }
            else if((*j).weights[Node::RIGHT] == PATH){
                out << "255 0 0\n";
            }
            else{
                out << "255 255 255\n";
            }
        }
        out << "0 0 0\n";
        for(auto j = (*i).begin(); j != (*i).end(); j++){
            if((*j).weights[Node::DOWN] == NOEDGE){
                out << "0 0 0\n";
            }
            else if((*j).weights[Node::DOWN] == PATH){
                out << "255 0 0\n";
            }
            else{
                out << "255 255 255\n";
            }
            out << "0 0 0\n";
        }
    }
}

void Grid::serialize(std::ostream &out) const {
    out << nodes_.front().size() << " " << nodes_.size() << "\n";
    unsigned x, y;
    for(auto i = nodes_.begin(); i != nodes_.end(); i++){
        for(auto j = (*i).begin(); j != (*i).end(); j++){
            y = std::distance(nodes_.begin(), i);
            x = std::distance((*i).begin(), j);
            if((*j).weights[Node::RIGHT] != NOEDGE){
                out << "(" << x << "," << y << ")" << (*j).weights[Node::RIGHT] << "(" << x+1 << "," << y << ")\n";
            }
            if((*j).weights[Node::DOWN] != NOEDGE){
                out << "(" << x << "," << y << ")" << (*j).weights[Node::DOWN] << "(" << x << "," << y+1 << ")\n";
            }
        }
    }
}

Grid Grid::load(std::istream &in) { 
    if(in.eof()){
        Grid op(0,0);
        return op;
    }

    weight_t a,b,x,y,edge = 0;
    std::string str;
    std::getline(in,str);
    sscanf(str.c_str(), "%u %u\n", &a,&b);
    Grid newg(b, a);


    while(!in.eof()){
        getline(in,str);
        sscanf(str.c_str(), "(%u,%u)%u(%u,%u)\n", &a, &b, &edge, &x, &y);

        //Point p1(std::stoul(str.substr(str.find("(")+1,str.find(",") - str.find("(")- 1)), std::stoul(str.substr(str.find(",") + 1,str.find(")") - str.find(",") - 1)));
        //unsigned edge = std::stoul(str.substr(str.find(")") + 1, str.rfind("(") - str.find(")") -1));
        //Point p2(std::stoul(str.substr(str.rfind("(") + 1,str.rfind(",") - str.rfind("(") - 1)), std::stoul(str.substr(str.rfind(",") + 1,str.rfind(")") - str.rfind(",") -1)));
        newg.setEdge({a,b},{x,y},edge);
    }

    
    return newg;

}