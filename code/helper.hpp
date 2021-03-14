#include "searchAgent.hpp"

using namespace std;

pair<int, int> pos[40]; // pos[i] stores co-ordinates of position of i in the goal state
vector<string> help;
vector<int> db(3*16*16*16*16*16+100,-1);int cc;

void preprocess(searchAgent &problem)
{
    int n = problem.goalState.size(), m = problem.goalState[0].size();
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            pos[problem.goalState[i][j]] = {i, j};
}

inline int manhattanDistance(pair<int, int> p1, pair<int, int> p2)
{
    return abs(p1.first - p2.first) + abs(p1.second - p2.second);
}

inline double euclideanDistance(pair<int, int> p1, pair<int, int> p2)
{
    return sqrt((p1.first - p2.first) * (p1.first - p2.first) + (p1.second - p2.second) * (p1.second - p2.second));
}

inline int randint(int l, int r)
{
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    return uniform_int_distribution<int>(l, r)(rng);
}

inline long double getRandomPositiveRealLessThanOne()
{
    return (long double)randint(1, 1e8) / 1e8;
}

inline int computeHash(vector<int> v, int k, int sw){
    int res=0,kk=1;
    for(auto y:v){
        res*=k;res+=y;
        kk*=k;
    }
    if((k-1)/3.0>v.size())kk*=k;
    res+=sw*kk;
    return res;
}

bool checkRowConflict(vector<vector<int>> state, vector<vector<int>> goal, int x, int y)
{
    int n = state.size(), m = state[0].size();
    bool yes = 0;
    for (int i = 0; i < y; ++i)
        for (int j = y + 1; j < m; ++j)
            if (state[x][i] == goal[x][j] || goal[x][i] == state[x][j])
            {
                yes = 1;
                break;
            }
    return yes;
}

bool checkColConflict(vector<vector<int>> &state, vector<vector<int>> &goal, int x, int y)
{
    int n = state.size(), m = state[0].size();
    bool yes = 0;
    for (int i = 0; i < x; ++i)
        for (int j = x + 1; j < n; ++j)
            if (state[i][y] == goal[j][y] || goal[i][y] == state[j][y])
            {
                yes = 1;
                break;
            }
    return yes;
}

bool checkCorner(vector<vector<int>> &state, vector<vector<int>> &goal, int x, int y)
{
    int cx = x, cy = y, n = state.size(), m = state[0].size();
    if (x == 1)
        cx--;
    else if (x == n - 2)
        cx++;
    else
        cy += (y == 1 ? -1 : 1);
    return (state[cx][cy] && goal[cx][cy] && state[x][y] && state[cx][cy] != goal[cx][cy] && state[x][y] == goal[x][y] && (cx == x ? !checkRowConflict(state, goal, x, y) : !checkColConflict(state, goal, x, y)));
}

void bfs(vector<int> startState, int n, int m, int sw){//cerr<<"S"<<endl;
    queue<int> que;int t=n*m, startStateHash=computeHash(startState,t,sw);
    int sz=startState.size(), C=pow(t,(int)ceil((t-1)/3.0));;
    //if(sw==1)cerr<<startStateHash<<endl;
    que.push(startStateHash);
    db[startStateHash]=0;
    vector<int> delta={-1,1};
    delta.push_back(-m);delta.push_back(m);
    int kk=0;
    while(!que.empty()){
        int curHash=que.front();que.pop();
        int curCost=db[curHash],tmp=curHash%C, tt=1;
        set<int> tiles;
        vector<int> tilesV;
        for(int jj=0;jj<startState.size();jj++){
            tiles.insert(tmp%t);
            tilesV.push_back(tmp%t);
            tmp/=t;
        }
        for(auto tile:tilesV){
            for(int k=0;k<4;++k){
                int newPos=tile+delta[k];
                if(newPos<0||newPos>=t||(delta[k]==1&&tile%m==m-1)||(delta[k]==-1&&tile%m==0)||tiles.find(newPos)!=tiles.end())continue;
                int newHash=curHash+delta[k]*tt;
                if(db[newHash]==-1){cc++;
                    db[newHash]=curCost+1;
                    que.push(newHash);
                }
            }
            tt*=t;
        }
    }
    //cerr<<cc<<" "<<mnm<<" "<<mxm<<endl;
    //cerr<<"E"<<endl;
}

void setup(vector<vector<int>> goal){
    int n=goal.size(), m=goal[0].size(), k=n*m-1;
    vector<int> s(3,k/3);
    if(k%3)s[0]++;
    if(k%3==2)s[1]++;
    vector<int> v[3];
    int idx=0;
    for(int i=1;i<n*m;++i){
        if(!s[idx])idx++;
        s[idx]--;
        v[idx].push_back(pos[i].first*m+pos[i].second);
    }

    for(int i=0;i<3;++i){
        bfs(v[i],n,m,i);
    }
}
