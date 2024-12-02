#include "ElfMath.h"
#include "Elf2D.h"
#include "ElfTime.h"
#include "ElfObject.h"

#define WIDTH 40
#define HEIGHT 24 // 화면 높이를 24로 변경

void Initialize(GameObject_Line* obj, int objNum)
{
    int i;
    for (i = 0; i < 3; i++)
    {
        obj[i].Position.x = 0;
        obj[i].Position.y = 0;

        obj[i].Rotation = 0;

        obj[i].Line[0].x = 0;
        obj[i].Line[0].y = 0;
        obj[i].Line[1].x = 3;
        obj[i].Line[1].y = 0;
    }

    obj[1].Rotation = 30;
    obj[2].Rotation = 30;

    obj[0].Symbol = "11";
    obj[1].Symbol = "22";
    obj[2].Symbol = "33";

    obj[0].Scale.x = 1;
    obj[0].Scale.y = 1;
    obj[1].Scale.x = 2;
    obj[1].Scale.y = 2;
    obj[2].Scale.x = 2;
    obj[2].Scale.y = 2;
}

int Input()
{
    if (_kbhit()) {
        char ch = _getch();
        if (ch == 27) {
            return 99;
        }
    }
    return 0;
}

void Update(GameObject_Line* obj, int objNum, int e)
{
    // 업데이트 로직 추가 가능
}

void Render(GameObject_Line* obj, int objNum, char* Buf, int width, int height)
{
    Vector3 lineA, lineB;
    Matrix3x3 world = identity_matrix();

    lineB.x = 0;
    lineB.y = 0;

    int i;
    for (i = 0; i < objNum; i++)
    {
        obj[i].Position.x = lineB.x;
        obj[i].Position.y = lineB.y;

        lineA.x = obj[i].Line[0].x;
        lineA.y = obj[i].Line[0].y;
        lineA.z = 1;
        lineB.x = obj[i].Line[1].x;
        lineB.y = obj[i].Line[1].y;
        lineB.z = 1;

        Matrix3x3 scale = scale_matrix(obj[i].Scale.x, obj[i].Scale.y);
        Matrix3x3 rotation = rotation_matrix(obj[i].Rotation);
        Matrix3x3 translation = translation_matrix(obj[i].Position.x, obj[i].Position.y);

        world = multiply_matrices(scale, world);
        world = multiply_matrices(rotation, world);
        world = multiply_matrices(translation, world);
        lineA = multiply_matrix_vector(world, lineA);
        lineB = multiply_matrix_vector(world, lineB);

        Elf2DDrawLine((int)lineA.x, (int)lineA.y, (int)lineB.x, (int)lineB.y, Buf, width, height);
    }
}

int main() {
    int fps = 60;
    double frameTime = 1000.0 / fps;

    // 변경: 화면 버퍼 크기를 (WIDTH * 2 + 1) * HEIGHT로 확장
    char screenBuffer[(WIDTH * 2 + 1) * HEIGHT];
    int screenWidth = WIDTH * 2; // 변경: 화면 너비를 픽셀당 2문자를 위해 2배로 확장
    int screenHeight = HEIGHT;

    GameObject_Line LineObj[3];

    Initialize(LineObj, 3);

    Elf2DInitScreen();
    Elf2DClearScreen(screenBuffer, screenWidth, screenHeight);
    Elf2DDrawBuffer(screenBuffer);

    ElfTimeInitialize();

    int isGameRunning = 1;
    while (isGameRunning) {
        int gameEvent = Input();

        if (gameEvent == 99)
        {
            isGameRunning = 0;
        }

        Update(LineObj, 3, gameEvent);
        Elf2DClearScreen(screenBuffer, screenWidth, screenHeight);
        Render(LineObj, 3, screenBuffer, screenWidth, screenHeight);

        // 변경: 각 줄 끝에 줄바꿈 문자를 추가하여 출력 형태 조정
        for (int y = 0; y < screenHeight; y++) {
            screenBuffer[(y + 1) * (screenWidth + 1) - 1] = '\n';
        }

        Elf2DDrawBuffer(screenBuffer);

        ElfTimeCalculateDeltaTime();
        Elf2DSleep(frameTime - ElfTimeGetDeltaTime());
    }

    return 0;
}