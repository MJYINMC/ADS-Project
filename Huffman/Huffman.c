#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 40000

typedef struct word{
	char c;
	int freq;
}Word;

typedef struct huffman{
	char c;
	char code[MAX];
}Huffman;

typedef struct PriorityQueue{
    int elements[MAX];
    int size; // default to 0
}Heap;

Word words[MAX];
Huffman encoded[MAX];
Heap minHeap;

void Insert(int X) {
    // Insert into back of the heap
    minHeap.elements[++minHeap.size] = X;
    int tmp;
    // Percolate Up, adjust structure
    for (int i = minHeap.size; minHeap.elements[i/2] > minHeap.elements[i]; i /= 2 ){
        tmp = minHeap.elements[i];
	    minHeap.elements[i] = minHeap.elements[i/2];
        minHeap.elements[i/2] = tmp;
    }
}

int DeleteMin(){
    int child;
    int tmp, min;
    min = minHeap.elements[1];
    // Replace the min item with last element
    minHeap.elements[1] = minHeap.elements[minHeap.size--];
    // Percolate Down, adjust structure
    for (int i = 1; i * 2 <= minHeap.size; i = child){
        child = i * 2; 
        if (child != minHeap.size && minHeap.elements[child+1] < minHeap.elements[child]) 
            child++;     
        if ( minHeap.elements[i] > minHeap.elements[child] ){   
            tmp = minHeap.elements[i];
            minHeap.elements[i] = minHeap.elements[child]; 
            minHeap.elements[child] = tmp;
        }
        else break;   
    } 
    return min; 
}

int CheckPrefix(int N){
    char * code1, * code2;
    // Check if there exists a substring relationship between two codes
	for(int i = 0; i < N; i++){
		for(int j = i+1; j < N; j++){
            code1 = encoded[i].code;
            code2 = encoded[j].code;
            if(strncmp(code1, code2, strlen(code2)) == 0){
                return 0;
            }
		}
	}
    return 1;
}

int main(int argc, char const *argv[])
{
    int N, M, opt_cost = 0;
    scanf("%d\n", &N);
    // Init heap structure
    for(int i = 0; i < N; i++){
    	scanf("%c %d", &words[i].c, &words[i].freq);
        getchar();
    	Insert(words[i].freq);
	}
    /* 
    Huffman's algorithm
    We need to operate N-1 times to merge the minHeap
    */
    for (int i = 1; i < N; i++){
        int left, right;
        //pop out two least elements
        left = DeleteMin();
        right = DeleteMin();
        // treat the two nodes as one single node
        Insert(left + right);
        opt_cost += left + right;
    }

    // Check the student's submission
    scanf("%d\n", &M);
    for (int i = 0; i < M; i++){
        int cur_cost = 0;
        for (int j = 0; j < N; j++){
            scanf("%c %s", &encoded[j].c, encoded[j].code);
            getchar();
            cur_cost += words[j].freq * strlen(encoded[j].code);
        }
        // To check if it gives an optimal solution and has no unambiguity
        if((cur_cost == opt_cost) && CheckPrefix(N)){
            puts("Yes");
        }else{
            puts("No");
        }
    }
    return 0;
}