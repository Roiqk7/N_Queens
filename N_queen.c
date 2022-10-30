// important!!! in progress


//  solution to n queen problem using backtracking algorithm


//TODO make formatAnalysis and show different data 


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>


//  _NUM are representatives in 2d array 'board' 
//  _CHAR are used in printOut for better visual
const char QUEEN_CHAR = 'Q';
const char EMPTY_CHAR = '.';
const int QUEEN_NUM = 1;
const int EMPTY_NUM = 0;

//return values for functions
// _NUM and _CHAR represent corresponding modes
const int ERROR_OCCURED = -1;
const int SUCCESS = 0;

const int HOME_MODE_NUM = 1;
const int EDIT_MODE_NUM = 2;
const int CLEAR_BOARD_NUM = 21;
const int ANALYSIS_MODE_NUM = 3;

const char HOME_MODE_CHAR = 'x';
const char EDIT_MODE_CHAR = 'w';
const char CLEAR_BOARD_CHAR = 'z';
const char ANALYSIS_MODE_CHAR = 'a';


int getUserInput(int mode, int n, int board[n][n]);                     //  gets user input properly 
void assignPos(char *userInput, int mode, int n, int board[n][n]);      //  places queen based on user input
void getAnalysis(int n);                                                //  makes analysis if user enters analysis mode
int NQmain(int n, int board[n][n]);                                     //  handles functions in order to solve and return the puzzle
bool solveNQueen(int row, int n, int board[n][n]);                      //  solves the puzzle
bool isValid(int row, int col, int n, int board[n][n]);                 //  helper function which checks if Queen can be placed 
void printOut(int n, int board[n][n]);                                  //  prints solved board with representative _CHARs
 

//  variables to calculate runtime and average runtime
double timeSum;
int timeLen;


                                                            
int main(void)
{  
    while (true) {                                          //  main loop for keeping the program going
        int n, input;
        timeSum = timeLen = 0.0;                            //  resets timeSum and Len at 0 to prevent inconvenient output        
        printf("\nEnter N: ");  
        scanf("%d", &n);
        int board[n][n];                                    //  board is most important variable which stores THE BOARD!
        memset(board, EMPTY_NUM, sizeof(board));        
        printOut(n, board);                                     
        while (true) {                                      //  after user inputs N -> user enters home mode
            input = getUserInput(1, n, board);
            if (input == HOME_MODE_NUM) break;              //  returns user back to home mode
            switch (input) {
                case ERROR_OCCURED:                             //  closes the program if malloc() returns NULL
                    return ERROR_OCCURED;
                case SUCCESS:                                   //  solves the position 
                    NQmain(n, board);
                    break;
                case EDIT_MODE_NUM:                             //  user entered edit mode
                    memset(board, EMPTY_NUM, sizeof(board));
                    printOut(n, board);
                    while (true) {
                        input = getUserInput(EDIT_MODE_NUM, n, board);
                        if (input == ERROR_OCCURED) return ERROR_OCCURED;
                        if (input == HOME_MODE_NUM) break;               
                        if (input == CLEAR_BOARD_NUM) memset(board, EMPTY_NUM, sizeof(board));
                        printf("\n\n");
                        printOut(n, board);
                    }
                    NQmain(n, board);
                    break;
                case ANALYSIS_MODE_NUM:                         //  user entered analysis mode
                    getAnalysis(n);
                    break;
                default:
                    return main();                              //  resets the program by default
            }
        } 
    }
}


/*

    ##     ##  ######  ######## ########     #### ##    ## ########  ##     ## ######## 
    ##     ## ##    ## ##       ##     ##     ##  ###   ## ##     ## ##     ##    ##    
    ##     ## ##       ##       ##     ##     ##  ####  ## ##     ## ##     ##    ##    
    ##     ##  ######  ######   ########      ##  ## ## ## ########  ##     ##    ##    
    ##     ##       ## ##       ##   ##       ##  ##  #### ##        ##     ##    ##    
    ##     ## ##    ## ##       ##    ##      ##  ##   ### ##        ##     ##    ##    
     #######   ######  ######## ##     ##    #### ##    ## ##         #######     ##    

*/


//  gets input from user and hands it to assignPos() function
int getUserInput(int mode, int n, int board[n][n])
{
    char *userInput = malloc(5); 
    if (userInput == NULL) return ERROR_OCCURED;     
    switch (mode)
    {
    case HOME_MODE_NUM:         //  home
        printf("([x] reassign N value | [w] edit mode | [a] analysis mode)");
        if (n < 27) printf("\n(eg 'a1') Enter starting position of the Queen: "); 
        else printf("\n(eg '37 11') Enter starting position of the Queen: "); 
        scanf("%s", userInput);
        break;
    case EDIT_MODE_NUM:         //  edit mode
        printf("([x] to finish | [z] to clear board)");  
        if (n < 27) printf("\n(edit mode) (eg 'a1') Enter starting position of the Queen: "); 
        else printf("\n(edit mode) (eg '37 11') Enter starting position of the Queen: "); 
        scanf("%s", userInput);
        break;
    case ANALYSIS_MODE_NUM:     //  analysis mode
        return ANALYSIS_MODE_NUM;
    default:
        return getUserInput(mode, n, board);
    }
    if (strlen(userInput) == 1) {           //user typed in only 1 letter 
        switch (userInput[0]) {
            case HOME_MODE_CHAR:
                return HOME_MODE_NUM;
            case EDIT_MODE_CHAR:
                return EDIT_MODE_NUM;
            case CLEAR_BOARD_CHAR:
                return CLEAR_BOARD_NUM;
            case ANALYSIS_MODE_CHAR:
                return ANALYSIS_MODE_NUM;
            default:                                    
                return getUserInput(mode, n, board);
        }
    }
    assignPos(userInput, mode, n, board);
    return SUCCESS;
}


