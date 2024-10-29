#include <stdio.h>
#include <math.h>
#include <windows.h>

#define WIDTH 80
#define HEIGHT 40

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct {
    float x, y, w; // 위치 및 동차 좌표
} vec3;

typedef struct {
    float m[3][3]; // 3x3 행렬
} Matrix3x3;

void clearScreen() {
    system("cls"); // Windows에서 콘솔 화면 지우기
}

vec3 Mul(vec3 a, Matrix3x3 b) {
    vec3 result;
    result.x = a.x * b.m[0][0] + a.y * b.m[1][0] + a.w * b.m[2][0];
    result.y = a.x * b.m[0][1] + a.y * b.m[1][1] + a.w * b.m[2][1];
    result.w = a.x * b.m[0][2] + a.y * b.m[1][2] + a.w * b.m[2][2];
    return result;
}

Matrix3x3 Mul(Matrix3x3 a, Matrix3x3 b) {
    Matrix3x3 result = { 0 };
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                result.m[i][j] += a.m[i][k] * b.m[k][j];
            }
        }
    }
    return result;
}

void draw(int angleO1, int angleO2) {
    clearScreen();

    int pX = WIDTH / 2;
    int pY = HEIGHT / 2;

    float radianO1 = angleO1 * M_PI / 180; // O1의 각도를 라디안으로 변환
    float radianO2 = angleO2 * M_PI / 180; // O2의 각도를 라디안으로 변환

    // O1의 회전 행렬
    Matrix3x3 rotationMatrixO1 = {
        {
            {cos(radianO1), -sin(radianO1), 0},
            {sin(radianO1), cos(radianO1), 0},
            {0, 0, 1}
        }
    };

    // O2의 회전 행렬
    Matrix3x3 rotationMatrixO2 = {
        {
            {cos(radianO2), -sin(radianO2), 0},
            {sin(radianO2), cos(radianO2), 0},
            {0, 0, 1}
        }
    };

    // O1의 초기 위치 (10, 0, 1)
    vec3 oPosition1 = { 10, 0, 1 };

    // O2의 초기 위치 (5, 0, 1) - O1을 중심으로 회전
    vec3 oPosition2 = { 5, 0, 1 };

    // O1의 새로운 위치 계산
    oPosition1 = Mul(oPosition1, rotationMatrixO1);
    // O2의 새로운 위치 계산 (O1을 중심으로 회전)
    oPosition2 = Mul(oPosition2, rotationMatrixO2);

    // O1의 최종 위치
    int o1X = (int)(pX + oPosition1.x);
    int o1Y = (int)(pY + oPosition1.y);

    // O2의 최종 위치 (O1의 위치를 기준으로)
    int o2X = (int)(pX + oPosition2.x + oPosition1.x);
    int o2Y = (int)(pY + oPosition2.y + oPosition1.y);

    // 화면 그리기
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == pX && y == pY) {
                printf("태양"); // 태양 출력
            }
            else if (x == o1X && y == o1Y) {
                printf("지구"); // 지구 출력
            }
            else if (x == o2X && y == o2Y) {
                printf("달"); // 달 출력
            }
            else {
                printf("  "); // 빈 공간 표시
            }
        }
        printf("\n");
    }
}#include <stdio.h>
#include <math.h>
#include <windows.h>

#define WIDTH 80
#define HEIGHT 40

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct {
    float x, y, w; // 위치 및 동차 좌표
} vec3;

typedef struct {
    float m[3][3]; // 3x3 행렬
} Matrix3x3;

void clearScreen() {
    system("cls"); // Windows에서 콘솔 화면 지우기
}

vec3 Mul(vec3 a, Matrix3x3 b) {
    vec3 result;
    result.x = a.x * b.m[0][0] + a.y * b.m[1][0] + a.w * b.m[2][0];
    result.y = a.x * b.m[0][1] + a.y * b.m[1][1] + a.w * b.m[2][1];
    result.w = a.x * b.m[0][2] + a.y * b.m[1][2] + a.w * b.m[2][2];
    return result;
}

Matrix3x3 Mul(Matrix3x3 a, Matrix3x3 b) {
    Matrix3x3 result = { 0 };
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                result.m[i][j] += a.m[i][k] * b.m[k][j];
            }
        }
    }
    return result;
}

void draw(int angleO1, int angleO2) {
    clearScreen();

    int pX = WIDTH / 2;
    int pY = HEIGHT / 2;

    float radianO1 = angleO1 * M_PI / 180; // O1의 각도를 라디안으로 변환
    float radianO2 = angleO2 * M_PI / 180; // O2의 각도를 라디안으로 변환

    // O1의 회전 행렬
    Matrix3x3 rotationMatrixO1 = {
        {
            {cos(radianO1), -sin(radianO1), 0},
            {sin(radianO1), cos(radianO1), 0},
            {0, 0, 1}
        }
    };

    // O2의 회전 행렬
    Matrix3x3 rotationMatrixO2 = {
        {
            {cos(radianO2), -sin(radianO2), 0},
            {sin(radianO2), cos(radianO2), 0},
            {0, 0, 1}
        }
    };

    // O1의 초기 위치 (10, 0, 1)
    vec3 oPosition1 = { 10, 0, 1 };

    // O2의 초기 위치 (5, 0, 1) - O1을 중심으로 회전
    vec3 oPosition2 = { 5, 0, 1 };

    // O1의 새로운 위치 계산
    oPosition1 = Mul(oPosition1, rotationMatrixO1);
    // O2의 새로운 위치 계산 (O1을 중심으로 회전)
    oPosition2 = Mul(oPosition2, rotationMatrixO2);

    // O1의 최종 위치
    int o1X = (int)(pX + oPosition1.x);
    int o1Y = (int)(pY + oPosition1.y);

    // O2의 최종 위치 (O1의 위치를 기준으로)
    int o2X = (int)(pX + oPosition2.x + oPosition1.x);
    int o2Y = (int)(pY + oPosition2.y + oPosition1.y);

    // 화면 그리기
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == pX && y == pY) {
                printf("태양"); // 태양 출력
            }
            else if (x == o1X && y == o1Y) {
                printf("자구"); // 지구 출력
            }
            else if (x == o2X && y == o2Y) {
                printf("달"); // 달 출력
            }
            else {
                printf("  "); // 빈 공간 표시
            }
        }
        printf("\n");
    }
}

int main() {
    for (int angle = 0; angle < 360; angle += 10) { // 각도 증가폭
        draw(angle, angle * 2); // O1과 O2의 각도
        Sleep(50); // 0.05초 대기
    }
    return 0;
}


int main() {
    for (int angle = 0; angle < 360; angle += 10) { // 각도 증가폭
        draw(angle, angle * 2); // O1과 O2의 각도
        Sleep(50); // 0.05초 대기
    }
    return 0;
}
