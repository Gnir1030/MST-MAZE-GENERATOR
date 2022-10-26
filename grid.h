#pragma once

#include <array>
#include <istream>
#include <ostream>
#include <vector>

// Point class -------------------------------------------------------
struct Point {
    unsigned x {};      // increasing x moves right
    unsigned y {};      // increasing y moves down

    std::strong_ordering operator<=>(Point const&) const = default;
};
std::ostream& operator<<(std::ostream&, Point const &);

// Grid class --------------------------------------------------------
class Grid {
private: /* Internal Classes */
    struct Node {
        using weight_t = unsigned;
        enum Direction {DOWN, RIGHT, NUM_DIRECTIONS};
        std::array<weight_t, NUM_DIRECTIONS> weights;
        Node();
    };

public:
    // Types
    using weight_t = Node::weight_t;

    // Constants
    static weight_t const NOEDGE;
    static weight_t const PATH;

    // Constructor
    Grid(unsigned height, unsigned width);

    // Accessors
    unsigned getHeight() const;
    unsigned getWidth() const;
    weight_t getEdge(Point const &a, Point const &b) const;

    // Mutators
    bool setEdge(Point const &a, Point const &b, Node::weight_t weight);

    // Output
    void draw(std::ostream&) const ;        /* generate PPM P3 image of Grid */
    void serialize(std::ostream&) const ;   /* output dimensions and edge weights */

    // Static
    static Grid load(std::istream &in);     /* the opposite of serialize */

private: /* Members */
    std::vector<std::vector<Node>> nodes_;
};
