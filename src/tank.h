#ifndef tank_h
#define tank_h
#include "projectile.h"

class AI
{
public:
	sf::Vector2f make_which_way()
	{
		srand(time(NULL));
		int number = rand() % 4;
		if (number == 0)
		{
			return sf::Vector2f(0.0, movement_speed);
		}
		else if (number == 1)
		{
			return sf::Vector2f(0.0, -movement_speed);
		}
		else if (number == 2)
		{
			return sf::Vector2f(movement_speed, 0.0);
		}
		else
		{
			return sf::Vector2f(-movement_speed, 0.0);
		}
	}

	AI()
	{
		ai.setSize(sf::Vector2f(30.0, 30.0));
		ai.setFillColor(sf::Color::Yellow);
		ai.setOrigin(15.0f, 15.0f);
		ai.setPosition(380.0f, 380.0f);
		gun.setSize(sf::Vector2f(7.0, 34.0));
		gun.setFillColor(sf::Color::Green);
		gun.setOrigin(4.0, 4.0);
		gun.setPosition(ai.getPosition());
		health = 300;
		which_way = make_which_way();
		predict_hit = false;
		evade_action = sf::Vector2f(0.0, 0.0);
		pause_for_mov = 10;
		run_away = false;
	}

	// where should AI turn
	int which_turn(int closeness)
	{
		if (closeness == east)
		{
			if (which_way.y <= 0)
			{
				return west;
			}
			else
			{
				return east;
			}
		}
		else if (closeness == west)
		{
			if (which_way.y <= 0)
			{
				return east;
			}
			else
			{
				return west;
			}
		}
		else if (closeness == north)
		{
			if (which_way.x <= 0)
			{
				return west;
			}
			else
			{
				return east;
			}
		}
		else if (closeness == south)
		{
			if (which_way.y <= 0)
			{
				return east;
			}
			else
			{
				return west;
			}
		}

		return closeness;
	}

	// changes angle on our which_way, close is for telling which barrier are we close to
	void change_which_way(int close)
	{
		const float sinpiquarts = 0.70710678118;
		// first 4 are for corners
		if (close % north == 0 && close % west == 0)
		{
			if (!run_away)
			{
				pause_for_mov = 0;
				run_away = true;
				which_way = sf::Vector2f(sinpiquarts * 2.0, sinpiquarts * 2.0);
			}
		}
		else if (close % north == 0 && close % east == 0)
		{
			if (!run_away)
			{
				pause_for_mov = 0;
				run_away = true;
				which_way = sf::Vector2f(-sinpiquarts * 2.0, sinpiquarts * 2.0);
			}
		}
		else if (close % south == 0 && close % west == 0)
		{
			if (!run_away)
			{
				pause_for_mov = 0;
				run_away = true;
				which_way = sf::Vector2f(-sinpiquarts * 2.0, sinpiquarts * 2.0);
			}
		}
		else if (close % south == 0 && close % east == 0)
		{
			if (!run_away)
			{
				pause_for_mov = 0;
				run_away = true;
				which_way = sf::Vector2f(-sinpiquarts * 2.0, sinpiquarts * 2.0);
			}
		}
		// avoid normal barriers
		else if (close >= 0)
		{
			run_away = false;
			float angle;
			float newAngle;
			float hypotenuse = sqrt((which_way.x * which_way.x) + (which_way.y * which_way.y));
			angle = asin(which_way.y / hypotenuse);
			if (close == east)
			{
				newAngle = angle;
			}
			else
			{
				newAngle = angle;
			}
			which_way.y = hypotenuse * sin(newAngle);
			which_way.x = hypotenuse * cos(newAngle);
			float angle_in_degrees = newAngle * radian_to_degrees;
			// determine in which square of scale we are
			if (angle_in_degrees > 0 && angle_in_degrees <= 90)
			{
				which_way.x *= -1;
			}
			else if (angle_in_degrees > 90 && angle_in_degrees <= 180)
			{
				which_way.x *= -1;
				which_way.y *= -1;
			}
			else if (angle_in_degrees > 180 && angle_in_degrees <= 270)
			{
				which_way.y *= -1;
			}
		}
		else
		{
			run_away = false;
		}
	}

