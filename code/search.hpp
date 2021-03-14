#include "heuristics.hpp"

// A* search with Manhattan heuristic

string aStarSearch(searchAgent &problem)
{
    preprocess(problem);
    set<pair<int, vector<vector<int>>>> fringe;
    map<vector<vector<int>>, int> cost, g;
    map<vector<vector<int>>, int> move;
    cost[problem.startState] = manhattanHeuristic(problem.startState);
    g[problem.startState] = 0;
    fringe.insert({cost[problem.startState], problem.startState});
    int n = problem.startState.size(), m = problem.startState[0].size();
    string DIR = "URLD";
    int dx[] = {-1, 0, 0, 1}, dy[] = {0, 1, -1, 0};
    while (!fringe.empty())
    {
        auto cur = *fringe.begin();
        int blankX = -1, blankY = -1;
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
                if (!cur.second[i][j])
                {
                    blankX = i;
                    blankY = j;
                    break;
                }
            if (blankY != -1)
                break;
        }

        if (problem.isGoalState(cur.second))
        {
            string ans;
            vector<vector<int>> tmp = cur.second;
            while (tmp != problem.startState)
            {
                int dir = move[tmp];
                ans.push_back(DIR[dir]);
                swap(tmp[blankX][blankY], tmp[blankX + dx[3 - dir]][blankY + dy[3 - dir]]);
                blankX += dx[3 - dir];
                blankY += dy[3 - dir];
            }
            reverse(ans.begin(), ans.end());
            return ans;
        }
        fringe.erase(fringe.begin());
        vector<pair<vector<vector<int>>, int>> successors = problem.getSuccessors(cur.second, blankX, blankY);
        int curg = g[cur.second];
        for (auto successor : successors)
        {
            int h = manhattanHeuristic(successor.first);
            if (cost.find(successor.first) == cost.end() || cost[successor.first] > curg + 1 + h)
            {
                fringe.erase({cost[successor.first], successor.first});
                cost[successor.first] = curg + 1 + h;
                g[successor.first] = curg + 1;
                move[successor.first] = successor.second;
                fringe.insert({cost[successor.first], successor.first});
            }
        }
        //cout<<fringe.size()<<endl;
    }
}

string breadthFirstSearch(searchAgent &problem)
{
    map<vector<vector<int>>, int> par;
    int n = problem.startState.size(), m = problem.startState[0].size();
    par[problem.startState] = -1;
    queue<vector<vector<int>>> que;
    que.push(problem.startState);
    int dx[] = {-1, 0, 0, 1}, dy[] = {0, 1, -1, 0};
    while (!que.empty())
    { //cerr<<"H"<<endl;
        vector<vector<int>> cur = que.front();
        que.pop();
        int blankX, blankY;
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < m; ++j)
                if (!cur[i][j])
                {
                    blankX = i;
                    blankY = j;
                    break;
                }
        if (cur == problem.goalState)
        {
            cerr << "H" << endl;
            string path, DIR = "URLD";
            vector<vector<int>> tmp = cur;
            while (tmp != problem.startState)
            {
                int dir = par[tmp];
                path.push_back(DIR[dir]);
                swap(tmp[blankX][blankY], tmp[blankX + dx[3 - dir]][blankY + dy[3 - dir]]);
                blankX += dx[3 - dir];
                blankY += dy[3 - dir];
            }
            reverse(path.begin(), path.end());
            return path;
        }

        for (int k = 0; k < 4; ++k)
        {
            int X = blankX + dx[k], Y = blankY + dy[k];
            if (X < 0 || X >= n || Y < 0 || Y >= m)
                continue;
            swap(cur[blankX][blankY], cur[X][Y]);
            if (par.find(cur) == par.end())
            {
                par[cur] = k;
                que.push(cur);
            }
            swap(cur[blankX][blankY], cur[X][Y]);
        }
    }
}

// IDA* search with Manhattan + Linear conflict + corner heuristic
int cnt = 0;

int dfs(string &path, vector<vector<int>> &cur, int blankX, int blankY, int g, int bound, searchAgent &problem, bool sw)
{
    cnt++;
    //cerr<<blankX<<" "<<blankY<<endl;
    int h = (sw ? (manhattanHeuristic(cur) + linearConflictHeuristic(cur) + cornersHeuristic(cur, problem.goalState)) : substatesHeuristic(cur));
    int f = g + h;
    if (f > bound)
        return f;
    if (!h)
        return -1;
    /* for(auto y:cur){
        for(auto z:y)cout<<z<<" ";cout<<endl;
    }
    cout<<h<<endl; */
    int res = INT_MAX;
    int dx[] = {-1, 0, 0, 1}, dy[] = {0, 1, -1, 0};
    string DIR = "URLD";
    for (int k = 0; k < 4; ++k)
    {
        if (DIR[3 - k] == path.back())
            continue;
        int X = blankX + dx[k], Y = blankY + dy[k];
        if (X >= cur.size() || X < 0 || Y < 0 || Y >= cur[0].size())
            continue;
        path.push_back(DIR[k]);
        /* int dh=-(manhattanDistance({X,Y},pos[cur[X][Y]])+2*(blankY==Y?calculateRowConflicts(cur,X):calculateColConflicts(cur,Y))+cornerCost(cur,problem.goalState,X,Y)); */
        //cout<<dh<<" ";
        swap(cur[blankX][blankY], cur[X][Y]);
        /* dh+=(manhattanDistance({blankX,blankY},pos[cur[blankX][blankY]])+2*(blankY==Y?calculateRowConflicts(cur,blankX):calculateColConflicts(cur,blankY))+cornerCost(cur,problem.goalState,blankX,blankY)); */
        //cout<<dh<<endl;
        int t = dfs(path, cur, X, Y, g + 1, bound, problem, sw);
        if (t == -1)
            return -1;
        res = min(t, res);
        swap(cur[blankX][blankY], cur[X][Y]);
        path.pop_back();
    }
    return res;
}

