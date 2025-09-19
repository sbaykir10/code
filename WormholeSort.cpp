// Source: https://usaco.guide/general/io

#include <bits/stdc++.h>

using namespace std;

typedef vector<vector<pair<int, int>>> vvpii;

bool solve(vvpii& adj, vector<int>& cows, int minsize){
    int cownum = cows.size();

    vector<int> visited(cownum, 0);//We will store which cows are part of which component
    int cindex = 1; //The index we will assign to newly filled cows

    for(int cow=0; cow<cownum; cow++){

        if(visited[cow]){continue;}//Already visited

        visited[cow]=cindex;//Place this cow in the current cindex
        queue<int> tosearch;

        //Initially, put all of the neighbors of the cow in
        for(const auto& a:adj[cow]){
            if(a.second<minsize || visited[a.first]){continue;}//Skip any wormholes that are too small
            tosearch.push(a.first);
        }

        while(!tosearch.empty()){
            int to = tosearch.front();
            visited[to]=cindex;
            
            for(const auto& a : adj[to]){
                if(a.second < minsize || visited[a.first]){continue; /*cout<<"Wormhold was too small"<<endl;*/}
                tosearch.push(a.first);
            }

            tosearch.pop();
        }

        cindex++;
    } 

    //for(const auto& cow: visited){cout<<cow<<endl;} debugging
    //Now, we loop through every cow and if it is misplaced
    //We will make sure its destination is part of the component

    for(int cow = 0; cow<cownum; cow++){
        if(cows[cow]==cow){continue;}//Already in place
        if(visited[cows[cow]] != visited[cow]){return false;}//Not part of same
    }

    return true;//Otherwise this size is valid!
}


int main() {

    freopen("wormsort.in", "r", stdin);
    freopen("wormsort.out", "w", stdout);

	int cownum, whnum; cin>>cownum>>whnum;
    vector<int> cows(cownum);

    vvpii adj(cownum);
    //For each cow, stores all cows that can be travelled from it
    //And how big the wormhold is

    for(int i=0; i<cownum; i++){cin>>cows[i]; cows[i]--;}

    //A quick check to see if alr sorted
    bool sorted = true;
    for(int i=0; i<cownum; i++){
        if(cows[i] != i){sorted=false; break;}
    }
    if(sorted){cout<<-1<<endl; return 0;}//Alr sorted
    
    int maxwormsize = 0; //Used in binary search later

    for(int i=0; i<whnum; i++){
        int a, b, w; cin>>a>>b>>w; a--;b--;
        adj[a].push_back({b,w});
        adj[b].push_back({a,w});
        maxwormsize = max(maxwormsize, w);
    }
    
    //Strategy: use a binary searching algorithm to find the maximum minimum wormhole size
    //Strategy: use a floodfill algorithm to create connected components

    //Find the maximum size that works;

    int lo = 0, hi = maxwormsize;

    while(lo<hi){
        int mid = lo + (hi-lo+1)/2;

        if(solve(adj, cows, mid)){//this size works
            lo=mid;
        } else {
            hi=mid-1;
        }
    }

    cout<<lo<<endl;


    return 0;
}
