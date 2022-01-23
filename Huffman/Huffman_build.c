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

// Huffman tree structure
typedef struct TNode
{
    int weight;
    struct TNode *left;
    struct TNode *right;
}* HFMTNode;

// Priority Queue, will be used as a min heap
typedef struct PriorityQueue{
    HFMTNode elements[MAX];
    int size; // default to 0
}Heap;

Word words[MAX];
Huffman encoded[MAX];
Heap minHeap;

void Insert(HFMTNode X) {
    // Insert into back of the heap
    minHeap.elements[++minHeap.size] = X;
    HFMTNode tmp;
    // Percolate Up, adjust structure
    for (int i = minHeap.size; minHeap.elements[i/2]->weight > minHeap.elements[i]->weight; i /= 2 ){
        // Swap two item
        tmp = minHeap.elements[i];
	    minHeap.elements[i] = minHeap.elements[i/2];
        minHeap.elements[i/2] = tmp;
    }
}

HFMTNode DeleteMin(){
    int child;
    HFMTNode tmp, min;
    min = minHeap.elements[1];
    // Replace the min item with last element
    minHeap.elements[1] = minHeap.elements[minHeap.size--];
    // Percolate Down, adjust structure
    for (int i = 1; i * 2 <= minHeap.size; i = child){
        child = i * 2; 
        if (child != minHeap.size && minHeap.elements[child+1]->weight < minHeap.elements[child]->weight){
            // Choose another child
            child++;
        }
        if ( minHeap.elements[i]->weight > minHeap.elements[child]->weight ){
            // Swap two items 
            tmp = minHeap.elements[i];
            minHeap.elements[i] = minHeap.elements[child]; 
            minHeap.elements[child] = tmp;
        }
        else break;   
    } 
    return min; 
}

void CreateMinHeap(int N){
    // Create one root node
    HFMTNode newNode = (HFMTNode)malloc(sizeof(struct TNode));
    // Init the root node
    newNode -> left = newNode -> right = NULL;
    newNode -> weight = 0;
    minHeap.elements[0] = newNode;
    // Keep doing insertions
    for (int i = 0; i < N; i++){
        newNode = (HFMTNode)malloc(sizeof(struct TNode));
        newNode -> left = newNode -> right = NULL;
        newNode -> weight = words[i].freq;
        Insert(newNode);
    }
}

void BuildHFM(int N){
    /* 
    Huffman's algorithm
    We need to operate N-1 times to merge the minHeap
    */
    for (int i = 1; i < N; i++){
        HFMTNode left, right;
        // Pop out two least elements
        left = DeleteMin();
        right = DeleteMin();
        // Treat the two nodes as one single node
        HFMTNode newNode = (HFMTNode)malloc(sizeof(struct TNode));
        // Link the left and right subtree
        newNode -> left = left;
        newNode -> right = right;
        newNode -> weight = left->weight + right -> weight;
        // Insert the new node and adjust its structure
        Insert(newNode);
    }
}

int GetCost(HFMTNode T, int depth){
    if (T->left == NULL && T -> right == NULL){
        /* Reaches a leaf node
        cost = freq(weight) * code_length(depth of the node)
        */
        return depth * T->weight;
    }else{
        // Accumulate the cost of its children
        return GetCost(T->left, depth + 1) + GetCost(T->right, depth + 1);
    }
}

int CheckPrefix(int N){
    char * code1, * code2;
    // Check if there exists a substring relationship between two codes
	for(int i = 0; i < N; i++){
		for(int j = i+1; j < N; j++){
            // Choose two codes and do a single check
            code1 = encoded[i].code;
            code2 = encoded[j].code;
            // Check if the leftmost n characters are the same
            if(strncmp(code1, code2, strlen(code2)) == 0){
                return 0;
            }
		}
	}
    return 1;
}

int main(int argc, char const *argv[])
{
    int N, M, opt_cost;
    scanf("%d\n", &N);
    // Init heap structure
    for(int i = 0; i < N; i++){
    	scanf("%c %d", &words[i].c, &words[i].freq);
        // Skip one space
        getchar();
	}
    // Insert N single node into heap
    CreateMinHeap(N);
    BuildHFM(N);
    opt_cost = GetCost(minHeap.elements[1], 0);
    // Check the student's submission
    scanf("%d\n", &M);
    for (int i = 0; i < M; i++){
        int cur_cost = 0;
        for (int j = 0; j < N; j++){
            scanf("%c %s", &encoded[j].c, encoded[j].code);
            // Skip a newline character
            getchar();
            // Update the cost of current coding given by the student
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