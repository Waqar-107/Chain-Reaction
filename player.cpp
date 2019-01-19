/***from dust i have come, dust i will be***/

#include<bits/stdc++.h>
#include<unistd.h>

#define dbg printf("in\n")
#define nl printf("\n")
#define inf 1e9

#define sf(n) scanf("%d", &n)
#define sff(n,m) scanf("%d%d",&n,&m)
#define sfl(n) scanf("%I64d", &n)
#define sffl(n,m) scanf("%I64d%I64d",&n,&m)

#define pf(n) printf("%d",n)
#define pff(n,m) printf("%d %d",n,m)
#define pffl(n,m) printf("%I64d %I64d",n,m)
#define pfl(n) printf("%I64d",n)
#define pfs(s) printf("%s",s)

#define pb push_back
#define pp pair<char,int>
#define ppi pair<int,int>

#define SZ 8
#define D 2

using namespace std;

string str, line;
vector<string> vec;
char player, otherPlayer;
pp grid[SZ][SZ];
int splitCondition[SZ][SZ];

int readFile()
{
    vec.clear();
    ifstream infile("shared_file.txt");

    while(getline(infile, line))
        vec.pb(line);

    if(!vec.size())
        return -1;

    if(vec[0][0] == player)
    {
        int len, k;
        str = "";

        for(int i = 1; i <= 8; i++)
        {
            k = 0;
            len = vec[i].length();

            //split the string
            for(int j = 0; j < len; j++)
            {
                if(vec[i][j] == ' ' || j == len - 1)
                {
                    if(str == "No")
                        grid[i-1][k] = {'X', 0};

                    else
                        grid[i-1][k] = {str[0], str[1] - 48};

                    str= "";
                    k++;
                }

                else
                    str.pb(vec[i][j]);
            }
        }

        return 0;
    }

    return -1;
}


void printGrid()
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
            cout<<grid[i][j].first<<grid[i][j].second<<" ";
        nl;
    }
}

void printDynamicGrid(pp **arr){
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
            cout<<arr[i][j].first<<arr[i][j].second<<" ";
        nl;
    }

    nl;
}


int dx[] = {1, -1, 0, 0};
int dy[] = {0, 0, 1, -1};

ppi __successor__;
int evaluate(pp **tempGrid)
{
    //Simle heuristic fr counting total orbs in the grid
    int r =0 ,g=0;
    for(int i = 0; i < SZ; i++)
    {
        for(int j = 0; j < SZ; j++)
        {
            if(tempGrid[i][j].first == 'R'){
                r+= tempGrid[i][j].second;
            }
            else{
                g+= tempGrid[i][j].second;
            }

        }
    }
    if(player == 'R'){
        return r-g;
    }
    else{
        return g-r;
    }
    // return 5000;
}


int check_winner(pp **tempGrid)
{
    int r = 0, g = 0;
    for(int i = 0; i < SZ; i++)
    {
        for(int j = 0; j < SZ; j++)
        {
            if(tempGrid[i][j].first == 'R')
                r += (tempGrid[i][j].second - '0');
            else if(tempGrid[i][j].second == 'G')
                g += (tempGrid[i][j].second - '0');
        }
    }

    //check winner after both of them have moved at least once
    if(r + g <= 2)return -1;

    if(r && !g)
        return 0;
    if(g && !r)
        return 1;

    return -1;
}


void chainReaction(pp **tempGrid, int x, int y, char col)
{
    tempGrid[x][y].first = col;
    tempGrid[x][y].second++;

    ppi u; 
    int x2, y2;
    
    queue<ppi> q;
    q.push({x, y});

    while(!q.empty())
    {
        u = q.front(); q.pop();

        //because of chain reaction the game may have finished but the chain reactions would not stop
        //this will cause segmentation fault in dfs, and an infinite loop in case of bfs
        //so check if the game is over or not
        if(check_winner(tempGrid) != -1)return;

        if(tempGrid[u.first][u.second].second >= splitCondition[u.first][u.second])
        {
            tempGrid[u.first][u.second].second = 0;
            tempGrid[u.first][u.second].first = 'X';

            for(int i = 0; i < 4; i++)
            {
                x2 = u.first + dx[i];
                y2 = u.second + dy[i];
                if(x2 >= 0 && y2 >= 0 && x2 < SZ && y2 < SZ){
                    tempGrid[x2][y2].first = col;
                    tempGrid[x2][y2].second++;
                    if(tempGrid[x2][y2].second >= splitCondition[x2][y2])
                        q.push({x2, y2});
                }
            }
    cout<<u.first<<" "<<u.second<<endl;
            printDynamicGrid(tempGrid);nl;
        }
    }
}


void updateGrid(pp **tempGrid, int x, int y,char col)
{
    if(tempGrid[x][y].first == 'X')
        tempGrid[x][y] = {col, 1};

    else
        pfs("start cr\n"),cout<<x<<" "<<y<<" "<<col<<endl,chainReaction(tempGrid, x, y, col),pfs("done cr\n");
}


