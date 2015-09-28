#include <iostream>
#include "mst_tsp.h"
#include <getopt.h>
using namespace std;

int main(int argc, char* argv[]){
    string mode;
    bool mst_mode = false;
    bool opttsp_mode = false;
    bool fasttsp_mode = false;
    
    struct option longOpts[] = {
        { "help", no_argument, NULL, 'h' },
        { "mode", required_argument, NULL, 'm' },
    };
    opterr = false;
    
    int opt = 0, index = 0;
    while ((opt = getopt_long(argc, argv, "m:h", longOpts, &index)) != -1) {
        switch (opt) {
            case 'm':
                mode.assign(optarg);
                if (mode == "MST"){
                    mst_mode = true;
                }
                else if (mode == "OPTTSP"){
                    opttsp_mode = true;
                }
                else if (mode == "FASTTSP"){
                    fasttsp_mode = true;
                }
                break;
            case 'h':
                cout << "I guess I can help :)";
                exit(0);
            default:
                cerr << "Use a mode :)";
                exit(1);
        }
    }
    
    bool land_ho = false;
    bool sea_ho = false;
    bool coast_ho = false;
    vector<Node> grid;
    int num_Poke, a, b;
    cin >> num_Poke;
    for (int i = 0; i < num_Poke; ++i){
        Node m;
        cin >> a >> b;
        if (land_pokemon(a, b) && !land_ho){
            land_ho = true;
        }
        else if (coast_pokemon(a, b) && !coast_ho){
            coast_ho = true;
        }
        else if (sea_pokemon(a, b) && !sea_ho){
            sea_ho = true;
        }
        m.x = a;
        m.y = b;
        m.ID = i;
        m.parent = NULL;
        m.edge = INFINITY;
        if (i == 0){
            m.visited = true;
        }
        else{
            m.parent = &grid[0];
            m.visited = false;
        }
        grid.push_back(m);
    }
    if (land_ho && sea_ho && !coast_ho){
        cerr << "Cannot construct MST";
        exit(1);
    }
    
    double weight = 0;
    
    if (mst_mode){
        weight = makeMST(grid);
        cout << fixed << setprecision(2) << weight << '\n';
        for (unsigned int i = 1; i < grid.size(); ++i){
            if (grid[i].parent->ID < grid[i].ID){
                cout << grid[i].parent->ID << ' ' << grid[i].ID << '\n';
            }
            else{
                cout << grid[i].ID << ' ' << grid[i].parent->ID << '\n';
            }
        }
    }
    
    else if (opttsp_mode){
        double currentBest = 0;
        list<Node> path;
        vector<Node> path2;
        list<Node> unvisited;
        deque<Node> unvisited2;
        vector<Node> best;
        for (unsigned int i = 1; i < grid.size(); ++i){
            unvisited.push_back(grid[i]);
            unvisited2.push_back(grid[i]);
        }
        path.push_back(grid[0]);
        path2.push_back(grid[0]);
        long long smallest = 0;
        list<Node>::iterator index;
        int index2 = 0;
        for (auto it = unvisited.begin(); it != unvisited.end(); ++it){
            if (distance2(*path.begin(), *it) >= smallest){
                smallest = distance2(*path.begin(), *it);
                index = it;
            }
        }
        path.push_back(*index);
        unvisited.erase(index);
        smallest = INFINITY;
        for (unsigned int i = 0; i < unvisited2.size(); ++i){
            if (distance2(path2[0], unvisited2[i]) <= smallest){
                smallest = distance2(path2[0], unvisited2[i]);
                index2 = i;
            }
        }
        path2.push_back(unvisited2[index2]);
        unvisited2.erase(unvisited2.begin() + index2);
        insertion(path, unvisited);
        
        for (auto it = path.begin(); it != --path.end(); ++it){
            index = it;
            index++;
            weight += sqrt(distance2(*it, *index));
        }
        weight += sqrt(distance2(*path.begin(), *--path.end()));//upper bound
        genPerms(path2, unvisited2, weight, best);
        for (unsigned int i = 0; i < best.size()-1; ++i){
            currentBest += sqrt(distance2(best[i], best[i+1]));
        }
        currentBest += sqrt(distance2(best[0], best[best.size()-1]));
        cout << fixed << setprecision(2) << currentBest << '\n';
        for (unsigned int i = 0; i < best.size() - 1; ++i){
            cout << best[i].ID << ' ';
        }
        cout << best[best.size() - 1].ID;
    }
    
    else if (fasttsp_mode){
        vector<Node> path;
        deque<Node> unvisited;
        path.push_back(grid[0]);
        for (unsigned int i = 1; i < grid.size(); ++i){
            unvisited.push_back(grid[i]);
        }
        while (!unvisited.empty()){
            long long smallest = INFINITY;
            for (unsigned int i = 0; i < unvisited.size(); ++i){
                if (distance2(path[path.size()-1], unvisited[i]) < smallest){
                    smallest = distance2(path[path.size()-1], unvisited[i]);
                    index = i;
                }
            }
            path.push_back(unvisited[index]);
            swap(unvisited[index], unvisited[0]);
            unvisited.pop_front();
        }
        two_opt(path);
        for (unsigned int i = 0; i < path.size()-1; ++i){
            weight += sqrt(distance2(path[i], path[i+1]));
        }
        weight += sqrt(distance2(path[0], path[path.size()-1]));
        cout << fixed << setprecision(2) << weight << '\n';
        for (unsigned int i = 0; i < path.size()-1; ++i){
            cout << path[i].ID << ' ';
        }
        cout << path[path.size()-1].ID;
    }
    return 0;
}
