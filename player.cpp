/***from dust i have come, dust i will be***/

#include<bits/stdc++.h>
#include<unistd.h>

#define dbg printf("in\n")
#define nl printf("\n")

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

using namespace std;

string str, line;
vector<string> vec;
char player, otherPlayer;
pp grid[SZ][SZ];

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

ppi select_move()
{
    while(true){
        int x = rand() % 8;
        int y = rand() % 8;

        if(grid[x][y].second == 0 || grid[x][y].first == player)
            return {x, y};
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

                usleep(10);
         }

         ppi x = select_move();
         writeFile(x);
     }

    return 0;
}
