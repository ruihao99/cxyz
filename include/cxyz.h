// parsing xyzfiles
#include <stdio.h>
#include <stdlib.h>
#define _FILE_OFFSET_BITS 64

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <float.h>
#include <math.h>
using namespace std;

typedef float coordinate[3];
typedef char element[2];

// use c-loop get file position stream 
// below is the c++ getline version, bad!!
/*
int get_pos (ifstream & f_xyz, unsigned long long * ppos){
    int n = 0;
    long m = 0;
    int na = 1;
    string line;
    //string line;
    unsigned long long cur;

    cur = f_xyz.tellg();
    f_xyz.seekg(0, ios::beg);
    ppos[n] = f_xyz.tellg();
    na = get_n_atoms(f_xyz);

    while ( getline (f_xyz, line) ){
        m++;
        if (fmod(m, (na+2)) == 0){ 
            n++;
            ppos[n] = f_xyz.tellg();
        }
    }
    f_xyz.clear();
    f_xyz.seekg(cur, ios::beg);
    return n;
}
*/
// this is the c-stdio version 
// could be improved using a larger buffer?
static void _get_fpos( const char * file , unsigned long long * pos, int & n_atoms, int & n_frames){
    FILE *fxyz;
    int n=0;
    char line [256];
    //open file
    fxyz = fopen(file, "r");
    fscanf(fxyz, "%d", &n_atoms);
    rewind(fxyz);
    int nd = n_atoms+2;
    n_frames=0;

    while (!feof(fxyz)){
        pos[n_frames] = ftell(fxyz);
        for (n=0; n<nd; n++)
            fgets(line, 256, fxyz);
        n_frames++;
    }
    fclose(fxyz);
    n_frames -= 1;
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

