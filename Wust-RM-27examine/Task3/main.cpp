#include <iostream>
using namespace std;
#include <vector>
#include <queue>
#include <cmath>
#include <climits>

struct Node{
    int x,y;
    int g,f;
    bool operator<(const Node& other) const{
        return f>other.f;
    }
};

void dfs(vector<vector<char>>& map,vector<vector<bool>>& visited,int& k,int x,int y){
visited[x][y]=true;
k++;
int dr[4]={-1,0,1,0};
int dc[4]={0,1,0,-1};
for(int i=0;i<4;i++){
    int nx=x+dr[i];
    int ny=y+dc[i];
    if(nx>=0 && nx<map.size() && ny>=0 && ny<map[0].size() && map[nx][ny]!='#' && !visited[nx][ny]) dfs(map,visited,k,nx,ny);
}
}

int main(){
int R,C;
int sx,sy,gx,gy;
int k=0;
cin>>R>>C;
vector<vector<char>> map(R, vector<char>(C));
vector<vector<bool>> visited(R, vector<bool>(C,false));
for(int i=0;i<R;i++){
    for(int j=0;j<C;j++){
        cin>>map[i][j];
        if(map[i][j]=='S'){
            sx=i;
            sy=j;//记录起点坐标
        }
        if(map[i][j]=='G'){
            gx=i;
            gy=j;//记录终点坐标
        }
    }
}//录入地图信息
dfs(map,visited,k,sx,sy);
cout<<"reachable cells: "<<k<<endl;

int regions=1;//计算孤岛
vector<vector<bool>> visited2(R, vector<bool>(C));
for(int i=0;i<R;i++){
    for(int j=0;j<C;j++){
        if(map[i][j]!='#' && !visited[i][j]){
            regions++;
            dfs(map,visited,k,i,j);
        }
    }
}
cout<<"regions: "<<regions<<endl;

//计算最小代价 
vector<vector<int>> dist(R, vector<int>(C,INT_MAX));
dist[sx][sy]=0;
priority_queue<Node> pq;
pq.push({sx,sy,0,abs(sx-gx)+abs(sy-gy)});
while(!pq.empty()){
    Node cur=pq.top();
    pq.pop();
    if(cur.x==gx && cur.y==gy){
        cout<<"cost:"<<cur.g<<endl;
        return 0;
    }
    else{
        int dr[4]={-1,0,1,0};
        int dc[4]={0,1,0,-1};
        for(int i=0;i<4;i++){
            int nx=cur.x+dr[i];
            int ny=cur.y+dc[i];
            if(nx>=0 && nx<map.size() && ny>=0 && ny<map[0].size() && map[nx][ny]!='#'){
                int step;
                if(map[nx][ny]<='9' && map[nx][ny]>='2'){
                    step=map[nx][ny]-'0';
                }
                if(map[nx][ny]=='.'||map[nx][ny]=='S'||map[nx][ny]=='G'){
                    step=1;
                }
                int new_g=cur.g+step;
                if(new_g<dist[nx][ny]){
                    dist[nx][ny]=new_g;
                    pq.push({nx,ny,new_g,new_g+abs(nx-gx)+abs(ny-gy)});
                }
            }
                
        }
    }

}
cout<<"-1"<<endl;
return 0;
}