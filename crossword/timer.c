#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // sleep() 함수 사용
#include <pthread.h> // pthread 라이브러리

pthread_mutex_t time_mutex = PTHREAD_MUTEX_INITIALIZER; // 타이머를 보호할 뮤텍스

// 시간을 1초씩 감소시키는 함수
void* decreaseTime(void* arg) {
    while (scoreTimeRecord.time > 0) {
        sleep(1);  // 1초 대기
        pthread_mutex_lock(&time_mutex); // 시간 감소를 안전하게 하기 위해 뮤텍스 잠금
        scoreTimeRecord.time -= 1;  // 시간 감소
        pthread_mutex_unlock(&time_mutex); // 뮤텍스 해제
    }
    return NULL;
}