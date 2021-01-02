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
#define my_namespace weakAI2

namespace my_namespace
{
  const int SEARCH_DEPTH = 4;
  using value_type = int;

  // function declare
  value_type evaluate(Board board, Player maximizingPlayer);
  value_type alphabeta(Board board, int depth, value_type alpha, value_type beta, bool isMaximizingPlayer, Player *maximizingPlayer, int index[]);
  bool isPlaceLegal(Board *board, const int row, const int col, Player *player);
} // namespace my_namespace

// function define
void algorithm_A(Board board, Player player, int index[])
{
  static int round = 0;
  char player_color = player.get_color();
  Board copy_board = board;
  Player copy_player = player;

  // algorithm
  srand(time(NULL) * time(NULL));
  my_namespace::alphabeta(copy_board, my_namespace::SEARCH_DEPTH, -inf, inf, true, &copy_player, index);

  // FIXME: manual pending
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
}

namespace my_namespace
{
  // place score
  value_type get_place_score(Board &board, int x, int y)
  {
    int dx[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
    int dy[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int orbs = board.get_orbs_num(x, y);
    int capa = board.get_capacity(x, y);
    char color = board.get_cell_color(x, y);
    char type = (capa == 3 ? 'c' : (capa == 5 ? 'e' : 'm'));
    bool is_critical = (orbs == (capa - 1) ? true : false);
    bool is_danger = false;
    int nx, ny, n_orbs, n_capa;
    char n_color, n_type;
    bool is_n_critical;
    value_type score = 0;

    for (int i = 0; i < 8; i++)
    {
      nx = x + dx[i];
      ny = y + dy[i];
      if (nx < 0 || nx >= 5 || ny < 0 || ny >= 6)
        continue;

      n_color = board.get_cell_color(nx, ny);
      n_orbs = board.get_orbs_num(nx, ny);
      n_capa = board.get_capacity(nx, ny);
      n_type = (n_capa == 3 ? 'c' : (n_capa == 5 ? 'e' : 'm'));
      is_n_critical = (n_orbs == (n_capa - 1) ? true : false);

      if (n_color == 'b' && color == 'r' ||
          n_color == 'r' && color == 'b')
      {
        if (is_n_critical)
        {
          score -= (10 - (capa - 1));
          is_danger = true;
        }

        if (is_critical)
        {
          score += n_orbs * 2;
        }
      }
    }

    if (!is_danger)
    {
      if (type == 'm')
        score += 1;
      else if (type == 'e')
        score += 2;
      else if (type == 'c')
        score += 3;

      if (is_critical)
        score += 2;
    }
    return score;
  }

  // evaluator
  value_type evaluate(Board board, Player maximizingPlayer)
  {
    value_type maximizing_cnt = 0;
    value_type minimizing_cnt = 0;
    value_type maximizing_score = 0;
    value_type minimizing_score = 0;
    char maximizing_color = maximizingPlayer.get_color();
    char minimizing_color = (maximizing_color == 'r' ? 'b' : 'r');
    char curr_color;

    for (int i = 0; i < 5; i++)
      for (int j = 0; j < 6; j++)
      {
        curr_color = board.get_cell_color(i, j);
        if (curr_color == maximizing_color)
        {
          maximizing_cnt += 1;
          maximizing_score += get_place_score(board, i, j);
        }
        else if (curr_color == minimizing_color)
        {
          minimizing_cnt += 1;
          // minimizing_score += get_place_score(board, i, j);
        }
      }

    if (maximizing_cnt == 0)
      return -inf;
    if (minimizing_cnt == 0)
      return inf;

    return maximizing_score + maximizing_cnt;
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
            // init
            if (row == -1 || col == -1)
            {
              row = i;
              col = j;
            }

            copy_board = board;
            copy_board.place_orb(i, j, maximizingPlayer);
            v = alphabeta(copy_board, depth - 1, alpha, beta, false, maximizingPlayer, index);

            // FIXME: debug
            /*
            if (depth == SEARCH_DEPTH)
            {
              cout << "Maximizing color: " << maximizingPlayer->get_color() << '\n';
              cout << "(" << i << ", " << j << ")"
                   << " v: " << v << '\n';
            }
            */

            if (v > alpha)
            {
              alpha = v;
              row = i;
              col = j;
            }
            else if (v == alpha && (rand() % 3 == 0))
            {
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
            // init
            if (row == -1 || col == -1)
            {
              row = i;
              col = j;
            }

            copy_board = board;
            copy_board.place_orb(i, j, minimizingPlayer);
            v = alphabeta(copy_board, depth - 1, alpha, beta, true, minimizingPlayer, index);
            if (v < beta)
            {
              beta = v;
              row = i;
              col = j;
            }
            else if (v == beta && (rand() % 3 == 0))
            {
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

} // namespace my_namespace