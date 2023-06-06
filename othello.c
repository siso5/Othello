#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BOARD_SIZE 8
#define EMPTY 0
#define BLACK 1
#define WHITE 2

// オセロ盤面
int board[BOARD_SIZE][BOARD_SIZE];

// 初期化
void initializeBoard() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = EMPTY;
        }
    }

    // 初期配置
    board[3][3] = WHITE;
    board[3][4] = BLACK;
    board[4][3] = BLACK;
    board[4][4] = WHITE;
}

// 盤面の表示
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

// 有効な手の判定
bool isValidMove(int row, int col, int player) {
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE || board[row][col] != EMPTY)
        return false;

    int opponent = (player == BLACK) ? WHITE : BLACK;

    // 上方向
    if (row > 0 && board[row - 1][col] == opponent) {
        for (int i = row - 2; i >= 0; i--) {
            if (board[i][col] == player)
                return true;
            if (board[i][col] != opponent)
                break;
        }
    }

    // 下方向
    if (row < BOARD_SIZE - 1 && board[row + 1][col] == opponent) {
        for (int i = row + 2; i < BOARD_SIZE; i++) {
            if (board[i][col] == player)
                return true;
            if (board[i][col] != opponent)
                break;
        }
    }

    // 左方向
    if (col > 0 && board[row][col - 1] == opponent) {
        for (int j = col - 2; j >= 0; j--) {
            if (board[row][j] == player)
                return true;
            if (board[row][j] != opponent)
                break;
        }
    }

    // 右方向
    if (col < BOARD_SIZE - 1 && board[row][col + 1] == opponent) {
        for (int j = col + 2; j < BOARD_SIZE; j++) {
            if (board[row][j] == player)
                return true;
            if (board[row][j] != opponent)
                break;
        }
    }

    // 左上方向
    if (row > 0 && col > 0 && board[row - 1][col - 1] == opponent) {
        for (int i = row - 2, j = col - 2; i >= 0 && j >= 0; i--, j--) {
            if (board[i][j] == player)
                return true;
            if (board[i][j] != opponent)
                break;
        }
    }

    // 右上方向
    if (row > 0 && col < BOARD_SIZE - 1 && board[row - 1][col + 1] == opponent) {
        for (int i = row - 2, j = col + 2; i >= 0 && j < BOARD_SIZE; i--, j++) {
            if (board[i][j] == player)
                return true;
            if (board[i][j] != opponent)
                break;
        }
    }

    // 左下方向
    if (row < BOARD_SIZE - 1 && col > 0 && board[row + 1][col - 1] == opponent) {
        for (int i = row + 2, j = col - 2; i < BOARD_SIZE && j >= 0; i++, j--) {
            if (board[i][j] == player)
                return true;
            if (board[i][j] != opponent)
                break;
        }
    }

    // 右下方向
    if (row < BOARD_SIZE - 1 && col < BOARD_SIZE - 1 && board[row + 1][col + 1] == opponent) {
        for (int i = row + 2, j = col + 2; i < BOARD_SIZE && j < BOARD_SIZE; i++, j++) {
            if (board[i][j] == player)
                return true;
            if (board[i][j] != opponent)
                break;
        }
    }

    return false;
}

// 石を反転させる
void flipStones(int row, int col, int player) {
    int opponent = (player == BLACK) ? WHITE : BLACK;

    // 上方向
    for (int i = row - 1; i >= 0; i--) {
        if (board[i][col] == EMPTY)
            break;
        if (board[i][col] == player) {
            for (int j = row - 1; j > i; j--)
                board[j][col] = player;
            break;
        }
    }

    // 下方向
    for (int i = row + 1; i < BOARD_SIZE; i++) {
        if (board[i][col] == EMPTY)
            break;
        if (board[i][col] == player) {
            for (int j = row + 1; j < i; j++)
                board[j][col] = player;
            break;
        }
    }

    // 左方向
    for (int j = col - 1; j >= 0; j--) {
        if (board[row][j] == EMPTY)
            break;
        if (board[row][j] == player) {
            for (int k = col - 1; k > j; k--)
                board[row][k] = player;
            break;
        }
    }

    // 右方向
    for (int j = col + 1; j < BOARD_SIZE; j++) {
        if (board[row][j] == EMPTY)
            break;
        if (board[row][j] == player) {
            for (int k = col + 1; k < j; k++)
                board[row][k] = player;
            break;
        }
    }

    // 左上方向
    for (int i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--) {
        if (board[i][j] == EMPTY)
            break;
        if (board[i][j] == player) {
            for (int k = row - 1, l = col - 1; k > i && l > j; k--, l--)
                board[k][l] = player;
            break;
        }
    }

    // 右上方向
    for (int i = row - 1, j = col + 1; i >= 0 && j < BOARD_SIZE; i--, j++) {
        if (board[i][j] == EMPTY)
            break;
        if (board[i][j] == player) {
            for (int k = row - 1, l = col + 1; k > i && l < j; k--, l++)
                board[k][l] = player;
            break;
        }
    }

    // 左下方向
    for (int i = row + 1, j = col - 1; i < BOARD_SIZE && j >= 0; i++, j--) {
        if (board[i][j] == EMPTY)
            break;
        if (board[i][j] == player) {
            for (int k = row + 1, l = col - 1; k < i && l > j; k++, l--)
                board[k][l] = player;
            break;
        }
    }

    // 右下方向
    for (int i = row + 1, j = col + 1; i < BOARD_SIZE && j < BOARD_SIZE; i++, j++) {
        if (board[i][j] == EMPTY)
            break;
        if (board[i][j] == player) {
            for (int k = row + 1, l = col + 1; k < i && l < j; k++, l++)
                board[k][l] = player;
            break;
        }
    }
}

