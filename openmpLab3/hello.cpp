#include <mpi.h>
#include <iostream>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <cstdlib>
#include <stdio.h>
using namespace std;
const int domain_size = 10000;
int matrix[domain_size][domain_size];

int main(int argc, char** argv) {
    MPI_Init(NULL, NULL);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    if(world_rank==0) {
        srand(time(NULL));
        for(int i=0; i<domain_size; i++) {
            for(int j=0;j<domain_size;j++) {
                matrix[i][j]=rand() % 10;
                //cout << " " << matrix[i][j] << " ";
            }
            //cout << endl;
        }
        clock_t start = clock();
        for (int i = 1; i < world_size; i++) {
            MPI_Send(&matrix, domain_size*domain_size, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        int subdomain_start = domain_size / world_size * world_rank;
        int subdomain_size = domain_size / world_size;
        int subdomain_end = subdomain_start+subdomain_size;
        for(int i=subdomain_start; i<subdomain_end;i++) {
            matrix[i][i]=0;
            for(int j=0; j<domain_size;j++){
                if(j!=i){
                    matrix[i][i]+=matrix[j][i]+matrix[i][j];
                }
            }
        }
        int sub;
        for (int i = 1; i < world_size; i++) {
            MPI_Recv(&sub,1,MPI_INT, i, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            int buff[sub];
            MPI_Recv(&buff, sub, MPI_INT, i, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            for(int j = 0; j < sub; j++){
                matrix[subdomain_end*i+j][subdomain_end*i+j]=buff[j];
            }
        }
        cout << "Time: " << float(clock()-start)/CLOCKS_PER_SEC << endl;
        for(int i=0; i<domain_size; i++) {
            for(int j=0;j<domain_size;j++) {
                //cout << " " << matrix[i][j] << " ";
            }
            //cout << endl;
        }
    } else {
        MPI_Recv(&matrix, domain_size*domain_size, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        int subdomain_start = domain_size / world_size * world_rank;
        int subdomain_size = domain_size / world_size;
        if (world_rank == world_size - 1) {
            // Give remainder to last process
            subdomain_size += domain_size % world_size;
        }
        int subdomain_end = subdomain_start+subdomain_size;
        //int *buff = new int[subdomain_size];
        int buff[subdomain_size];
        int k = 0;
        for(int i=subdomain_start; i<subdomain_end;i++) {
            buff[k]=0;
            for(int j=0; j<domain_size;j++){
                if(j!=i){
                    buff[k]+=matrix[j][i]+matrix[i][j];
                }
            }
            k++;
            //cout << buff[i] << endl;
        }
        //cout << world_rank << ":" << subdomain_size << endl;
        MPI_Send(&subdomain_size,1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&buff, subdomain_size, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    
    
    MPI_Finalize();
    return 0;
}