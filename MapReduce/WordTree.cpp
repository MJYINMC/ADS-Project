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

// Compare
bool cmp(const pair<char *, int>& a, const pair<char *, int>& b){
        return a.second < b.second;
}

// data structure related
typedef struct tree
{
    struct tree * next [26];
    char * v;
    int freq;
}* Tree;


void traverse(Tree t, vector<pair<char *, int>> & vec){
    if(t->v) vec.push_back(pair<char *, int>(t->v, t->freq));
    for (int i = 0; i < 26; i++)
    {
        if(t->next[i]) traverse(t->next[i], vec);
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

    Tree dummy = (Tree)malloc(sizeof(struct tree));
    memset(dummy, 0, sizeof(struct tree));

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
        dummy->next[s[0] - 'a'] = insert(dummy->next[s[0] - 'a'], s, 0);
    }

    // Sort 
    vector<pair<char *, int>> vec;
    vec.reserve(30000);
    traverse(dummy, vec);
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