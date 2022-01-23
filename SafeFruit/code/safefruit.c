#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX 1000
#define INF 1e9
clock_t start_time, finish_time;
double duration;

int Price[MAX];
int G[MAX][MAX];
// IndexToID
int ID[MAX];
// temp data need for dfs
int selc[MAX];
// res records the maximum fruits from subset which index from i to m
int res[MAX];
// final answer to the problem
int max_sel;
int min_price;
int final_selc[MAX];
// n tips, m kinds of fruit
int n, m;
// Used for comparsion in quick sort
int cmp(const void *a, const void *b){
    return *(int*)a - *(int*)b;
}
// When the start location is different
// Everytime maxIndependSet calls dfs, clean previous data first
void Init(){
    max_sel = 0;
    min_price = INF;
}
// Save the current best choices
void copy_selc(){
    memset(final_selc, 0, MAX * sizeof(int));
    int count = 0;
    for (int i = 0; i < m; i++){
        if(selc[i]){
            final_selc[count++] = ID[i];
        }
    }  
}
// DFS, calc maximum number of fruits in set [cur, cur+1, cur+2, ..., m-1]
void dfs(int cur, int price, int count){
    for(int i = cur; i < m; i++){
        /* four cases that pruning should occur:
        1. Adding all the fruit that can be eaten together is still smaller than the current best choice
        2. Adding all the fruit left is still smaller than the current best choice
        3. Adding all the fruit left can't produce better cost
        4. Adding all the fruit that can be eaten together can't produce better cost
        */
        if  (count + res[i] < max_sel ||count + m - i < max_sel
            ||(count + m - i == max_sel && price >= min_price)
            ||(count + res[i] == max_sel && price >= min_price)) return;
        // Check if there exists a conflict with previous selected fruits, if so, move to next fruit
        for (int j = 0; j < m; j++){
            if (selc[j]){
                if(G[ID[j]][ID[i]]) goto next;
            }               
        }
        selc[i] = 1;
        // Pass the new price and let number of fruits selected plus one
        dfs(i + 1, price + Price[ID[i]], count + 1);
        // Backtracking
        selc[i] = 0;
        next:;
    }
    // Check and update the optimal solution 
    if(count > max_sel){
        max_sel = count;
        min_price = price;
        copy_selc();
    }else if(count == max_sel && price < min_price){
        min_price = price;
        copy_selc();
    }
}

void maxIndependSet(){
    for(int i = m - 1; i >= 0; i--){
        Init();
        dfs(i, 0, 0);
        // Save the maxIndependSet 
        res[i] = max_sel;
    }
}

int main(int argc, char const *argv[]){
    int i, j;
    scanf("%d %d", &n, &m);
    for (int c = 0; c < n; c++){
        scanf("%d %d", &i, &j);
        G[i][j] = 1;
        G[j][i] = 1;
    }
    for (int c = 0; c < m; c++){
        scanf("%d %d", &i, &j);
        ID[c] = i;
        Price[i] = j;
}
    // Make ID increasing to simplify final print step
    qsort(ID, m, sizeof(int), cmp);
    start_time = clock();
    maxIndependSet();
    printf("%d\n",max_sel);
    for (i = 0; i < max_sel - 1; i++){
        printf("%03d ", final_selc[i]);
    }
    printf("%03d\n", final_selc[max_sel-1]);
    printf("%d\n",min_price);
    #ifdef TIMETEST
    finish_time = clock();
    duration = (double)(finish_time - start_time) / CLOCKS_PER_SEC;
    printf( "%lf seconds\n", duration);
    #endif
    return 0;
}