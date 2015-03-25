/* 
 * File:   main.cpp
 * Author: roman
 *
 * Created on March 23, 2015, 11:00 AM
 */

#include <cstdlib>
#include <iostream>
#include <omp.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
using namespace std;
const int n = 15000;
int matrix[n][n];

void openmp(){
    for(int i=0; i<n; i++) {
        for(int j=0;j<n;j++) {
            matrix[i][j]=rand() % 10;
            //cout << " " << matrix[i][j] << " ";
        }
        //cout << endl;
    }
    clock_t start = clock();
    #pragma omp parallel for
    for(int i=0; i<n;i++) {
        //printf("%d\n", omp_get_thread_num());
        //cout << i << endl;
        matrix[i][i]=0;
        for(int j=0; j<n;j++){
            if(j!=i){
                matrix[i][i]+=matrix[j][i]+matrix[i][j];
            }
        }
    }
    cout << "Time: " << float(clock()-start)/CLOCKS_PER_SEC;
    
    /*
    for(int i=0; i<n; i++) {
        for(int j=0;j<n;j++) {
            cout << " " << matrix[i][j] << " ";
        }
        cout << endl;
    }*/
}

int main(int argc, char** argv) {
    srand(time(NULL));
    openmp();
    cout << endl;
    return 0;
}