	// determines movement
	void move()
	{
		pause_for_mov++;
		if (pause_for_mov > 100)
		{
			change_which_way(which_turn(game_leve.sprite_wall_distance(ai.getPosition())));
			pause_for_mov = 0;
		}
		if (game_leve.collision(ai.getGlobalBounds()))
		{

			sf::RectangleShape west(sf::Vector2f(1.0, 1.0));
			west.setPosition(ai.getPosition().x - 16, ai.getPosition().y);

			sf::RectangleShape east(sf::Vector2f(1.0, 1.0));
			east.setPosition(ai.getPosition().x + 16, ai.getPosition().y);

			sf::RectangleShape top(sf::Vector2f(1.0, 1.0));
			top.setPosition(ai.getPosition().x, ai.getPosition().y - 16);

			sf::RectangleShape bottom(sf::Vector2f(1.0, 1.0));
			bottom.setPosition(ai.getPosition().x, ai.getPosition().y + 16);

			if (game_leve.collision(west.getGlobalBounds()) || game_leve.collision(east.getGlobalBounds()))
			{
				which_way = sf::Vector2f(which_way.x * -1, which_way.y);
			}
			else if (game_leve.collision(top.getGlobalBounds()) || game_leve.collision(bottom.getGlobalBounds()))
			{
				which_way = sf::Vector2f(which_way.x, which_way.y * -1);
			}
		}
		ai.move(which_way);
		gun.move(which_way);
	}

	// rotate gun at myTank position
	void rotate(sf::Vector2i target)
	{
		sf::Vector2f position = sf::Vector2f(target.x - gun.getPosition().x, target.y - gun.getPosition().y);
		float angle;
		float hypotenuse = sqrt((position.x * position.x) + (position.y * position.y));
		position = sf::Vector2f(position.x, position.y * -1);
		if (position.x > 0 && position.y > 0)
		{
			angle = asin(position.y / hypotenuse);
			// circle devided into approx 6.28 radians
			angle *= (360.0 / 6.28);
			angle = 90.0 - angle;
			gun.setRotation(angle + 180.0);
		}
		else if (position.x < 0 && position.y > 0)
		{
			angle = asin(position.y / hypotenuse);
			angle *= (360.0 / 6.28);
			gun.setRotation(angle + 90.0);
		}
		else if (position.x < 0 && position.y < 0)
		{
			angle = asin(position.y / hypotenuse);
			angle *= (360.0 / 6.28);
			gun.setRotation(angle + 90.0);
		}
		else if (position.x > 0 && position.y < 0)
		{
			angle = asin(position.y / hypotenuse);
			angle *= (360.0 / 6.28);
			angle = 90.0 - angle;
			gun.setRotation(angle + 180.0);
		}
		else if (position.x == 0 && position.y < 0)
		{
			gun.setRotation(0.0);
		}
		else if (position.x == 0 && position.y > 0)
		{
			gun.setRotation(1800.0);
		}
		else if (position.x < 0 && position.y == 0)
		{
			gun.setRotation(90.0);
		}
		else if (position.x > 0 && position.y == 0)
		{
			gun.setRotation(270.0);
		}
	}

	// check nearest wall and check if projectile is moving horizontal or vertical and move perpendicularr away from it
	sf::Vector2f evasive_action(sf::Vector2f projectle)
	{
		int closeness;
		if (abs(projectle.x) >= abs(projectle.y))
		{
			closeness = game_leve.check_closeness(ai.getPosition(), x_hor);
			if (closeness == north)
			{
				return sf::Vector2f(0.0, movement_speed);
			}
			else if (closeness == south)
			{
				return sf::Vector2f(0.0, -movement_speed);
			}
		}
		else
		{
			closeness = game_leve.check_closeness(ai.getPosition(), y_vert);
			if (closeness == west)
			{
				return sf::Vector2f(movement_speed, 0.0);
			}
			else if (closeness == east)
			{
				return sf::Vector2f(-movement_speed, 0.0);
			}
		}
		return sf::Vector2f(0.0, 0.0);
	}

