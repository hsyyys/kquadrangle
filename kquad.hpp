//
//  kquad.hpp
//  community
//
//  Created by lily on 2024/7/4.
//

#ifndef kquad_hpp
#define kquad_hpp


#include "common.h"


class Kquad
{
public:
    int quadNo = 0;
    vector<vector<int>> tree;
    vector<vector<int>> parent;
    
    Kquad(){
        neighbor.resize(NODENUM);
    }
    
    bool cmpElement(vector<int> &a, vector<int> &b);
    int partitions(vector<vector<int>> &arr, int left, int right);
    void quick_sort(vector<vector<int>> &arr, int left, int right);
    void quick(vector<vector<int>> &arr, int length);
    
    int partitions_list(vector<int> &arr, int left, int right);
    void quick_sort_list(vector<int> &arr, int left, int right);
    void quick_list(vector<int> &arr, int length);
    int intersaction(vector<int> &a, vector<int> &b, int &s);
    int readcommunity(const char* compath, int level, int id, vector<vector<int>> &edgelist);
    int copyquad(const char* compath, int oldlevel, int oldid, int newlevel, int newid);
    
    int computesupport(const char* datapath, const char* suppath, const char* neipath);
    int connectedsubgraph(const char* compath, vector<vector<int>> &kedge, vector<int> &curlevel, int level, int parentid);
    int compute1quad(const char* datapath, const char* suppath, const char* compath);
    int computekquad(const char* compath, const char* parentpath);
    
    int buildindex(const char* compath, const char* indexpath);
    int readindex(const char* indexpath, vector<vector<int>> &index);
    int readrangequery(const char* querypath, vector<vector<int>> &query);
    int readpointquery(const char* querypath, vector<int> &query);
    int pointquery(const char* indexpath, const char* querypath, const char* pointresult);
    int rangequery(const char* indexpath, const char* querypath, const char* rangeresult);
    
};
    
    
    

#endif /* kquad_hpp */

