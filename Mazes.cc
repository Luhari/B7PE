#include <iostream> //cout
#include <vector>
#include <cstdlib> //rand()
#include <ctime>   //time(), clock_t
#include <time.h>
#include <unistd.h> //fork, read, open
#include <stdio.h> //sprintf
#include <stdlib.h>
#include <string.h> //strlen
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  //pipes
#include <sys/wait.h> //waitpid
#include <fstream>  //ofstream
#include <climits>  //INT_MAX
#include <set>
#include <queue>
#include <signal.h>
using namespace std;

typedef vector<vector<char> > Matrix;

Matrix astar, BFS;

void read(int h, int w, int pipo) {
    char c;
    int i = 0, j = 0;
    while (read(pipo, &c, sizeof(char)) > 0) {
        if (c != '\n' && j < w) {
            BFS[i][j] = astar[i][j] = c;
            ++j;
        }
        else if (i <= w){
            ++i;
            j = 0;
        }
    }

}

int distancia(int i, int j, int n, int m)
{
    int r = n - i;
    int q = m - j;
    if (r < 0) r *= -1;
    if (q < 0) q *= -1;
    return r+q;
}

void Astar(const vector<vector<char> > &mapa, int n, int m)
{
    vector<vector<int> > dist(n, vector<int>(m));
    for(int k = 0; k < dist.size(); ++k)
    {
        for(int l = 0; l < dist[0].size(); ++l)
        {
            dist[k][l] = distancia(k, l, n-1, m-1);
        }
    }
    priority_queue<pair<int, pair<int,int> > > pendent;
    set<pair<int,int> > visitats;
    pendent.push(make_pair(dist[1][1], make_pair(1, 1)));
    int a = 1;
    int b = 1;
    visitats.insert(make_pair(a,b));
    while(!pendent.empty() and (a != n-2 and b != m-2))
    {
        a = pendent.top().second.first;
        b = pendent.top().second.second;
        pendent.pop();
        if(mapa[a-1][b] != '#' and visitats.find(make_pair(a-1, b)) == visitats.end())
        {
            pendent.push(make_pair(dist[a-1][b], make_pair(a-1, b)));
            visitats.insert(make_pair(a-1,b));
        }
        if(mapa[a+1][b] != '#' and visitats.find(make_pair(a+1, b)) == visitats.end())
        {
            pendent.push(make_pair(dist[a+1][b], make_pair(a+1, b)));
            visitats.insert(make_pair(a+1,b));
        }
        if(mapa[a][b+1] != '#' and visitats.find(make_pair(a, b+1)) == visitats.end())
        {
            pendent.push(make_pair(dist[a][b+1], make_pair(a, b+1)));
            visitats.insert(make_pair(a,b+1));
        }
        if(mapa[a][b-1] != '#' and visitats.find(make_pair(a, b-1)) == visitats.end())
        {
            pendent.push(make_pair(dist[a][b-1], make_pair(a, b-1)));
            visitats.insert(make_pair(a,b-1));
        }
    }
    if(a != n-2 and b != m-2) cerr << "NO HI HA CAMI DE (1,1) a (" << n-2 << "," << m-2 << ")\n";
    else cerr << "HI HA CAMI DE (1,1) a (" << n-2 << "," << m-2 << ")\n";
}

