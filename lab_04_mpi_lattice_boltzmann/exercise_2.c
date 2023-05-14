/*****************************************************
    AUTHOR  : Sébastien Valat
    MAIL    : sebastien.valat@univ-grenoble-alpes.fr
    LICENSE : BSD
    YEAR    : 2021
    COURSE  : Parallel Algorithms and Programming
*****************************************************/

//////////////////////////////////////////////////////
//
// Goal: Implement odd/even 1D blocking communication scheme 
//       along X axis.
//
// SUMMARY:
//     - 1D splitting along X
//     - Blocking communications
// NEW:
//     - >>> Odd/even communication ordering <<<<
//
//////////////////////////////////////////////////////

/****************************************************/
#include "src/lbm_struct.h"
#include "src/exercises.h"

/****************************************************/
void lbm_comm_init_ex2(lbm_comm_t * comm, int total_width, int total_height)
{
	//we use the same implementation then ex1
	lbm_comm_init_ex1(comm, total_width, total_height);
}

/****************************************************/
void lbm_comm_ghost_exchange_ex2(lbm_comm_t * comm, lbm_mesh_t * mesh) {
    int rank;
    int comm_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

    double *ghost_right = lbm_mesh_get_cell(mesh, comm->width - 1, 0);
    double *my_right = lbm_mesh_get_cell(mesh, comm->width - 2, 0);
    double *ghost_left = lbm_mesh_get_cell(mesh, 0, 0);
    double *my_left = lbm_mesh_get_cell(mesh, 1, 0);


    MPI_Status status;

    //odd : send then receive
    if (rank % 2 == 1) {
        if (rank != comm_size -1) {
            MPI_Send(my_right, comm->height * DIRECTIONS, MPI_DOUBLE, rank + 1, 1, MPI_COMM_WORLD);
        }
        if (rank != 0) {
            MPI_Send(my_left, comm->height * DIRECTIONS, MPI_DOUBLE, rank - 1, 1, MPI_COMM_WORLD);
        }

        if( rank != comm_size -1 ) {
            MPI_Recv(ghost_right, comm->height * DIRECTIONS, MPI_DOUBLE, rank + 1, 1, MPI_COMM_WORLD, &status);
        }
        if (rank != 0) {
            MPI_Recv(ghost_left, comm->height * DIRECTIONS, MPI_DOUBLE, rank - 1, 1, MPI_COMM_WORLD, &status);
        }
        return  ;
    }

    //Even recv then send
    if (rank % 2 == 0 ) {
        if( rank != comm_size -1 ) {
            MPI_Recv(ghost_right, comm->height * DIRECTIONS, MPI_DOUBLE, comm->rank_x + 1, 1, MPI_COMM_WORLD, &status);
        }

        if (rank != 0) {
            MPI_Recv(ghost_left, comm->height * DIRECTIONS, MPI_DOUBLE, comm->rank_x - 1, 1, MPI_COMM_WORLD, &status);
        }
        if( rank != comm_size -1 ) {
            MPI_Send(my_right, comm->height * DIRECTIONS, MPI_DOUBLE, comm->rank_x + 1, 1, MPI_COMM_WORLD);
        }

        if (rank != 0) {
            MPI_Send(my_left, comm->height * DIRECTIONS, MPI_DOUBLE, comm->rank_x - 1, 1, MPI_COMM_WORLD);
        }
        return  ;
    }
    return  ;

}
