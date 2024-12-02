// crossword.c : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <ctype.h>
#include <stdbool.h>

#define EXIT 0
#define GAME_MODE_NORMAL 1
#define GAME_MODE_TIME_ATTACK 2

#define MAX_MAP_HEIGHT 31
#define MAX_LINE_WIDTH 62
#define MAX_DESCRIPTION_LENGTH 1000
#define NUM_OF_WORDS 16

#define ACROSS 0
#define DOWN 1

#define MAP_CNT 3
#define MAX_DIR_LENGTH 100
#define Animals "..\\crosswords\\Animals\\"
#define CountriesNLanguages "..\\crosswords\\Countries&Languages\\"
#define Jobs "..\\crosswords\\Jobs\\"


void gotoxy(int x, int y) {
    COORD pos = { x, y };
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(output, pos);
}

char* toUpperCase(char str[]) {
    int i = 0;
    while (str[i] != '\0') {
        str[i] = toupper((unsigned char)str[i]);
        i++;
    }
    str[i] = '\0';
    return str;
}

bool isAlphabet(char c) {
    return (c >= 'A' && c <= 'Z');
}

void sendAltEnter() {
    keybd_event(VK_MENU, 0, 0, 0);    // Alt 키 누르기
    keybd_event(VK_RETURN, 0, 0, 0);  // Enter 키 누르기
    keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);  // Enter 키 떼기
    keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);     // Alt 키 떼기
}

// intro
void showIntro(int* gameMode) {
    while (1) {
        system("cls"); // clear display

        printf("crossword game\n"
            "press 1 to play normal crossword game\n"
            "press 2 to play time attack crossword game\n"
            "press 0 to exit game\n");

        int input;
        scanf("%d", &input);

        if (input == GAME_MODE_NORMAL || input == GAME_MODE_TIME_ATTACK) {
            *gameMode = input;
            break;
        }
        else if (input == EXIT) {
            exit(-1);
        }
    }
}

// initialize functions
void initializeDirections(char* directions) {
    for (int i = 1; i <= NUM_OF_WORDS; ++i) {
        if (i % 2 == 0) directions[i] = ACROSS;
        else directions[i] = DOWN;
    }
}

void initializeSheet(char map[MAX_MAP_HEIGHT][MAX_LINE_WIDTH], char sheet[MAX_LINE_WIDTH][MAX_LINE_WIDTH]) {
    for (int i = 0; i < MAX_MAP_HEIGHT; ++i) {
        for (int j = 0; j < MAX_LINE_WIDTH; ++j) {
            sheet[i][j] = map[i][j];
        }
    }
    return;
}

