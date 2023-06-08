#include "raylib.h"
#include "iostream"

Color BTN_COLOR = BLUE;
Color BTN_HOVERED = DARKBLUE;
Color BACKGROUND = {53, 170, 176, 255};

enum SCREENS {MENU, GAME};
int currentScreen;

enum GRID {EMPTY, CIRCLE, CROSS};
int grid[3][3] = {EMPTY};

enum TURN {TURN_CIRCLE, TURN_CROSS};
int turn = TURN_CIRCLE;

Texture2D crossTexture;
Texture2D circleTexture;

Rectangle gridRec = {160, 100, 409, 409};
#define SQUARE_SIZE 133
#define LINE_WIDTH 10


void mainMenu(){

    auto mousePoint = GetMousePosition();

    Rectangle startRec = {230, 250, 250, 50};
    Rectangle exitRec = {300, 310, 110, 50};
    Color startBtnColor = BTN_COLOR;
    Color exitBtnColor = BTN_COLOR;

    // wykrywanie klików i zmienianie koloru jeżeli jest najechane
    if (CheckCollisionPointRec(mousePoint, startRec)) {
        startBtnColor = BTN_HOVERED;
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            currentScreen = GAME;
        }
    }

    if (CheckCollisionPointRec(mousePoint, exitRec)) {
        exitBtnColor = BTN_HOVERED;
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            CloseWindow();
        }
    }

    DrawRectangle(startRec.x, startRec.y, startRec.width, startRec.height, startBtnColor);
    DrawText("START GAME", startRec.x + 20, startRec.y + 10, 30, WHITE);

    DrawRectangle(exitRec.x, exitRec.y, exitRec.width, exitRec.height, exitBtnColor);
    DrawText("EXIT", exitRec.x + 20, exitRec.y + 10, 30, WHITE);


}


// rysuje zagrane kółka i krzyżyki
void populateGrid(){

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {

            int square = grid[i][j];
            Texture2D texture;

            // wybieramy jaki obrazek
            if(square == EMPTY)
                continue;
            else if (square == CIRCLE)
                texture = circleTexture;
            else if (square == CROSS)
                texture = crossTexture;

            // gdzie obrazek sie powinien pojawić
            int x = gridRec.x + i * SQUARE_SIZE + 20 ;
            int y = gridRec.y + j * SQUARE_SIZE + 20 ;

            DrawTexture(texture, x, y, WHITE);

        }
    }
}



// tutaj pewnie bedziesz chciał coś pozmieniać
void play(int i, int j){

    // zmnienianie tury
    if (turn == TURN_CIRCLE){
        grid[i][j] = CIRCLE;
        turn = TURN_CROSS;
    }
    else if (turn == TURN_CROSS){
        grid[i][j] = CROSS;
        turn = TURN_CIRCLE;
    }
}


void drawWinnerAndReset(int winner){

    // rysuje kto wygrał
    std::string win_text = (winner == CROSS) ? "CROSS WON" : "CIRCLE WON!";
    DrawText(win_text.c_str(), gridRec.x + 15, 550, 60, WHITE);

    // rysuje przycisk reset
    Rectangle resetRec = {gridRec.x + 120, 630, 180, 50};
    Color btnColor = BTN_COLOR;
    auto mousePoint = GetMousePosition();

    if (CheckCollisionPointRec(mousePoint, resetRec)) {
        btnColor = BTN_HOVERED;
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {

            // tutaj dać funckje do resetowania (jak na razie po prostu czyśći array)
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    grid[i][j] = EMPTY;
                }
            }
        }
    }

    DrawRectangle(resetRec.x, resetRec.y, resetRec.width, resetRec.height, btnColor);
    DrawText("RESTART", resetRec.x + 20, resetRec.y + 10, 30, WHITE);

}

void drawKogoTura(){
    std::string turn_text = (turn == TURN_CROSS) ? "cross' turn!" : "circle's turn!";
    DrawText(turn_text.c_str(), gridRec.x, 30, 30, WHITE);
}

void gameScreen(){
    // rysuje grid
    for (int i = 0; i < 4; ++i) {
        DrawRectangle(gridRec.x + i*SQUARE_SIZE, gridRec.y, LINE_WIDTH, gridRec.height, WHITE);
        DrawRectangle(gridRec.x , gridRec.y + i*SQUARE_SIZE, gridRec.width, LINE_WIDTH, WHITE);
    }

    drawKogoTura();

    // pojedyńcza kratka w gridzie
    Rectangle square = {gridRec.x + LINE_WIDTH,
                        gridRec.y + LINE_WIDTH,
                        SQUARE_SIZE - LINE_WIDTH,
                        SQUARE_SIZE - LINE_WIDTH};


    // wykrywa która kratka została kliknięta
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {

            Rectangle rec = square;
            rec.x += i * SQUARE_SIZE;
            rec.y += j * SQUARE_SIZE;
            auto mousePoint = GetMousePosition();

            if (CheckCollisionPointRec(mousePoint, rec)) {
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                    play(i, j);
                    std::cout << i << j << " ";
                }
            }
        }
    }

    // rysuje zagrane kółka i krzyżyki
    populateGrid();

    // tu podepnąc wykrywanie wygranej
    if(true)
        drawWinnerAndReset(EMPTY);

}

int main(void){

    InitWindow(720, 720, "Tic Tac Toe");
    SetTargetFPS(60);

    // ładowanie tekstur
    Image circleImg = LoadImage("circle.png");
    Image crossImg = LoadImage("cross.png");
    ImageResize(&circleImg, 100, 100);
    ImageResize(&crossImg, 100, 100);
    circleTexture = LoadTextureFromImage(circleImg);
    crossTexture = LoadTextureFromImage(crossImg);
    UnloadImage(circleImg);
    UnloadImage(crossImg);

    currentScreen = MENU;

    // głowny loop
    while (!WindowShouldClose()) {

        BeginDrawing();

        switch (currentScreen) {
            case MENU:
                mainMenu();
                break;
            case GAME:
                gameScreen();
                break;
        }

        ClearBackground(BACKGROUND);

        EndDrawing();
    }

    // sprzątanie
    CloseWindow();

    return 0;
}

