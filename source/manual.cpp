#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../include/algorithm.h"

using namespace std;

/******************************************************
 * In your algorithm, you can just use the the functions
 * listed by TA to get the board information.(functions
 * 1. ~ 4. are listed in next block)
 *
 * The STL library functions is not allowed to use.
******************************************************/

/*************************************************************************
 * 1. int board.get_orbs_num(int row_index, int col_index)
 * 2. int board.get_capacity(int row_index, int col_index)
 * 3. char board.get_cell_color(int row_index, int col_index)
 * 4. void board.print_current_board(int row_index, int col_index, int round)
 *
 * 1. The function that return the number of orbs in cell(row, col)
 * 2. The function that return the orb capacity of the cell(row, col)
 * 3. The function that return the color fo the cell(row, col)
 * 4. The function that print out the current board statement
*************************************************************************/

void algorithm_B(Board board, Player player, int index[])
{
    static int round = 0;
    static int row, col;

    cout << "Player color: " << player.get_color() << '\n';
    // board.print_current_board(row, col, round++);

    algorithm_A(board, player, index);
    cout << "recommend place: " << index[0] << ", " << index[1] << '\n';

    cin >> row >> col;

    // place orb
    index[0] = row;
    index[1] = col;
}