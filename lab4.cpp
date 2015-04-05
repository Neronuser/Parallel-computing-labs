//
//  main.cpp
//  parallelLab4
//
//  Created by Roman on 4/4/15.
//  Copyright (c) 2015 Roman. All rights reserved.
//

#include <iostream>
#include <cmath>
#include <mpi.h>
using namespace std;

const int n=4;
//double matrix[n][n] = {1.0,-6.0,8.0,10.0,9.0,-3.0,1.0,4.0,1.0};
//double matrix[n][n] = {3,-5,2,9,2,1,4,-1,3,-1,6,7,-1,-4,5,5,9,3,6,-1,2,-1,5,-1,8};
//double matrix[n][n] = {1,1,1,0,2,5,2,5,-1};
double matrix[n][n] = {1,1,1,1,5,-3,2,-8,3,5,1,4,4,2,3,1};
//double f[n] = {23.0,-5.0,-1.0};
//double f[n] = {-7,10,-18,-5,12};
//double f[n] = {6,-4,27};
double f[n] = {0,1,0,3};
int key[n];
double c;
int l,k,i,j,d_size,d_ost;

int main(int argc, const char * argv[]){
    MPI_Init(NULL, NULL);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    if(world_rank==0) {

//    for(i=0;i<n;i++){
//        for(j=0;j<n;j++) cout << matrix[i][j] << " ";
//        cout << "| " << f[i] << endl;
//    }
        for(i = 0; i<n; i++){
        // виключення в поточному рядку
            for(k = 0; k < i; k++){
                c = matrix[i][key[k]];
                for(j = 0; j < n; j++){
                    matrix[i][j]=matrix[i][j]-c*matrix[k][j];
                }
                f[i]=f[i]-c*f[k];
            }
        // нормування поточного рядка
            c=matrix[i][i];
            key[i]=i;
            for(j=0; j<n; j++){
                matrix[i][j]=matrix[i][j]/c;
            }
            f[i]=f[i]/c;
        // виключення в попередніх рядках
//        int subdomain_start = domain_size / world_size * world_rank;
//        int subdomain_size = domain_size / world_size;
//        int subdomain_end = subdomain_start+subdomain_size;
            l=key[i];
            
            
        //for(k=0;k<i;k++){
            if(i!=0){
                if(i==1){
                    c=matrix[0][l];
                    for(j=0;j<n;j++){
                        matrix[0][j]=matrix[0][j]-c*matrix[i][j];
                    }
                    f[0]=f[0]-c*f[i];
                }
                else {
                    MPI_Bcast(&l,1, MPI_INT, 0, MPI_COMM_WORLD);
                    MPI_Bcast(&matrix[i],n,MPI_DOUBLE,0,MPI_COMM_WORLD);
                    MPI_Bcast(&f[i],1,MPI_DOUBLE,0,MPI_COMM_WORLD);
                    d_size = (int) i/world_size;
                    for(k=1;k<world_size;k++){
                        double buf[d_size*n];
                        double f_buf[d_size];
                        memcpy(&f_buf,&f[(k-1)*d_size],d_size*sizeof(double));
                        memcpy(&buf,matrix[(k-1)*d_size],n*d_size*sizeof(double));
                        MPI_Send(&buf, d_size*n,MPI_DOUBLE, k, 0, MPI_COMM_WORLD);
                        MPI_Send(&f_buf,d_size,MPI_DOUBLE,k,0,MPI_COMM_WORLD);
                    }
                    d_ost = i % world_size;
                    for(int g=(k-1)*d_size;g<(k)*d_size+d_ost;g++){
                        c=matrix[g][l];
                        for(j=0;j<n;j++){
                            matrix[g][j]=matrix[g][j]-c*matrix[i][j];
                        }
                        f[g]=f[g]-c*f[i];
                    }
                    for(k=1;k<world_size;k++) {
                        double buf[d_size*n];
                        double f_buf[d_size];
                        MPI_Recv(&buf, d_size*n,MPI_DOUBLE,k, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
                        MPI_Recv(&f_buf, d_size,MPI_DOUBLE,k, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
//                        cout << "buf" << endl;
//                        for(int z=0; z < d_size*n; z++){
//                            cout << buf[z] << " ";
//                        }
//                        cout << endl;
//                        cout << "f_buf" << endl;
//                        for(int z=0; z < d_size; z++){
//                            cout << f_buf[z] << " ";
//                        }
//                        cout << endl;
                        memcpy(&f[(k-1)*d_size],&f_buf,d_size*sizeof(double));
                        memcpy(matrix[(k-1)*d_size],&buf,n*d_size*sizeof(double));
//                        cout << "f" << endl;
//                        for(int z=0; z < n; z++){
//                            cout << f[z] << " ";
//                        }
                        cout << endl;
                    }
                }
            }
//            c=matrix[k][l];
//            for(j=0;j<n;j++){
//                matrix[k][j]=matrix[k][j]-c*matrix[i][j];
//            }
//            f[k]=f[k]-c*f[i];
            for(int g=0;g<n;g++){
                for(int h=0;h<n;h++) cout << matrix[g][h] << " ";
                cout << "| " << f[g] << endl;
            }
        }
        for(int i=0; i < n; i++) cout << f[i] << " ";
    }
    else {
        for(int i=2;i<n;i++){
            double row_res;
            double row[n];
            d_size = i/world_size;
            double res[d_size];
            double fix[d_size*n];
            MPI_Bcast(&l,1, MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Bcast(&row,n,MPI_DOUBLE,0,MPI_COMM_WORLD);
            MPI_Bcast(&row_res,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
            MPI_Recv(&fix,d_size*n,MPI_DOUBLE,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            MPI_Recv(&res,d_size,MPI_DOUBLE,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
//            cout << "row_res: " << row_res << endl;
//            cout << "row: " << endl;
//            for(int z=0; z < n; z++){
//                cout << row[z] << " ";
//            }
//            cout << endl;
//            cout << "fix: " << endl;
//            for(int z=0; z < d_size*n; z++){
//                cout << fix[z] << " ";
//            }
//            cout << endl;
//            cout << "res: " << endl;
//            for(int z=0; z < d_size; z++){
//                cout << res[z] << " ";
//            }
//            cout << endl;
            double fix_matrix[d_size][n];
            memcpy(fix_matrix,fix,d_size*n*sizeof(double));
//            cout << "FIX_1" << endl;
//            for(int z=0; z < d_size; z++){
//                for(int y=0; y<n; y++) cout << fix_matrix[z][y] << " ";
//                cout << endl;
//            }
            for(int g=0;g<d_size;g++){
                c=fix_matrix[g][l];
                for(j=0;j<n;j++){
                    fix_matrix[g][j]=fix_matrix[g][j]-c*row[j];
                }
                res[g]=res[g]-c*row_res;
            }
//            cout << "FIX2" << endl;
//            for(int z=0; z < d_size; z++){
//                for(int y=0; y<n; y++) cout << fix_matrix[z][y] << " ";
//                cout << endl;
//            }
//            cout << "res" << endl;
//            for(int z=0; z < d_size; z++){
//                cout << res[z] << " ";
//            }
//            cout << endl;
            memcpy(fix,fix_matrix,d_size*n*sizeof(double));
//            cout << "fix_3" << endl;
//            for(int z=0; z < d_size*n; z++){
//                cout << fix[z] << " ";
//            }
//            cout << endl;
            cout << " HERE: " << world_rank << endl;
            MPI_Send(&fix,n*d_size,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
            MPI_Send(&res,d_size,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
        }
    }
    MPI_Finalize();
    return 0;
}