int minimax(pp **tempGrid, int depth, bool ismax, int alpha, int beta)
{
    if(depth <= 0 || check_winner(tempGrid) != -1)
        return evaluate(tempGrid);

    pp backupGrid[SZ][SZ];
    for(int i = 0; i < SZ; i++)
    {
        for(int j = 0; j < SZ; j++)
            backupGrid[i][j] = tempGrid[i][j];
    }

    int curr_value, best_value;
    ppi successor;
    bool f = false;

    if(ismax)
    {
        best_value = -inf;

        for(int i = 0; i < SZ; i++)
        {
            for(int j = 0; j < SZ; j++)
            {
                if(tempGrid[i][j].second == 0 || tempGrid[i][j].first == player)
                {
                    cout<<i<<" "<<j<<" sel-max "<<tempGrid[i][j].first<<" "<<tempGrid[i][j].second<<endl;
                    //update tempgrid
                    updateGrid(tempGrid, i, j, player);

                    curr_value = minimax(tempGrid, depth - 1, false, alpha, beta);

                    if(curr_value > best_value){
                        best_value = curr_value, successor = {i, j};
                        cout<<"Better val :"<<best_value<<" Coord :"<<successor.first<<" "<<successor.second<<endl;
                    }

                    alpha = max(alpha, best_value);

                    for(int i2 = 0; i2 < SZ; i2++)
                    {
                        for(int j2 = 0; j2 < SZ; j2++)
                            tempGrid[i2][j2] = backupGrid[i2][j2];
                    }

                    if(beta <= alpha)
                    {
                        f = true;
                        break;
                    }
                }
            }

            if(f)
                break;
        }

        //set successor
        if(depth == D)
            __successor__ = successor;
    }
    else
    {
        best_value = inf;
        cout<<"inside min\n";
        for(int i = 0; i < SZ; i++)
        {
            for(int j = 0; j < SZ; j++)
            {
                if(tempGrid[i][j].second == 0 || tempGrid[i][j].first == otherPlayer)
                {
                    cout<<i<<" "<<j<<" sel-min "<<tempGrid[i][j].first<<" "<<tempGrid[i][j].second<<endl;
                    //update tempgrid
                    updateGrid(tempGrid, i, j, otherPlayer);
                    cout<<"upgraded\n";
                    curr_value = minimax(tempGrid, depth - 1, true, alpha, beta);

                    best_value = min(best_value, curr_value);
                    beta = min(beta, best_value);
                    cout<<"start rest\n";
                    for(int i2 = 0; i2 < SZ; i2++)
                    {
                        for(int j2 = 0; j2 < SZ; j2++)
                            tempGrid[i2][j2] = backupGrid[i2][j2];
                    }cout<<"restored\n";

                    if(beta <= alpha)
                    {
                        f = true;
                        break;
                    }
                }
            }

            if(f)
                break;
        }
    }

    return best_value;
}

ppi select_move(int ch)
{
    //random
    if(ch == 1)
    {
        while(true)
        {
            int x = rand() % 8;
            int y = rand() % 8;

            if(grid[x][y].second == 0 || grid[x][y].first == player)
                return {x, y};
        }
    }

    //minimax
    else if(ch == 2)
    {
        __successor__ = {-1, -1};
        pp **arr = new pp*[SZ];
        for(int i = 0; i < SZ; i++)
            arr[i] = new pp[SZ];

        for(int i = 0; i < SZ; i++)
        {
            for(int j = 0; j < SZ; j++)
                arr[i][j] = grid[i][j];
        }
        printDynamicGrid(arr);nl;
        minimax(arr, D, true, -inf, inf);

        //free memory
        for(int i  = 0; i < SZ; i++)
            delete arr[i];

        delete arr;

        return __successor__;
    }
}


void writeFile(ppi x)
{
    str = "0\n";
    str.pb(x.first + '0');
    str.pb(' ');
    str.pb(x.second + '0');

    ofstream outfile;
    outfile.open("shared_file.txt");

    outfile << str;
    outfile.close();
}

int main(int argc, char *argv[])
{
    int i,j,k;
    int n,m;

    //--------------------------------------------------------------------
    //split conditions
    for(i = 0; i < SZ; i++)
    {
        for(j = 0; j < SZ; j++)
            splitCondition[i][j] = 4;
    }

    for(i = 0; i < SZ; i ++)
    {
        splitCondition[0][i] = 3;
        splitCondition[SZ - 1][i] = 3;
        splitCondition[i][0] = 3;
        splitCondition[i][SZ - 1] = 3;
    }

    splitCondition[0][0] = splitCondition[SZ - 1][SZ - 1] = 2;
    splitCondition[0][SZ - 1] = splitCondition[SZ - 1][0] = 2;
    //--------------------------------------------------------------------

    //replace with arg
    player = *argv[1];
    k = *argv[2] - '0';

    otherPlayer = 'R';
    if(player == 'R')
        otherPlayer = 'G';

    while(true)
    {
        while(true)
        {
            m = readFile();
            if(m != -1)
                break;
        }

        ppi x = select_move(k);
        
        writeFile(x);
    }

    return 0;
}
