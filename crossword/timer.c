#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // sleep() �Լ� ���
#include <pthread.h> // pthread ���̺귯��

pthread_mutex_t time_mutex = PTHREAD_MUTEX_INITIALIZER; // Ÿ�̸Ӹ� ��ȣ�� ���ؽ�

// �ð��� 1�ʾ� ���ҽ�Ű�� �Լ�
void* decreaseTime(void* arg) {
    while (scoreTimeRecord.time > 0) {
        sleep(1);  // 1�� ���
        pthread_mutex_lock(&time_mutex); // �ð� ���Ҹ� �����ϰ� �ϱ� ���� ���ؽ� ���
        scoreTimeRecord.time -= 1;  // �ð� ����
        pthread_mutex_unlock(&time_mutex); // ���ؽ� ����
    }
    return NULL;
}