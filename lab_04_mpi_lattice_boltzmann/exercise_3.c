/*****************************************************
    AUTHOR  : Sébastien Valat
    MAIL    : sebastien.valat@univ-grenoble-alpes.fr
    LICENSE : BSD
    YEAR    : 2021
    COURSE  : Parallel Algorithms and Programming
*****************************************************/

//////////////////////////////////////////////////////
//
// Goal: Implement non-blocking 1D communication scheme
//       along X axis.
//
// SUMMARY:
//     - 1D splitting along X
// NEW:
//     - >>> Non-blocking communications <<<
//
//////////////////////////////////////////////////////

/****************************************************/
#include "src/lbm_struct.h"
#include "src/exercises.h"

/****************************************************/
void lbm_comm_init_ex3(lbm_comm_t * comm, int total_width, int total_height)
{
	//we use the same implementation then ex1
	lbm_comm_init_ex1(comm, total_width, total_height);
}

/****************************************************/
void lbm_comm_ghost_exchange_ex3(lbm_comm_t * comm, lbm_mesh_t * mesh)

{
    int rank;
    int comm_size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size) ;

    double *ghost_right = lbm_mesh_get_cell(mesh, comm->width - 1, 0);
    double *my_right = lbm_mesh_get_cell(mesh, comm->width - 2, 0);
    double *ghost_left = lbm_mesh_get_cell(mesh, 0, 0);
    double *my_left = lbm_mesh_get_cell(mesh, 1, 0);


    if ( rank == 0 ) {
        int request_count = 0 ;
        MPI_Request requests[2] ;
        MPI_Irecv(ghost_right, comm->height * DIRECTIONS, MPI_DOUBLE, rank + 1, 1, MPI_COMM_WORLD, &requests[request_count++]);
        MPI_Isend(my_right, comm->height * DIRECTIONS, MPI_DOUBLE, rank + 1, 1, MPI_COMM_WORLD, &requests[request_count++]);
        MPI_Waitall(request_count,requests, MPI_STATUSES_IGNORE) ;
    }
    if (rank == comm_size -1 ) {
        int request_count = 0 ;
        MPI_Request requests[2] ;
        MPI_Irecv(ghost_left, comm->height * DIRECTIONS, MPI_DOUBLE, rank -1, 1, MPI_COMM_WORLD, &requests[request_count++]);
        MPI_Isend(my_left, comm->height * DIRECTIONS, MPI_DOUBLE, rank - 1, 1, MPI_COMM_WORLD, &requests[request_count++]);
        MPI_Waitall(request_count,requests, MPI_STATUSES_IGNORE) ;

    }

    if (rank > 0 && rank < comm_size-1 ) {
        int request_count = 0 ;

        MPI_Request requests[4] ;
        MPI_Irecv(ghost_left, comm->height * DIRECTIONS, MPI_DOUBLE, rank - 1, 1, MPI_COMM_WORLD, &requests[request_count++]);
        MPI_Irecv(ghost_right, comm->height * DIRECTIONS, MPI_DOUBLE, rank + 1 , 1, MPI_COMM_WORLD, &requests[request_count++]);
        MPI_Isend(my_left, comm->height * DIRECTIONS, MPI_DOUBLE, rank - 1, 1, MPI_COMM_WORLD, &requests[request_count++]);
        MPI_Isend(my_right, comm->height * DIRECTIONS, MPI_DOUBLE, rank + 1, 1, MPI_COMM_WORLD, &requests[request_count++]);
        MPI_Waitall(request_count,requests, MPI_STATUSES_IGNORE) ;

    }

    return ;
}
