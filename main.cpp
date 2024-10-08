//
//  main.cpp
//  community
//
//  Created by lily on 2024/7/3.
//

#include "common.h"
#include "kquad.hpp"
#include <chrono>
#include <iostream>
#include <fstream>
using namespace std;
using namespace std::chrono;

vector<vector<int>> neighbor;
vector<vector<int>> edge;
vector<int16_t> support;
int edgenum;

int main(int argc, const char * argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <dataset> " << std::endl;
        return 1;
    }
    const string path = argv[1];
    std::cout << "Hello, World!\n";
    //string dataf = "/Users/lily/Desktop/c++/test/community/data/";//   ./data/
    string dataf = "/home/app/Experiment712/quad_bilibili/data/" + path +'/';
    //const char* dataFolder = dataf.c_str();
    // /home/app/Experiment712/quad_bilibili/data/bilibili4/bilibili4.txt
    string datapath = dataf + path + ".txt";
    const char* dataPath = datapath.c_str();
    string neipath = dataf + "neighbor.txt";
    const char* neiPath = neipath.c_str();
    string suppath = dataf + "support.txt";
    const char* supPath = suppath.c_str();
    string compath = dataf + "community/";
    const char* comPath = compath.c_str();
    string parentpath = dataf + "parent.txt";
    const char* parentPath = parentpath.c_str();
    string indexpath = dataf + "index.txt";
    const char* indexPath = indexpath.c_str();
    
    string pquerypath = dataf + "pquery.txt";
    const char* pqueryPath = pquerypath.c_str();
    string rquerypath = dataf + "rquery.txt";
    const char* rqueryPath = rquerypath.c_str();
    string pqueryresult = dataf + "pqueryresult.txt";
    const char* pqueryResult = pqueryresult.c_str();
    string rqueryresult = dataf + "rqueryresult.txt";
    const char* rqueryResult = rqueryresult.c_str();
    
    Kquad* q = new Kquad();

    
//    string line;
//    ifstream fin(supPath);
//    if (fin)
//    {
//        while (getline(fin, line))
//        {
//            istringstream iss(line);
//            string temp;
//            vector<int> tup;
//            while (getline(iss, temp, ' ')) {
//                tup.push_back(stoi(temp));
//            }
//            vector<int> e;
//            e.push_back(tup[0]);
//            e.push_back(tup[1]);
//            edge.push_back(e);
//            support.push_back(tup[2]);
//            neighbor[tup[0]].push_back(tup[1]);
//            neighbor[tup[1]].push_back(tup[0]);
//        }
//    }
//    cout<<edge.size()<<endl;
//    cout<<support.size()<<endl;
//    fin.clear();
//    fin.close();
//    for(int i = 0; i < neighbor.size(); i++){
//        q->quick_list(neighbor[i], neighbor[i].size());
//    }
    auto start = high_resolution_clock::now(); 

    q->computesupport(dataPath, supPath, neiPath);
    q->compute1quad(dataPath, supPath, comPath);
    q->computekquad(comPath, parentPath);
    q->buildindex(comPath, indexPath);
    
    auto end = chrono::high_resolution_clock::now(); 
    auto duration = chrono::duration_cast<chrono::seconds>(end - start); // 计算运行时间
    cout << "make_index_Duration: " << duration.count() << " seconds" << endl;

    q->pointquery(indexPath, pqueryPath, pqueryResult);
    
    return 0;
}
