#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BOARD_SIZE 8
#define EMPTY 0
#define BLACK 1
#define WHITE 2

// Othello board
int board[BOARD_SIZE][BOARD_SIZE];

// Function prototypes
void initializeBoard();
void printBoard();
bool isValidMove(int row, int col, int player);
void flipStones(int row, int col, int player);
void countStones(int* blackCount, int* whiteCount);
void userTurn();
int monteCarloSearch(int player, int simulations);
void aiTurn(int simulations);
void playGame(int simulations);
void printResult(int blackCount, int whiteCount);
int switchPlayer(int currentPlayer);

// Initializes the board
void initializeBoard() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = EMPTY;
        }
    }

    // Initial configuration
    board[3][3] = WHITE;
    board[3][4] = BLACK;
    board[4][3] = BLACK;
    board[4][4] = WHITE;
}

// Prints the board
void printBoard() {
    printf("  1 2 3 4 5 6 7 8\n");
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%d ", i + 1);
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == EMPTY)
                printf("- ");
            else if (board[i][j] == BLACK)
                printf("● ");
            else if (board[i][j] == WHITE)
                printf("○ ");
        }
        printf("\n");
    }
}

// Checks if a move is valid
bool isValidMove(int row, int col, int player) {
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE || board[row][col] != EMPTY)
        return false;

    int opponent = (player == BLACK) ? WHITE : BLACK;

    // Check in all eight directions for possible flips
    int directions[8][2] = {
        {-1, 0}, {-1, 1}, {0, 1}, {1, 1},
        {1, 0}, {1, -1}, {0, -1}, {-1, -1}
    };

    for (int i = 0; i < 8; i++) {
        int dirX = directions[i][0];
        int dirY = directions[i][1];
        int r = row + dirX;
        int c = col + dirY;

        bool isValid = false;
        while (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE) {
            if (board[r][c] == opponent) {
                r += dirX;
                c += dirY;
            } else if (board[r][c] == player) {
                isValid = true;
                break;
            } else {
                break;
            }
        }

        if (isValid)
            return true;
    }

    return false;
}

// Flips the opponent's stones
void flipStones(int row, int col, int player) {
    int opponent = (player == BLACK) ? WHITE : BLACK;

    // Check in all eight directions for possible flips
    int directions[8][2] = {
        {-1, 0}, {-1, 1}, {0, 1}, {1, 1},
        {1, 0}, {1, -1}, {0, -1}, {-1, -1}
    };

    for (int i = 0; i < 8; i++) {
        int dirX = directions[i][0];
        int dirY = directions[i][1];
        int r = row + dirX;
        int c = col + dirY;

        bool isValid = false;
        while (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE) {
            if (board[r][c] == opponent) {
                r += dirX;
                c += dirY;
            } else if (board[r][c] == player) {
                isValid = true;
                break;
            } else {
                break;
            }
        }

        if (isValid) {
            r -= dirX;
            c -= dirY;
            while (r != row || c != col) {
                board[r][c] = player;
                r -= dirX;
                c -= dirY;
            }
        }
    }
}

// Counts the number of stones
void countStones(int* blackCount, int* whiteCount) {
    *blackCount = 0;
    *whiteCount = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == BLACK)
                (*blackCount)++;
            else if (board[i][j] == WHITE)
                (*whiteCount)++;
        }
    }
}

// Handles the user's turn
void userTurn() {
    int row, col;
    char input[10]; // Temporary storage for input value

    while (true) {
        printf("Your move (row col): ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            // Prompt for re-entering if input is invalid
            printf("Invalid input. Please enter row and column numbers.\n");
            continue;
        }

        if (sscanf(input, "%d%d", &row, &col) != 2) {
            // Prompt for re-entering if input is invalid
            printf("Invalid input. Please enter row and column numbers.\n");
            continue;
        }

        if (row < 1 || row > BOARD_SIZE || col < 1 || col > BOARD_SIZE) {
            // Prompt for re-entering if input is out of range
            printf("Invalid move. Please enter row and column numbers within the board range.\n");
            continue;
        }

        // Convert input values to indices
        row--;
        col--;

        if (!isValidMove(row, col, BLACK)) {
            // Prompt for re-entering if the move is invalid
            printf("Invalid move. Please enter a valid move.\n");
            continue;
        }

        break;  // Exit the loop if input is valid
    }

    board[row][col] = BLACK;
    flipStones(row, col, BLACK);
}

