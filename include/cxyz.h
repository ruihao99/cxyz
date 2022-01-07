// parsing xyzfiles
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <float.h>
using namespace std;

typedef float coordinate[3];
typedef char element[2];

// from file stream to n_atom
int get_n_atoms (ifstream & f_xyz){
    int n_atoms;
    string line;
    unsigned long long cur;

    cur = f_xyz.tellg();
    f_xyz.seekg(0, ios::beg);

    getline (f_xyz, line);
    stringstream(line) >> n_atoms;

    f_xyz.seekg(cur, ios::beg);
    return n_atoms;
}

// use c-loop get file position stream 
int get_pos (ifstream & f_xyz, unsigned long long * ppos){
    int n = 0;
    int m = 0;
    int na = 1;
    char buf[200]={0};
    //string line;
    unsigned long long cur;

    cur = f_xyz.tellg();
    f_xyz.seekg(0, ios::beg);

    na = get_n_atoms(f_xyz);
    ppos[n] = f_xyz.tellg();

    //while ( getline (f_xyz, line) ){
    while ( f_xyz.getline(buf, sizeof(buf)) ){
        m++;
        if (m == na+2){
            m = 0;
            n++;
            ppos[n] = f_xyz.tellg();
        }
    }
    f_xyz.clear();
    f_xyz.seekg(cur, ios::beg);
    return n;
}

static void _get_coords (string file, const unsigned long long & pos, coordinate * coords,  int & n_atoms){
    int i=0;
    string line;
    string elems;

    ifstream f_xyz(file);
    f_xyz.seekg(pos, ios::beg);
    getline(f_xyz, line);
    getline(f_xyz, line);

    for (i=0; i<n_atoms; i++){
        getline(f_xyz, line);
        stringstream ss (line);
        ss >> elems >> coords[i][0] >> coords[i][1] >> coords[i][2];
    } 
    f_xyz.close();
}

static void _get_fpos( string file , unsigned long long * pos, int & n_atoms, int & n_frames){
    ifstream f_xyz(file);
    n_atoms = get_n_atoms(f_xyz);
    n_frames = get_pos(f_xyz, pos);
    f_xyz.close();
}
