#include <algorithm>
#include "mst_tsp.h"
using namespace std;

double lowerBound(vector<Node> path, deque<Node> unvisited){
    double weight = makeMST(unvisited);
    double shortest = INFINITY;
    double shortest2 = INFINITY;
    for (unsigned int i = 0; i < path.size()-1; ++i){
        weight += sqrt(distance2(path[i], path[i+1]));
    }
    for (unsigned int i = 0; i < unvisited.size(); ++i){
        if (distance2(path[0], unvisited[i]) < shortest){
            shortest = distance2(path[0], unvisited[i]);
        }
        if (distance2(path[path.size() - 1], unvisited[i]) < shortest2){
            shortest2 = distance2(path[path.size()-1], unvisited[i]);
        }
    }
    weight += sqrt(shortest) + sqrt(shortest2);
    return weight;
}

void genPerms(vector<Node>& path, deque<Node>& unvisited, double& weight, vector<Node>& currentBest){
    if(!unvisited.empty()){
        double lower_bound = lowerBound(path, unvisited);
        if (lower_bound > weight){
            return;
        }
    }
    else{
        double best_weight = 0;
        double other_weight = 0;
        for (unsigned int i = 0; i < path.size() - 1; ++i){
            other_weight += sqrt(distance2(path[i], path[i+1]));
        }
        other_weight += sqrt(distance2(path[path.size() - 1], path[0]));
        if (!currentBest.empty()){
            for (unsigned int i = 0; i < currentBest.size()-1; ++i){
                best_weight += sqrt(distance2(currentBest[i], currentBest[i+1]));
            }
            best_weight += sqrt(distance2(currentBest[0], currentBest[currentBest.size()-1]));
        }
        else{
            best_weight = INFINITY;
        }
        if (other_weight < best_weight){
            currentBest = path;
        }
        return;
    }
    for (unsigned int i = 0; i < unvisited.size(); ++i){
        path.push_back(unvisited.front());
        unvisited.pop_front();
        genPerms(path, unvisited, weight, currentBest);
        unvisited.push_back(path.back());
        path.pop_back();
    }
}

double makeMST(vector<Node>& grid){
    Node* n;
    n = &grid[0];
    unsigned int l = 1;
    int index = 0;
    double weight = 0;
    long long smallest = INFINITY;
    while (l != grid.size()){
        for (unsigned int i = 1; i < grid.size(); ++i){
            
            //updating the distances
            if (grid[i].ID != n->ID){
                if (!(land_pokemon(grid[i].x, grid[i].y) && sea_pokemon(n->x, n->y))){
                    if (!(sea_pokemon(grid[i].x, grid[i].y) && land_pokemon(n->x, n->y))){
                        if (!grid[i].visited){
                            if (distance2(*n, grid[i]) < grid[i].edge){
                                grid[i].edge = distance2(*n, grid[i]);
                                grid[i].parent = n;
                            }
                        }
                    }
                }
            }
        }
        
        for (unsigned int i = 1; i < grid.size(); ++i){
            if (!grid[i].visited){
                if (grid[i].edge < smallest){
                    smallest = static_cast<long long>(grid[i].edge);
                    index = i;
                }
            }
        }
        
        
        //checking the content of the grid
        grid[index].visited = true;
        weight += sqrt(grid[index].edge);
        smallest = INFINITY;
        n = &grid[index];
        l++;
        if (l == grid.size()){
            break;
        }
    }
    return weight;
}

double makeMST(deque<Node>& grid){
    Node* n;
    n = &grid[0];
    unsigned int l = 1;
    int index = 0;
    double weight = 0;
    long long smallest = INFINITY;
    while (l != grid.size()){
        for (unsigned int i = 1; i < grid.size(); ++i){
            
            //updating the distances
            if (grid[i].ID != n->ID){
                if (!grid[i].visited){
                    if (distance2(*n, grid[i]) < grid[i].edge){
                        grid[i].edge = distance2(*n, grid[i]);
                        grid[i].parent = n;
                    }
                }
            }
        }
        
        for (unsigned int i = 1; i < grid.size(); ++i){
            if (!grid[i].visited){
                if (grid[i].edge < smallest){
                    smallest = static_cast<long long>(grid[i].edge);
                    index = i;
                }
            }
        }
        
        
        //checking the content of the grid
        grid[index].visited = true;
        weight += sqrt(grid[index].edge);
        smallest = INFINITY;
        n = &grid[index];
        l++;
        if (l == grid.size()){
            break;
        }
    }
    return weight;
}

void insertion(list<Node>& path, list<Node>& unvisited){
    list<Node>::iterator vertex1;
    if (unvisited.empty()){
        return;
    }
    long long smallest = 0;
    list<Node>::iterator index;
    Node smaller = *path.begin();
    for (auto it = unvisited.begin(); it != unvisited.end(); ++it){
        for (auto ab = path.begin(); ab != path.end(); ++ab){
            if (smaller.ID != ab->ID){
                if (distance2(smaller, *it) >= distance2(*ab, *it)){
                    smaller = *ab;
                }
            }
        }
        if (distance2(smaller, *it) >= smallest){
            smallest = distance2(smaller, *it);
            index = it;
        }
    }
    smallest = distance2(*path.begin(), *index) + distance2(*--path.end(), *index) - distance2(*path.begin(), *--path.end());
    vertex1 = --path.end();
    for (auto ab = path.begin(); ab != --path.end(); ++ab){
        list<Node>::iterator ab2 = ab;
        ab2++;
        auto dist = distance2(*ab, *index) + distance2(*ab2, *index) - distance2(*ab, *ab2);
        if (dist < smallest){
            smallest = dist;
            vertex1 = ab;
        }
    }
    vertex1++;
    path.insert(vertex1, *index);
    unvisited.erase(index);
    insertion(path, unvisited);
}

void two_opt(vector<Node>& path){
    for (unsigned int i = 2; i < path.size(); ++i){
        for (unsigned int l = 1; l < path.size()-1; ++l){
            if (i != l && i!= l+1 && i != l+2){
                cout << path[i].ID << ' ' << path[l].ID << endl;
                cout << sqrt(distance2(path[i-1], path[l])) << ' ' << sqrt(distance2(path[i], path[l+1])) << ' ' << sqrt(distance2(path[i-1], path[i])) << ' ' << sqrt(distance2(path[l], path[l+1])) << endl;
                if (sqrt(distance2(path[i-1], path[l])) + sqrt(distance2(path[i], path[l+1])) < sqrt(distance2(path[i-1], path[i])) + sqrt(distance2(path[l], path[l+1]))){
                    cout << "Reversing " << path[i].ID << " and " << path[l].ID << endl;
                    reverse(&path[i], &path[l+1]);
                }
            }
            for (int i = 0; i < path.size(); ++i){
                cout << path[i].ID << ' ';
            }
            cout << endl;
        }
    }
    swap(path[path.size()-2], path[path.size()-1]);
}

bool land_pokemon(const int x, const int y){
    if (x > 0){
        return true;
    }
    else if (x <= 0 && y > 0){
        return true;
    }
    return false;
}

bool sea_pokemon(const int x, const int y){
    return (x < 0 && y < 0);
}

bool coast_pokemon(const int x, const int y){
    if (x == 0 && y <= 0){
        return true;
    }
    else if (x < 0 && y == 0){
        return true;
    }
    return false;
}

long long distance2(const Node a, const Node b) {
    long long x_diff = b.x - a.x;
    long long y_diff = b.y - a.y;
    return (x_diff * x_diff)  + (y_diff * y_diff);
}


