// crossword.c : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#define MAX_MAP_HEIGHT 31
#define MAX_LINE_WIDTH 62
#define MAX_DESCRIPTION_LENGTH 1000

void showIntro() {
    printf("crossword game\n");
    printf("press 1 to play normal crossword game\n");
    printf("press 2 to play time attack crossword game\n");
}

void initializeBoard(char board[MAX_MAP_HEIGHT][MAX_MAP_HEIGHT]) {
    for (int i = 0; i < MAX_MAP_HEIGHT; i++) {
        for (int j = 0; j < MAX_MAP_HEIGHT; j++) {
            board[i][j] = '.'; // 빈 공간은 '.'으로 표시
        }
    }
}

// map 출력 함수
void displayBoard(char board[MAX_MAP_HEIGHT][MAX_MAP_HEIGHT]) {
    printf("\ncrossword puzzle:\n");
    for (int i = 0; i < MAX_MAP_HEIGHT; i++) {
        for (int j = 0; j < MAX_MAP_HEIGHT; j++) {
            printf("+---");
        }
        printf("+\n");

        for (int j = 0; j < MAX_MAP_HEIGHT; j++) {
            printf("| %c ", board[i][j]);
        }
        printf("|\n");
    }
    for (int j = 0; j < MAX_MAP_HEIGHT; j++) {
        printf("+---");
    }
    printf("+\n");
}

void loadMap(char map[MAX_MAP_HEIGHT][MAX_LINE_WIDTH]) {
    FILE* fs;
    fs = fopen("..\\crosswords\\Countries&Languages\\map.txt", "r");

    if (fs == NULL) {
        printf("Failed to read map file\n");
        return;
    }

    char str[1000];

    for (int i = 0; i < MAX_MAP_HEIGHT; ++i) {
        if (fgets(str, sizeof(str), fs) != NULL) {
            strncpy(map[i], str, MAX_LINE_WIDTH - 1);
            map[i][MAX_LINE_WIDTH - 1] = '\0';
        }
        else {
            map[i][0] = '\0'; 
        }
    }

    fclose(fs);
}

void loadDescription(char description[MAX_DESCRIPTION_LENGTH]) {

    setlocale(LC_ALL, "ko_KR.UTF-8");

    FILE* fs;
    fs = fopen("..\\crosswords\\Countries&Languages\\description.txt", "r");

    if (fs == NULL) {
        printf("Failed to read map file\n");
        return;
    }

    char str[1000];

    if (fgets(str, sizeof(str), fs) != NULL) {
        strncpy(description, str, MAX_LINE_WIDTH - 1);
        description[MAX_DESCRIPTION_LENGTH - 1] = '\0';
    }

    fclose(fs);
}

void displayMap(int gameMode, char map[MAX_MAP_HEIGHT][MAX_LINE_WIDTH]) {
    if (gameMode == 1) {
        printf("normal crossword puzzle\n");
    }
    else {
        printf("normal crossword puzzle\n");
    }

    for (int i = 0; i < MAX_MAP_HEIGHT; ++i) {
        printf("%s\n", map[i]);
    }
}

void displayDescription(char description[MAX_DESCRIPTION_LENGTH]) {
    printf("%s\n", description);
}

int main() {
    int gameMode = 0;

    while (1) {
        showIntro();
        int input;
        scanf("%d", &input);

        if (input == 1 || input == 2) {
            gameMode = input;
            break;
        }
        else {
            system("cls");
        }
    }
    
    char map[MAX_LINE_WIDTH][MAX_LINE_WIDTH];
    char description[MAX_DESCRIPTION_LENGTH];
    memset(description, 0, MAX_DESCRIPTION_LENGTH);

    loadMap(map);
    loadDescription(description);
    /*
    while (1) {
        system("cls");
        displayMap(gameMode, map);
        printf("\n\n");
        displayDescription(description);
    }
    */
    return 0;
}