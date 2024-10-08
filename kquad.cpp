//
//  kquad.cpp
//  community
//
//  Created by lily on 2024/7/4.
//

#include "kquad.hpp"
#include <chrono>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;
using namespace std::chrono;


///---------------------------------------------------------------------------------------------------
bool Kquad::cmpElement(vector<int> &a, vector<int> &b) {///b大于等于a，返回true
    int i;
    bool sign = true;//0表示a大，1表示b大
    for (i = 0; i < a.size(); i++) {
        //cout << dim1[dimOrder[i]] << "  " << dim2[dimOrder[i]] << endl;
        if (a[i] < b[i]) {
            sign = true;
            break;
        }
        if (a[i] > b[i]) {
            sign = false;
            break;
        }
    }
    return sign;
}
///----------------------------------------------------------------------------------------------------
int Kquad::partitions(vector<vector<int>> &arr, int left, int right) {
    swap(arr[left], arr[(left + right) / 2]);
    //cout << "left = " << left << " , right = " << right << endl;
    vector<int> pivot = arr[left];
    int tmp;
    while (left < right) {
        while (left < right && cmpElement(pivot, arr[right]))//arr[right] > pivot
            right--;
        if (left < right) {
            arr[left++] = arr[right];
        }
        while (left < right && cmpElement(arr[left], pivot))//arr[left] < pivot
            left++;
        if (left < right) {
            arr[right--] = arr[left];
        }
    }
    arr[left] = pivot;
    return left;
}
///-----------------------------------------------------------------------------------------------------
void Kquad::quick_sort(vector<vector<int>> &arr, int left, int right) {
    if (right - left <= 0) {
        return;
    }
    int pivot;
    pivot = partitions(arr, left, right);
    // leave the pivot where it was
    quick_sort(arr, left, pivot - 1);
    quick_sort(arr, pivot + 1, right);
    return;
}
///-----------------------------------------------------------------------------------------------------
void Kquad::quick(vector<vector<int>> &arr, int length) {
    quick_sort(arr, 0, length - 1);
}
///----------------------------------------------------------------------------------------------------
int Kquad::partitions_list(vector<int> &arr, int left, int right) {
    swap(arr[left], arr[(left + right) / 2]);
    int pivot = arr[left];
    int tmp;
    while (left < right) {
        while (left < right && pivot < arr[right])//arr[right] > pivot
            right--;
        if (left < right) {
            arr[left++] = arr[right];
        }
        while (left < right && arr[left] < pivot)//arr[left] < pivot
            left++;
        if (left < right) {
            arr[right--] = arr[left];
        }
    }
    arr[left] = pivot;
    return left;
}
///---------------------------------------------------------------------------------------
void Kquad::quick_sort_list(vector<int> &arr, int left, int right) {
    if (right - left <= 0) {
        return;
    }
    int pivot;
    pivot = partitions_list(arr, left, right);
    // leave the pivot where it was
    quick_sort_list(arr, left, pivot - 1);
    quick_sort_list(arr, pivot + 1, right);
    return;
}
///---------------------------------------------------------------------------------------
void Kquad::quick_list(vector<int> &arr, int length) {
    quick_sort_list(arr, 0, length - 1);
}
///---------------------------------------------------------------------------------------
int Kquad::intersaction(vector<int> &a, vector<int> &b, int &s) {
    int i = 0, j = 0;
    while(true){
        if(a[i]==b[j]) {
            s++; i++; j++;
        }
        else{
            if(a[i] < b[j]) i++;
            else j++;
        }
        if(i>=a.size() || j>=b.size()) break;
    }
    s--;
    return s;
}
///---------------------------------------------------------------------------------------
int Kquad::computesupport(const char* datapath, const char* suppath, const char* neipath) {
    int i, j, g;
    string line;
    ifstream fin(datapath);
    if (fin)
    {
        while (getline(fin, line))
        {
            istringstream iss(line);
            string temp;
            vector<int> tup;
            while (getline(iss, temp, ' ')) {
                tup.push_back(stoi(temp));
            }
            if(tup.size()>2) {cout<<"abnormal"<<endl;return 0;}
            if(tup.size()==1) break;
            if(tup[0]<USERNUM && tup[1]<USERNUM) continue;
            if(tup[0]>=USERNUM && tup[1]>=USERNUM) continue;
            if(tup[0] > tup[1]) {int a = tup[0]; tup[0] = tup[1]; tup[1] = a;}
            neighbor[tup[0]].push_back(tup[1]);
            neighbor[tup[1]].push_back(tup[0]);
            edge.push_back(tup);
        }
    }
    fin.clear();
    fin.close();
    edgenum = edge.size();
    
    //sort the neighgbors
    ofstream fout(neipath, ios::out);
    for(i = 0; i < neighbor.size(); i++){
        quick_list(neighbor[i], neighbor[i].size());
        string out;// = to_string(i) + ' ';
        for(j = 0; j < neighbor[i].size(); j++)
            out = out + to_string(neighbor[i][j]) + ' ' ;
        fout<<out<<endl;
    }
    fout.clear();
    fout.close();
    cout<<"sort completes!"<<endl;
    
    //support
    support.resize(edgenum,0);
    ofstream fout1(suppath, ios::out);
    for(i = 0; i < edge.size(); i++){
        if(i%100000==0) cout<<"line "<<i<<endl;
        int sup = 0;
        for(j = 0; j < neighbor[edge[i][0]].size(); j++){//user's neighbors
            int video = neighbor[edge[i][0]][j];
            if(video==edge[i][1]) continue;
            intersaction(neighbor[edge[i][1]], neighbor[video], sup);
        }
        support[i] = sup;
        
        string ss = to_string(edge[i][0]) + " " + to_string(edge[i][1]) + " " + to_string(support[i]);
        fout1<< ss <<endl;
    }
    fout1.clear();
    fout1.close();
    cout<<"support complete"<<endl;
    return 0;
}
///---------------------------------------------------------------------------------------
int Kquad::compute1quad(const char* datapath, const char* suppath, const char* compath) {
    int i;
    vector<vector<int>> newedge;//all the edges whose support is at least 1
    for(i = 0; i < edge.size(); i++){
        if(support[i] >= 1)
            newedge.push_back(edge[i]);
    }
    vector<int> rootlevel;
    tree.push_back(rootlevel);

    //get connected subgraphs
    int level = 1, parentid = -1;
    vector<int> onelevelquad;
    connectedsubgraph(compath, newedge, onelevelquad, level, parentid);
    tree.push_back(onelevelquad);
    cout<<"1-quad end!"<<endl;
    return 0;
}