void bfs(const vector<vector<char> > &mapa, int n, int m)
{
    queue<pair<int,int> > pendent;
    set<pair<int,int> > visitats;
    pendent.push(make_pair(1, 1));
    int a = 1;
    int b = 1;
    visitats.insert(make_pair(a,b));
    while(!pendent.empty() and (a != n-2 and b != m-2))
    {
        a = pendent.front().first;
        b = pendent.front().second;
        pendent.pop();
        if(mapa[a-1][b] != '#' and visitats.find(make_pair(a-1, b)) == visitats.end())
        {
            pendent.push(make_pair(a-1, b));
            visitats.insert(make_pair(a-1,b));
        }
        if(mapa[a+1][b] != '#' and visitats.find(make_pair(a+1, b)) == visitats.end())
        {
            pendent.push(make_pair(a+1, b));
            visitats.insert(make_pair(a+1,b));
        }
        if(mapa[a][b+1] != '#' and visitats.find(make_pair(a, b+1)) == visitats.end())
        {
            pendent.push(make_pair(a, b+1));
            visitats.insert(make_pair(a,b+1));
        }
        if(mapa[a][b-1] != '#' and visitats.find(make_pair(a, b-1)) == visitats.end())
        {
            pendent.push(make_pair(a, b-1));
            visitats.insert(make_pair(a,b-1));
        }
    }
    if(a != n-2 and b != m-2) cout << "NO HI HA CAMI DE (1,1) a (" << n-2 << "," << m-2 << ")\n";
    else cout << "HI HA CAMI DE (1,1) a (" << n-2 << "," << m-2 << ")\n";
}


bool stillExecuting, stillReading, waiting;
int count;

void trat_alarm(int s) {
        count %= 4;
        if(stillReading) {
            cout << "Still reading";
            for (int i = 0; i < count;++i) {
                cout << ".";
            }
            cout << endl;
            alarm(2);
        }
        if (stillExecuting) {
            cout << "Still calculatig";
            for (int i = 0; i < count;++i) {
                cout << ".";
            }
            count %= 4;
            cout << endl;
            alarm(2);
        }
        ++count;
}

void write(Matrix& M, int h, int w) {
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j)cout << M[i][j];
        cout << endl;
    }
    cout << endl;

}

int main() {
    srand(time(NULL));
    for (int i = 0; i < 80; ++i) {
        int max, w, h;
        max = rand() % 20000; //1 to 10000
        while ((w = rand() % max + 500)%2 == 0); //width odd
        while ((h = rand() % max + 500)%2 == 0); //height [500,max+1]

        int pid = fork();
        if (pid == 0) {
            char width[80], height[80];
            sprintf(width, "%d", w);
            sprintf(height, "%d", h);
            execlp("./MazeGen", "MazeGen", width, height, (char*) NULL);
            cout << "Mutation didnt work..." << endl;
        }
        else if (pid > 0) {
            close(0);
            int pipo = open("pipo.txt", O_RDONLY);
            dup(pipo);
            //Reading from pipo
            waitpid(pid, NULL, 0);

            //Saving the results into results.txt
            ofstream outfile;
            outfile.open("results.txt", ios_base::app);

            //Mazes ready to execute codes!
            cerr << i+1 <<  ": "<< w << " " << h << endl;

            astar = BFS = Matrix (h, vector<char> (w));

            signal(SIGALRM, trat_alarm);

            char buf[10];
            sprintf (buf, "%d %d %d", i+1, w, h);
            outfile << buf;
            //cerr << "We are indeed doing PE's things"<< endl;
            waiting = false;
            stillReading = true;
            count = 1;
            alarm(2);
            clock_t t;
            cerr << "Reading lab"<< endl;
            read(h, w, pipo); //Leer matrices
            stillReading = false;

            //First Algorithm
            cerr << "Executing Astar"<< endl;
            t = clock();
            stillExecuting = true;
            alarm(2);
            Astar(astar, h, w);
            stillExecuting = false;
            alarm(0);
            sprintf(buf, " %f ", float(clock()-t)/CLOCKS_PER_SEC);
            outfile << buf;

            cout << "Waiting a min." << endl;
            alarm(60);
            pause();

            //Second Algorithm
            cerr << "Executing BFS" << endl;
            t = clock();
            stillExecuting = true;
            alarm(2);
            bfs(BFS, h, w);
            stillExecuting = false;
            sprintf(buf, "%f \n", float(clock()-t)/CLOCKS_PER_SEC);
            outfile << buf;
        }
        else cout << "Error forking."<< endl;
    }
    exit(1);
}
