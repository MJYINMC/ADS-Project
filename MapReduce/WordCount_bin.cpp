#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>

#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;
// time related
clock_t start, finish;
double duration;

// word related
int loc;
char word [1000];

// data structure related
typedef struct tree
{
    struct tree * left;
    struct tree * right;
    char * v;
    int freq;
}* Tree;

Tree root = NULL;

void traverse(Tree t, vector<pair<char *, int>> & vec){
    if(!t) return;
    vec.push_back(pair<char *, int>(t->v, t->freq));
    traverse(t->left, vec);
    traverse(t->right, vec);
}

bool cmp(const pair<char *, int>& a, const pair<char *, int>& b) {
        return a.second < b.second;
}

Tree insert(Tree t, char * v){
    if(!t){
        t = (Tree)malloc(sizeof(struct tree));
        t -> left = t -> right = NULL;
        t -> v = v;
        t -> freq = 1;
        return t;
    }
    int res = strcmp(v, t->v);
    if(res == 0){
        free(v);
        t -> freq++;
    }else if(res < 0){
        t->left = insert(t->left, v);
    }else if(res > 0){
        t->right = insert(t->right, v);
    }
    return t;
}

// entry
int main(void)
{
    start = clock();

    FILE * fp  = fopen("input.txt","rb");
    fseek(fp, 0, SEEK_END);
    int length  = ftell(fp);
    char * buf = (char *)malloc(length+1);
	rewind(fp);
    fread(buf, 1, length, fp);
    fclose(fp);

    buf[length] = 0;
    stringstream ss(buf);
    while(ss >> word){
        loc = 0;
        char * s = (char *)malloc(strlen(word)+1);
        for (int count = 0; word[count]; count++){
            if(word[count] <= 'z' && word[count] >= 'a') s[loc++] = word[count];
            if(word[count] <= 'Z' && word[count] >= 'A') s[loc++] = word[count] + 32;
        }
        if(!loc){free(s);continue;};
        s[loc] = 0;
        root = insert(root, s);
    }

    // Sort 
    vector<pair<char *, int>> vec;
    vec.reserve(30000);
    traverse(root, vec);
    sort(vec.begin(), vec.end(), cmp);
    
    // Write to file
    FILE * out  = fopen("output.txt","w");
    for (int i = 0; i < vec.size(); ++i)
        fprintf(fp,"%s %d\n",vec[i].first, vec[i].second);
    fclose(out);
    
    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    printf( "%f seconds\n", duration);
    return 0;
}