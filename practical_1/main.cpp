#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;
Vector2f ballVelocity;
bool isPlayer1Serving = false;
int player1Score = 0; // Initialize player 1 score
int player2Score = 0; // Initialize player 2 score
const float initialVelocityX = 100.f;
const float initialVelocityY = 60.f;
const float velocityMultiplier = 1.1f;

const Keyboard::Key controls[4] = {
    Keyboard::A,   // Player1 UP
    Keyboard::Z,   // Player1 Down
    Keyboard::Up,  // Player2 UP
    Keyboard::Down // Player2 Down
};
const Vector2f paddleSize(25.f, 100.f);
const float ballRadius = 10.f;
const int gameWidth = 800;
const int gameHeight = 600;
const float paddleSpeed = 400.f;
const float paddleOffsetWall = 10.f;

CircleShape ball;
RectangleShape paddles[2];

Font font; // Declare Font object
Text text; // Declare Text object

void Reset() {
    // Set size and origin of paddles
    for (auto& p : paddles) {
        p.setSize(paddleSize);
        p.setOrigin(paddleSize / 2.f);
    }
    // Set size and origin of ball
    ball.setRadius(ballRadius);
    ball.setOrigin(5.f, 5.f); //Should be half the ball width and height
    // reset paddle position
    paddles[0].setPosition(Vector2f(paddleOffsetWall + paddleSize.x / 2.f, gameHeight / 2.f));
    paddles[1].setPosition(Vector2f(gameWidth - paddleOffsetWall - paddleSize.x / 2.f, gameHeight / 2.f));
    // reset Ball Position
    ball.setPosition(Vector2f(gameWidth / 2.f, gameHeight / 2.f));
    ballVelocity = { (isPlayer1Serving ? initialVelocityX : -initialVelocityX), initialVelocityY };

    // Update Score Text
    text.setString("Player 1: " + to_string(player1Score) + "  Player 2: " + to_string(player2Score));
    // Keep Score Text Centered
    text.setPosition((gameWidth * 0.5f) - (text.getLocalBounds().width * 0.5f), 20.f); // Adjust y-position if needed

}

void Load() {
    Reset();

    // Load font-face from res dir
    if (!font.loadFromFile("res/fonts/RobotoMono-Regular.ttf")) {
        // Handle error (e.g., log it or throw an exception)
        cerr << "Error loading font" << endl;
    }

    // Set text element to use font
    text.setFont(font);
    // Set the character size to 24 pixels
    text.setCharacterSize(24);
    text.setFillColor(Color::White); // Optional: set text color
    text.setString("PONG Game"); // Example text
    text.setPosition(gameWidth / 2.f - text.getGlobalBounds().width / 2.f, 20.f); // Center text horizontally

}

void Update(RenderWindow& window) {
    // Reset clock, recalculate deltatime
    static Clock clock;
    float dt = clock.restart().asSeconds();
    // check and consume events
    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed) {
            window.close();
            return;
        }

    }
    ball.move(ballVelocity * dt);

    // Quit Via ESC Key
    if (Keyboard::isKeyPressed(Keyboard::Escape)) {
        window.close();
    }

    // handle paddle movement
    float direction = 0.0f;
    if (Keyboard::isKeyPressed(controls[0])) {
        direction--;
    }
    if (Keyboard::isKeyPressed(controls[1])) {
        direction++;
    }
    paddles[0].move(Vector2f(0.f, direction * paddleSpeed * dt));

    // Handle paddle movement for Player 2
    float directionP2 = 0.0f;
    if (Keyboard::isKeyPressed(controls[2])) {
        directionP2--;
    }
    if (Keyboard::isKeyPressed(controls[3])) {
        directionP2++;
    }
    paddles[1].move(Vector2f(0.f, directionP2 * paddleSpeed * dt));


    // check ball collision
    const float bx = ball.getPosition().x;
    const float by = ball.getPosition().y;
    if (by > gameHeight) { //bottom wall
        // bottom wall
        ballVelocity.x *= velocityMultiplier;
        ballVelocity.y *= -velocityMultiplier;
        ball.move(Vector2f(0.f, -10.f));
    }
    else if (by < 0) { //top wall
        // top wall
        ballVelocity.x *= velocityMultiplier;
        ballVelocity.y *= -velocityMultiplier;
        ball.move(Vector2f(0.f, 10.f));
    }

    // Check ball collision with left and right walls
    if (bx > gameWidth || bx < 0) {
        Reset(); // Reset the game state
    }

    // Ball collision with left paddle
    if (
        // Ball is inline or behind paddle AND
        bx < paddleSize.x + paddleOffsetWall &&
        // Ball is below top edge of paddle AND
        by > paddles[0].getPosition().y - (paddleSize.y * 0.5) &&
        // Ball is above bottom edge of paddle
        by < paddles[0].getPosition().y + (paddleSize.y * 0.5)
        ) {
        // Bounce off left paddle
        ballVelocity.x = -ballVelocity.x * velocityMultiplier; // Reverse direction and increase speed
        ball.move(Vector2f(10.f, 0.f)); // Move the ball slightly to avoid sticking
    }

    // Ball collision with right paddle
    if (
        // Ball is inline or behind paddle AND
        bx > gameWidth - paddleOffsetWall - paddleSize.x &&
        // Ball is below top edge of paddle AND
        by > paddles[1].getPosition().y - (paddleSize.y * 0.5) &&
        // Ball is above bottom edge of paddle
        by < paddles[1].getPosition().y + (paddleSize.y * 0.5)
        ) {
        // Bounce off right paddle
        ballVelocity.x = -ballVelocity.x * velocityMultiplier; // Reverse direction and increase speed
        ball.move(Vector2f(-10.f, 0.f)); // Move the ball slightly to avoid sticking
    }
}

void Render(RenderWindow& window) {
    // Draw Everything
    window.draw(paddles[0]);
    window.draw(paddles[1]);
    window.draw(ball);
}

int main() {
    RenderWindow window(VideoMode(gameWidth, gameHeight), "PONG");
    Load();
    while (window.isOpen()) {
        window.clear();
        Update(window);
        Render(window);
        window.display();
    }
    return 0;
}