#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <math.h>
#include <string.h>

#define WIDTH 40
#define HEIGHT 24
#define CIRCLE_SEGMENTS 16

typedef struct {
    float x, y, z;
} Vector3;

typedef struct {
    float m[3][3];
} Matrix3x3;

typedef struct {
    Vector3* points;
    int num_points;
} Shape;

typedef struct {
    Shape shape;
    Vector3 position;
    float rotation_angle;
    float orbit_angle;
    float rotation_speed;
    float orbit_speed;
    float orbit_radius;
    char symbol[3];
    int rotation_direction;
    Vector3 orbit_center;
} Planet;

// Global variables
char screen[HEIGHT][WIDTH];
Planet sun, earth, moon;
int animation_state = 0; // 상태를 0으로 시작, 0: 기본 출력, 1: 움직임

// Math utility functions
float deg_to_rad(float degrees) {
    return degrees * (3.14159265359f / 180.0f);
}

Vector3 create_vector3(float x, float y, float z) {
    Vector3 v = { x, y, z };
    return v;
}

// Matrix functions
Matrix3x3 rotation_matrix(float theta) {
    float rad = deg_to_rad(theta);
    float c = cosf(rad);
    float s = sinf(rad);
    Matrix3x3 m = { {{c, -s, 0.0f}, {s, c, 0.0f}, {0.0f, 0.0f, 1.0f}} };
    return m;
}

// Shape creation
Shape create_circle(float radius, int segments) {
    Shape shape = { 0 };
    shape.num_points = segments;
    shape.points = (Vector3*)malloc(sizeof(Vector3) * segments);

    float angle_step = 360.0f / segments;
    for (int i = 0; i < segments; i++) {
        float angle = deg_to_rad(i * angle_step);
        shape.points[i] = create_vector3(cosf(angle) * radius, sinf(angle) * radius, 1);
    }

    return shape;
}

// Initialization
void init_planets() {
    // 태양
    sun.shape = create_circle(3.0f, CIRCLE_SEGMENTS);
    sun.position = create_vector3(WIDTH / 2, HEIGHT / 2, 1);
    strcpy_s(sun.symbol, sizeof(sun.symbol), "SS");
    sun.rotation_speed = -120.0f; // 반시계방향 자전
    sun.orbit_speed = 0.0f; // 공전 없음
    sun.orbit_center = sun.position;
    sun.rotation_angle = 0.0f;
    sun.orbit_angle = 0.0f;

    // 지구
    earth.shape = create_circle(2.0f, CIRCLE_SEGMENTS);
    earth.position = create_vector3(WIDTH / 2 + 12, HEIGHT / 2, 1);
    earth.orbit_radius = 12.0f;
    earth.orbit_speed = 120.0f; // 시계방향 공전
    earth.rotation_speed = 180.0f; // 시계방향 자전
    strcpy_s(earth.symbol, sizeof(earth.symbol), "EE");
    earth.orbit_center = sun.position;
    earth.rotation_angle = 0.0f;
    earth.orbit_angle = 0.0f;

    // 달
    moon.shape = create_circle(1.5f, CIRCLE_SEGMENTS);
    moon.position = create_vector3(WIDTH / 2 + 16, HEIGHT / 2, 1);
    moon.orbit_radius = 4.0f;
    moon.orbit_speed = -180.0f; // 반시계방향 공전
    moon.rotation_speed = 360.0f; // 시계방향 자전
    strcpy_s(moon.symbol, sizeof(moon.symbol), "MM");
    moon.orbit_center = earth.position;
    moon.rotation_angle = 0.0f;
    moon.orbit_angle = 0.0f;
}

void cleanup_planets() {
    free(sun.shape.points);
    free(earth.shape.points);
    free(moon.shape.points);
}

// Screen functions
void clear_screen() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            screen[y][x] = ' ';
        }
    }
}

void draw_pixel(int x, int y, char symbol) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        screen[y][x] = symbol;
    }
}

void draw_filled_circle(Vector3 position, float radius, char symbol) {
    for (int y = (int)(position.y - radius); y <= (int)(position.y + radius); y++) {
        for (int x = (int)(position.x - radius); x <= (int)(position.x + radius); x++) {
            float distance = sqrtf(powf(x - position.x, 2) + powf(y - position.y, 2));
            if (distance <= radius) {
                draw_pixel(x, y, symbol);
            }
        }
    }
}

void render_screen() {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD cursorPosition = { 0, 0 };
    SetConsoleCursorPosition(console, cursorPosition);

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            printf("%c", screen[y][x]);
        }
        printf("\n");
    }
}

// Update function
void update_planet(Planet* planet, float delta_time, Vector3 orbit_center) {
    // 행성 공전
    planet->orbit_angle += planet->orbit_speed * delta_time;
    float orbit_rad = deg_to_rad(planet->orbit_angle);

    planet->position.x = orbit_center.x + cosf(orbit_rad) * planet->orbit_radius;
    planet->position.y = orbit_center.y + sinf(orbit_rad) * planet->orbit_radius;

    // 행성 자전
    planet->rotation_angle += planet->rotation_speed * delta_time;
    if (planet->rotation_angle >= 360.0f) planet->rotation_angle -= 360.0f;  // 자전 각도 리셋
}

// 행성들을 정렬
void align_planets() {
    // 태양 위치
    sun.position = create_vector3(WIDTH / 2, HEIGHT / 2, 1);

    // 지구 정렬
    earth.position = create_vector3(WIDTH / 2 + 12, HEIGHT / 2, 1);
    earth.orbit_angle = 0.0f;

    // 달 정렬
    moon.position = create_vector3(WIDTH / 2 + 16, HEIGHT / 2, 1);
    moon.orbit_angle = 0.0f;
}

// Main
int main() {
    // Hide console cursor to avoid it interfering with drawing
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

    init_planets();

    printf("20231375_박은서\n");
    printf("스페이스바를 누르면 행성이 정렬됩니다...\n");

    ULONGLONG last_time = GetTickCount64();
    float delta_time = 0.0f;

    while (1) {
        if (_kbhit()) {
            char key = _getch();
            if (key == 27) break;  // ESC 키를 누르면 종료
            if (key == ' ') {
                if (animation_state == 0) {
                    animation_state = 2;  // 행성 정렬
                    align_planets();  // 행성 정렬
                }
                else if (animation_state == 2) {
                    animation_state = 1;  // 움직임 시작
                }
                else if (animation_state == 1) {
                    animation_state = 0;  // 기본 상태로 돌아가기
                    align_planets();  // 다시 정렬
                }
            }
        }

        ULONGLONG current_time = GetTickCount64();
        delta_time = (current_time - last_time) / 1000.0f;
        last_time = current_time;

        clear_screen();

        if (animation_state == 0 || animation_state == 2) {
            // 기본 출력 상태 또는 행성 정렬 상태
            draw_filled_circle(sun.position, 3.0f, 'S');
            draw_filled_circle(earth.position, 2.0f, 'E');
            draw_filled_circle(moon.position, 1.5f, 'M');
        }

        if (animation_state == 1) {
            // 스페이스바를 눌렀을 때 행성들 움직임
            update_planet(&earth, delta_time, sun.position);
            update_planet(&moon, delta_time, earth.position);
            draw_filled_circle(sun.position, 3.0f, 'S');
            draw_filled_circle(earth.position, 2.0f, 'E');
            draw_filled_circle(moon.position, 1.5f, 'M');
        }

        render_screen();
        Sleep(50);  // 화면 갱신 속도
    }

    cleanup_planets();
    return 0;
}