	// choose movement normal or evade_action when needed
	void choose_movement()
	{
		if (!game_leve.collision(ai.getGlobalBounds()))
		{
			ai.move(evade_action);
			gun.move(evade_action);
		}
		if (game_leve.collision(ai.getGlobalBounds()))
		{
			ai.move(sf::Vector2f(evade_action.x * -1, evade_action.y * -1));
			gun.move(sf::Vector2f(evade_action.x * -1, evade_action.y * -1));
		}
	}

	// all actions
	void ai_Brain()
	{
		sf::Vector2f projectle = cannons.will_hit(ai);
		if (projectle != sf::Vector2f(0.0, 0.0))
		{
			if (!predict_hit)
			{
				evade_action = evasive_action(projectle);
				predict_hit = true;
			}
			choose_movement();
		}
		else
		{
			move();
			predict_hit = false;
		}
	}

	int hit()
	{
		return cannons.collision(ai.getGlobalBounds());
	}

	void getHit()
	{
		ai.setFillColor(sf::Color::Red);
		health -= 10;
	}

	int getHealth()
	{
		return health;
	}

	void notHit()
	{
		ai.setFillColor(sf::Color::Yellow);
	}

	sf::FloatRect getBorders()
	{
		return ai.getGlobalBounds();
	}

	void draw(sf::RenderWindow& window)
	{
		window.draw(ai);
		window.draw(gun);
	}

	void fire(sf::Vector2i target)
	{
		cannons.fire(ai.getPosition(), target);
	}

private:
	Specprojectile cannons;	   // get acress to Projectiles
	sf::RectangleShape ai;	   // enemy sprite
	sf::RectangleShape gun;	   // enemy gun
	sf::Vector2f which_way;	   // enemy vector for movement
	sf::Vector2f evade_action; // change which_way when ai is under fire
	bool predict_hit;		   // check if enemy will get hit
	Borders game_leve;		   // get access to barriers
	int health;				   // enemy health
	int pause_for_mov;		   // more human movement
	bool run_away;			   // bool for checking when ai is close to wall and is moveing form the wall
};

class Player
{
public:
	Player()
	{
		myTank.setSize(sf::Vector2f(30.0f, 30.0f));
		myTank.setFillColor(sf::Color::Green);
		myTank.setOrigin(14.0f, 14.0f);
		myTank.setPosition(120.0f, 120.0f);
		gun.setSize(sf::Vector2f(7.0, 30.0));
		gun.setFillColor(sf::Color::Blue);
		gun.setOrigin(4.0, 4.0);
		gun.setPosition(myTank.getPosition());
		health = 100;
	}

