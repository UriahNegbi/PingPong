#include <iostream>
#include "raylib.h"

using namespace std;

short playerScore = 0;
short cpuScore = 0;

class Ball {
public:
    float x, y;
    int speedY;
    int speedX;
    int radius;

    void Draw() {
        DrawCircle(x, y, radius, WHITE);
    }

    void Update() {
        x += speedX;
        y += speedY;
        if (y + radius >= GetScreenHeight() || y - radius <= 0) {
            speedY *= -1;
        }
        // CPU scores
        if (x + radius >= GetScreenWidth()) {
            cpuScore++;
            ResetBall();
        }

        // Player scores
        if (x - radius <= 0) {
            playerScore++;
            ResetBall();
        }
    }

    void ResetBall() {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;
        int speedChoices[2] = {-1, 1};
        speedX = 7 * speedChoices[GetRandomValue(0, 1)];
        speedY = 7 * speedChoices[GetRandomValue(0, 1)];
    }
};

class Paddle {
protected:
    void LimitMovement() {
        if (y <= 0) {
            y = 0;
        }
        if (y + height >= GetScreenHeight()) {
            y = GetScreenHeight() - height;
        }
    }

public:
    float x, y;
    float width, height;
    int speed;

    void Draw() {
        DrawRectangle(x, y, width, height, WHITE);
    }

    void Update() {
        if (IsKeyDown(KEY_W)) {
            y -= speed;
        }
        if (IsKeyDown(KEY_S)) {
            y += speed;
        }
        LimitMovement();
    }
};

class Paddle2 {
protected:
    void LimitMovement() {
        if (y <= 0) {
            y = 0;
        }
        if (y + height >= GetScreenHeight()) {
            y = GetScreenHeight() - height;
        }
    }

public:
    float x, y;
    float width, height;
    int speed;

    void Draw() {
        DrawRectangle(x, y, width, height, WHITE);
    }

    void Update() {
        if (IsKeyDown(KEY_UP)) {
            y -= speed;
        }
        if (IsKeyDown(KEY_DOWN)) {
            y += speed;
        }
        LimitMovement();
    }
};

class CpuPaddle : public Paddle {
public:
    void Update(int bally, int ballx) {
        if (y + height / 2 > bally && ballx < x + width * 24) {
            y -= speed;
        }
        if (y + height / 2 <= bally && ballx < x + width * 24) {
            y += speed;
        }
        LimitMovement();
    }
};

void gameModeAI(Ball &ball, Paddle &player, CpuPaddle &cpu, Sound &pingSound, Sound &pongSound, Sound &music1, bool &musicPlaying) {
    // Update
    ball.Update();
    player.Update();
    cpu.Update(ball.y, ball.x);

    // Music control
    if (!musicPlaying) {
        PlaySound(music1);
        musicPlaying = true;
    }

    // Check for collisions
    if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height})) {
        ball.speedX *= -1;
        PlaySound(pingSound);
    }
    if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height})) {
        ball.speedX *= -1;
        PlaySound(pongSound);
    }

    // Draw
    BeginDrawing();
    ClearBackground(BLUE);
    DrawLine(GetScreenWidth() / 2, 0, GetScreenWidth() / 2, GetScreenHeight(), BLACK);
    ball.Draw();
    cpu.Draw();
    player.Draw();

    // Display scores
    DrawText(TextFormat("%i", cpuScore), GetScreenWidth() / 4 - 20, 20, 80, RED);
    DrawText(TextFormat("%i", playerScore), 3 * GetScreenWidth() / 4 - 20, 20, 80, YELLOW);

    // Check for winner
    if (cpuScore >= 9) {
        DrawText("YOU LOST :(", GetScreenWidth() / 4, GetScreenHeight() / 2, 90, RED);
        ball.speedX = 0;
        ball.speedY = 0;
    }
    if (playerScore >= 9) {
        DrawText("YOU WIN! :D", GetScreenWidth() / 4, GetScreenHeight() / 2, 90, YELLOW);
        ball.speedX = 0;
        ball.speedY = 0;
    }

    EndDrawing();
}

