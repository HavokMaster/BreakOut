#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include <math.h>

using blockVector = std::vector<sf::RectangleShape>;


const int SPEED = 300;
int speedX = SPEED;
int speedY = SPEED;

void gameLogic(sf::RenderWindow &window, sf::RectangleShape &paddle, sf::CircleShape &ball, blockVector &blocks, float delta);
void changePaddleColor(sf::RectangleShape &paddle);
void displayText(sf::Text &text, sf::RenderWindow &window, std::string msg);

bool showCollisions = false;
bool moveBall = false;
bool hasStarted = false;
bool hasEnded = false;
bool hasWon = false;

int main(){
    sf::RenderWindow window(sf::VideoMode(800, 600), "BreakOut!");
    sf::Clock clock;
    sf::Font font;
    sf::Text text;

    if(!font.loadFromFile("sans.ttf")) return -1;
    text.setFont(font);
    text.setPosition(200, 20);
    srand(time(NULL));

    //creating the game objects
    blockVector blocks;
    int x;
    int y = 100;
    for(int i = 0; i<50; i++){
        sf::RectangleShape temp(sf::Vector2f(50, 20));
        x = 150 + (i%10)*50;
        if(i%10 == 0) y += 20;
        temp.setOrigin(temp.getSize()/2.f);
        temp.setPosition(x, y);
        temp.setFillColor(sf::Color::Red);
        temp.setOutlineThickness(-1);
        temp.setOutlineColor(sf::Color::White);
        blocks.push_back(temp);
        
    }
    sf::RectangleShape paddle(sf::Vector2f(150, 20));
    sf::CircleShape ball(10);

    paddle.setFillColor(sf::Color::White);
    paddle.setOrigin(paddle.getSize()/2.f);
    paddle.setPosition(400, 500);
    
    ball.setFillColor(sf::Color::Red);
    ball.setOrigin(ball.getRadius(), ball.getRadius());
    ball.setPosition(400, 300);

    while(window.isOpen()){
        sf::Event event;
        float delta = clock.restart().asSeconds();

        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed) window.close();
            if(event.type == sf::Event::KeyPressed){
                if(event.key.code == sf::Keyboard::F) showCollisions = !showCollisions;
                if(event.key.code == sf::Keyboard::R && !hasStarted){moveBall = !moveBall; hasStarted = true;}
                if(event.key.code == sf::Keyboard::E) changePaddleColor(paddle);
                //restart logic

                if(event.key.code == sf::Keyboard::R && hasEnded){
                    int x = 200;
                    int y = 100;
                    for(int i = 0; i<50; i++){
                        sf::RectangleShape temp(sf::Vector2f(50, 20));
                        x = 150 + (i%10)*50;
                        if(i%10 == 0) y += 20;
                        temp.setOrigin(temp.getSize()/2.f);
                        temp.setPosition(x, y);
                        temp.setFillColor(sf::Color::Red);
                        temp.setOutlineThickness(-1);
                        temp.setOutlineColor(sf::Color::White);
                        blocks.push_back(temp);
                        
                    }
                    paddle.setPosition(400, 500);
                    ball.setPosition(400, 300);
                    hasStarted = false;
                    hasEnded = false;
                    hasWon = false;
                }

            }
             
        }
        window.clear(sf::Color::Black);
        gameLogic(window, paddle, ball, blocks, delta);
        if(!hasStarted){
            displayText(text, window, "Press R to start the game!");
        }
        if(hasEnded){
            if(hasWon) displayText(text, window, "You win! Press R to restart");
            else displayText(text, window, "You lose! Press R to restart");
        }
        window.display();
    }

}

void gameLogic(sf::RenderWindow &window, sf::RectangleShape &paddle, sf::CircleShape &ball, blockVector &blocks,float delta){
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
        paddle.move(-SPEED*delta, 0);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        paddle.move(SPEED*delta, 0);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) && paddle.getScale().x <= 1.5){
        paddle.scale(1+delta, 1);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && paddle.getScale().x >= 0.5){
        paddle.scale(1-delta, 1);
    }
    if(moveBall){
        if(ball.getPosition().x - ball.getRadius() <= 0 || ball.getPosition().x + ball.getRadius() >= 800){
            speedX = -speedX;
        }
        if(ball.getPosition().y - ball.getRadius() <= 0 || ball.getPosition().y + ball.getRadius() >= 600){
            speedY = -speedY;
        }

        ball.move(speedX*delta, speedY*delta);
    }

    if(paddle.getGlobalBounds().intersects(ball.getGlobalBounds())){
        float relativePosition = ball.getPosition().x - paddle.getPosition().x;
        std::cout << "Collision Detected! ballPos:" << ball.getPosition().x << " paddlePos:" << paddle.getPosition().x << " relativePos:" << relativePosition << std::endl;
        speedY = -speedY;
        if((ball.getPosition().x+ball.getRadius() == paddle.getPosition().x - paddle.getSize().x/2) || (ball.getPosition().x+ball.getRadius() == paddle.getPosition().x + paddle.getSize().x/2)){
            speedX = -speedX;
        }

    }
    if(showCollisions == true){
        sf::FloatRect boundingBox1 = paddle.getGlobalBounds();
        sf::RectangleShape boundingBoxShape1(sf::Vector2f(boundingBox1.width, boundingBox1.height));
        boundingBoxShape1.setFillColor(sf::Color::Transparent);
        boundingBoxShape1.setOutlineThickness(2);
        boundingBoxShape1.setOutlineColor(sf::Color::Green);
        boundingBoxShape1.setPosition(boundingBox1.left, boundingBox1.top);

        sf::FloatRect boundingBox2 = ball.getGlobalBounds();
        sf::RectangleShape boundingBoxShape2(sf::Vector2f(boundingBox2.width, boundingBox2.height));
        boundingBoxShape2.setFillColor(sf::Color::Transparent);
        boundingBoxShape2.setOutlineThickness(2);
        boundingBoxShape2.setOutlineColor(sf::Color::Green);
        boundingBoxShape2.setPosition(boundingBox2.left, boundingBox2.top);
        window.draw(boundingBoxShape1);
        window.draw(boundingBoxShape2);
    }
    window.draw(paddle);
    window.draw(ball);

    if(ball.getPosition().y >= 580){
        hasEnded = true;
        moveBall = false;
        return;
    }
    for(auto it = blocks.begin(); it != blocks.end();){
        sf::Vector2f ballCenter = ball.getPosition();
        sf::Vector2f blockCenter = it->getPosition();
        float distance = std::sqrt(std::pow(ballCenter.x - blockCenter.x, 2) + std::pow(ballCenter.y - blockCenter.y, 2));
        float combinedRadius = ball.getRadius() + it->getSize().x / 2.0f;
        if(distance <= combinedRadius){
            speedY = -speedY;
            it = blocks.erase(it);
        }
        else{
            window.draw(*it);
            it++;
        }
    }
    if(blocks.size() != 0) std::cout << blocks.size() << "blocks remaining" << std::endl;
    else{
        std::cout << "You win!" << std::endl;
        hasEnded = true;
        hasWon = true;
        moveBall = false;
    }
}

void changePaddleColor(sf::RectangleShape &paddle){
    sf::Color colors[] = {sf::Color::Blue, sf::Color::Red, sf::Color::Green};
    sf::Color newColor;
    do{
        newColor = colors[rand()%3];
    }while(newColor == paddle.getFillColor());
    paddle.setFillColor(newColor);
}

void displayText(sf::Text &text, sf::RenderWindow &window, std::string msg){
    text.setString(msg);
    window.draw(text);
}