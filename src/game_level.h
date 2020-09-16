#ifndef game_level_h
#define game_level_h
#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

float shooting_speed = 0.2;
float movement_speed = 1;
//closest wall check
int y_vert = 1;
int x_hor = 2;
int closest_wall = -1;
// for movement
int west = 3;
int east = 4;
int north = 5;
int south = 7;
//convert values
float radian_to_degrees = 360.0 / 6.28;
float pixels_in_sprite = 30.0;

class Borders
{
public:
	Borders()
	{
	}

	void set_borders()
	{
		// Create all barriers for square borders and two horizontal barriers
		sf::RectangleShape west(sf::Vector2f(5.0f, 500.0f));
		west.setFillColor(sf::Color::White);
		west.setPosition(60.0f, 60.0f);
		barriers.push_back(west);
		vertical_indexes.push_back(0);

		sf::RectangleShape south(sf::Vector2f(500.0f, 5.0f));
		south.setFillColor(sf::Color::White);
		south.setPosition(60.0f, 560.0f);
		barriers.push_back(south);
		horizontal_indexes.push_back(1);

		sf::RectangleShape east(sf::Vector2f(5.0f, 505.0f));
		east.setFillColor(sf::Color::White);
		east.setPosition(560.0f, 60.0f);
		barriers.push_back(east);
		vertical_indexes.push_back(2);

		sf::RectangleShape north(sf::Vector2f(500.0f, 5.0f));
		north.setFillColor(sf::Color::White);
		north.setPosition(60.0f, 60.0f);
		barriers.push_back(north);
		horizontal_indexes.push_back(3);

		sf::RectangleShape midNorth(sf::Vector2f(300.0f, 5.0f));
		midNorth.setFillColor(sf::Color::White);
		midNorth.setPosition(150.0f, 185.0f);
		barriers.push_back(midNorth);
		horizontal_indexes.push_back(4);

		sf::RectangleShape midSouth(sf::Vector2f(300.0f, 5.0f));
		midSouth.setFillColor(sf::Color::White);
		midSouth.setPosition(150.0f, 400.0f);
		barriers.push_back(midSouth);
		horizontal_indexes.push_back(5);
	}

	int getSize()
	{
		return barriers.size();
	}

	// Check if object is close enough to wall to turn, return direction if so or -1 if not close enough
	int sprite_wall_distance(sf::Vector2f position)
	{
		float threshold = 30;
		int closeness = closest_wall;
		for (auto i : vertical_indexes)
		{
			float temp = position.x - barriers[i].getPosition().x;
			if ((abs(temp) < threshold))
			{
				if (temp < 0)
				{
					closeness *= east;
				}
				else
				{
					closeness *= west;
				}
			}
		}
		for (auto i : horizontal_indexes)
		{
			float temp = position.y - barriers[i].getPosition().y;
			if ((abs(temp) < threshold))
			{
				if (temp < 0)
				{
					closeness *= south;
				}
				else
				{
					closeness *= north;
				}
			}
		}
		return abs(closeness);
	}

	// checks direction of closest_wall wall for dodging Projectles
	int check_closeness(sf::Vector2f position, int vert_or_hor)
	{
		float threshold = 100000;
		// which way where the closest wall is
		int closeness;
		if (vert_or_hor == y_vert)
		{
			for (auto i : vertical_indexes)
			{
				if (position.y + (pixels_in_sprite / 2.0) >= barriers[i].getPosition().y
					&& position.y + (pixels_in_sprite / 2.0) <= barriers[i].getPosition().y + barriers[i].getSize().y)
				{
					float temp = position.x - barriers[i].getPosition().x;
					if ((abs(temp) < threshold))
					{
						threshold = abs(temp);
						if (temp < 0)
						{
							closeness = east;
						}
						else
						{
							closeness = west;
						}
					}
				}
			}
		}
		else if (vert_or_hor == x_hor)
		{
			for (auto i : horizontal_indexes)
			{
				if (position.x + (pixels_in_sprite / 2.0) >= barriers[i].getPosition().x
					&& position.x + (pixels_in_sprite / 2.0) <= barriers[i].getPosition().x + barriers[i].getSize().x)
				{
					float temp = position.y - barriers[i].getPosition().y;
					if ((abs(temp) < threshold))
					{
						threshold = abs(temp);
						if (temp < 0)
						{
							closeness = south;
						}
						else
						{
							closeness = north;
						}
					}
				}
			}
		}
		return closeness;
	}

	//check ghost collision
	bool collision(sf::FloatRect main)
	{
		for (auto i : barriers)
		{
			if (main.intersects(i.getGlobalBounds()))
			{
				return true;
			}
		}
		return false;
	}

	void draw(sf::RenderWindow& window)
	{
		for (auto i : barriers)
		{
			window.draw(i);
		}
	}

private:
	static std::vector<sf::RectangleShape> barriers;
	static std::vector<int> vertical_indexes;
	static std::vector<int> horizontal_indexes;
};

//static vars initialization
std::vector<sf::RectangleShape> Borders::barriers = {};
std::vector<int> Borders::vertical_indexes = {};
std::vector<int> Borders::horizontal_indexes = {};

#endif