// load funtcions
void loadMap(char map[MAX_MAP_HEIGHT][MAX_LINE_WIDTH], char sheet[MAX_LINE_WIDTH][MAX_LINE_WIDTH], const char* mapDir) {
    FILE* fs;

    const char fullPath[MAX_DIR_LENGTH];
    snprintf(fullPath, sizeof(fullPath), "%smap.txt", mapDir);
    fs = fopen(fullPath, "r");

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

void loadDescription(char description[MAX_DESCRIPTION_LENGTH], const char* mapDir) {

    FILE* fs;

    const char fullPath[MAX_DIR_LENGTH];
    snprintf(fullPath, sizeof(fullPath), "%sdescription_eng.txt", mapDir);
    fs = fopen(fullPath, "r");

    if (fs == NULL) {
        printf("Failed to read description file\n");
        return;
    }

    char str[MAX_DESCRIPTION_LENGTH];
    memset(str, 0, MAX_DESCRIPTION_LENGTH); // str을 빈 문자열로 초기화

    while (fgets(str, MAX_DESCRIPTION_LENGTH, fs) != NULL) {
        // str을 description에 이어 붙이기
        strncat(description, str, MAX_DESCRIPTION_LENGTH - strlen(description) - 1);
    }

    fclose(fs);
}

void loadAnswers(char answers[NUM_OF_WORDS + 1][15], const char* mapDir) {
    FILE* fs;

    const char fullPath[MAX_DIR_LENGTH];
    snprintf(fullPath, sizeof(fullPath), "%sanswer.txt", mapDir);
    fs = fopen(fullPath, "r");

    if (fs == NULL) {
        printf("Failed to read map file\n");
        return;
    }

    char str[1000];
    int idx = 1;

    while (fgets(str, sizeof(str), fs) != NULL && idx <= NUM_OF_WORDS) {
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

// display functions
void displayMap(int gameMode, char map[MAX_MAP_HEIGHT][MAX_LINE_WIDTH]) {
    gotoxy(0, 0);

    if (gameMode == 1) {
        printf("original map\n");
    }
    else {
        printf("normal crossword puzzle\n");
    }

    for (int i = 0; i < MAX_MAP_HEIGHT; ++i) {
        printf("%s\n", map[i]);
    }
}

void displaySheet(int gameMode, char sheet[MAX_MAP_HEIGHT][MAX_LINE_WIDTH]) {
    gotoxy(0, 0);

    if (gameMode == 1) {
        printf("input sheet  \n");
    }
    else {
        printf("normal crossword puzzle\n");
    }

    for (int i = 0; i < MAX_MAP_HEIGHT; ++i) {
        printf("%s\n", sheet[i]);
    }
}

void displayDescription(char description[MAX_DESCRIPTION_LENGTH]) {
    int x = 70;
    int y = 5;
    char* line = description;

    // description에서 줄마다 개행을 기준으로 나누어 출력
    while (*line != '\0') {
        gotoxy(x, y++);

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

void displayMenu() {
    // 메뉴 출력
    gotoxy(0, 33);
    printf("---- Menu ----\n\n"
        "1. 단어 입력\n"
        "2. 단어 삭제\n"
        "3. 전체 삭제\n"
        "4. 맵 보기\n"
        "5. 힌트 보기\n\n"
        "--------------");
}

// write word function
void writeWordOnSheet(char map[MAX_MAP_HEIGHT][MAX_LINE_WIDTH], 
                      char sheet[MAX_MAP_HEIGHT][MAX_LINE_WIDTH], 
                      char inputWords[NUM_OF_WORDS + 1][15],
                      char directions[NUM_OF_WORDS + 1], 
                      int lineNum, 
                      char* inputStr) {

    int lengthOfWord = strlen(inputStr);
    int dir = -1;
    int startY, startX;

    bool two_digit = ((lineNum / 10) > 0) ? true : false;

    strcpy(inputWords[lineNum], inputStr);

    for (int i = 0; i < MAX_MAP_HEIGHT; ++i) {
        for (int j = 0; j < MAX_LINE_WIDTH; ++j) {
            // 단어 번호를 찾았다면
            if (two_digit && map[i][j - 1] == (char)(lineNum / 10 + '0') && map[i][j] == (char)(lineNum % 10 + '0') ||
                !two_digit && map[i][j] == (char)(lineNum + '0')) {

                dir = directions[lineNum];
                startY = i;
                startX = j;
                break;
            }
        }

        if (dir != -1) break;
    }

    if (dir == ACROSS) {
        int idx = startX;

        for (int i = 0; i < lengthOfWord; ++i) {
            // 추가하고자 하려는 칸에 숫자가 있을 경우
            if (sheet[startY][idx] >= '0' && sheet[startY][idx] <= '9') {

                if (i == 0) {
                    sheet[startY][idx - 1] = ' ';
                    sheet[startY][idx] = inputStr[i];
                    sheet[startY][idx + 1] = ' ';
                    idx += 4;
                    continue;
                }

                // 두 자리 숫자의 경우
                if (i != 0 && two_digit) {
                    sheet[startY][idx + 1] = inputStr[i];
                }
                else {
                    sheet[startY][idx + 1] = inputStr[i];
                }
            }
            else {
                sheet[startY][idx] = inputStr[i];
            }
            idx += 4;
        }
        return;
    }

    if (dir == DOWN) {
        int idx = startY;

        for (int i = 0; i < lengthOfWord; ++i) {
            // 추가하고자 하려는 칸에 숫자가 있을 경우
            if (sheet[idx][startX] >= '0' && sheet[idx][startX] <= '9') {

                if (i == 0) {
                    sheet[idx][startX - 1] = ' ';
                    sheet[idx][startX] = inputStr[i];
                    sheet[idx][startX + 1] = ' ';
                    idx += 2;
                    continue;
                }

                // 두 자리 숫자의 경우
                if (i != 0 && two_digit) {
                    sheet[idx][startX + 1] = inputStr[i];
                }
                else {
                    sheet[idx][startX + 1] = inputStr[i];
                }
            }
            else {
                sheet[idx][startX] = inputStr[i];
            }
            idx += 2;
        }
        return;
    }
}

// write hint function
void writeHintOnSheet(char map[MAX_MAP_HEIGHT][MAX_LINE_WIDTH],
    char sheet[MAX_MAP_HEIGHT][MAX_LINE_WIDTH],
    char answers[NUM_OF_WORDS + 1][15],
    char directions[NUM_OF_WORDS + 1],
    bool hintUsedLines[NUM_OF_WORDS],
    int lineNum
) {


    srand((unsigned int)time(NULL));

    int lengthOfWord = strlen(answers[lineNum]);
    int dir = -1;
    int startY, startX;

    bool two_digit = ((lineNum / 10) > 0) ? true : false;

    if (hintUsedLines[lineNum] == true)
    {
        printf("이미 힌트를 사용한 문제입니다.");
        Sleep(2000);
    }
    else {
        hintUsedLines[lineNum] = true;
        for (int i = 0; i < lengthOfWord; i++) {
            if (rand() % 2 == 0) {
                char hint = answers[lineNum][i];

                for (int i = 0; i < MAX_MAP_HEIGHT; ++i) {
                    for (int j = 0; j < MAX_LINE_WIDTH; ++j) {
                        // 단어 번호를 찾았다면
                        if (two_digit && map[i][j - 1] == (char)(lineNum / 10 + '0') && map[i][j] == (char)(lineNum % 10 + '0') ||
                            !two_digit && map[i][j] == (char)(lineNum + '0')) {

                            dir = directions[lineNum];
                            startY = i;
                            startX = j;
                            break;
                        }
                    }

                    if (dir != -1) break;
                }

                if (dir == ACROSS) {
                    int idx = startX + i * 4;

                    // 추가하고자 하려는 칸에 숫자가 있을 경우
                    if (sheet[startY][idx] >= '0' && sheet[startY][idx] <= '9') {

                        if (i == 0) {
                            sheet[startY][idx - 1] = ' ';
                            sheet[startY][idx] = hint;
                            sheet[startY][idx + 1] = ' ';
                            idx += 4;
                            continue;
                        }

                        // 두 자리 숫자의 경우
                        if (i != 0 && two_digit) {
                            sheet[startY][idx + 1] = hint;
                        }
                        else {
                            sheet[startY][idx + 1] = hint;
                        }
                    }
                    else {
                        sheet[startY][idx] = hint;
                    }
                }

                if (dir == DOWN) {
                    int idx = startY + i * 2;

                    // 추가하고자 하려는 칸에 숫자가 있을 경우
                    if (sheet[idx][startX] >= '0' && sheet[idx][startX] <= '9') {

                        if (i == 0) {
                            sheet[idx][startX - 1] = ' ';
                            sheet[idx][startX] = hint;
                            sheet[idx][startX + 1] = ' ';
                            idx += 2;
                            continue;
                        }

                        // 두 자리 숫자의 경우
                        if (i != 0 && two_digit) {
                            sheet[idx][startX + 1] = hint;
                        }
                        else {
                            sheet[idx][startX + 1] = hint;
                        }
                    }
                    else {
                        sheet[idx][startX] = hint;
                    }
                }
            }
        }
    }
    return;
}

// delete word function
void deleteWordFromSheet(char map[MAX_MAP_HEIGHT][MAX_LINE_WIDTH], 
                         char sheet[MAX_MAP_HEIGHT][MAX_LINE_WIDTH], 
                         char inputWords[NUM_OF_WORDS + 1][15],
                         char directions[NUM_OF_WORDS + 1], 
                         int lineNum,
                         int lengthOfWord) {
    int dir = -1;
    int startY, startX;

    memset(inputWords[lineNum], 0, 15);

    bool two_digit = ((lineNum / 10) > 0) ? true : false;

    for (int i = 0; i < MAX_MAP_HEIGHT; ++i) {
        for (int j = 0; j < MAX_LINE_WIDTH; ++j) {
            // 단어 번호를 찾았다면
            if (two_digit && map[i][j - 1] == (char)(lineNum / 10 + '0') && map[i][j] == (char)(lineNum % 10 + '0') ||
                !two_digit && map[i][j] == (char)(lineNum + '0')) {

                dir = directions[lineNum];
                startY = i;
                startX = j;
            }
        }

        if (dir != -1) break;
    }

    if (dir == ACROSS) {
        int idx = startX - 1;

        for (int i = idx; i < idx + 4*lengthOfWord; ++i) {
            sheet[startY][i] = map[startY][i];
        }
        return;
    }

    if (dir == DOWN) {
        int idx = startY;

        for (int i = idx; i < idx + 2 * lengthOfWord; ++i) {
            sheet[i][startX] = map[i][startX];
        }
        return;
    }
}

int getInput() {
    gotoxy(0, 43);
    printf("메뉴 중 하나를 선택하세요.\n");

    int selectedOption = 0;
    scanf("%d", &selectedOption);
    return selectedOption;
}

bool checkCorrect(char answers[NUM_OF_WORDS + 1][15], char inputWords[NUM_OF_WORDS + 1][15]) {
    for (int i = 1; i <= NUM_OF_WORDS; ++i) {
        if (strcmp(answers[i], inputWords[i]) != 0) return false;
    }
    return true;
}

void update(char map[MAX_MAP_HEIGHT][MAX_LINE_WIDTH],
            char sheet[MAX_MAP_HEIGHT][MAX_LINE_WIDTH],
            char description[MAX_DESCRIPTION_LENGTH],
            char answers[NUM_OF_WORDS + 1][15],
            char inputWords[NUM_OF_WORDS + 1][15],
            char directions[NUM_OF_WORDS + 1],
            bool hintUsedLines[NUM_OF_WORDS],
            int gameMode) {
    while (1) {
        system("cls"); // clear display

        // display
        displaySheet(gameMode, sheet);
        displayDescription(description);
        displayMenu();

        // get input
        int selectedOption = getInput();
        int toggle = 1; // 상태 토글용 변수

        switch (selectedOption) {
        case 1:
            printf("1번 선택됨: 단어 입력\n");
            int lineNum = 0;
            char inputStr[100] = { 0 };
            printf("입력하고자 하는 라인의 번호를 누르세요: ");
            scanf("%d", &lineNum);
            if (lineNum < 1 || lineNum  > NUM_OF_WORDS) {
                printf("1에서 16중의 숫자 중에 하나를 고르세요\n");
                Sleep(2000);
            }
            else {
                printf("입력하고자 하는 라인의 단어를 누르세요: ");
                scanf("%s", &inputStr);

                // 글자 수가 맞지 않을 경우 알림
                if (strlen(answers[lineNum]) != strlen(inputStr)) {
                    printf("글자 수가 올바르지 않습니다.");
                    Sleep(1000);
                }
                else {
                    writeWordOnSheet(map, sheet, inputWords, directions, lineNum, toUpperCase(inputStr));
                }
            }
            break;
        case 2:
            printf("2번 선택됨: 단어 삭제\n");
            int lineToDelete = 0;
            printf("삭제하고자 하는 라인의 번호를 누르세요: ");
            scanf("%d", &lineToDelete);
            deleteWordFromSheet(map, sheet, inputWords, directions, lineToDelete, strlen(answers[lineToDelete]));
            break;
        case 3:
            printf("3번 선택됨: 전체 삭제\n");
            initializeSheet(map, sheet);
            break;
        case 4:
            printf("4번 선택됨: 맵 보기\n");
            if (toggle) {
                displayMap(GAME_MODE_NORMAL, map);
                printf("다시 원래 sheet로 돌아가기 원한다면 4를 누르세요.\n");
                Sleep(2000);
            }
            else {
                displaySheet(GAME_MODE_NORMAL, sheet);
            }
            toggle = !toggle;
            break;
        case 5:
            printf("5번 선택됨: 힌트\n");
            int hintNum = 0;
            printf("입력하고자 하는 라인의 번호를 누르세요: ");
            scanf("%d", &hintNum);
            lineNum = hintNum;
            if (lineNum < 1 || lineNum > NUM_OF_WORDS) {
                printf("1에서 16중의 숫자 중에 하나를 고르세요\n");
                Sleep(2000);
            }
            else {
                writeHintOnSheet(map, sheet, answers, directions, hintUsedLines, lineNum);
            }
            break;
        default:
            printf("보기에 선택된 번호만을 골라주세요.");
            Sleep(1000);
            break;
        }

        if (checkCorrect(answers, inputWords)) {
        //if (true) {
            printf("축하합니다 모든 단어를 다 맞추셨습니다.\n"
                    "5초 뒤에 로비로 돌아가게 됩니다.\n");
            Sleep(5000);
            return;
        };

        Sleep(1000); // delay
    }
}

int main() {
    // 전체화면 키우기
    sendAltEnter();

    while (1) {
        // variables
        int gameMode = 0;

        char map[MAX_LINE_WIDTH][MAX_LINE_WIDTH]; // 오리지널 맵, 수정 불가
        char sheet[MAX_LINE_WIDTH][MAX_LINE_WIDTH]; // 이용자가 문자를 입력하는 맵

        char description[MAX_DESCRIPTION_LENGTH];
        memset(description, 0, MAX_DESCRIPTION_LENGTH);

        char answers[NUM_OF_WORDS + 1][15];
        char inputWords[NUM_OF_WORDS + 1][15];

        bool hintUsedLines[NUM_OF_WORDS];
        memset(hintUsedLines, 0, NUM_OF_WORDS);

        char directions[NUM_OF_WORDS + 1];
        memset(directions, 0, 16);

        showIntro(&gameMode);
        if (gameMode == GAME_MODE_TIME_ATTACK) {
            // 타임 어택을 위한 추가적인 함수 필요

            /*
                제한 시간 설정 => 고정적으로
            */
        }

        // map을 랜덤히 뽑는 알고리즘 필요 <- 토요일까지 구현

        const char* maps[MAP_CNT] = {Animals, CountriesNLanguages, Jobs};
        int mapIdx = rand() % MAP_CNT;
        mapIdx = 2; // 임시로 작성
        const char* dir = maps[mapIdx];

        // load
        loadMap(map, sheet, dir);
        loadDescription(description, dir);
        loadAnswers(answers, dir);

        // initialize
        initializeDirections(directions);
        initializeSheet(map, sheet);

        // update
        update(map, sheet, description, answers, inputWords, directions, hintUsedLines, gameMode);
    }
    
    return 0;
}