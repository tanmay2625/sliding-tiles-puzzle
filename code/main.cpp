#include "testGenerator.hpp"

using namespace std;
using namespace std::chrono;

int main()
{
    cout << "Welcome to NxM Puzzle Solver!\n";
    cout << "Enter the dimensions in the form N space M such that N <= 6 and M <= 6 \n";

    int n, m;
    cin >> n >> m;
    ofstream out;
    out.open("input.txt");
    out << n << " " << m << endl;
    cout << "Do you want to : \nA. Input your own test-case?\nB. Use randomly generated test-case?\n";
    char ch;
    cin >> ch;
    vector<vector<int>> startState(n, vector<int>(m)), goalState(n, vector<int>(m));
    if (ch == 'A' || ch=='a')
    {
        set<int> st;
        for(int i=0;i<n*m;++i)st.insert(i);
        cout << "Enter start state with N*M entries\n";
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < m; ++j){
                cin >> startState[i][j];
                st.erase(startState[i][j]);
            }
        if(!st.empty()){
            cout<<"Invalid start state!"<<endl;
            for (int i = 0; i < n; ++i)
                {
                    for (int j = 0; j < m; ++j)
                        out << startState[i][j] << " ";
                    out << endl;
                }
            out<<" "<<endl;
            return 0;
        }
        for(int i=0;i<n*m;++i)st.insert(i);
        cout << "Enter goal state with N*M entries\n";
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < m; ++j){
                cin >> goalState[i][j];
                st.erase(goalState[i][j]);
            }
        if(!st.empty()){
            cout<<"Invalid goal state!"<<endl;
            for (int i = 0; i < n; ++i)
                {
                    for (int j = 0; j < m; ++j)
                        out << startState[i][j] << " ";
                    out << endl;
                }
            out<<" "<<endl;
            return 0;
        }
    }
    else
    {
        pair<vector<vector<int>>, vector<vector<int>>> p = generateTest(n, m);
        startState = p.first;
        goalState = p.second;
        cout << "Start State :\n";
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
                cout << startState[i][j] << " ";
            cout << endl;
        }
        cout << "Goal State :\n";
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
                cout << goalState[i][j] << " ";
            cout << endl;
        }
    }
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
            out << startState[i][j] << " ";
        out << endl;
    }
    searchAgent problem(startState, goalState);
    if (!problem.isPuzzleSolvable())
    {
        cout << "Oops!!The given puzzle is unsolvable!\n";
        out << "" << endl;
        return 0;
    }
    string path;
    if(n*m<=9){
        cout<<"Choose the algorithm :\nA. BFS\nB. A* with Manhattan"<<endl;
        char cc;cin>>cc;
        auto start = high_resolution_clock::now();
        if(cc=='A'||cc=='a')path=breadthFirstSearch(problem);
        else path=aStarSearch(problem);
         auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "Running Time: "
            << duration.count() / 1e6 << " seconds" << endl;
    }
    else if (n * m <= 16)
    {
        cout<<"Choose the algorithm :\nA. Slower but optimal : IDA* with pattern database heuristic\nB. Faster but suboptimal : IDA* with Manhattan+novel+corner-tile heuristic"<<endl;
        char cc;cin>>cc;
        auto start = high_resolution_clock::now();
        if(cc=='A'||cc=='a'){
            cout << "Running...\nIt might take 5 minutes in the worst case."<<endl;
            path = idaStarSearch(problem);
        }
        else path=idaStarSearch(problem,1);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "Running Time: "
            << duration.count() / 1e6 << " seconds" << endl;
    }
    else {
        auto start = high_resolution_clock::now();
        path = greedySearch(problem);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "Running Time: "
            << duration.count() / 1e6 << " seconds" << endl; 
    }

    /* TO CHECK OUT SIMULATED ANNEALING (IT'S SLOW AND NON-OPTIMAL, IT WAS JUST AN EXPERIMENT!!), 
    UNCOMMENT BELOW LINE AND COMMENT ABOVE PARAGRAPH */
    //string path = simulatedAnnealing(problem);

    cout << "Solution Cost :" << path.size() << "\n";
    cout << path << "\n";
    cout << "The verdict of solution verifier is :" << (problem.isSolutionCorrect(path) ? "Accepted" : "Rejected") << "\n";

    out << path << endl;
    out.close();

    return 0;
}
