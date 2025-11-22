#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

int screen_active = 1;
#define NUM_FRAMES  2

#define MAX_INPUT_CHARS 10
#define MAX_PROBLEMS 2
#define MAX_BULLETS 10

typedef struct Problem {
    int x, y;
    char text[32];
    double solution;
    bool active;
} Problem;

typedef struct Bullet {
    bool active;
    float x, y;
    float dirX, dirY;
    float speed;
} Bullet;

// Function declarations
int randomX_Priklad(void);
int priklad_randomCislo(void);
int priklad_randomOperacia(void);
void priklad_Generuj(Problem *p);
void FireBullet(Bullet bullets[], float startX, float startY, float targetX, float targetY);
void UpdateBullets(Bullet bullets[], int count, int screenWidth, int screenHeight);
void DrawBullets(Bullet bullets[], int count);

// Global game variables
int maxCislo = 20;
int operacia = 3;
int score = 0;

const int HITBOX_PADDING_X = 14;  // pixels added horizontally
const int HITBOX_PADDING_Y = 14;  // pixels added vertically

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 533;

    InitWindow(screenWidth, screenHeight, "MATH BOIIIII - Patrik Nagy");
    SetTargetFPS(60);
    InitAudioDevice();

    Texture2D pozadie = LoadTexture("assets/pozadie.jpg");
    Texture2D kanon = LoadTexture("assets/kanon.png");
    Texture2D Play_BTN = LoadTexture("assets/PLAYbtn.png");
    Sound explosion_sound = LoadSound("assets/explode.wav");
    Sound click_sound = LoadSound("assets/click.wav");

    SetSoundVolume(explosion_sound, 0.2f);
    SetSoundVolume(click_sound, 0.2f);

    Vector2 mousePoint = { 0.0f, 0.0f };
    int btnState = 0;               // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
    bool btnAction = false;         // Button action should be activated

    // Player input
    char name[MAX_INPUT_CHARS + 1] = {0};
    int letterCount = 0;

    // Bullets
    Bullet bullets[MAX_BULLETS] = {0};

    // Problems
    Problem problems[MAX_PROBLEMS] = {0};
    for (int i = 0; i < MAX_PROBLEMS; i++)
    {
        problems[i].active = true;
        problems[i].y = 50 + i * 50; // stagger initial positions
        problems[i].x = randomX_Priklad();
        priklad_Generuj(&problems[i]);
    }

    while (!WindowShouldClose())
    {
        mousePoint = GetMousePosition();
        // Handle user input
        int key = GetCharPressed();
        while (key > 0)
        {
            if (((key >= '0' && key <= '9') || key == '-' || key == '.') && letterCount < MAX_INPUT_CHARS)
            {
                name[letterCount++] = (char)key;
                name[letterCount] = '\0';
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_SPACE)) {
            letterCount = 0;
            name[0] = '\0';
        }

        double userInput = atof(name);

        // Update problems
        for (int i = 0; i < MAX_PROBLEMS; i++)
        {
            if (!problems[i].active) continue;

            problems[i].y++;

            // Problem reaches bottom
            if (problems[i].y >= 450)
            {
                score--;
                problems[i].y = 50;
                problems[i].x = randomX_Priklad();
                priklad_Generuj(&problems[i]);
            }

            if (letterCount > 0 && fabs(userInput - problems[i].solution) < 0.001)
            {
                FireBullet(bullets,
                           screenWidth / 2, 460,
                           problems[i].x + MeasureText(problems[i].text, 20)/2,
                           problems[i].y + 10);
                letterCount = 0;
                name[0] = '\0';
            }

        }

        // Update bullets
        UpdateBullets(bullets, MAX_BULLETS, screenWidth, screenHeight);

        // Check bullet collisions
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (!bullets[i].active) continue;

            for (int j = 0; j < MAX_PROBLEMS; j++)
            {
                if (!problems[j].active) continue;

                Rectangle problemRect = {
                    problems[j].x - HITBOX_PADDING_X,
                    problems[j].y - HITBOX_PADDING_Y,
                    MeasureText(problems[j].text, 20) + HITBOX_PADDING_X * 2,
                    20 + HITBOX_PADDING_Y * 2
                };
                
                Rectangle bulletRect = { bullets[i].x - 2, bullets[i].y - 20, 4, 10 };

                if (CheckCollisionRecs(problemRect, bulletRect))
                {
                    bullets[i].active = false;
                    score++;
                    PlaySound(explosion_sound);

                    problems[j].y = 50;
                    problems[j].x = randomX_Priklad();
                    priklad_Generuj(&problems[j]);
                }
            }
        }

        float frameHeight = (float)Play_BTN.height / NUM_FRAMES;
        float btnScale = 3.0f;

        // Base button size (scaled)
        float btnWidthScaled = Play_BTN.width * btnScale;
        float btnHeightScaled = frameHeight * btnScale;

        // Destination rectangle for drawing
        Rectangle destRec = { screenWidth/2.0f - btnWidthScaled/2.0f,
                            screenHeight/2.0f - btnHeightScaled/2.0f,
                            btnWidthScaled,
                            btnHeightScaled };

        // Source rectangle from sprite sheet
        Rectangle sourceRec = { 0, frameHeight * btnState, (float)Play_BTN.width, frameHeight };

        // Origin for rotation (center)
        Vector2 origin = { 0, 0 };

        // Hover effect: slightly enlarge on hover
        Rectangle playbtn_bounds = destRec; // hitbox matches drawn size
        if (CheckCollisionPointRec(mousePoint, playbtn_bounds))
        {
            btnScale = 3.2f;
            btnWidthScaled = Play_BTN.width * btnScale;
            btnHeightScaled = frameHeight * btnScale;
            destRec.width = btnWidthScaled;
            destRec.height = btnHeightScaled;
            destRec.x = screenWidth/2.0f - btnWidthScaled/2.0f;
            destRec.y = screenHeight/2.0f - btnHeightScaled/2.0f;

            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) btnState = 1; // pressed
            else btnState = 1; // hover

            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                PlaySound(click_sound);
                screen_active = 2;
                
            };
        }
        else btnState = 0;

        // Drawing
        BeginDrawing();
        if (screen_active == 1)
        {
            ClearBackground(RAYWHITE);
            DrawTexturePro(Play_BTN, sourceRec, destRec, origin, 0.0f, WHITE);
        }
        if (screen_active == 2) {
            ClearBackground(RAYWHITE);
            DrawTextureEx(pozadie, (Vector2){0,0}, 0.0f, (float)screenWidth/pozadie.width, WHITE);
            DrawTextureEx(kanon, (Vector2){screenWidth/2 - kanon.width, 470}, 0.0f, 2.0f, WHITE);

            for (int i = 0; i < MAX_PROBLEMS; i++)
            {
                if (problems[i].active)
                {
                    DrawText(problems[i].text, problems[i].x, problems[i].y, 20, BLACK);

                    // Draw the enlarged hitbox for debugging
                    Rectangle debugRect = {
                        problems[i].x - HITBOX_PADDING_X,
                        problems[i].y - HITBOX_PADDING_Y,
                        MeasureText(problems[i].text, 20) + HITBOX_PADDING_X * 2,
                        20 + HITBOX_PADDING_Y * 2
                    };
                    DrawRectangleLines((int)debugRect.x, (int)debugRect.y,
                                    (int)debugRect.width, (int)debugRect.height, BLUE);
                }
            }

            DrawBullets(bullets, MAX_BULLETS);

            DrawText(name, 50, screenHeight - 50, 20, RED);

            char scoreText[32];
            sprintf(scoreText, "Score: %d", score);
            DrawText(scoreText, 10, 10, 30, LIGHTGRAY);
        }

        
        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;
}