//  transfers user input into usable format and places queen on desired position if its valid
void assignPos(char *userInput, int mode, int n, int board[n][n])
{
    int row, col, posNum;
    if (mode == HOME_MODE_NUM) memset(board, EMPTY_NUM, n*sizeof(*board)); 
    if (n < 27) {                 //  checks if alphabetical or numberical marking should be used
        col = userInput[0] - 'a'; 
        userInput++;
        posNum = atoi(userInput);
        if (posNum == 0) return;
        row = n - posNum;
    } 
    else {
        col = atoi(userInput);
        userInput += 2;
        row = atoi(userInput);
    } 
    if (isValid(row, col, n, board)) {
        board[row][col] = QUEEN_NUM;
        return;
    }
    return;
}


/*
     ######  ##        #######   ######  ##    ## 
    ##    ## ##       ##     ## ##    ## ##   ##  
    ##       ##       ##     ## ##       ##  ##   
    ##       ##       ##     ## ##       #####    
    ##       ##       ##     ## ##       ##  ##   
    ##    ## ##       ##     ## ##    ## ##   ##  
     ######  ########  #######   ######  ##    ## 
*/


//  stopwatch using coroutines
double stopwatch(void)        
{
    static int state = 0;
    static clock_t t;
    if (state == 0) {
        state = 1;
        t = clock();
        return -1.0;
    }
    state = 0;
    t = clock() - t;
    double run_time = (double) t/CLOCKS_PER_SEC;
    timeSum += run_time;                //  adds times to timeSum and timeLen so time average can be easily calculated
    timeLen += 1;
    return run_time;
}


//TODO
//  formats time in seconds into desired format
//  showed acuracy -> how accurate the return should be eg(showedAccuracy=1, 1min; showedAccuracy=3, 1min 43secs 872ms)
char *formatTime(int seconds, int showedAccuracy)
{
    //TODO https://www.codewars.com/kata/52742f58faf5485cae000b9a/solutions/c
    return "1min 43sec 21ms";
}


/*  
    ##     ##    ###    ##    ## ########  ##       ########       ###    ##    ##    ###    ##       ##    ##  ######  ####  ######  
    ##     ##   ## ##   ###   ## ##     ## ##       ##            ## ##   ###   ##   ## ##   ##        ##  ##  ##    ##  ##  ##    ## 
    ##     ##  ##   ##  ####  ## ##     ## ##       ##           ##   ##  ####  ##  ##   ##  ##         ####   ##        ##  ##       
    ######### ##     ## ## ## ## ##     ## ##       ######      ##     ## ## ## ## ##     ## ##          ##     ######   ##   ######  
    ##     ## ######### ##  #### ##     ## ##       ##          ######### ##  #### ######### ##          ##          ##  ##        ## 
    ##     ## ##     ## ##   ### ##     ## ##       ##          ##     ## ##   ### ##     ## ##          ##    ##    ##  ##  ##    ## 
    ##     ## ##     ## ##    ## ########  ######## ########    ##     ## ##    ## ##     ## ########    ##     ######  ####  ######  

*/                                                                                                     


/*
/user entered analysis mode

/table while performing analysis

+-------------------------------------+
|    ANALYSIS IN PROCESS FOR  15N     |
|            FINISHED: 20             |
|            LEFT: 180                |
+-------------------------------------+


+-------------------------------------+
|   A N A L Y S I S   F O R    20 N   |
+-------------------------------------+
|   T O T A L:   12min 12sec 821ms   |         <--- show 3 most significant times 
+-------------------------------------+
|     A V E R A G E:  12sec 123ms     |         | make time converter with smart functions -> human readable time kata
+-------------------------------------+
| SLOWEST STARTING POSITION: a8 43sec |         <--- round up to most significant 
| FASTEST STARTING POSITION: f1 123ms |  
+-------------------------------------+ 
|  1150% INCREASE FROM LAST ANALYSIS  |          <--- dont forget the decrease
+-------------------------------------+
|           H I S T O R Y:            |
|   N       TOTAL         AVERAGE     |
|  10     1min 12sec      11s 7812ms  |          <--- dont sort it! 
|   5     2sec 123ms           123ms  |           | show only 2 most significant times   
|  30    27min 45sec     3min 28 sec  |      
|   1            0ms             0ms  |      
+-------------------------------------+
([x] exit analysis mode)
(analysis mode) Enter N:
...

/after finished analysis user can continue in analysis mode or leave

*/


