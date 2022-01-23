#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 64

// Store origin character and its frequency
typedef struct word{
	char c;
	int freq;
}Word;

// Store the student's submission
typedef struct huffman{
	char c;
	char code[MAX];
}Huffman;

// Priority Queue, will be used as a min heap
typedef struct PriorityQueue{
    int elements[MAX];
    int size; // default to 0
}Heap;

// Word Tree structure, use for checking prefix
typedef struct TNode
{
    struct TNode * left;
    struct TNode * right;
    int mark;
}* WordTree;

Word words[MAX];
Huffman encoded[MAX];
Heap minHeap;

// Use in check prefix process
int flag;

void Insert(int X) {
    // Insert into back of the heap
    minHeap.elements[++minHeap.size] = X;
    int tmp;
    // Percolate Up, adjust structure
    for (int i = minHeap.size; minHeap.elements[i/2] > minHeap.elements[i]; i /= 2 ){
        // Swap two item
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
            // Choose another child
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

// If any node in t's subtree has been encoded, t is a invaild code
int CheckChildren(WordTree t){
    if(!t){
        return 1;
    }
    if(t->mark){
        return 0;
    }
    return CheckChildren(t->left) && CheckChildren(t->right);
}

WordTree insert(WordTree t, char code[MAX], int start){
    //  Reach a null ptr
    if(!t){
        t = (WordTree)malloc(sizeof(struct TNode));
        memset(t, 0, sizeof(struct TNode));
    }
    //  Next character is '/0', found its position
    if(!code[start+1]){
    //  Check the prefix relationship
        if(!CheckChildren(t)){
            flag = 0;
            return NULL;
        }else{
            t -> mark = 1;
        }
    }else{
        // Continue insertion
        if(t -> mark){
        //  current position is a prefix of the code to be inserted
            flag = 0;
            return NULL;
        }
        if(code[start+1] == '0'){
            t->left = insert(t->left, code, start+1);
        }else{
            t->right = insert(t->right, code, start+1);
        }
    }
    return t;
}

int CheckPrefix(int N){
    flag = 1;
    WordTree dummy = (WordTree)malloc(sizeof(struct TNode));
    memset(dummy, 0, sizeof(struct TNode));
    for (int i = 0; i < N; i++){
        // Judge it belongs to the left subtree or right subtree
        if(encoded[i].code[0] == '0'){
            dummy->left = insert(dummy->left, encoded[i].code, 0);
        }else{
            dummy->right = insert(dummy->right, encoded[i].code, 0);
        }
        if(!flag){
            // conflict occurs, no need to check anymore
            return 0;
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
        // Skip one space
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
            // Skip a newline character
            getchar();
            cur_cost += words[j].freq * strlen(encoded[j].code);
        }
        // To check if it gives an optimal solution and has no unambiguity
        if((cur_cost == opt_cost) && CheckPrefix(N)){
            // Output the check result
            puts("Yes");
        }else{
            // Output the check result
            puts("No");
        }
    }
    return 0;
}