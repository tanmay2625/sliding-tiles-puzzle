#include <bits/stdc++.h>

using namespace std;

class searchAgent
{
public:
    vector<vector<int>> startState, goalState;

    searchAgent(vector<vector<int>> startState, vector<vector<int>> goalState)
    {
        this->startState = startState;
        this->goalState = goalState;
    }

    bool isGoalState(vector<vector<int>> state)
    {
        return state == this->goalState;
    }

    vector<pair<vector<vector<int>>, int>> getSuccessors(vector<vector<int>> state, int blankSpaceRow, int blankSpaceCol)
    {
        int dx[] = {-1, 0, 0, 1}, dy[] = {0, 1, -1, 0};
        vector<pair<vector<vector<int>>, int>> successors;
        vector<vector<int>> tmpSuccessor = state;
        for (int k = 0; k < 4; ++k)
        {
            int x = blankSpaceRow + dx[k], y = blankSpaceCol + dy[k];
            if (x < 0 || y < 0 || x >= state.size() || y >= state[0].size())
                continue;
            swap(tmpSuccessor[x][y], tmpSuccessor[blankSpaceRow][blankSpaceCol]);
            successors.push_back({tmpSuccessor, k});
            swap(tmpSuccessor[x][y], tmpSuccessor[blankSpaceRow][blankSpaceCol]);
        }
        return successors;
    }

    bool isSolutionCorrect(string path)
    {
        int blankX = -1, blankY = -1, n = this->startState.size(), m = this->startState[0].size();
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
                if (!this->startState[i][j])
                {
                    blankX = i;
                    blankY = j;
                    break;
                }
            if (blankX != -1)
                break;
        }
        vector<vector<int>> cur = this->startState;
        int dx[] = {-1, 0, 0, 1}, dy[] = {0, 1, -1, 0};
        map<char, int> dir;
        dir['U'] = 0;
        dir['R'] = 1;
        dir['L'] = 2;
        dir['D'] = 3;
        for (auto y : path)
        {
            int X = blankX + dx[dir[y]], Y = blankY + dy[dir[y]];
            if (X < 0 || Y < 0 || X >= n || Y >= m)
                return false;
            swap(cur[blankX][blankY], cur[X][Y]);
            blankX = X;
            blankY = Y;
        }
        return this->isGoalState(cur);
    }

    bool isPuzzleSolvable()
    {
        int n = this->startState.size(), m = this->startState[0].size();
        int posn[n * m];
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < m; ++j)
                posn[this->goalState[i][j]] = i * m + j;
        int inversions = 0, blankX;
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < m; ++j)
            {
                if (!this->startState[i][j])
                {
                    blankX = i;
                    continue;
                }
                for (int k = 0; k <= i; ++k)
                    for (int l = 0; l < m && (k < i || l < j); ++l)
                        inversions += (this->startState[k][l] && posn[this->startState[i][j]] < posn[this->startState[k][l]]);
            }
        if (m & 1)
            return (inversions & 1) == 0;
        return ((inversions & 1) ^ ((abs(blankX - posn[0] / m)) & 1)) == 0;
    }
};
