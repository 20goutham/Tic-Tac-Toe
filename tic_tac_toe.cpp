#include <bits/stdc++.h>
using namespace std;

const int SIZE = 3;
char player = 'x', opponent = 'o'; // Human is 'x' and AI is 'o'

// Function to check if a player has won
bool checkWin(vector<vector<char>> &board, char player) {
    int win_patterns[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, // Horizontal win patterns
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, // Vertical win patterns
        {0, 4, 8}, {2, 4, 6}             // Diagonal win patterns
    };
    for (auto &pattern : win_patterns) {
        if (board[pattern[0] / SIZE][pattern[0] % SIZE] == player &&
            board[pattern[1] / SIZE][pattern[1] % SIZE] == player &&
            board[pattern[2] / SIZE][pattern[2] % SIZE] == player) {
            return true; // Return true if the player has won
        }
    }
    return false; // Return false if the player has not won
}

// Function to check if there are any moves left on the board
bool isMovesLeft(vector<vector<char>> &board) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (board[i][j] == '_') {
                return true; // Return true if there is at least one empty cell
            }
        }
    }
    return false; // Return false if there are no empty cells
}

// Evaluation function to determine the score of the board
int evaluate(vector<vector<char>> &b) {
    // Check rows for victory
    for (int row = 0; row < SIZE; ++row) {
        if (b[row][0] == b[row][1] && b[row][1] == b[row][2]) {
            if (b[row][0] == opponent)
                return 10; // AI wins
            else if (b[row][0] == player)
                return -10; // Human wins
        }
    }
    // Check columns for victory
    for (int col = 0; col < SIZE; ++col) {
        if (b[0][col] == b[1][col] && b[1][col] == b[2][col]) {
            if (b[0][col] == opponent)
                return 10; // AI wins
            else if (b[0][col] == player)
                return -10; // Human wins
        }
    }
    // Check diagonals for victory
    if (b[0][0] == b[1][1] && b[1][1] == b[2][2]) {
        if (b[0][0] == opponent)
            return 10; // AI wins
        else if (b[0][0] == player)
            return -10; // Human wins
    }
    if (b[0][2] == b[1][1] && b[1][1] == b[2][0]) {
        if (b[0][2] == opponent)
            return 10; // AI wins
        else if (b[0][2] == player)
            return -10; // Human wins
    }
    return 0; // Return 0 if no one has won yet
}

// Minimax function to determine the best move
int minimax(vector<vector<char>> &board, int depth, bool isMax) {
    int score = evaluate(board);
    // Return the score if the game is over
    if (score == 10)
        return score - depth; // AI wins
    if (score == -10)
        return score + depth; // Human wins
    if (!isMovesLeft(board))
        return 0; // Return 0 if it's a tie

    if (isMax) { // Maximizer's move (AI)
        int best = -1000;
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                if (board[i][j] == '_') {
                    board[i][j] = opponent; // Make the move
                    best = max(best, minimax(board, depth + 1, !isMax)); // Call minimax recursively
                    board[i][j] = '_'; // Undo the move
                }
            }
        }
        return best;
    } else { // Minimizer's move (Human)
        int best = 1000;
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                if (board[i][j] == '_') {
                    board[i][j] = player; // Make the move
                    best = min(best, minimax(board, depth + 1, !isMax)); // Call minimax recursively
                    board[i][j] = '_'; // Undo the move
                }
            }
        }
        return best;
    }
}

// Function to find the best move for the AI
pair<int, int> findBestMove(vector<vector<char>> &board) {
    int bestVal = -1000;
    pair<int, int> bestMove = make_pair(-1, -1);
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (board[i][j] == '_') {
                board[i][j] = opponent; // Make the move
                int moveVal = minimax(board, 0, false); // Compute the minimax value for this move
                board[i][j] = '_'; // Undo the move
                if (moveVal > bestVal) {
                    bestMove = make_pair(i, j); // Update the best move
                    bestVal = moveVal;
                }
            }
        }
    }
    return bestMove; // Return the best move
}

// Function to print the board
void printBoard(vector<vector<char>> &board) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            cout << board[i][j] << " "; // Print each cell of the board
        }
        cout << endl; // Print a newline after each row
    }
}

int main() {
    vector<vector<char>> board(SIZE, vector<char>(SIZE, '_')); // Initialize the board with empty cells
    int current_player;

    // Asking the user who should make the first move
    cout << "Who should make the first move? (0 for Player, 1 for AI): ";
    cin >> current_player;

    printBoard(board); // Print the initial board

    // Main game loop
    while (true) {
        if (!isMovesLeft(board)) {
            cout << "Game drawn!" << endl;
            break;
        }

        if (current_player == 1) {
            pair<int, int> bestMove = findBestMove(board); // Find the best move for AI
            board[bestMove.first][bestMove.second] = 'o'; // Make the move
            cout << "AI Move:" << endl;
            printBoard(board); // Print the board after AI's move
            if (checkWin(board, 'o')) {
                cout << "Player o (AI) wins!" << endl;
                break;
            }
            current_player = 0; // Switch to the player
        } else {
            int row, col;
            cout << "Your Move (row and column): ";
            cin >> row >> col; // Get the player's move
            if (row < 0 || row >= SIZE || col < 0 || col >= SIZE || board[row][col] != '_') {
                cout << "Invalid Move!" << endl;
                continue; // Ask for another move if the current move is invalid
            }
            board[row][col] = 'x'; // Make the player's move
            cout << "Player Move:" << endl;
            printBoard(board); // Print the board after the player's move
            if (checkWin(board, 'x')) {
                cout << "Player x wins!" << endl;
                break;
            }
            current_player = 1; // Switch to the AI
        }
    }

    return 0;
}