string idaStarSearch(searchAgent &problem, bool sw = 0)
{
    preprocess(problem); //cerr<<"H"<<endl;
    if (!sw)
        setup(problem.goalState); //cerr<<"H"<<endl;
    int h0 = (sw ? (manhattanHeuristic(problem.startState) + linearConflictHeuristic(problem.startState) + cornersHeuristic(problem.startState, problem.goalState)) : substatesHeuristic(problem.startState));
    int blankX, blankY;
    int bound = h0;
    for (int i = 0; i < problem.startState.size(); ++i)
        for (int j = 0; j < problem.startState[0].size(); ++j)
            if (!problem.startState[i][j])
            {
                blankX = i;
                blankY = j;
            }
    while (1)
    {
        vector<vector<int>> cur = problem.startState;
        string path = "";
        int t = dfs(path, cur, blankX, blankY, 0, bound, problem, sw);
        if (t == -1)
        {
            cout << "Number of nodes travelled : " << cnt << endl;
            return path;
        }
        if (t == INT_MAX)
            return "";
        bound = t;
    }
}

// Greedy search with Manhattan + linear conflict + corner heuristic

string greedySearch(searchAgent &problem)
{
    preprocess(problem);
    set<pair<int, vector<vector<int>>>> fringe;
    map<vector<vector<int>>, int> cost;
    map<vector<vector<int>>, int> move;
    cost[problem.startState] = manhattanHeuristic(problem.startState) + linearConflictHeuristic(problem.startState) + cornersHeuristic(problem.startState, problem.goalState);
    fringe.insert({cost[problem.startState], problem.startState});
    int n = problem.startState.size(), m = problem.startState[0].size();
    string DIR = "URLD";
    int dx[] = {-1, 0, 0, 1}, dy[] = {0, 1, -1, 0};
    while (!fringe.empty())
    {
        auto cur = *fringe.begin();
        int blankX = -1, blankY = -1;
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
                if (!cur.second[i][j])
                {
                    blankX = i;
                    blankY = j;
                    break;
                }
            if (blankY != -1)
                break;
        }
        //for(int i=0;i<3;++i)for(int j=0;j<3;++j)cout<<cur.second[i][j]<<" ";
        //int curh=manhattanHeuristic(cur.second,pos)+linearConflictHeuristic(cur.second,help);
        if (problem.isGoalState(cur.second))
        {
            string ans;
            vector<vector<int>> tmp = cur.second;
            while (tmp != problem.startState)
            {
                int dir = move[tmp];
                ans.push_back(DIR[dir]);
                swap(tmp[blankX][blankY], tmp[blankX + dx[3 - dir]][blankY + dy[3 - dir]]);
                blankX += dx[3 - dir];
                blankY += dy[3 - dir];
            }
            reverse(ans.begin(), ans.end());
            return ans;
        }
        fringe.erase(fringe.begin());
        vector<pair<vector<vector<int>>, int>> successors = problem.getSuccessors(cur.second, blankX, blankY);
        for (auto successor : successors)
        {
            int h = manhattanHeuristic(successor.first) + linearConflictHeuristic(successor.first);
            if (cost.find(successor.first) == cost.end() || cost[successor.first] > h)
            {
                fringe.erase({cost[successor.first], successor.first});
                cost[successor.first] = h;
                move[successor.first] = successor.second;
                fringe.insert({cost[successor.first], successor.first});
            }
        }
        //cout<<fringe.size()<<endl;
    }
}

// Local search technique called simulated annealing

string simulatedAnnealing(searchAgent &problem)
{
    preprocess(problem);
    // it maps number to its co-ordinates in the goal state

    string ans;
    vector<vector<int>> cur = problem.startState;
    double h = euclideanHeuristic(cur);
    int blankX, blankY;
    for (int i = 0; i < cur.size(); ++i)
        for (int j = 0; j < cur[0].size(); ++j)
            if (!cur[i][j])
            {
                blankX = i;
                blankY = j;
            }
    string DIR = "URLD";
    int dx[] = {-1, 0, 0, 1}, dy[] = {0, 1, -1, 0};
    for (long double X = -2.0; X < 2.0; X += 0.000001)
    {
        if (problem.isGoalState(cur))
        {
            cout << X << " " << ans.size() << "\n";
            break;
        }
        vector<int> valid;
        for (int t = 0; t < 4; ++t)
        {
            int x = blankX + dx[t], y = blankY + dy[t];
            if (x < 0 || y < 0 || x >= cur.size() || y >= cur[0].size())
                continue;
            valid.push_back(t);
        }
        int k = valid[rand() % valid.size()];
        pair<int, int> tmp = pos[cur[blankX + dx[k]][blankY + dy[k]]];
        double dh = euclideanDistance(tmp, {blankX, blankY}) - euclideanDistance(tmp, {blankX + dx[k], blankY + dy[k]});
        if (dh < 0)
        {
            swap(cur[blankX][blankY], cur[blankX + dx[k]][blankY + dy[k]]);
            blankX += dx[k];
            blankY += dy[k];
            h += dh;
            ans.push_back(DIR[k]);
        }
        else
        {
            long double T = getRandomPositiveRealLessThanOne() * (2 - X);
            long double P = exp(-dh / T);
            if (getRandomPositiveRealLessThanOne() < P)
            {
                swap(cur[blankX][blankY], cur[blankX + dx[k]][blankY + dy[k]]);
                blankX += dx[k];
                blankY += dy[k];
                h += dh;
                ans.push_back(DIR[k]);
            }
        }
    }

    return ans;
}