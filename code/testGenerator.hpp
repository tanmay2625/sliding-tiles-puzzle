#include "search.hpp"

using namespace std;

pair<vector<vector<int>>, vector<vector<int>>> generateTest(int n, int m)
{

    // fast input
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);

    pair<vector<vector<int>>, vector<vector<int>>> test;
    vector<vector<int>> state(n, vector<int>(m));
    vector<int> perm;
    for (int i = 0; i < n * m; ++i)
        perm.push_back(i);
    int blankX, blankY;
    for (int i = 0; i < n * m; ++i)
    {
        int randomIndex = randint(0, perm.size() - 1);
        state[i / m][i % m] = perm[randomIndex];
        if (!perm[randomIndex])
        {
            blankX = i / m;
            blankY = i % m;
        }
        swap(perm.back(), perm[randomIndex]);
        perm.pop_back();
    }

    test.first = state;

    int dx[] = {-1, 0, 0, 1}, dy[] = {0, 1, -1, 0};
    for (int i = 0; i < 1e5; ++i)
    {
        int dir = randint(0, 3);
        int x = blankX + dx[dir], y = blankY + dy[dir];
        if (x < 0 || y < 0 || x >= n || y >= m)
            continue;
        swap(state[blankX][blankY], state[x][y]);
        blankX += dx[dir];
        blankY += dy[dir];
    }

    test.second = state;
    return test;
}