#ifndef MST_TSP_H
#define MST_TSP_H

#include <cmath>
#include <vector>
#include <queue>
#include <iostream>
#include <iomanip>
#include <list>

struct Node{
    int x, y;
    bool visited;
    long long edge;
    Node* parent;
    int ID;
};

long long distance2(const Node a, const Node b);

class Heap{
public:
    bool operator()(const Node a, const Node b) const{
        return (a.edge > b.edge);
    }
    bool operator()(const int a, const int b) const{
        return (a > b);
    }
};

double makeMST(std::vector<Node>& grid);

double makeMST(std::deque<Node>& grid);

double lowerBound(std::vector<Node> path, std::deque<Node> unvisited);

void genPerms(std::vector<Node>& path, std::deque<Node>& unvisited, double& weight, std::vector<Node>& currentBest);

void insertion(std::list<Node>& path, std::list<Node>& unvisited);

void two_opt(std::vector<Node>& path);

bool land_pokemon(const int x, const int y);

bool sea_pokemon(const int x, const int y);

bool coast_pokemon(const int x, const int y);

#endif
