//
//  common.h
//  community
//
//  Created by lily on 2024/7/4.
//

#ifndef common_h
#define common_h

#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <queue>
#include <set>
#include <unordered_map>
#include <filesystem>

using namespace std;
#define NODENUM 4561085
#define NODE 4561084
#define USERNUM 4546532
#define USER 4546531
#define SPACE_CHAR ' '
#define TAB_CHAR '\t'

//#define NODENUM 16
//#define NODE 15
//#define USERNUM 8
//#define USER 7

//#define NODENUM 16
//#define NODE 15
//#define USERNUM 10
//#define USER 9



typedef struct anedge{
    int from;
    int to;
    anedge(int from, int to) : from(from), to(to){}
}anedge;


extern vector<vector<int>> neighbor;
extern vector<vector<int>> edge;
extern vector<int16_t> support;
extern int edgenum;


#endif /* common_h */
