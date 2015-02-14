/* 
 * File:   main.cpp
 * Author: roman
 *
 * Created on February 10, 2015, 5:58 PM
 */

#include <cstdlib>
#include <iostream>
#include <time.h>
#include <math.h>
#include <pthread.h>
using namespace std;
const int n = 98;
int matrix[n][n];
const int numThreads = 3;
pthread_t threads[numThreads];

void poslidovno() {
    srand(time(NULL));
    for(int i=0; i<n; i++) {
        for(int j=0;j<n;j++) {
            matrix[i][j]=rand() % 10;
            cout << " " << matrix[i][j] << " ";
        }
        cout << endl;
    }
    for(int i=0; i<n;i++) {
        matrix[i][i]=0;
        for(int j=0; j<n;j++){
            if(j!=i){
                matrix[i][i]+=matrix[j][i]+matrix[i][j];
            }
        }
    }
    cout << endl;
    for(int i=0; i<n; i++) {
        for(int j=0;j<n;j++) {
            cout << " " << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

void *diagonal(void* idw) {
    int id = *((int*)(&idw));
    float koef = (n/numThreads);
    if(id==numThreads) koef++;
    for(int i=(id-1)*(n/numThreads); i<id*koef;i++) {
        matrix[i][i]=0;
        for(int j=0; j<n;j++){
            if(j!=i){
                matrix[i][i]+=matrix[j][i]+matrix[i][j];
            }
        }
    }
}

void multi() {
    srand(time(NULL));
    void* status;
    for(int i=0; i<n; i++) {
        for(int j=0;j<n;j++) {
            matrix[i][j]=rand() % 10;
        }
    }
    for(long i=0;i<numThreads;i++) {
        pthread_create(&threads[i],NULL,diagonal,(void *) i+1);
    }
    for(long i=0;i<numThreads;i++) {
        pthread_join(threads[i],&status);
    }
    cout << endl;
    for(int i=0; i<n; i++) {
        for(int j=0;j<n;j++) {
            cout << " " << matrix[i][j] << " ";
        }
        cout << endl;
    }
}
/*
 * 
 */
int main(int argc, char** argv) {
    clock_t start = clock();
    poslidovno();
    //multi();
    cout << "Time: " << float(clock()-start)/CLOCKS_PER_SEC;
    pthread_exit(NULL);
}