void gameModeMultiplayerOffline(Ball &ball, Paddle &player, Paddle2 &player2, Sound &pingSound, Sound &pongSound, Sound &music1, bool &musicPlaying) {
    // Update
    ball.Update();
    player.Update();
    player2.Update();

    // Music control
    if (!musicPlaying) {
        PlaySound(music1);
        musicPlaying = true;
        
    }

    // Check for collisions
    if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height})) {
        ball.speedX *= -1;
        PlaySound(pingSound);
    }
    if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player2.x, player2.y, player2.width, player2.height})) {
        ball.speedX *= -1;
        PlaySound(pongSound);
    }

    // Draw
    BeginDrawing();
    ClearBackground(BLUE);
    DrawLine(GetScreenWidth() / 2, 0, GetScreenWidth() / 2, GetScreenHeight(), BLACK);
    ball.Draw();
    player2.Draw();
    player.Draw();

    // Display scores
    DrawText(TextFormat("%i", cpuScore), GetScreenWidth() / 4 - 20, 20, 80, RED);
    DrawText(TextFormat("%i", playerScore), 3 * GetScreenWidth() / 4 - 20, 20, 80, YELLOW);

    // Check for winner
    if (cpuScore >= 9) {
        DrawText("YOU LOST :(", GetScreenWidth() / 4, GetScreenHeight() / 2, 90, RED);
        ball.speedX = 0;
        ball.speedY = 0;
    }
    if (playerScore >= 9) {
        DrawText("YOU WIN! :D", GetScreenWidth() / 4, GetScreenHeight() / 2, 90, YELLOW);
        ball.speedX = 0;
        ball.speedY = 0;
    }

    EndDrawing();
}

int main() {
    const int screenWidth = 1280;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Menu");
    InitAudioDevice();
    SetTargetFPS(60);

    // Button settings
    const char* text = "Training";
    int fontSize = 90;
    int textWidth = MeasureText(text, fontSize);
    int textHeight = fontSize;
    int posX = (screenWidth - textWidth) / 2;
    int posY = (screenHeight - textHeight) / 2;

    const char* text2 = "Multiplayer (offline)";
    int fontSize2 = 90;
    int textWidth2 = MeasureText(text2, fontSize2);
    int textHeight2 = fontSize2;
    int posX2 = (screenWidth - textWidth2) / 2;
    int posY2 = (screenHeight - textHeight2) / 4;

    enum GameMode { MENU, TRAINING, MULTIPLAYER_OFFLINE };
    GameMode currentGameMode = MENU;

    Ball ball;
    Paddle player;
    Paddle2 player2;
    CpuPaddle cpu;

    // Ball settings
    ball.radius = 20;
    ball.x = screenWidth / 2;
    ball.y = screenHeight / 2;
    ball.speedX = 7;
    ball.speedY = 7;

    // Player1 settings
    player.width = 25;
    player.height = 120;
    player.x = screenWidth - player.width - 10;
    player.y = screenHeight / 2 - player.height / 2;
    player.speed = 6;

    // Player2 settings
    player2.width = 25;
    player2.height = 120;
    player2.x = 10;
    player2.y = screenHeight / 2 - player2.height / 2;
    player2.speed = 6;

    // AI settings
    cpu.width = 25;
    cpu.height = 120;
    cpu.x = 10;
    cpu.y = screenHeight / 2 - cpu.height / 2;
    cpu.speed = 6;

    Sound pingSound = LoadSound("Ping.wav");
    Sound pongSound = LoadSound("Pong.wav");
    Sound music1 = LoadSound("retro.wav");
    Sound musicMenu = LoadSound("retroMenu.wav");
    bool musicPlaying = false;
    bool musicPlayingMenu = false;
    if(!musicPlayingMenu) {
        PlaySound(musicMenu);
        musicPlayingMenu = true;
        SetSoundVolume(musicMenu, 0.2);
    }
    SetSoundVolume(music1, 0.2);

    while (!WindowShouldClose()) {
        switch (currentGameMode) {
            case MENU:
                BeginDrawing();
                ClearBackground(RED);
                DrawText("Training", posX, posY, fontSize, YELLOW);
                DrawText("Multiplayer (offline)", posX2, posY2, fontSize2, YELLOW);
                EndDrawing();

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    Vector2 mousePosition = GetMousePosition();
                    if (mousePosition.x >= posX && mousePosition.x <= posX + textWidth &&
                        mousePosition.y >= posY && mousePosition.y <= posY + textHeight) {
                        UnloadSound(musicMenu);
                        currentGameMode = TRAINING;
                        playerScore = 0;
                        cpuScore = 0;
                    }
                    if (mousePosition.x >= posX2 && mousePosition.x <= posX2 + textWidth2 &&
                        mousePosition.y >= posY2 && mousePosition.y <= posY2 + textHeight2) {
                        UnloadSound(musicMenu);
                        currentGameMode = MULTIPLAYER_OFFLINE;
                        playerScore = 0;
                        cpuScore = 0;
                    }
                }
                break;

            case TRAINING:
                gameModeAI(ball, player, cpu, pingSound, pongSound, music1, musicPlaying);
                break;

            case MULTIPLAYER_OFFLINE:
                gameModeMultiplayerOffline(ball, player, player2, pingSound, pongSound, music1, musicPlaying);
                break;
        }
    }

    UnloadSound(pingSound);
    UnloadSound(pongSound);
    UnloadSound(music1);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
