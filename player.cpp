/***from dust i have come, dust i will be***/

#include<bits/stdc++.h>
#include<unistd.h>
#include <ctime>

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
// #define D 2

using namespace std;

string str, line;
vector<string> vec;
char player, otherPlayer;
pp grid[SZ][SZ];
int splitCondition[SZ][SZ];
int D = 2;

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
    //Simple heuristic fr counting total orbs in the grid
    int r =0 , g = 0;
    for(int i = 0; i < SZ; i++)
    {
        for(int j = 0; j < SZ; j++)
        {
            if(tempGrid[i][j].first == 'R') {
                r+= tempGrid[i][j].second;
            }

            else {
                g+= tempGrid[i][j].second;
            }
        }
    }

    if(player == 'R') {
        int diff = r-g;
        g == 0 ? diff = 20000 :diff= r-g;
        return diff;
    }

    else {
        int diff = g-r;
        r == 0 ? diff = 20000 :diff= g-r;
        return diff;
    }
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

//-------------------------------------------------------------------------------------------------
vector<ppi> wec;
void update_grid(pp **tempGrid, int x,int y, char col)
{
    if(tempGrid[x][y].first == 'X')
        tempGrid[x][y].first = col, tempGrid[x][y].second = 1;

    else
    {
        if(splitCondition[x][y] == tempGrid[x][y].second + 1)
            wec.pb({x, y});

        tempGrid[x][y].first = col;
        tempGrid[x][y].second++;
    }
}


void reaction(pp **tempGrid, int x,int y,char col)
{
    int x2, y2, cnt;
    if(tempGrid[x][y].second == splitCondition[x][y])
        tempGrid[x][y].first = 'X', tempGrid[x][y].second = 0;

    else
    {
        cnt = tempGrid[x][y].second - splitCondition[x][y];
        if(cnt >= splitCondition[x][y])
            wec.pb({x, y});

        tempGrid[x][y].first = col;
        tempGrid[x][y].second = cnt;
    }

    for(int i = 0;i < 4; i++)
    {
        x2 = x + dx[i];
        y2 = y + dy[i];

        if(x2 >= 0 && x2 < SZ && y2 >= 0 && y2 < SZ)
            update_grid(tempGrid, x2, y2, col);
    }
}


void update(pp **tempGrid, int x, int y, char col)
{
    wec.clear();
    update_grid(tempGrid, x, y, col);

    for(ppi e : wec){
        reaction(tempGrid, e.first, e.second, col);
    }
}
//-------------------------------------------------------------------------------------------------


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

    //priority_queue <ppi> priorityQue;
    int iteration_count = 0;

    if(ismax)
    {
        best_value = -inf;

        for(int i = 0; i < SZ; i++)
        {
            for(int j = 0; j < SZ; j++)
            {
                if(tempGrid[i][j].second == 0 || tempGrid[i][j].first == player)
                {
                    //cout<<i<<" "<<j<<" sel-max "<<tempGrid[i][j].first<<" "<<tempGrid[i][j].second<<endl;

                    //update tempgrid
                    update(tempGrid, i, j, player);

                    curr_value = minimax(tempGrid, depth - 1, false, alpha, beta);

                    if(curr_value > best_value) {
                        best_value = curr_value, successor = {i, j};
                        //cout<<"Better val :"<<best_value<<" Coord :"<<successor.first<<" "<<successor.second<<endl;
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
        for(int i = 0; i < SZ; i++)
        {
            for(int j = 0; j < SZ; j++)
            {
                if(tempGrid[i][j].second == 0 || tempGrid[i][j].first == otherPlayer)
                {
                   // cout<<i<<" "<<j<<" sel-min "<<tempGrid[i][j].first<<" "<<tempGrid[i][j].second<<endl;

                    //update tempgrid
                    update(tempGrid, i, j, otherPlayer);

                    curr_value = minimax(tempGrid, depth - 1, true, alpha, beta);

                    best_value = min(best_value, curr_value);
                    beta = min(beta, best_value);

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
        clock_t start,end;

	    start = clock();
        D = 2;
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
        float time = 0.0;
        while(time < 500){
            minimax(arr, D, true, -inf, inf);
            end = clock() - start;
            time = end/CLOCKS_PER_SEC * 1000;
            cout<<"Time : "<<time<<" D: "<<D<<endl;
            D++;
            if(D >= 5)
                break;
        }

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
    int i, j, k;
    int n, m;

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
