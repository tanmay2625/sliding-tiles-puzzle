#include "helper.hpp"

using namespace std;

int manhattanHeuristic(vector<vector<int>> &state)
{
    int cost = 0;
    for (int i = 0; i < state.size(); ++i)
        for (int j = 0; j < state[0].size(); ++j)
            if (state[i][j])
                cost += manhattanDistance(pos[state[i][j]], {i, j});
    return cost;
}

double euclideanHeuristic(vector<vector<int>> &state)
{
    double cost = 0;
    for (int i = 0; i < state.size(); ++i)
        for (int j = 0; j < state[0].size(); ++j)
            if (state[i][j])
                cost += euclideanDistance(pos[state[i][j]], {i, j});
    return cost;
}

int linearConflictHeuristic(vector<vector<int>> &state)
{
    int cost = 0, n = state.size(), m = state[0].size();

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
        {
            bool isRowConflict = 0, isColConflict = 0;
            int pj = pos[state[i][j]].second;
            for (int k = 0; k < j; ++k)
                if (state[i][k] && pj < pos[state[i][k]].second)
                {
                    isRowConflict = 1;
                    break;
                }
            int pi = pos[state[i][j]].first;
            for (int k = 0; k < i; ++k)
                if (state[k][j] && pi < pos[state[k][j]].first)
                {
                    isColConflict = 1;
                    break;
                }
            cost += 2 * (isColConflict + isRowConflict);
        }

    return cost;
}

int cornersHeuristic(vector<vector<int>> &state, vector<vector<int>> &goal)
{
    int n = state.size(), m = state[0].size();
    if (n < 4 || m < 4)
        return 0;
    pair<int, int> corners[] = {{0, 1}, {1, 0}, {0, m - 2}, {1, m - 1}, {n - 2, 0}, {n - 1, 1}, {n - 1, m - 2}, {n - 1, m - 1}};
    int cost = 0;
    for (int i = 0; i < 8; ++i)
        cost += 2 * checkCorner(state, goal, corners[i].first, corners[i].second);
    return cost;
}

int substatesHeuristic(vector<vector<int>> &state){
    int n=state.size(), m=state[0].size(), k=n*m-1;
    int posn[n*m];
    for(int i=0;i<n;++i)
        for(int j=0;j<m;++j)
            posn[state[i][j]]=i*m+j;
    int i=1, cost=0;
    int tmp=0, C=pow(k+1,(int)ceil(k/3.0));
    for(;i<=k/3+(bool)(k%3);++i){
        tmp*=(k+1);
        tmp+=posn[i];
    }
    cost+=db[tmp];tmp=0;
    for(;i<=k-k/3;++i){
        tmp*=(k+1);
        tmp+=posn[i];
    }
    tmp+=C;
    cost+=db[tmp];tmp=0;
    for(;i<=k;++i){
        tmp*=(k+1);
        tmp+=posn[i];
    }
    tmp+=2*C;
    cost+=db[tmp];
    return cost;
}