// 石の数をカウント
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


// ユーザーの手番
void userTurn() {
    int row, col;
    char input[10]; // 入力値を一時的に保存するための配列

    while (true) {
        printf("Your move (row col): ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            // 入力が正しくない場合は再入力を促す
            printf("Invalid input. Please enter row and column numbers.\n");
            continue;
        }

        if (sscanf(input, "%d%d", &row, &col) != 2) {
            // 入力が正しくない場合は再入力を促す
            printf("Invalid input. Please enter row and column numbers.\n");
            continue;
        }

        if (row < 1 || row > BOARD_SIZE || col < 1 || col > BOARD_SIZE) {
            // 入力が範囲外の場合は再入力を促す
            printf("Invalid move. Please enter row and column numbers within the board range.\n");
            continue;
        }

        // 入力値をインデックスに変換
        row--;
        col--;

        if (!isValidMove(row, col, BLACK)) {
            // 有効な手でない場合は再入力を促す
            printf("Invalid move. Please enter a valid move.\n");
            continue;
        }

        break;  // 入力が正しい場合はループを終了
    }

    board[row][col] = BLACK;
    flipStones(row, col, BLACK);
}

// AIのモンテカルロ木探索
int monteCarloSearch(int player, int simulations) {
    int scores[BOARD_SIZE][BOARD_SIZE] = {0}; // 各手のスコア

    for (int i = 0; i < simulations; i++) {
        // ゲームを複製する
        int boardCopy[BOARD_SIZE][BOARD_SIZE];
        for (int row = 0; row < BOARD_SIZE; row++) {
            for (int col = 0; col < BOARD_SIZE; col++) {
                boardCopy[row][col] = board[row][col];
            }
        }

        int row, col;
        while (true) {
            // ランダムな手を選択
            row = rand() % BOARD_SIZE;
            col = rand() % BOARD_SIZE;

            if (isValidMove(row, col, player))
                break; // 有効な手の場合はループを終了
        }

        // 手を適用
        boardCopy[row][col] = player;
        flipStones(row, col, player);

        // シミュレーションの結果を評価
        int blackCount, whiteCount;
        countStones(&blackCount, &whiteCount);
        int score = (player == BLACK) ? blackCount - whiteCount : whiteCount - blackCount;

        // スコアを更新
        scores[row][col] += score;
    }

    // スコアが最も高い手を選択
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

    return (bestRow + 1) * 10 + (bestCol + 1); // 行と列を1つの数値にエンコードして返す
}

// ゲームの進行
void playGame(int simulations) {
    int currentPlayer = BLACK;
    bool gameEnd = false;

    while (!gameEnd) {
        printBoard();

        if (currentPlayer == BLACK) {
            printf("Black's turn.\n");
            userTurn();
        } else {
            printf("White's turn.\n");
            int move = monteCarloSearch(WHITE, simulations);
            int row = move / 10;
            int col = move % 10;
            printf("AI's move: %d %d\n", row, col);
            board[row - 1][col - 1] = WHITE;
            flipStones(row - 1, col - 1, WHITE);
        }

        // ゲーム終了の判定
        bool blackMoves = false, whiteMoves = false;
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (board[i][j] == EMPTY && isValidMove(i, j, BLACK))
                    blackMoves = true;
                if (board[i][j] == EMPTY && isValidMove(i, j, WHITE))
                    whiteMoves = true;
            }
        }

        if (!blackMoves && !whiteMoves) {
            printf("Game Over\n");
            gameEnd = true;
        } else if (!blackMoves) {
            printf("Black has no moves. White wins!\n");
            gameEnd = true;
        } else if (!whiteMoves) {
            printf("White has no moves. Black wins!\n");
            gameEnd = true;
        }

        // 手番を交代
        currentPlayer = (currentPlayer == BLACK) ? WHITE : BLACK;
    }

    printBoard();

    // 結果の表示
    int blackCount, whiteCount;
    countStones(&blackCount, &whiteCount);
    printf("Black: %d\nWhite: %d\n", blackCount, whiteCount);
    if (blackCount > whiteCount)
        printf("Black wins!\n");
    else if (whiteCount > blackCount)
        printf("White wins!\n");
    else
        printf("It's a tie!\n");
}

int main() {
    int simulations = 1;

    initializeBoard();
    playGame(simulations);

    return 0;
}