//  returns analysis in desired format
void formatAnalysis(void)
{
    //TODO
}


//  puts queen on every position and keeps track of time
void getAnalysis(int n)
{
    timeSum = timeLen = 0.0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int board[n][n];
            memset(board, EMPTY_NUM, sizeof(board));
            board[i][j] = QUEEN_NUM;
            NQmain(n, board);
            printf("Valid solutions found: %i\n", timeLen);
        }
    }
    formatAnalysis();
}


/*
       ###    ##        ######    #######  ########  #### ######## ##     ## ##     ##  ######  
      ## ##   ##       ##    ##  ##     ## ##     ##  ##     ##    ##     ## ###   ### ##    ## 
     ##   ##  ##       ##        ##     ## ##     ##  ##     ##    ##     ## #### #### ##       
    ##     ## ##       ##   #### ##     ## ########   ##     ##    ######### ## ### ##  ######  
    ######### ##       ##    ##  ##     ## ##   ##    ##     ##    ##     ## ##     ##       ## 
    ##     ## ##       ##    ##  ##     ## ##    ##   ##     ##    ##     ## ##     ## ##    ## 
    ##     ## ########  ######    #######  ##     ## ####    ##    ##     ## ##     ##  ######  
*/



//  directs the process of generating solution for the puzzle and handing it back to the user properly
int NQmain(int n, int board[n][n]) 
{
    stopwatch();                                                //  starts timer using stopwatch() function
    if (solveNQueen(0, n, board)) {
        double run_time = stopwatch();
        printf("\n\n\nFinished in %.3fs\n", run_time);
        printf("Average runtime is %.6fs\n", timeSum/timeLen);
        printOut(n, board);
        return SUCCESS;
    }
    printf("\nNo solutions found.\n\n");
    return SUCCESS;
}


//  checks if queen can be placed on specific position on the board
bool isValid(int row, int col, int n, int board[n][n])
{
    for (int i = 0; i < n; i++) if (board[i][col] == QUEEN_NUM) return false;
    for (int i = 0; i < n; i++) { 
        for (int j = 0; j < n; j++) {
            if (i + j == row + col || i - j == row - col) {
                if (board[i][j] == QUEEN_NUM) return false;
            }
        }
    }
    return true;
}

//  time complexity: O(n!)
//  solve the puzzle using backtracking algorithm
bool solveNQueen(int row, int n, int board[n][n])
{
    if (row == n) return true;
    for (int i = 0; i < n; i++) if (board[row][i] == QUEEN_NUM) return solveNQueen(row+1, n, board);
    for (int i = 0; i < n; i++) {
        if (isValid(row, i, n, board)) {
            board[row][i] = QUEEN_NUM;
            if (solveNQueen(row+1, n, board)) return true;
        }
        board[row][i] = EMPTY_NUM;
    }
    return false;
}


/*
    ########  ########  #### ##    ## ########    ########   #######     ###    ########  ########  
    ##     ## ##     ##  ##  ###   ##    ##       ##     ## ##     ##   ## ##   ##     ## ##     ## 
    ##     ## ##     ##  ##  ####  ##    ##       ##     ## ##     ##  ##   ##  ##     ## ##     ## 
    ########  ########   ##  ## ## ##    ##       ########  ##     ## ##     ## ########  ##     ## 
    ##        ##   ##    ##  ##  ####    ##       ##     ## ##     ## ######### ##   ##   ##     ## 
    ##        ##    ##   ##  ##   ###    ##       ##     ## ##     ## ##     ## ##    ##  ##     ## 
    ##        ##     ## #### ##    ##    ##       ########   #######  ##     ## ##     ## ########  
*/


//  gets called by printOut if n < 27 else its not called since it may be too distracting
//  returns Queen poitions in chess-like format eg("a2, b4, c1, d3") 
//  also automaticaly sorts the output based on columns
void returnf(int n, int board[n][n])
{   
    int count = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {   
            if (board[j][i] == QUEEN_NUM) {
                printf("%c%i", 'a' + i%26, n - j);
                count++;
                if (count != n) printf(", ");
            }
        }
        if (count % (n/2) == 0 && count > 0) printf("\n");
    }
    printf("\n\n");
}


//  prints the board in proper format and calls returnf() function to format the result
/*
    a b c d 
    . Q . . |4
    . . . Q |3
    Q . . . |2
    . . Q . |1
*/
void printOut(int n, int board[n][n])
{
    for (int i = 0; i < n; i++) {
        if (n < 27) printf("%c ", 'a' + i%26);
        else if ((i+1) % 5 == 0) printf("%9i ", i+1);
    }
    printf("\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == QUEEN_NUM) printf("%c ", QUEEN_CHAR);
            else printf("%c ", EMPTY_CHAR);
        }
        if (n < 27) printf("|%i\n", n - i);
        else if ((i+1) % 5 == 0) printf("|%i\n", n - i);
    }
    if (n < 27) returnf(n, board);
}
