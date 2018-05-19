#include <vector>
#include <iostream>
#include <string>
#include <random>
#include <stack>
using namespace std;

vector<Point> trash;
vector<Point> onepath;

vector<Point> neib(const vector<vector<int> > &mat, const Point &p){
    int colum = mat[0].size();
    int row = mat.size();
    vector<Point> bruches;
    int counter =0;
    for(int i = p.X-1;i<p.X+2;i++){
        for(int j = p.Y-1; j<p.Y+2;j++){//1 is alive(road)
            if(i<0||i>= row||j<0||j>=colum||(i==p.X && j == p.Y)){}
            else{
                if(mat[i][j]){ bruches.push_back(Point(i,j));}
            }
        }
    }
    return bruches;
}


vector<Point> roll(vector<Point> vec, int a){
    vector<Point> messed;
    int size = vec.size();
    for(int i = 0; i < vec.size(); i ++){
        messed.push_back(vec[(i+a)%size]);
    }
    return messed;
}

vector<vector<Point> > paths;
bool recurSolve(vector<vector<int> > &maze, stack<Point> &start, const Point &end, int mode){

    static stack<Point> crossing;
    if(!start.empty()){
        Point move = start.top();
        maze[move.X][move.Y] = 0;
        onepath.push_back(move);
        start.pop();
        if(move == end) {
            while(!crossing.empty()){
                crossing.pop();
            }
            paths.push_back(onepath);
            while(!onepath.empty()){
                onepath.pop_back();
            }
            return true;
        }else{
            vector<Point> next;
            next = neib(maze, move);
            next = roll(next,mode);
            for(Point i: next){
                start.push(i);
            }
            if(next.size() > 1){
                int times = next.size();
                for(int i = 0; i < times; i++){
                    crossing.push(move);
                }
            }else if(next.size() == 0){
                while( onepath[onepath.size()-1] != crossing.top()){
                    onepath.pop_back();
                }
                if(!crossing.empty())crossing.pop();
            }
            return recurSolve(maze, start, end, mode);    
        }
    }else return false;
}

bool pureSolve(vector<vector<int> > &maze, stack<Point> &start, const Point &end){
    if(!start.empty()){
        Point move = start.top();
        maze[move.X][move.Y] = 0;
        start.pop();
        if(move == end) {
            return true;
        }else{
            vector<Point> next = neib(maze, move);
            for(Point i: next){
                start.push(i);
            }
            return pureSolve(maze, start, end);    
        }
    }else return false;
}

void rollingNeib(const vector<vector<int> > &_maze, const stack<Point> &_start, const Point &end){
    vector<vector<int> > maze = _maze;
    stack<Point> start = _start;
    int rolling_time = 0;
    if(recurSolve(maze, start, end,0)){
        paths.pop_back();
        maze = _maze;
        start = _start;
        while(paths.size()<20){
            recurSolve(maze, start, end, rolling_time);
            rolling_time++;
            maze = _maze;
            start = _start;
        }
    }else cout<<"no solution"<<endl;
}

void print(vector<Point> onepath, vector<vector<int> > maze2, int height, int width){
for(int i = 0; i < onepath.size(); i++)
        {
            int x,y;
            x = onepath[i].X;
            y = onepath[i].Y;
            maze2[x][y] = 2;
        }
        for(int i = 0; i < trash.size(); i++)
        {
            int x,y;
            x = trash[i].X;
            y = trash[i].Y;
            maze2[x][y] = 3;
        }
        for(int i = 0; i < height; i++)
        {
            for(int j = 0; j < width; j++)
            {
                if (maze2[i][j] == 1)
                {
                    cout<<YELLOW<<maze2[i][j];
                } else if(maze2[i][j]==0){
                    cout<<GREEN<<maze2[i][j];
                } else if(maze2[i][j]==2){
                    cout<<UNK<<maze2[i][j];
                } else cout<<UNK<<maze2[i][j];
            }
            cout<<endl;
        }
        cout<<endl;
}

void clear(stack<Point>&a){
    while(!a.empty()){
        a.pop();
    }
}

vector<vector<int> > generate(int width, int height)
{
    mt19937 rng;
    rng.seed(random_device()());

    default_random_engine fixed;
    uniform_int_distribution<int> distribution(0, 9);

    vector<vector<int> > maze(height, vector<int>(width));
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            maze[i][j] = distribution(rng) < DENSITY ? 1 : 0;//fixed or rng
        }
    }
    return maze;
}

bool isNeib(Point a, Point b){
    int res = (a.X-b.X)*(a.X-b.X) + (a.Y-b.Y)*(a.Y-b.Y);
    if(res<=2) return true;
    else return false;
}

bool reach(vector<Point> onepath,int i, int height, int width){
    vector<Point> de_path;
    for(int j = 0; j < onepath.size();j++){
        if(j!= i) de_path.push_back(onepath[j]);
    }

    vector<vector<int> > one_path_map;
    for(int row = 0; row<height;row++){
        vector<int> line;
        for(int col = 0; col<width;col++){
            line.push_back(0);
        }
        one_path_map.push_back(line);
    }

    for(Point p: de_path){
        one_path_map[p.X][p.Y] = 1;
    }

    stack<Point> init;
    init.push(onepath[0]);

    return pureSolve(one_path_map, init, onepath[onepath.size()-1]);
}

vector<Point> de_path(vector<Point> path, int pos){
    int len = path.size();
    vector<Point> de_path;
    for(int i = 0; i < len; i ++){
        if(i!= pos) de_path.push_back(path[i]);
    }
    return de_path;
}

vector<Point> shorten(vector<Point> path, int height, int width){
    for(int j = 1; j < path.size(); j++){
        if(reach(path, j, height, width)){
            path = de_path(path, j);
        }
    }
    return path;
}

vector<Point> complete_shorten(vector<Point> path, int height, int width){
        int pre = 1;
        int con = 0;
        while(pre!=con){
            pre = path.size();
            path = shorten(path,height,width);
            con = path.size();
        }
        return path;
}
