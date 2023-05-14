/*****************************************************
    AUTHOR  : Sébastien Valat
    MAIL    : sebastien.valat@univ-grenoble-alpes.fr
    LICENSE : BSD
    YEAR    : 2021
    COURSE  : Parallel Algorithms and Programming
*****************************************************/

//////////////////////////////////////////////////////
//
//
// GOAL: Implement a 1D communication scheme along
//       X axis with blocking communications.
//
// SUMMARY:
//     - 1D splitting along X
//     - Blocking communications
//
//////////////////////////////////////////////////////

/****************************************************/
#include "src/lbm_struct.h"
#include "src/exercises.h"

/****************************************************/
void lbm_comm_init_ex1(lbm_comm_t * comm, int total_width, int total_height)
{

    int rank; // = mon id de noeud a moi quand je lance le code, commence a 0 et va a comm size -1

    int comm_size;
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &comm_size ); //comm_size replis auto avec le N passé en parametre (c'est le nombre de process)

    //
	// TODO: calculate the splitting parameters for the current task.
	//
	// HINT: You can look in exercise_0.c to get an example for the sequential case.
	//

	// TODO: calculate the number of tasks along X axis and Y axis.
	comm->nb_x = total_width / comm_size ; // une seule ligne de domaine, chaque domaine de taille egale
	comm->nb_y = 1 //une dimension en y ;

    //chaque process doit avoir une part égale
    if (total_width % comm->nb_x != 0 ) {
        //abort
    }


    // TODO: calculate the current task position in the splitting
	comm->rank_x = -1;
	comm->rank_y = -1;

	// TODO : calculate the local sub-domain size (do not forget the 
	//        ghost cells). Use total_width & total_height as starting 
	//        point.
    if (//premier de la ligne) {
        comm>width = (total_width / comm_size) + 1 ; // juste ue voisin a droite
    else {
        comm->width = (total_width / comm_size) + 2 ; //left and rigth neighboor
    }

	comm->height = total_height ; //tout le monde a la meme hauteur (c'est que coupé en ligne)

	// TODO : calculate the absolute position in the global mesh.
	//        without accounting the ghost cells
	//        (used to setup the obstable & initial conditions).
	comm->x = -1;
	comm->y = -1;

	//if debug print comm
	//lbm_comm_print(comm);

    /*
#ifndef NDEBUG
    lbm_comm_print( comm );
#endif
     */
}

/****************************************************/
void lbm_comm_ghost_exchange_ex1(lbm_comm_t * comm, lbm_mesh_t * mesh)
{
	//
	// TODO: Implement the 1D communication with blocking MPI functions (MPI_Send & MPI_Recv)
	//
	// To be used:
	//    - DIRECTIONS: the number of doubles composing a cell
	//    - double[DIRECTIONS] lbm_mesh_get_cell(mesh, x, y): function to get the address of a particular cell.
	//    - comm->width : The with of the local sub-domain (containing the ghost cells)
	//    - comm->height : The height of the local sub-domain (containing the ghost cells)
	
	//example to access cell
	//double * cell = lbm_mesh_get_cell(mesh, local_x, local_y);
	//double * cell = lbm_mesh_get_cell(mesh, comm->width - 1, 0);
}