///---------------------------------------------------------------------------------------
int Kquad::connectedsubgraph(const char* compath, vector<vector<int>> &kedge, vector<int> &newlevel, int level, int parentid) {
    int i, j;
    //1) find the neighbors
    vector<vector<int>> neighbor1(NODENUM);
    for(i = 0; i < kedge.size(); i++){
        neighbor1[kedge[i][0]].push_back(kedge[i][1]);
        neighbor1[kedge[i][1]].push_back(kedge[i][0]);
    }
    //2) union the neighbors to get the connected nodes
    queue<int> que;// the node list to find the neighbors
    vector<bool> nodebitmap(NODENUM, false);
    vector<bool> edgebitmap(kedge.size(), false);
    int enum_edgenum = 0;
    vector<int> nodeset;
    vector<int> edgeset;
    while(true){
        vector <int>().swap(nodeset);
        vector <int>().swap(edgeset);
        bool sign = true;
        //select a start edge to find the connected subgraph
        for(i = 0; i < kedge.size(); i++){
            if(edgebitmap[i]==false) {
                sign = false;
                que.push(kedge[i][0]); que.push(kedge[i][1]);
                nodebitmap[kedge[i][0]] = true;
                nodebitmap[kedge[i][1]] = true;
                nodeset.push_back(kedge[i][0]);
                nodeset.push_back(kedge[i][1]);
                edgebitmap[i] = true;
                enum_edgenum++;
                break;
            }
        }
        if(sign==true || nodeset.size()==0) break;
        
        //find the node set of the connected subgraph
        while(que.size()>0){
            int v = que.front();
            for(i = 0; i < neighbor1[v].size(); i++){
                if(nodebitmap[neighbor1[v][i]]==false) {
                    nodeset.push_back(neighbor1[v][i]);
                    que.push(neighbor1[v][i]);
                    nodebitmap[neighbor1[v][i]] = true;
                }
            }
            que.pop();
        }
        int max = (level/2 > 4) ? level/2 : 4;
        if(nodeset.size() < max) continue;
        
        //find the edge set of the connected subgraph
        for(i = 0; i < nodeset.size(); i++){
            if(nodeset[i] >= USERNUM) continue;
            for(j = 0; j < neighbor1[nodeset[i]].size(); j++){
                edgeset.push_back(nodeset[i]);
                edgeset.push_back(neighbor1[nodeset[i]][j]);
            }
        }
        for(i = 0; i < kedge.size(); i++){
            if(nodebitmap[kedge[i][0]]==true) {//***add edge[i][0] is true for other k
                if(edgebitmap[i]==false) {
                    edgebitmap[i] = true;
                    enum_edgenum++;
                }
            }
        }
        cout<<"community "<<quadNo<<": node "<<nodeset.size()<<", edge "<<edgeset.size()/2<<endl;
        //output
        string comfolder = compath;
        string communitypath = comfolder + to_string(level)+ "-" +to_string(quadNo) + ".txt";
        const char* communityPath = communitypath.c_str();
        ofstream fout(communityPath, ios::out);
        string out = to_string(level) + " " + to_string(quadNo) + " " + to_string(parentid) + " " + to_string(edgeset.size()/2) + " " + to_string(nodeset.size());
        fout<< out <<endl;
        vector<int> v ;
        v.push_back(quadNo); v.push_back(level); v.push_back(parentid);
        parent.push_back(v);
        out = "";
        for(i = 0; i < edgeset.size()/2; i++){
            out = to_string(edgeset[2*i]) + " " + to_string(edgeset[2*i+1]);
            fout<<out<<endl;
        }
        out = "";
        quick_list(nodeset, nodeset.size());
        for(i = 0; i < nodeset.size(); i++){
            out = to_string(nodeset[i]);
            fout<<out<<endl;
        }
        fout.clear();
        fout.close();
        newlevel.push_back(quadNo);
        quadNo++;
        if(enum_edgenum>=kedge.size()) break;
    }
    vector <int>().swap(nodeset);
    vector <int>().swap(edgeset);
    vector <vector<int>>().swap(neighbor1);
    return 0;
}
///---------------------------------------------------------------------------------------
int Kquad::readcommunity(const char* compath, int level, int id, vector<vector<int>> &edgelist) {
    string comfolder = compath;
    string communitypath = comfolder + to_string(level)+ "-" +to_string(id) + ".txt";
    const char* communityPath = communitypath.c_str();
    string line;
    int lineno = 0, edgenumber = 0;
    ifstream fin(communityPath);
    if (fin)
    {
        while (getline(fin, line))
        {
            istringstream iss(line);
            string temp;
            vector<int> tup;
            while (getline(iss, temp, ' ')) {
                tup.push_back(stoi(temp));
            }
            if(lineno==0) edgenumber = tup[3];
            else{
                edgelist.push_back(tup);
            }
            if(lineno++ == edgenumber) break;
        }
    }
    fin.clear();
    fin.close();
    
    return 0;
}
///---------------------------------------------------------------------------------------
int Kquad::copyquad(const char* compath, int oldlevel, int oldid, int newlevel, int newid) {
    string comfolder = compath;
    string oldpath = comfolder + to_string(oldlevel)+ "-" +to_string(oldid) + ".txt";
    const char* oldPath = oldpath.c_str();
    string newpath = comfolder + to_string(newlevel)+ "-" +to_string(newid) + ".txt";
    const char* newPath = newpath.c_str();
    ofstream fout(newPath, ios::out);
    string out;
    ifstream fin(oldPath);
    string line;
    int lnum = 0;
    int edgesetsize, nodesetsize;
    if (fin)
    {
        while (getline(fin, line))
        {
            if(lnum++==0) {
                istringstream iss(line);
                string temp;
                vector<int> tup;
                while (getline(iss, temp, ' ')) {
                    tup.push_back(stoi(temp));
                }
                out = to_string(newlevel) + " " + to_string(newid) + " " + to_string(oldid) + " " + to_string(tup[3]) + " " + to_string(tup[4]);
                fout<<out<<endl;
            }
            else
                fout<<line<<endl;
        }
    }
    fin.clear();
    fin.close();
    fout.clear();
    fout.close();
    return 0;
}
///---------------------------------------------------------------------------------------
int Kquad::computekquad(const char* compath, const char* parentpath) {
    int i, j, g, k;
    string out;
    int totaldeledgenum = 0;
    vector<int16_t> support1 = support;
    vector<bool> edgeexist(edge.size(), true);//if an edge is still in k-quad (undeleted)
    for(i = 0; i < edge.size(); i++){
        if (support[i]==0){
            edgeexist[i] = false;
            totaldeledgenum++;
        }
    }
    unordered_map<string, int> map;//map each edge to its id
    for(i = 0; i < edge.size(); i++){
        out = to_string(edge[i][0]) + " " + to_string(edge[i][1]);
        map.insert(make_pair(out, i));
    }
    queue<vector<int>> splitquad;//store the k-quad to be decomposed to (k+1)-quad, vector<int> stores the quad level 'k' and the quad id
    for(i = 0; i < tree[1].size(); i++){//1-quad
        vector<int> v = {1,tree[1][i]};
        splitquad.push(v);
    }
    vector<int> newlevelquad;//store the obtained new (k+1)-quads
    vector<vector<int>> quadedge;
    vector<vector<int>> neighbor1(NODENUM);
    int splitnum = 0;
    
    //decompose the quads in splitqaud to get the nested communities
    while(true){//pop splitquad
        int quadlevel = splitquad.front()[0];
        int quadid = splitquad.front()[1];//the current kquad that should be splited
        //the edges of the current kquad
        vector<vector<int>>().swap(quadedge);
        neighbor1.clear();
        neighbor1.resize(NODENUM);
        
        readcommunity(compath, quadlevel, quadid, quadedge);
        if(quadedge.size()==0) {
            if(splitquad.size()==0) break;
            else{splitquad.pop(); continue;}
        }
        //find the neighbors
        for(i = 0; i < quadedge.size(); i++){
            neighbor1[quadedge[i][0]].push_back(quadedge[i][1]);
            neighbor1[quadedge[i][1]].push_back(quadedge[i][0]);
        }
        for(i = 0; i < neighbor1.size(); i++){
            quick_list(neighbor1[i], neighbor1[i].size());
        }
        
        //find edge whose support is less than k and delete
        queue<anedge> deleteedge;
        int eid;
        for(i = 0; i < quadedge.size(); i++){
            eid = map.at(to_string(quadedge[i][0])+" "+to_string(quadedge[i][1]));
            if(support1[eid] < quadlevel+1) {//delete the edge
                edgeexist[eid] = false;
                deleteedge.push(anedge(edge[eid][0], edge[eid][1]));
                totaldeledgenum++;
            }
        }
        if(deleteedge.size()==0) {//k+1-quad is complete k-quad
            if(quadedge.size()==0) {splitquad.pop();continue;}
            copyquad(compath, quadlevel, quadid, quadlevel+1, quadNo);
            vector<int> v = {quadNo, quadlevel+1, quadid};//quad, level, parent
            parent.push_back(v);
            splitquad.pop();
            newlevelquad.push_back(quadNo);
            quadNo++;
            continue;
        }
        
        //when minus support, each rectangle must be minus once
        int rectangleid = 0;
        unordered_map<string, int> rectangle;
        //delete edges whose support is less than k+1, i.e.(quadlevel+1)
        while(true){//per delete edge
            //delete the edges and change the support of its quad edges
            if(deleteedge.size()==0) break;
            anedge a = deleteedge.front();
            if(support1[map.at(to_string(a.from)+" "+to_string(a.to))]==0) {//the edge is already deleted
                deleteedge.pop();
                continue;
            }
            //minus the support of the edges that compose rectangle with the current edge
            for(j = 0; j < neighbor1[a.from].size(); j++){//user's video neighbors
                int video = neighbor1[a.from][j];//a video neighbor
                if(video==a.to) continue;
                i = 0, g = 0;
                while(i < neighbor1[a.to].size() && g < neighbor1[video].size()){//find common user neighbors
                    if(neighbor1[a.to][i]==neighbor1[video][g]) {
                        int commonuser = neighbor1[a.to][i];
                        if(commonuser==a.from) {
                            i++;g++;
                            continue;
                        }
                        vector<int> v = {a.from, commonuser, a.to, video};
                        quick_list(v, v.size());
                        string s = "";
                        for(k = 0; k < v.size(); k++)
                            s = s + to_string(v[k]) + " ";
                        if(rectangle.find(s)!=rectangle.end()) {i++;g++;continue;}//already minus
                        rectangle.insert(make_pair(s, rectangleid++));
                        int eid0 = map.at(to_string(a.from)+" "+to_string(a.to));
                        int eid1 = map.at(to_string(a.from)+" "+to_string(video));
                        int eid2 = map.at(to_string(commonuser)+" "+to_string(video));
                        int eid3 = map.at(to_string(commonuser)+" "+to_string(a.to));
                        support1[eid0] = 0;
                        if(--support1[eid1] < quadlevel+1 && edgeexist[eid1]==true) {
                            deleteedge.push(anedge(edge[eid1][0], edge[eid1][1]));
                            edgeexist[eid1] = false;
                            totaldeledgenum++;
                        }
                        if(--support1[eid2] < quadlevel+1 && edgeexist[eid2]==true) {
                            deleteedge.push(anedge(edge[eid2][0], edge[eid2][1]));
                            edgeexist[eid2] = false;
                            totaldeledgenum++;
                        }
                        if(--support1[eid3] < quadlevel+1 && edgeexist[eid3]==true) {
                            deleteedge.push(anedge(edge[eid3][0], edge[eid3][1]));
                            edgeexist[eid3] = false;
                            totaldeledgenum++;
                        }
                        i++; g++;
                    }
                    else{
                        if(neighbor1[a.to][i] < neighbor1[video][g]) i++;
                        else g++;
                    }
                }
            }
            deleteedge.pop();
        }
        
        //find connected graphs
        splitquad.pop();
        vector<vector<int>> newquadedge;
        for(i = 0; i < quadedge.size(); i++){
            int eid1 = map.at(to_string(quadedge[i][0])+" "+to_string(quadedge[i][1]));
            if(edgeexist[eid1] == true) newquadedge.push_back(quadedge[i]);
        }
        vector<int> localnewquad;
        if(newquadedge.size()!=0)
            connectedsubgraph(compath, newquadedge, localnewquad, quadlevel+1, quadid);
        for(i = 0; i < localnewquad.size(); i++){
            vector<int> v = {quadlevel+1, localnewquad[i]};
            splitquad.push(v);
        }
        //cout<<"quadid = "<<quadid<<", newquad size = "<<localnewquad.size()<<", delete edge = "<<totaldeledgenum<<endl;
        localnewquad.clear();
        
        //break the loop
        if(totaldeledgenum>=edge.size()) {
            cout<<totaldeledgenum<<" "<<edge.size()<<endl;
            break;
        }
        if(splitquad.size()==0) {
            break;
        }
        
    }
    vector<vector<int>>().swap(quadedge);
    vector<vector<int>>().swap(neighbor1);
    
    ofstream fout(parentpath, ios::out);
    for(i = 0; i < parent.size(); i++)
        fout<< to_string(parent[i][0]) + " " + to_string(parent[i][1]) + " " + to_string(parent[i][2])<<endl;
    fout.clear();
    fout.close();
    
    return 0;
}
///---------------------------------------------------------------------------------------
int Kquad::buildindex(const char* compath, const char* indexpath) {
    int i, j;
    string line;
    filesystem::path folderPath(compath);
    string folder = compath;
    vector<string> filename;
    vector<vector<int>> commu;
    for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            filename.push_back(entry.path().filename().string());
            i = filename[filename.size()-1].find("-");
            j = filename[filename.size()-1].find(".");
            string str1 = filename[filename.size()-1].substr(i+1, j-i-1);
            string str = filename[filename.size()-1].substr(0,i);
            vector<int> v = {stoi(str), stoi(str1)};//level, community ID
            commu.push_back(v);
        }
    }
    quick(commu, commu.size());
    vector<vector<int>> index(NODENUM);//-2 represents the node belongs to no community
    vector<int> level(NODENUM, -1);
    
    for(i = commu.size()-1; i >= 0; i--){
        string path = folder + to_string(commu[i][0]) + "-" + to_string(commu[i][1]) + ".txt";
        const char* Path = path.c_str();
        ifstream fin(Path);
        if (fin)
        {
            while (getline(fin, line))
            {
                istringstream iss(line);
                string temp;
                vector<int> tup;
                while (getline(iss, temp, ' ')) {
                    tup.push_back(stoi(temp));
                }
                if(tup.size()>1) continue;
                //if(level[tup[0]] == -1){
                    index[tup[0]].push_back(commu[i][0]);//level
                    index[tup[0]].push_back(commu[i][1]);//com ID
                    level[tup[0]] = commu[i][0];
                //}
            }
        }
        fin.clear();
        fin.close();
    }
    ofstream fout(indexpath, ios::out);
    for(i = 0; i < NODENUM; i++){
        line = to_string(i) + " " ;
        for(j = 0; j < index[i].size()/2; j++){
            line = line + to_string(index[i][2*j]) + " " + to_string(index[i][2*j+1]) + " ";
        }
        fout<< line <<endl;
    }
    fout.clear();
    fout.close();    
    
    return 0;
}
///---------------------------------------------------------------------------------------
int Kquad::readindex(const char* indexpath, vector<vector<int>> &index) {
    int i;
    string line;
    index.resize(NODENUM);
    ifstream fin(indexpath);
    if (fin)
    {
        while (getline(fin, line))
        {
            istringstream iss(line);
            string temp;
            vector<int> tup;
            while (getline(iss, temp, ' ')) {
                tup.push_back(stoi(temp));
            }
            for(i = 1; i < tup.size(); i++)
                index[tup[0]].push_back(tup[i]);
        }
    }
    fin.clear();
    fin.close();
    
    return 0;
}
///---------------------------------------------------------------------------------------
int Kquad::readrangequery(const char* querypath, vector<vector<int>> &query) {
    string line;
    ifstream fin(querypath);
    if (fin)
    {
        while (getline(fin, line))
        {
            istringstream iss(line);
            string temp;
            vector<int> tup;
            while (getline(iss, temp, ' ')) {
                tup.push_back(stoi(temp));
            }
            query.push_back(tup);
        }
    }
    fin.clear();
    fin.close();
    
    return 0;
}
///---------------------------------------------------------------------------------------
int Kquad::readpointquery(const char* querypath, vector<int> &query) {
    string line;
    ifstream fin(querypath);
    if (fin)
    {
        while (getline(fin, line))
        {
            istringstream iss(line);
            query.push_back(stoi(line));
        }
    }
    fin.clear();
    fin.close();
    
    return 0;
}
///---------------------------------------------------------------------------------------
int Kquad::pointquery(const char* indexpath, const char* querypath, const char* pointresult) {
    int i,j;
    vector<vector<int>> index;
    vector<int> query;
    readindex(indexpath, index);
    readpointquery(querypath, query);
    ofstream fout(pointresult, ios::out);

    auto start_time = high_resolution_clock::now();
    auto total_start_time = start_time;

    int result;
    for(i = 0; i < query.size(); i++){
        if(index[query[i]].size() > 1){
            //level, com ID
            fout << to_string(index[query[i]][0]) + " " + to_string(index[query[i]][1])<<endl;
//            cout << to_string(index[query[i]][0]) + " " + to_string(index[query[i]][1])<<endl;
            
        }
        else fout<< to_string(-1) <<endl;
        if ((i + 1) % 100 == 0) {
            auto end_time = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(end_time - start_time);
            cout << "Processed " << (i + 1) << " queries in "
                << fixed << setprecision(3) << duration.count() / 1000.0 << " milliseconds" << endl;
            start_time = high_resolution_clock::now();
        }
    }
    
    auto total_end_time = high_resolution_clock::now();
    auto total_duration = duration_cast<microseconds>(total_end_time - total_start_time);
    cout << "Total query time for " << query.size() << " queries: "
        << fixed << setprecision(3) << total_duration.count() / 1000.0 << " milliseconds" << endl;
    fout.clear();
    fout.close();
    
    return 0;
}
///---------------------------------------------------------------------------------------
int Kquad::rangequery(const char* indexpath, const char* querypath, const char* rangeresult) {
    int i,j;
    vector<vector<int>> index;
    vector<vector<int>> query;
    readindex(indexpath, index);
    readrangequery(querypath, query);
//    ofstream fout(rangeresult, ios::out);
//    int result;
//    for(i = 0; i < query.size(); i++){
//        if(index[query[i]].size() > 1){
//            //level, com ID
//            fout << to_string(index[query[i]][0]) + " " + to_string(index[query[i]][1])<<endl;
////            cout << to_string(index[query[i]][0]) + " " + to_string(index[query[i]][1])<<endl;
//            
//        }
//        else fout<< to_string(-1) <<endl;
//    }
//    fout.clear();
//    fout.close();
    
    return 0;
}
