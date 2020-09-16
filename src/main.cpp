#include "tank.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

enum gameState
{
	Running = 0,
	Stopped = 1,
	Lost = 2,
	Won = 3
};

gameState state = gameState::Running;

int windowWidth = 800;
int windowHeight = 600;

int main()
{
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Tanks Game by S0ul", sf::Style::Close | sf::Style::Titlebar);
	// Limit the framerate to 60 frames per second
	window.setFramerateLimit(60);
	// constructor called for myTank and enemy AI
	Player myTank;
	AI enemy;

	Borders game_leve;
	game_leve.set_borders();

	// setting up font for later
	sf::Font font;
	if (!font.loadFromFile("src/whitrabt.ttf"))
	{
		return EXIT_FAILURE;
	}

	//text
	sf::Text winMessage("You won!!!", font, 100);
	auto bounds1 = winMessage.getLocalBounds();
	winMessage.setFillColor(sf::Color::Red);
	winMessage.setPosition((windowWidth - bounds1.width) / 2, (windowHeight - bounds1.height) / 2);
	sf::Text loseMessage("You lost!!!", font, 100);
	auto bounds2 = loseMessage.getLocalBounds();
	loseMessage.setFillColor(sf::Color::Red);
	loseMessage.setPosition((windowWidth - bounds2.width) / 2, (windowHeight - bounds2.height) / 2);
	sf::Text healthPlayer("Player: " + std::to_string(myTank.getHealth()), font, 25);
	healthPlayer.setPosition(200.0, 15.0);
	healthPlayer.setFillColor(sf::Color::Green);
	sf::Text healthEnemy("Enemy: " + std::to_string(enemy.getHealth()), font, 25);
	healthEnemy.setPosition(400.0, 15.0);
	healthEnemy.setFillColor(sf::Color::Red);

	//Variables
	bool holdMB = false;		// prevent multishottinh while holding LMB
	int playerHit = 0;			// for being hit on myTank
	int enemyHit = 0;			// for being hit on enemy
	int player_noHit_timer = 0; // timer to set noHit myTank
	int enemy_noHit_timer = 0;	// timer to set noHit enemy
	int enemy_shoot_timer = 60; // enemy shooting speed

	while (window.isOpen())
	{

		//when not hit
		playerHit = -1;
		enemyHit = -1;

		//Events
		sf::Event evnt;
		while (window.pollEvent(evnt))
		{
			// Pause game with P
			if (state == gameState::Running && evnt.type == sf::Event::KeyPressed && evnt.key.code == sf::Keyboard::P)
			{
				state = gameState::Stopped;
			}
			else if (state == gameState::Stopped && evnt.type == sf::Event::KeyPressed && evnt.key.code == sf::Keyboard::P)
			{
				state = gameState::Running;
			}
			// Close game with Esc or press x on top right corner
			if ((evnt.type == sf::Event::KeyPressed && evnt.key.code == sf::Keyboard::Escape) || evnt.type == sf::Event::Closed)
			{
				window.close();
			}
		}
		if (state == gameState::Running)
		{
			//shootingh with LMB
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (!holdMB)
				{
					myTank.fire(sf::Mouse::getPosition(window));
					holdMB = true;
				}
			}
			else
			{
				holdMB = false;
			}

			//enemy shoot with cooldown
			enemy_shoot_timer--;
			if (enemy_shoot_timer == 0)
			{
				enemy.fire(static_cast<sf::Vector2i>(myTank.getLocation()));
				enemy_shoot_timer = 60;
			}

			//barrel rotation
			myTank.rotate(sf::Mouse::getPosition(window));
			enemy.rotate(static_cast<sf::Vector2i>(myTank.getLocation()));

			//Projectles continue to move
			myTank.move_bullets(enemy.getBorders());

			//AI movement
			myTank.move_player(game_leve, enemy);
			enemy.ai_Brain();

			//getting hit on player
			playerHit = myTank.hit();
			myTank.update(playerHit);
			healthPlayer.setString("Player: " + std::to_string(myTank.getHealth()));
			if (playerHit >= 0)
			{
				myTank.getHit();
				player_noHit_timer = 0;
			}
			else
			{
				if (player_noHit_timer > 150)
				{
					myTank.notHit();
				}
			}

			//getting hit on AI
			enemyHit = enemy.hit();
			myTank.update(enemyHit);
			healthEnemy.setString("Enemy: " + std::to_string(enemy.getHealth()));
			if (enemyHit >= 0)
			{
				enemy.getHit();
				enemy_noHit_timer = 0;
			}
			else
			{
				if (enemy_noHit_timer > 150)
				{
					enemy.notHit();
				}
			}

			//increase timers every loop to get flag on noHit
			player_noHit_timer++;
			enemy_noHit_timer++;
			// set gameStates for losing and wining
			if (myTank.getHealth() <= 0)
			{
				state = gameState::Lost;
			}
			if (enemy.getHealth() <= 0)
			{
				state = gameState::Won;
			}
		}

		window.clear();

		// Drawing sprites, strings, HP
		game_leve.draw(window);
		myTank.draw(window);
		enemy.draw(window);
		if (state == gameState::Won)
		{
			window.draw(winMessage);
		}
		if (state == gameState::Lost)
		{
			window.draw(loseMessage);
		}
		window.draw(healthPlayer);
		window.draw(healthEnemy);

		// Update for the window
		window.display();
	}
	return EXIT_SUCCESS;
}