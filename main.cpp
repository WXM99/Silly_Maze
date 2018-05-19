#include <vector>
#include <iostream>
#include <string>
#include <random>
#include <stack>
#include "randomMaze.h"
#include "solve.h"

int main(){
    int width ;
    int height ;

    cout<<"Please input the width: ";
    cin>>width;
    cout<<"Please input the height:";
    cin>>height;

    mt19937 rng;
    rng.seed(random_device()());

    uniform_int_distribution<int> distribution(0, 9);

    vector<vector<int> > maze = generate(width, height);
    vector<vector<int> > maze2 = maze;

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            if (maze[i][j] == 1)
            {
                cout<<YELLOW<<maze[i][j];
            } else{
                cout<<GREEN<<maze[i][j];
            }
        }
        cout<<endl;
    }
    
    cerr<<RED;
    int x1,y1,x2,y2;
    cout<<"point #1: ";
    cin>>x1>>y1;
    cout<<"point #2: ";
    cin>>x2>>y2;

    stack<Point> init;
    init.push(Point(x1,y1));
    rollingNeib(maze, init, Point(x2,y2));
    
    //if(paths.empty()) cerr<<"check";
    if(!paths.empty()){
        
        int max = paths[paths.size()-1].size();
        vector<Point> most;
        for(vector<Point> move : paths){
            if (move.size() >= max){
                max = move.size();
                most = move;
            }
        }
        cout<<most.size()<<endl;
        print(most, maze, height, width);
        most = complete_shorten(most,height,width);
        print(most, maze, height, width);

        int min = paths[paths.size()-1].size();
        vector<Point> less;
        for(vector<Point> move : paths){
            if (move.size() <= min){
                min = move.size();
                less = move;
            }
        }

        cout<<less.size()<<endl;
        print(less, maze, height, width);
        less = complete_shorten(less,height,width);
        cout<<less.size()<<endl;
        print(less, maze, height, width);

    }

    return 0;
}
