#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 7  // ����� ��������� �������(���� ���� �������)  

int matrix[N][N];

// ������� ��������� ���������� ����� � �������� �������
int RandomRange(const int min, const int max) {
    return rand() % (max - min + 1) + min;
}

// ���������� ������� ����������� ����������
void FillRandomMatrix() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = RandomRange(-100, 100);
        }
    }
}

// ���� ������� �� �����
void PrintMatrix() {
    printf("\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// ���������� ������� ������� ������� �����
void ShellSortMainDiagonal() {
    int temp;
    for (int gap = N / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < N; i++) {
            temp = matrix[i][i];
            int j;
            for (j = i; j >= gap && matrix[j - gap][j - gap] > temp; j -= gap) {
                matrix[j][j] = matrix[j - gap][j - gap];
            }
            matrix[j][j] = temp;
        }
    }
}

int main(void) {
    srand((unsigned int)time(NULL));

    FillRandomMatrix();

    printf("��������� �������:");
    PrintMatrix();

    ShellSortMainDiagonal();

    printf("������� ���� ���������� ������� �������:");
    PrintMatrix();

    return 0;
}