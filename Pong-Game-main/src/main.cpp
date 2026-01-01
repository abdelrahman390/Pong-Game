#include <iostream>
#include <raylib.h>
#include <string>
using namespace std;

class Player{
protected:
    int paddle_pos_start = GetScreenHeight() / 4;
    int paddle_pos_end = GetScreenHeight() / 2;
    int maximum_boundaries = GetScreenHeight();
    int score = 0;

public:
    // If the value of c == u, this means going up, but if the value of c == d, this means going down.
    void move_paddle(char c){
        if(c == 'u' && paddle_pos_start - 1 >= 0){
            paddle_pos_start = max(0, paddle_pos_start - 1);
            paddle_pos_end--;
        } else if(c == 'd' && paddle_pos_end + 1 <= maximum_boundaries){
            paddle_pos_end = min(maximum_boundaries, paddle_pos_end + 1);
            paddle_pos_start++;
        }
    }

    virtual void draw_paddle() = 0;

    int getScore() {
        return score;
    }

    void increaseScore() {
        score++;
    }

    void resetScore() {
        score = 0;
    }
};


class LeftPlayer : public Player {
    
public:
    void draw_paddle(){
        // DrawLineEx({20, (float)GetScreenHeight() / 4}, {20, (float)GetScreenHeight() / 2}, 5, GREEN); 
        DrawLineEx({20, (float)paddle_pos_start}, {20, (float)paddle_pos_end}, 5, GREEN); 
    }
};

class rightPlayer : public Player {
    
public:
    void draw_paddle(){
        DrawLineEx({(float)GetScreenWidth() - 20, (float)paddle_pos_start}, {(float)GetScreenWidth() - 20, (float)paddle_pos_end}, 5, GREEN); 
    }
};


class GameScreen
{
private:
    Color bg_color = {255, 255, 255, 255};
    Color border_color = BLUE;
    float screen_width = 800;
    float screen_height = 600;
    const char *game_title = "Ping Pong Game";

    void draw_borders()
    {
        DrawLineEx({0, 0}, {0, screen_height}, 10, border_color);
        DrawLineEx({screen_width, 0}, {screen_width, screen_height}, 10, border_color);
        DrawLineEx({screen_width / 2, 0}, {screen_width / 2, screen_height}, 3, WHITE);
    }

    void draw_score(int &score1, int &score2)
    {
        DrawText(("Player 1: " + to_string(score1)).c_str(), screen_width / 6, screen_height - 20, 20, border_color);
        DrawText(("Player 2: " + to_string(score2)).c_str(), screen_width / 6 * 4, screen_height - 20, 20, border_color);
    }

public:
    GameScreen()
    {
        InitWindow(screen_width, screen_height, game_title);
    }

    void draw_screen(int &score1, int &score2)
    {
        DrawFPS(screen_width - 100, 10);
        draw_borders();
        draw_score(score1, score2);
    }

    void draw_menu(int fps)
    {
        DrawFPS(screen_width - 100, 10);
        DrawText(TextFormat("Target FPS: %i (Use UP/DOWN to change)", fps), 10, 10, 20, WHITE);
        const char *text = "Press Enter To Start!";
        int font_size = 50;
        int text_width = MeasureText(text, font_size);
        DrawText(text, screen_width / 2 - text_width / 2, screen_height / 2 - font_size / 2, font_size, border_color);
    }

    void draw_end_screen(int winner)
    {
        DrawFPS(screen_width - 100, 10);
        string text1 = "Player " + to_string(winner) + " Won! Press Enter To Restart!";
        if (winner == 0)
        {
            text1 = "Game Resseted! Press Enter To Restart!";
        }
        const char *text = text1.c_str();
        int font_size = 50;
        int text_width = MeasureText(text, font_size);
        DrawText(text, screen_width / 2 - text_width / 2, screen_height / 2 - font_size / 2, font_size, border_color);
    }
};

class Game
{
private:
    int fps = 60;
    GameScreen screen;
    bool game_state = 0;
    bool game_end = false;
    int winner = 0;

    LeftPlayer lPlayer;
    rightPlayer rPlayer;

    int winner_score = 5;

    bool check_win()
    {
        if (lPlayer.getScore() >= winner_score)
        {
            winner = 1;
            screen.draw_end_screen(1);
            return true;
        }
        else if (rPlayer.getScore() >= winner_score)
        {
            winner = 2;
            screen.draw_end_screen(2);
            return true;
        }
        return false;
    }

    void reset_score()
    {
        lPlayer.resetScore();
        rPlayer.resetScore();
    }

    void increase_score(int player)
    {
        if (player == 1)
        {
            lPlayer.increaseScore();
        }
        else if (player == 2)
        {
            rPlayer.increaseScore();
        }
    }

public:
    void run_game_loop()
    {
        SetTargetFPS(fps);
        while (!WindowShouldClose())
        {

            BeginDrawing();
            ClearBackground(BLACK);
            if (game_state == 1) // while game is on / in game
            {
                int lS = lPlayer.getScore();
                int rS = rPlayer.getScore();

                screen.draw_screen(lS, rS);

                lPlayer.draw_paddle();
                rPlayer.draw_paddle();
                if (check_win())
                {
                    game_state = 0;
                    game_end = true;
                }

                // Move left paddale up 
                if (IsKeyPressed(KEY_W) || IsKeyDown(KEY_W))
                {
                    lPlayer.move_paddle('u');
                }
                // Move left paddale down
                if (IsKeyPressed(KEY_S) || IsKeyDown(KEY_S))
                {
                    lPlayer.move_paddle('d');
                }

                // Move right paddale up
                if (IsKeyPressed(KEY_UP) || IsKeyDown(KEY_UP))
                {
                    rPlayer.move_paddle('u');
                }
                // Move right paddale down
                if (IsKeyPressed(KEY_DOWN) || IsKeyDown(KEY_DOWN))
                {
                    rPlayer.move_paddle('d');
                }

                // press backspace to reset game
                if (IsKeyPressed(KEY_BACKSPACE))
                {
                    game_state = 0;
                    game_end = true;
                }

                // when left player scores a goal 
                // if(){
                //     lPlayer.increaseScore();
                // }

                // when right player scores a goal 
                // if(){
                //     rPlayer.increaseScore();
                // }
            }
            else if (game_state == 0 && game_end == false) // in menu
            {
                screen.draw_menu(fps);

                if (IsKeyPressed(KEY_ENTER))
                {
                    game_state = 1;
                    cout << "enter pressed" << endl;
                }
                if (IsKeyPressed(KEY_DOWN))
                {
                    if (fps == 30)
                        fps = 30;
                    else
                    {
                        fps -= 10;
                        SetTargetFPS(fps);
                    }
                }
                if (IsKeyPressed(KEY_UP))
                {
                    fps += 10;
                    SetTargetFPS(fps);
                }
            }
            else if (game_state == 0 && game_end == true) // end game / after game ended
            {
                screen.draw_end_screen(winner);
                if (IsKeyPressed(KEY_ENTER))
                {
                    game_state = 0;
                    game_end = false;
                    winner = 0;
                    reset_score();
                }
            }
            EndDrawing();
        }
    }
};

int main()
{
    Game game;
    game.run_game_loop();

    CloseWindow();
    return 0;
}