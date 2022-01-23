#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <ctime>
#include <cstring>
#include <algorithm>

using namespace std;

// time related
clock_t start, finish;
double duration;
// word related
char word [1000];
int loc;

bool cmp(const pair<string, int>& a, const pair<string, int>& b) {
        if (a.second != b.second)
            return a.second > b.second;
        else
            return a.first < b.first;
}

int main()
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
    map<string, int> words;
    while(ss >> word){
        loc = 0;
        char * s = (char *)malloc(strlen(word)+1);
        for (int count = 0; word[count]; count++){
            if(word[count] <= 'z' && word[count] >= 'a') s[loc++] = word[count];
            if(word[count] <= 'Z' && word[count] >= 'A') s[loc++] = word[count] + 32;
        }
        if(!loc){free(s);continue;};
        s[loc] = 0;
        words[string(s)]++;
    }
    // Sort 
    vector<pair<string, int>> vec(words.begin(), words.end());
    sort(vec.begin(), vec.end(), cmp);

    // Write to file
    FILE * out  = fopen("output.txt","w");
    for (int i = 0; i < vec.size(); ++i)
        fprintf(fp,"%s %d\n",vec[i].first.c_str(), vec[i].second);
    fclose(out);

    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    printf( "%f seconds\n", duration);
    return 0;
}