	// movememnt and collission with walls and enemy
	void move_player(Borders game_leve, AI enemy)
	{

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		{
			if (!myTank.getGlobalBounds().intersects(enemy.getBorders())
				&& !game_leve.collision(myTank.getGlobalBounds()))
			{
				myTank.move(movement_speed, 0.0f);
				gun.move(movement_speed, 0.0f);
			}
			if (myTank.getGlobalBounds().intersects(enemy.getBorders())
				|| game_leve.collision(myTank.getGlobalBounds()))
			{
				myTank.move(-movement_speed, 0.0f);
				gun.move(-movement_speed, 0.0f);
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		{
			if (!myTank.getGlobalBounds().intersects(enemy.getBorders())
				&& !game_leve.collision(myTank.getGlobalBounds()))
			{
				myTank.move(-movement_speed, 0.0f);
				gun.move(-movement_speed, 0.0f);
			}
			if (myTank.getGlobalBounds().intersects(enemy.getBorders())
				|| game_leve.collision(myTank.getGlobalBounds()))
			{
				myTank.move(movement_speed, 0.0f);
				gun.move(movement_speed, 0.0f);
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		{
			if (!myTank.getGlobalBounds().intersects(enemy.getBorders())
				&& !game_leve.collision(myTank.getGlobalBounds()))
			{
				myTank.move(0.0f, -movement_speed);
				gun.move(0.0f, -movement_speed);
			}
			if (myTank.getGlobalBounds().intersects(enemy.getBorders())
				|| game_leve.collision(myTank.getGlobalBounds()))
			{
				myTank.move(0.0f, movement_speed);
				gun.move(0.0f, movement_speed);
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		{
			if (!myTank.getGlobalBounds().intersects(enemy.getBorders())
				&& !game_leve.collision(myTank.getGlobalBounds()))
			{
				myTank.move(0.0f, movement_speed);
				gun.move(0.0f, movement_speed);
			}
			if (myTank.getGlobalBounds().intersects(enemy.getBorders())
				|| game_leve.collision(myTank.getGlobalBounds()))
			{
				myTank.move(0.0f, -movement_speed);
				gun.move(0.0f, -movement_speed);
			}
		}
	}

	// gun facing where coursor is
	void rotate(sf::Vector2i coursor)
	{
		sf::Vector2f which_way = sf::Vector2f(coursor.x - gun.getPosition().x, coursor.y - gun.getPosition().y);
		float angle;
		float hypotenuse = sqrt((which_way.x * which_way.x) + (which_way.y * which_way.y));
		which_way = sf::Vector2f(which_way.x, which_way.y * -1);
		if (which_way.x > 0 && which_way.y > 0)
		{
			angle = asin(which_way.y / hypotenuse);
			angle *= (360.0 / 6.28);
			angle = 90.0 - angle;
			gun.setRotation(angle + 180.0);
		}
		else if (which_way.x < 0 && which_way.y > 0)
		{
			angle = asin(which_way.y / hypotenuse);
			angle *= (360.0 / 6.28);
			//fix for snapping
			//angle = 90.0 - angle;
			gun.setRotation(angle + 90.0);
		}
		else if (which_way.x < 0 && which_way.y < 0)
		{
			angle = asin(which_way.y / hypotenuse);
			angle *= (360.0 / 6.28);
			//fix for snapping
			//angle = 90.0 - angle;
			gun.setRotation(angle + 90.0);
		}
		else if (which_way.x > 0 && which_way.y < 0)
		{
			angle = asin(which_way.y / hypotenuse);
			angle *= (360.0 / 6.28);
			angle = 90.0 - angle;
			gun.setRotation(angle + 180.0);
		}
		else if (which_way.x == 0 && which_way.y < 0)
		{
			gun.setRotation(0.0);
		}
		else if (which_way.x == 0 && which_way.y > 0)
		{
			gun.setRotation(1800.0);
		}
		else if (which_way.x < 0 && which_way.y == 0)
		{
			gun.setRotation(90.0);
		}
		else if (which_way.x > 0 && which_way.y == 0)
		{
			gun.setRotation(270.0);
		}
	}

	int hit()
	{
		return cannons.collision(myTank.getGlobalBounds());
	}

	void getHit()
	{
		myTank.setFillColor(sf::Color::Red);
		health -= 10;
	}

	int getHealth()
	{
		return health;
	}

	void notHit()
	{
		myTank.setFillColor(sf::Color::Green);
	}

	void update(int position)
	{
		cannons.update(position);
	}

	void fire(sf::Vector2i target)
	{
		cannons.fire(myTank.getPosition(), target);
	}

	void move_bullets(sf::FloatRect obsticle)
	{
		cannons.bullet_movement(obsticle);
	}

	sf::FloatRect getBorders()
	{
		return myTank.getGlobalBounds();
	}

	sf::Vector2f getLocation()
	{
		return myTank.getPosition();
	}

	void draw(sf::RenderWindow& window)
	{
		window.draw(myTank);
		window.draw(gun);
		cannons.draw(window);
	}

private:
	sf::RectangleShape myTank; // myTank tank sprite
	sf::RectangleShape gun;	   // myTank cannon sprite
	Specprojectile cannons;	   // get acress to Projectiles
	int health;				   // myTank health
};

#endif