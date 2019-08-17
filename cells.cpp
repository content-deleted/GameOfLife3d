// Controls the various game rules
#include <vector>
#include <iostream>

using namespace std;

class grid {
    public:
    // 3d grid of cells
    vector<vector<vector<int> > > cells;

    // constructor gennerates a random grid based on size
    grid(int sx = 20, int sy = 20, int sz = 20) {
        cells.resize(sx);
        for(int x = 0; x < sx; ++x) {
            cells[x].resize(sy);
            for (int y = 0; y < sy; ++y) {
                cells[x][y].resize(sz);
                for (int z = 0; z < sz; ++z) {
                    cells[x][y][z] = rand() % 2;
                }
            }
        }
    }

    void nextGeneration () {
        vector<vector<vector<int>>> nextcells = cells;

        /*nextcells.resize(cells.size());
        for(int x = 0; x < cells.size(); ++x) {
            nextcells[x].resize(cells[x].size());
            for (int y = 0; y < cells[x].size(); ++y) {
                nextcells[x][y].resize(cells[x][y].size());
            }
        }*/

        for(int x = 0; x<cells.size(); ++x){
            for(int y = 0; y<cells[x].size(); ++y){
                for(int z = 0; z<cells[x][y].size(); ++z){
                    int currentstate = cells[x][y][z];
                    int livingNeighbors = -currentstate;

                    for(int j = -1; j<=1; ++j) for(int k = -1; k<=1; k++) for(int l = -1; l<=1; l++)
                        livingNeighbors+=cells[(x+j)%cells.size()][(y+k)%cells[x].size()][(z+l)%cells[x][y].size()];

                    if(currentstate) //if alive
                        nextcells[x][y][z] = (livingNeighbors < 3 || livingNeighbors > 6) ? 0 : 1;
                    else
                        nextcells[x][y][z] = (livingNeighbors > 2 && livingNeighbors < 5) ? 1 : 0;
                }
            }
        }

        cells = nextcells;
    }
};