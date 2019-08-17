// Controls the various game rules
#include <vector>
#include <iostream>

using namespace std;

class grid {
    public:
    // 3d grid of cells
    vector<vector<vector<int> > > cells;

    // dimensions
    grid(int sx = 5, int sy = 5, int sz = 5) {
        cells.resize(sx);
        for(int x = 0; x < sx; x++) {
            cells[x].resize(sy);
            for (int y = 0; y < sy; y++) {
                cells[x][y].resize(sz);
            }
        }

        cout<< cells.size()<< endl;
        cout << cells[0].size()<<endl;
        cout << cells[0][0].size()<<endl;
    }

    void nextGeneration () {
        vector<vector<vector<int>>> nextcells = cells;

        int x,y,z=0;
        for(; x<cells.size(); x++){
            for(; y<cells[x].size(); y++){
                for(; z<cells[x][y].size(); z++){
                    int currentstate = cells[x][y][z];
                    int livingNeighbors = -currentstate;

                    int j,k,l = -1;
                    for(; j<=1; j++) for(; k<=1; k++) for(; l<=1; l++) livingNeighbors+=cells[(x+j)%cells.size()][(y+k)%cells[x].size()][(z+l)%cells[x][y].size()];

                    if(currentstate) //if alive
                        nextcells[x][y][z] = (livingNeighbors < 2 || livingNeighbors > 3) ? 0 : 1;
                    else
                        nextcells[x][y][z] = (livingNeighbors == 3) ? 1 : 0;
                }
            }
        }

        cells = nextcells;
    }
};