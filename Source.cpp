#include <iostream>
#include <raylib.h>
#include <string>
using namespace std;


struct Ball
{
    float x=400, y=250;
    int speedx=7, speedy=7;
    int radius=10;

    void draw()
    {
        DrawCircle(x, y, radius, WHITE);
    }


    void update()
    {
        x += speedx;
        y += speedy;

        if (y + radius >= GetScreenHeight() || y - radius <= 0)
        {
            speedy *= -1;
        }
       
    }

    void restball()
    {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speedchoices[2] = { -1,1 };

        speedx *= speedchoices[GetRandomValue(0, 1)];
        speedy *= speedchoices[GetRandomValue(0, 1)];
       

    }


};


class Player {
protected:

    int paddle_pos_start = GetScreenHeight() / 4;
    int paddle_pos_end = GetScreenHeight() / 2;
    int maximum_boundaries = GetScreenHeight();
    int score = 0;
    int speed = 10;

public:
    // If the value of c == u, this means going up, but if the value of c == d, this means going down.
    void move_paddle(char c) {
        if (c == 'u' && paddle_pos_start - 1 >= 0) {
            paddle_pos_start = max(0, paddle_pos_start - speed);
            paddle_pos_end-= speed;
        }
        else if (c == 'd' && paddle_pos_end + 1 <= maximum_boundaries) {
            paddle_pos_end = min(maximum_boundaries, paddle_pos_end + speed);
            paddle_pos_start+= speed;
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

    int getPaddleStart() const
    {
        return paddle_pos_start;
    }

    int getPaddleEnd() const
    {
        return paddle_pos_end;
    }

};


class LeftPlayer : public Player {

public:
    void draw_paddle() {
        // DrawLineEx({20, (float)GetScreenHeight() / 4}, {20, (float)GetScreenHeight() / 2}, 5, GREEN); 
        DrawLineEx({ 20, (float)paddle_pos_start }, { 20, (float)paddle_pos_end }, 5, GREEN);
    }
   
};

class rightPlayer : public Player {

public:
    void draw_paddle() {
        DrawLineEx({ (float)GetScreenWidth() - 20, (float)paddle_pos_start }, { (float)GetScreenWidth() - 20, (float)paddle_pos_end }, 5, GREEN);
    }
    
   

};


class GameScreen 
{

private:

    Color bg_color = { 255, 255, 255, 255 };
    Color border_color = BLUE;
    float screen_width = 800;
    float screen_height = 500;


    const char* game_title = "Ping Pong Game";

    void draw_borders()
    {
        DrawLineEx({ 0, 0 }, { 0, screen_height }, 10, border_color);
        DrawLineEx({ screen_width, 0 }, { screen_width, screen_height }, 10, border_color);
        DrawLineEx({ screen_width / 2, 0 }, { screen_width / 2, screen_height }, 3, WHITE);
    }

    void draw_score(int& score1, int& score2)
    {
        DrawText(("Player 1: " + to_string(score1)).c_str(), screen_width / 6, screen_height - 20, 20, border_color);
        DrawText(("Player 2: " + to_string(score2)).c_str(), screen_width / 6 * 4, screen_height - 20, 20, border_color);

        DrawText(TextFormat("Ball Speed: %d", ball.speedy), 20, 20, 20, WHITE);

    }

public:
    Ball ball;
    GameScreen()
    {
        InitWindow(screen_width, screen_height, game_title);
    }

    void draw_screen(int& score1, int& score2)
    {
        DrawFPS(screen_width - 100, 10);
        draw_borders();
        draw_score(score1, score2);
    }

    void draw_menu(int fps)
    {
        DrawFPS(screen_width - 100, 10);
        DrawText(TextFormat("Target FPS: %i (Use UP/DOWN to change)", fps), 10, 10, 20, WHITE);
        const char* text = "Press Enter To Start!";
        int font_size = 50;
        int text_width = MeasureText(text, font_size);
        DrawText(text, screen_width / 2 - text_width / 2, screen_height / 2 - font_size , font_size, border_color);

    }

    void draw_end_screen(int winner)
    {
        DrawFPS(screen_width - 100, 10);
        string text1 = "Player " + to_string(winner) + " Won! Press Enter To Restart!";
        if (winner == 0)
        {
            text1 = "Game Resseted! Press Enter To Restart!";
        }
        const char* text = text1.c_str();
        int font_size = 50;
        int text_width = MeasureText(text, font_size);
        DrawText(text, screen_width / 2 - text_width / 4, screen_height / 2 - font_size / 2, font_size/2, border_color);
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

    int hitCount = 0;
    Ball ball;
    void run_game_loop()
    {
       
        SetTargetFPS(fps);
        ;
        while (!WindowShouldClose())
        {

            BeginDrawing();
            ClearBackground(BLACK);
            if (game_state == 1) // while game is on / in game
            {
                int lS = lPlayer.getScore();
                int rS = rPlayer.getScore();

                screen.draw_screen(lS, rS);
                screen. ball.draw();
                screen.ball.update();


                // line drawing size
                const float paddleThickness = 5.0f;

                // left racket line points
                Vector2 lStart{ 20.0f, (float)lPlayer.getPaddleStart() };
                Vector2 lEnd{ 20.0f, (float)lPlayer.getPaddleEnd() };

                // Right racket line points
                float rx = (float)GetScreenWidth() - 20.0f;
                Vector2 rStart{ rx, (float)rPlayer.getPaddleStart() };
                Vector2 rEnd{ rx, (float)rPlayer.getPaddleEnd() };

                // Collision with the left
                if (CheckCollisionCircleLine(Vector2{ screen.ball.x, screen.ball.y }, screen.ball.radius, lStart, lEnd)
                    && screen.ball.speedx < 0)
                {
                    screen.ball.speedx *= -1;
                   
                //Move the ball out of front of the line so it doesn't get stuck
                    screen.ball.x = 20.0f + paddleThickness * 0.5f + screen.ball.radius + 1.0f;
                    hitCount++;
                }

                // Collision with the right
                if (CheckCollisionCircleLine(Vector2{ screen.ball.x, screen.ball.y }, screen.ball.radius, rStart, rEnd)
                    && screen.ball.speedx > 0)
                {
                    screen.ball.speedx *= -1;
                  
                    screen.ball.x = rx - paddleThickness * 0.5f - screen.ball.radius - 1.0f;
                    hitCount++;
                }
               
                //Increase the speed of the ball
                if (hitCount >= 3)  
                {
                    
                    if (screen.ball.speedx > 0)
                        screen.ball.speedx += 1;
                    else
                        screen.ball.speedx -= 1;

                    if (screen.ball.speedy > 0)
                        screen.ball.speedy += 1;
                    else
                        screen.ball.speedy -= 1;

                    
                    hitCount = 0;
                }

                if (screen.ball.x - screen.ball.radius <= 0)
                {
                   
                    rPlayer.increaseScore();
                    screen.ball.restball();
                    screen.ball.speedx = 7;
                    screen.ball.speedy = 7;
                   
                }
                
               
                if (screen.ball.x + screen.ball.radius >= GetScreenWidth())
                    
                {
                    lPlayer.increaseScore();
                    screen.ball.restball();
                    screen.ball.speedx = 7;
                    screen.ball.speedy = 7;
                }
                


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

                // press keypad 3 to reset scores
                if (IsKeyPressed(KEY_KP_3))
                {
                    reset_score();
                }
            }
            else if (game_state == 0 && game_end == false) // in menu
            {
                screen.draw_menu(fps);

                if (IsKeyPressed(KEY_ENTER))
                {
                    game_state = 1;
                    cout << "enter pressed" << endl;
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
            float ballSpeed = sqrt(ball.speedx * ball.speedx + ball.speedy * ball.speedy);
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