#include "the3.h"
static const long long INF=1000000000000000LL;
// You  can define extra functions here



// INPUT :
//            n         : number of nodes in the graph
//            edgeList  : edges in the graph
//            scores    : importance scores
// return value :
//                 number of disconnected components

void DFS(int i,int n, int**& d, bool*& visited){
  if(visited[i]) return;
  visited[i]=true;
    for(int j=0;j<n;j++){
      if(d[i][j]!=INF && d[i][j]>0){
        DFS(j,n,d,visited);
      }
    }
}

void scoreUpdate(long**& d, double*& score, int n){
   long up,down;
   int a,i,k,j;
   for(a=0;a<n;a++){
     score[a]=0;
   }
   for(k=0;k<n;k++){
     for(i=0;i<n;i++){
       for(j=0;j<n;j++){
         if(d[i][j]!=INF && d[i][k]!=INF && d[k][j]!=INF){
           if(i==j){
             up=d[i][k]+d[k][j];
             down=1;
             score[k]+= ((double)up/down);
           }
           else if(k==i || k==j){
             up=0;
             down=d[i][j];
             score[k]+=((double)up/down);
           }
           else{
             up=d[i][k]+d[k][j];
             down=d[i][j];
             score[k]+=((double)up/down);
           }
         }

       }
     }
   }
}

void ShortestPath(int**& edgeList,long**& d,int n){

  int i,j,k;

  for(i=0;i<n;i++){

    for(j=0;j<n;j++){

      if(i==j){
         d[i][j]=0;
      }

      else if(edgeList[i][j]==0){
        d[i][j]= INF;
      }

      else{
         d[i][j]=edgeList[i][j];
      }

    }
  }


  for(k=0;k<n;k++){

    for(i=0;i<n;i++){

      for(j=0;j<n;j++){

        if(d[i][k]+d[k][j]<d[i][j]){
          d[i][j]=d[i][k]+d[k][j];
        }

      }
    }
  }
}

int Important (int n, int**& edgeList, double*& scores)
{
   long** d = new long*[n];
   int cmp=0; int i;
   for(i = 0; i < n; i++)
   {
     d[i] = new long[n];
   }
   ShortestPath(edgeList,d,n);
   scoreUpdate(d,scores,n);
   bool *visited= new bool[n];
   for(i=0;i<n;i++){
     visited[i]=false;
   }
   for(i=0;i<n;i++){
     if(!visited[i]){
       DFS(i,n,edgeList,visited);
       cmp++;
     }
   }


   return cmp;
}