// Performs Monte Carlo tree search for the AI's turn
int monteCarloSearch(int player, int simulations) {
    int scores[BOARD_SIZE][BOARD_SIZE] = {0}; // Scores for each move

    // Check if valid moves exist
    bool validMovesExist = false;
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (isValidMove(row, col, player)) {
                validMovesExist = true;
                break;
            }
        }
        if (validMovesExist)
            break;
    }

    if (!validMovesExist) {
        // If no valid moves exist, end the game
        return -1;
    }

    // If valid moves exist, perform the search
    for (int i = 0; i < simulations; i++) {
        // Create a copy of the game board
        int boardCopy[BOARD_SIZE][BOARD_SIZE];
        for (int row = 0; row < BOARD_SIZE; row++) {
            for (int col = 0; col < BOARD_SIZE; col++) {
                boardCopy[row][col] = board[row][col];
            }
        }

        int row, col;
        while (true) {
            // Select a random move
            row = rand() % BOARD_SIZE;
            col = rand() % BOARD_SIZE;

            if (isValidMove(row, col, player))
                break; // Exit the loop if the move is valid
        }

        // Apply the move
        boardCopy[row][col] = player;
        flipStones(row, col, player);

        // Evaluate the result of the simulation
        int blackCount, whiteCount;
        countStones(&blackCount, &whiteCount);
        int score = (player == BLACK) ? blackCount - whiteCount : whiteCount - blackCount;

        // Update the scores
        scores[row][col] += score;
    }

    // Select the move with the highest score
    int bestScore = -10000;
    int bestRow = -1, bestCol = -1;

    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (isValidMove(row, col, player) && scores[row][col] > bestScore) {
                bestScore = scores[row][col];
                bestRow = row;
                bestCol = col;
            }
        }
    }

    return bestRow * BOARD_SIZE + bestCol; // Encode the row and column into a single value
}

// Handles the AI's turn
void aiTurn(int simulations) {
    int move = monteCarloSearch(WHITE, simulations);

    if (move == -1) {
        printf("AI has no valid moves.\n");
        return;
    }

    int row = move / BOARD_SIZE;
    int col = move % BOARD_SIZE;

    if (!isValidMove(row, col, WHITE)) {
        printf("AI error: Invalid move.\n");
        return;
    }

    board[row][col] = WHITE;
    flipStones(row, col, WHITE);
    ("AI's move board[row col]: %d, %d\n", row + 1, col + 1);
}

// Plays the game
void playGame(int simulations) {
    initializeBoard();
    printBoard();

    while (true) {
        // User's turn
        userTurn();
        printBoard();

        // Count the stones
        int blackCount, whiteCount;
        countStones(&blackCount, &whiteCount);
        printf("Black: %d, White: %d\n", blackCount, whiteCount);

        // AI's turn
        aiTurn(simulations);
        printBoard();

        // Count the stones
        countStones(&blackCount, &whiteCount);
        printf("Black: %d, White: %d\n", blackCount, whiteCount);
    }
}

// Prints the result of the game
void printResult(int blackCount, int whiteCount) {
    printf("Game over!\n");
    printf("Black: %d, White: %d\n", blackCount, whiteCount);
    if (blackCount > whiteCount) {
        printf("Black wins!\n");
    } else if (whiteCount > blackCount) {
        printf("White wins!\n");
    } else {
        printf("It's a tie!\n");
    }
}

// Switches the player
int switchPlayer(int currentPlayer) {
    return (currentPlayer == BLACK) ? WHITE : BLACK;
}

int main() {
    int simulations = 1;  // Number of simulations for the AI
    playGame(simulations);
    return 0;
}
