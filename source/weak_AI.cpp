#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../include/algorithm.h"

using namespace std;
using std::cout;

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

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define inf 2147483647

using value_type = int;

// function declare
value_type evaluate(Board board, Player maximizingPlayer);
value_type alphabeta(Board board, int depth, value_type alpha, value_type beta, bool isMaximizingPlayer, Player *maximizingPlayer, int index[]);
bool isPlaceLegal(Board *board, const int row, const int col, Player *player);

// function define
void algorithm_A(Board board, Player player, int index[])
{
  static int round = 0;
  int row, col;
  int player_cnt = 0, oppnent_cnt = 0;
  char player_color = player.get_color();
  Board copy_board = board;
  Player copy_player = player;

  // algorithm
  srand(time(NULL) * time(NULL));
  alphabeta(copy_board, 4, -inf, inf, true, &copy_player, index);

  // manual pending
  /*
  char next_round_input = '*';
  board.print_current_board(index[0], index[1], round++);
  cout << "Player color: " << player.get_color() << '\n';
  while (next_round_input != 'y')
  {
    cout << "Next round? (y/n): ";
    cin >> next_round_input;
  }
  */

  // place orb
  // index[0] = row;
  // index[1] = col;
}

// evaluator
value_type evaluate(Board board, Player maximizingPlayer)
{
  value_type maximizing_cnt = 0;
  value_type minimizing_cnt = 0;
  char maximizing_color = maximizingPlayer.get_color();
  char minimizing_color = (maximizing_color == 'r' ? 'b' : 'r');
  for (int i = 0; i < 5; i++)
    for (int j = 0; j < 6; j++)
    {
      if (board.get_cell_color(i, j) == maximizing_color)
        maximizing_cnt += board.get_capacity(i, j) * board.get_orbs_num(i, j);
      else if (board.get_cell_color(i, j) == minimizing_color)
        minimizing_cnt += board.get_capacity(i, j) * board.get_orbs_num(i, j);
    }
  return maximizing_cnt - minimizing_cnt;
}

// alphabeta template
value_type alphabeta(Board board, int depth, value_type alpha, value_type beta, bool isMaximizingPlayer, Player *maximizingPlayer, int index[])
{
  if (depth == 0)
  {
    return evaluate(board, *maximizingPlayer);
  }

  int row, col;
  value_type v;
  Board copy_board;
  if (isMaximizingPlayer)
  {
    row = -1;
    col = -1;
    v = -inf;
    for (int i = 0; i < 5; i++)
    {
      for (int j = 0; j < 6; j++)
        if (isPlaceLegal(&board, i, j, maximizingPlayer))
        {
          copy_board = board;

          // init
          if (row == -1 || col == -1)
          {
            row = i;
            col = j;
          }

          copy_board.place_orb(i, j, maximizingPlayer);
          v = alphabeta(copy_board, depth - 1, alpha, beta, false, maximizingPlayer, index);
          if (v > alpha)
          {
            alpha = v;
            row = i;
            col = j;
          }

          if (beta <= alpha)
            break;
        }
      if (beta <= alpha)
        break;
    }
    index[0] = row;
    index[1] = col;

    return v;
  }

  else
  {
    row = -1;
    col = -1;
    v = inf;
    Player *minimizingPlayer = new Player((maximizingPlayer->get_color() == 'r' ? 'b' : 'r'));
    for (int i = 0; i < 5; i++)
    {
      for (int j = 0; j < 6; j++)
        if (isPlaceLegal(&board, i, j, minimizingPlayer))
        {
          copy_board = board;

          // init
          if (row == -1 || col == -1)
          {
            row = i;
            col = j;
          }

          copy_board.place_orb(i, j, maximizingPlayer);
          v = alphabeta(copy_board, depth - 1, alpha, beta, true, maximizingPlayer, index);
          if (v < beta)
          {
            beta = v;
            row = i;
            col = j;
          }

          if (beta <= alpha)
            break;
        }
      if (beta <= alpha)
        break;
    }
    delete minimizingPlayer;
    // index[0] = row;
    // index[1] = col;
    return v;
  }
}

// utility
bool isPlaceLegal(Board *board, const int row, const int col, Player *player)
{
  if (board->get_cell_color(row, col) == player->get_color() || board->get_cell_color(row, col) == 'w')
    return true;
  return false;
}
