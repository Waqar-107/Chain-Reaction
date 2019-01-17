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
#define D 3

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


int dx[] = {1, -1, 0, 0};
int dy[] = {0, 0, 1, -1};

ppi __successor__;
int evaluate()
{
    return 5000;
}


int check_winner(pp **tempGrid)
{
    int r = 0, g = 0, emp = 0;
    for(int i = 0; i < SZ; i++)
    {
        for(int j = 0; j < SZ; j++)
        {
            if(tempGrid[i][j].first == 'R')
                r++;
            else if(tempGrid[i][j].second == 'G')
                g++;
            else
                emp++;
        }
    }

    if(r && !g && !emp)
        return 0;
    if(g && !r && !emp)
        return 1;
    return -1;
}


void chainReaction(pp **tempGrid, int x, int y, char col)
{
    tempGrid[x][y].first = col;
    tempGrid[x][y].second++;

    if(tempGrid[x][y].second < splitCondition[x][y])
        return;

    tempGrid[x][y].second = 0;

    int x2, y2;
    for(int i = 0; i < 4; i++)
    {
        x2 = x + dx[i];
        y2 = y + dy[i];

        if(x2 > -1 && y2 > -1 && x2 < SZ && y2 < SZ)
            chainReaction(tempGrid, x2, y2, col);
    }
}


void updateGrid(pp **tempGrid, int x, int y,char col)
{
    if(tempGrid[x][y].first == 'X')
        tempGrid[x][y] = {col, 1};

    else
        chainReaction(tempGrid, x, y, col);
}


int minimax(pp **tempGrid, int depth, bool ismax, int alpha, int beta)
{
    if(depth <= 0 || check_winner(tempGrid) != -1)
        return evaluate();

    pp backupGrid[SZ][SZ];
    for(int i = 0; i < SZ; i++)
    {
        for(int j =0; j < SZ; j++)
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
                    //update tempgrid
                    updateGrid(tempGrid, i, j, player);
                    curr_value = minimax(tempGrid, depth - 1, false, alpha, beta);

                    if(curr_value > best_value)
                        best_value = curr_value, successor = {i, j};

                    alpha = max(alpha, best_value);
                    if(beta <= alpha)
                    {
                        f = true;
                        break;
                    }

                    for(int x = 0; x < SZ; x++)
                    {
                        for(int y =0; y < SZ; y++)
                            tempGrid[x][y] = backupGrid[x][y];
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
                    //update tempgrid
                    updateGrid(tempGrid, i, j, otherPlayer);
                    curr_value = minimax(tempGrid, depth - 1, true, alpha, beta);

                    best_value = min(best_value, curr_value);
                    beta = max(beta, best_value);

                    if(beta <= alpha)
                    {
                        f = true;
                        break;
                    }

                    for(int x = 0; x < SZ; x++)
                    {
                        for(int y =0; y < SZ; y++)
                            tempGrid[x][y] = backupGrid[x][y];
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

int main()
{
    int i,j,k;
    int n,m;

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

    //replace with arg
    cout<<"player color?\n";
    cin>>player;

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

        ppi x = select_move(2);
        cout<<"selected "<<x.first<<" "<<x.first<<endl;
        writeFile(x);
    }

    return 0;
}