void FireBullet(Bullet bullets[], float startX, float startY, float targetX, float targetY)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!bullets[i].active)
        {
            bullets[i].active = true;
            bullets[i].x = startX;
            bullets[i].y = startY;
            float dx = targetX - startX;
            float dy = targetY - startY;
            float length = sqrtf(dx*dx + dy*dy);
            bullets[i].dirX = dx / length;
            bullets[i].dirY = dy / length;
            bullets[i].speed = 8.0f;
            break;
        }
    }
}

void UpdateBullets(Bullet bullets[], int count, int screenWidth, int screenHeight)
{
    for (int i = 0; i < count; i++)
    {
        if (bullets[i].active)
        {
            bullets[i].x += bullets[i].dirX * bullets[i].speed;
            bullets[i].y += bullets[i].dirY * bullets[i].speed;

            if (bullets[i].x < 0 || bullets[i].x > screenWidth || bullets[i].y < 0 || bullets[i].y > screenHeight)
                bullets[i].active = false;
        }
    }
}

void DrawBullets(Bullet bullets[], int count)
{
    for (int i = 0; i < count; i++)
    {
        if (bullets[i].active)
            DrawRectangle((int)bullets[i].x, (int)bullets[i].y, 4, 10, RED);
    }
}



int randomX_Priklad(void)
{
    return GetRandomValue(100, 700);
}

int priklad_randomCislo(void)
{
    return GetRandomValue(0, maxCislo);
}

int priklad_randomOperacia(void)
{
    return GetRandomValue(0, operacia);
}

void priklad_Generuj(Problem *p)
{
    int cislo1, cislo2;
    char operaciaChar;

    while (true)
    {
        cislo1 = priklad_randomCislo();
        cislo2 = priklad_randomCislo();
        int oper = priklad_randomOperacia();

        switch (oper)
        {
            case 0: operaciaChar = '+'; p->solution = cislo1 + cislo2; break;
            case 1: operaciaChar = '-'; p->solution = cislo1 - cislo2; break;
            case 2: operaciaChar = '*'; p->solution = cislo1 * cislo2; break;
            case 3:
                operaciaChar = '/';
                if (cislo2 == 0) continue;
                if (cislo1 % cislo2 != 0) continue;
                p->solution = (double)cislo1 / cislo2;
                break;
        }
        break;
    }

    sprintf(p->text, "%d %c %d", cislo1, operaciaChar, cislo2);
}
