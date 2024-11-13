// crossword.c : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <ctype.h>

#define GAME_MODE_NORMAL 1
#define GAME_MODE_TIME_ATTACK 2

#define MAX_MAP_HEIGHT 31
#define MAX_LINE_WIDTH 62
#define MAX_DESCRIPTION_LENGTH 1000
#define NUM_OF_WORDS 16

void gotoxy(int x, int y) {
    COORD pos = { x, y };
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(output, pos);
}

char* toUpperCase(char str[]) {
    int i = 0;
    char temp[100];
    while (str[i] != '\0') {
        str[i] = toupper((unsigned char)str[i]);
        i++;
    }
    return temp;
}

void showIntro() {
    printf("crossword game\n");
    printf("press 1 to play normal crossword game\n");
    printf("press 2 to play time attack crossword game\n");
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

    FILE* fs;
    fs = fopen("..\\crosswords\\Countries&Languages\\description_eng.txt", "r");

    if (fs == NULL) {
        printf("Failed to read map file\n");
        return;
    }

    char str[MAX_DESCRIPTION_LENGTH];
    description[0] = '\0';  // description을 빈 문자열로 초기화

    while (fgets(str, MAX_DESCRIPTION_LENGTH, fs) != NULL) {
        // str을 description에 이어 붙이기
        strncat(description, str, MAX_DESCRIPTION_LENGTH - strlen(description) - 1);
    }

    fclose(fs);
}

void loadAnswers(char answers[NUM_OF_WORDS][15]) {
    FILE* fs;
    fs = fopen("..\\crosswords\\Countries&Languages\\answer.txt", "r");

    if (fs == NULL) {
        printf("Failed to read map file\n");
        return;
    }

    char str[1000];
    int idx = 0;

    while (fgets(str, sizeof(str), fs) != NULL && idx < NUM_OF_WORDS) {
        char* start_of_word = strchr(str, ' ') + 1;  // 첫 번째 공백 이후의 단어 시작 위치
        if (start_of_word != NULL) {
            // 끝의 개행 문자 제거
            start_of_word[strcspn(start_of_word, "\n")] = '\0'; // strcspn(): 문자열에서 다른 문자열에 포함되어 있는 문자들을 검색하되, 첫번째로 일치하는 문자에 도달하기 까지 읽어들인 문자의 개수를 리턴
            // 단어를 answers 배열에 저장
            strcpy(answers[idx++], start_of_word);
        }
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
    int y = 5;
    char* line = description;

    // description에서 줄마다 개행을 기준으로 나누어 출력
    while (*line != '\0') {
        gotoxy(70, y++);

        char* end_of_line = strchr(line, '\n'); // strchr() 함수는 string의 문자로 변환되는 c의 첫 번째 표시에 대한 포인터를 리턴
        if (end_of_line != NULL) {
            // 개행 문자가 있으면 그 전까지 출력
            *end_of_line = '\0';  // 개행 문자를 제거하고 출력
            printf("%s", line);
            *end_of_line = '\n';  // 다시 원래대로 복구
            line = end_of_line + 1;  // 개행 이후로 넘어감
        }
        else {
            // 더 이상 개행 문자가 없으면 남은 텍스트를 출력
            printf("%s", line);
            break;
        }
    }
}

int main() {
    int gameMode = 0;

    while (1) {
        showIntro();
        int input;
        scanf("%d", &input);

        if (input == GAME_MODE_NORMAL || input == GAME_MODE_TIME_ATTACK) {
            gameMode = input;
            break;
        }
        else {
            system("cls");
        }
    }
    
    char map[MAX_LINE_WIDTH][MAX_LINE_WIDTH];
    char description[MAX_DESCRIPTION_LENGTH];
    char answers[NUM_OF_WORDS][15];

    if (gameMode == GAME_MODE_TIME_ATTACK) {
        // 타임 어택을 위한 추가적인 함수 필요
    }

    // map을 랜덤히 뽑는 알고리즘 필요

    // load
    loadMap(map);
    loadDescription(description);
    loadAnswers(answers);

    while (1) {
        system("cls"); // clear map

        // display
        displayMap(gameMode, map);
        displayDescription(description);

        // get input
        int input = 0;
        char inputStr[100] = { 0 };
        gotoxy(0, MAX_MAP_HEIGHT + 3);

        printf("입력하고자 하는 라인의 번호를 누르세요: ");
        scanf("%d", &input);
        if (input < 1 || input > NUM_OF_WORDS) {
            printf("1에서 16중의 숫자 중에 하나를 고르세요\n");
            Sleep(2000);
        }
        else {
            printf("입력하고자 하는 라인의 단어를 누르세요: ");
            scanf("%s", &inputStr);

            // 추가적인 작업 필요
            if (strcmp(answers[input], toUpperCase(inputStr)) == 0) printf("correct");
            else printf("wrong");
        }
        
        Sleep(1000); // delay
    }
    
    return 0;
}