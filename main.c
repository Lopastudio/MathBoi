#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define MAX_INPUT_CHARS 10 

int randomX_Priklad(void);
int priklad_randomCislo(void);
int priklad_randomOperacia(void);
void priklad_Generuj(void);

int maxCislo = 30;
int operacia = 3;

char priklad_Text[32] = "";

int cislo1, cislo2;
char operaciaChar;
double rieseniePrikladu;

int main(void)
{
    int prikladPosY = 50;
    int prikladPosX = 200;
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "MATH BOIIIII - Patrik Nagy");
    SetTargetFPS(60);

    char name[MAX_INPUT_CHARS + 1] = {0};
    char name2[MAX_INPUT_CHARS + 1] = {0};
    int letterCount = 0;

    Texture2D background = LoadTexture("assets/background.jpg");

    priklad_Generuj();

    while (!WindowShouldClose())
    {
        int key = GetCharPressed();

        if (prikladPosY >= 450)
        {
            prikladPosY = 50;
            prikladPosX = randomX_Priklad();
            priklad_Generuj();
            letterCount = 0;
            name[0] = '\0';
        }

        while (key > 0)
        {
          if (key == ' ') {
            letterCount = 0;
            name[0] = '\0';
            name2[0] = '\0';
          }

          // Allow digits, minus sign, and decimal point
          if (((key >= '0' && key <= '9') || key == '-' || key == '.') && (letterCount < MAX_INPUT_CHARS))
          {
              name[letterCount] = (char)key;
              letterCount++;
              name[letterCount] = '\0';
          }

          strcpy(name2, name); // zobrazovanie celeho vysledku - skopirovanie do druhej premennej

          // convert input to double
          double userEnteredVysledok = atof(name);

          // skontrolujeme, ci je spravny vysledok
          if (fabs(userEnteredVysledok - rieseniePrikladu) < 0.001)
          {
              prikladPosY = 50;
              prikladPosX = randomX_Priklad();
              priklad_Generuj();
              letterCount = 0;
              name[0] = '\0';
          }

          key = GetCharPressed();
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTextureEx(background, (Vector2){ 0, 0 }, 0.0f, (float)screenWidth / background.width, WHITE);

        DrawLine(0, 470, screenWidth, 470, RED); // 50, 450

        DrawText("MATHBOI", (screenWidth / 2) - (MeasureText("MATHBOI", 20) / 2), 20, 20, BLACK);
        DrawText(priklad_Text, prikladPosX, prikladPosY, 20, BLACK);
        DrawText(name2, 50, screenHeight - 50, 20, RED);  // ukaz input

        EndDrawing();

        prikladPosY++;
    }

    CloseWindow();
    return 0;
}

int randomX_Priklad(void)
{
    return GetRandomValue(50, 750);
}

int priklad_randomCislo(void)
{
    return GetRandomValue(0, maxCislo);
}

int priklad_randomOperacia(void)
{
    return GetRandomValue(0, operacia);
}

void priklad_Generuj(void)
{
    cislo1 = priklad_randomCislo();
    cislo2 = priklad_randomCislo();
    int oper = priklad_randomOperacia();

    switch (oper)
    {
    case 0:
        operaciaChar = '+';
        rieseniePrikladu = (double)cislo1 + cislo2;
        break;
    case 1:
        operaciaChar = '-';
        rieseniePrikladu = (double)cislo1 - cislo2;
        break;
    case 2:
        operaciaChar = '*';
        rieseniePrikladu = (double)cislo1 * cislo2;
        break;
    case 3:
        operaciaChar = '/';
        if (cislo2 == 0) cislo2 = 1;  // nech nedelime nulou
        rieseniePrikladu = (double)cislo1 / cislo2;
        break;
    }

    sprintf(priklad_Text, "%d %c %d", cislo1, operaciaChar, cislo2);
}
