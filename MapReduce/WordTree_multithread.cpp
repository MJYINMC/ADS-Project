#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>

#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <string>
#include <pthread.h>

using namespace std;

// multithreading related
#define CORES 8
#define HYPERTHREADING 2
#define NUM_THREADS CORES*HYPERTHREADING

// time related
clock_t start_time, finish_time;
double duration;

// Compare
bool cmp(const pair<string, int>& a, const pair<string, int>& b){
        return a.second < b.second;
}

// data structure related
typedef struct tree
{
    struct tree * next [26];
    char * v;
    int freq;
}* Tree;

void traverse(Tree t, map <string, int> & words){
    if(t->v){
        string * v = new string(t->v);
        words[*v] =  t->freq;
    }
    for (int i = 0; i < 26; i++)
    {
        if(t->next[i]) traverse(t->next[i], words);
    }
}

Tree insert(Tree t, char * v, int start){
    if(!t){
        t = (Tree)malloc(sizeof(struct tree));
        memset(t, 0, sizeof(struct tree));
    }
    // Next character is '/0'
    if(!v[start+1]){
        if(t -> freq == 0)
            t -> v = v;
        else
            free(v);
        t -> freq++;
    }else{
        t->next[v[start+1] - 'a'] = insert(t->next[v[start+1] - 'a'], v, start+1);
    }
    return t;
}

// Thread related
void * One_block(void * args){
    int loc;
    char word [1000];
    char * buf = (char *) args;
    stringstream ss(buf);
    Tree dummy = (Tree)malloc(sizeof(struct tree));
    memset(dummy, 0, sizeof(struct tree));
    map <string, int> * words =  new map<string, int>;
    while(ss >> word){
        loc = 0;
        char * s = (char *)malloc(strlen(word)+1);
        for (int count = 0; word[count]; count++){
            if(word[count] <= 'z' && word[count] >= 'a') s[loc++] = word[count];
            if(word[count] <= 'Z' && word[count] >= 'A') s[loc++] = word[count] + 32;
        }
        if(!loc){
            free(s);
            continue;
        };
        s[loc] = 0;
        dummy->next[s[0] - 'a'] = insert(dummy->next[s[0] - 'a'], s, 0);
    }
    traverse(dummy, * words);
    return (void*)words;
}

// entry
int main(void)
{
    start_time = clock();

    // Read from file
    FILE * fp  = fopen("input.txt","rb");
    fseek(fp, 0, SEEK_END);
    int length  = ftell(fp);
    char * buf = (char *)malloc(length+1);
	rewind(fp);
    fread(buf, 1, length, fp);
    fclose(fp);
    buf[length] = 0;

    // Separate into blocks
    pthread_t tids[NUM_THREADS];

    int block_length = length/(NUM_THREADS);
    map<string, int> * words_per_block [NUM_THREADS];
    int start = 0, end = block_length;
    for (int i = 0; i < NUM_THREADS; i++){
        while (buf[end] != ' ' && buf[end] != 0 && buf[end] != '\n' && buf[end] !='\r') end++;
        buf[end] = 0;
        pthread_create(&tids[i], NULL, One_block, buf+start);
        start = end + 1;
        end = min(start + block_length, length);
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(tids[i], (void **)(words_per_block+i));
    }
    
    // Merge
    map <string, int> words;
    map <string, int>::iterator iter;
    for (int i = 0; i < NUM_THREADS; i++){
        for(iter = words_per_block[i]->begin(); iter != words_per_block[i]->end(); iter++){
            words[iter->first] += iter->second;
        }        
    }
    // Sort
    vector<pair<string, int>> vec(words.begin(), words.end());
    sort(vec.begin(), vec.end(), cmp);

    // Write to file
    FILE * out  = fopen("output.txt","w");
    for (int i = 0; i < vec.size(); ++i)
        fprintf(fp,"%s %d\n",vec[i].first.c_str(), vec[i].second);
    fclose(out);
    
    finish_time = clock();
    duration = (double)(finish_time - start_time) / CLOCKS_PER_SEC;
    printf( "%f seconds\n", duration);
    return 0;
}