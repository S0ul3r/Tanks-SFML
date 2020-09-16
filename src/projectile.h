#ifndef projectile_h
#define projectile_h
#include "game_level.h"

class Projectles
{
public:
	Projectles()
	{
		projectle.setRadius(3.0f);
		projectle.setOrigin(3.0f, 3.0f);
		projectle.setFillColor(sf::Color::Magenta);
		which_way = sf::Vector2f(0, 0);
		rebound = false;
	}

	// set position of projectile where mouse is
	void fire(sf::Vector2f start, sf::Vector2i destination)
	{
		sf::Vector2f constant(destination.x - start.x, destination.y - start.y);
		float scale = sqrt((constant.x * constant.x) + (constant.y * constant.y));
		constant = sf::Vector2f(constant.x / (scale / shooting_speed), constant.y / (scale / shooting_speed));
		which_way = constant;
		//fix for fps cap and shoot_speed
		which_way.x *= 20;
		which_way.y *= 20;
		projectle.setPosition(start.x + (constant.x * 150), start.y + (constant.y * 150));
	}

	// get which_way
	sf::Vector2f getwhich_way()
	{
		return which_way;
	}

	// projectile keeps on flying
	void move()
	{
		projectle.move(which_way);
	}

	bool getRebound()
	{
		return rebound;
	}

	// projectile ricochettingh
	void ricochet()
	{
		sf::RectangleShape west(sf::Vector2f(1.0, 1.0));
		west.setPosition(projectle.getPosition().x - 4, projectle.getPosition().y);

		sf::RectangleShape east(sf::Vector2f(1.0, 1.0));
		east.setPosition(projectle.getPosition().x + 4, projectle.getPosition().y);

		sf::RectangleShape north(sf::Vector2f(1.0, 1.0));
		north.setPosition(projectle.getPosition().x, projectle.getPosition().y - 4);

		sf::RectangleShape south(sf::Vector2f(1.0, 1.0));
		south.setPosition(projectle.getPosition().x, projectle.getPosition().y + 4);

		//check collision N/S or W/E and *-1 hor/vert
		if (barriers.collision(west.getGlobalBounds()) || barriers.collision(east.getGlobalBounds()))
		{
			which_way = sf::Vector2f(which_way.x * -1, which_way.y);
		}
		else if (barriers.collision(north.getGlobalBounds()) || barriers.collision(south.getGlobalBounds()))
		{
			which_way = sf::Vector2f(which_way.x, which_way.y * -1);
		}
		rebound = true;
	}

	sf::FloatRect getBorders()
	{
		return projectle.getGlobalBounds();
	}

	// check if projectle will hit by getting distance from the projectle to corners and center of ghost and check all distances and return which_way of projectile if no hit  return 0,0
	sf::Vector2f will_hit(sf::RectangleShape destination)
	{
		sf::Vector2f size = destination.getSize();
		sf::Vector2f target = destination.getPosition();
		sf::Vector2f position = projectle.getPosition();
		sf::Vector2f trajectory = sf::Vector2f(target.x - position.x, target.y - position.y);
		float XYdistance = sqrt((trajectory.x * trajectory.x) + (trajectory.y * trajectory.y));
		float scale = XYdistance / sqrt((which_way.x * which_way.x) + (which_way.y * which_way.y));
		sf::Vector2f end = sf::Vector2f(position.x + (which_way.x * scale), position.y + (which_way.y * scale));
		sf::CircleShape hitbox;
		hitbox.setRadius(9.0f);
		hitbox.setOrigin(2.0, 2.0);
		hitbox.setPosition(end);
		if (destination.getGlobalBounds().intersects(hitbox.getGlobalBounds()))
		{
			return which_way;
		}

		// North-West corner
		target = sf::Vector2f(target.x - (size.x / 2.0), target.y - (size.y / 2.0));
		position = projectle.getPosition();
		trajectory = sf::Vector2f(target.x - position.x, target.y - position.y);
		XYdistance = sqrt((trajectory.x * trajectory.x) + (trajectory.y * trajectory.y));
		scale = XYdistance / sqrt((which_way.x * which_way.x) + (which_way.y * which_way.y));
		end = sf::Vector2f(position.x + (which_way.x * scale), position.y + (which_way.y * scale));
		hitbox.setPosition(end);
		if (destination.getGlobalBounds().intersects(hitbox.getGlobalBounds()))
		{
			return which_way;
		}

		// North-East corner
		target = sf::Vector2f(target.x + size.x, target.y);
		position = projectle.getPosition();
		trajectory = sf::Vector2f(target.x - position.x, target.y - position.y);
		XYdistance = sqrt((trajectory.x * trajectory.x) + (trajectory.y * trajectory.y));
		scale = XYdistance / sqrt((which_way.x * which_way.x) + (which_way.y * which_way.y));
		end = sf::Vector2f(position.x + (which_way.x * scale), position.y + (which_way.y * scale));
		hitbox.setPosition(end);
		if (destination.getGlobalBounds().intersects(hitbox.getGlobalBounds()))
		{
			return which_way;
		}

		// South-East corner
		target = sf::Vector2f(target.x, target.y + size.y);
		position = projectle.getPosition();
		trajectory = sf::Vector2f(target.x - position.x, target.y - position.y);
		XYdistance = sqrt((trajectory.x * trajectory.x) + (trajectory.y * trajectory.y));
		scale = XYdistance / sqrt((which_way.x * which_way.x) + (which_way.y * which_way.y));
		end = sf::Vector2f(position.x + (which_way.x * scale), position.y + (which_way.y * scale));
		hitbox.setPosition(end);
		if (destination.getGlobalBounds().intersects(hitbox.getGlobalBounds()))
		{
			return which_way;
		}

		// South-West corner
		target = sf::Vector2f(target.x - size.x, target.y);
		position = projectle.getPosition();
		trajectory = sf::Vector2f(target.x - position.x, target.y - position.y);
		XYdistance = sqrt((trajectory.x * trajectory.x) + (trajectory.y * trajectory.y));
		scale = XYdistance / sqrt((which_way.x * which_way.x) + (which_way.y * which_way.y));
		end = sf::Vector2f(position.x + (which_way.x * scale), position.y + (which_way.y * scale));
		hitbox.setPosition(end);
		if (destination.getGlobalBounds().intersects(hitbox.getGlobalBounds()))
		{
			return which_way;
		}

		//return 0,0 when no barriers intersect
		return sf::Vector2f(0.0, 0.0);
	}

	void draw(sf::RenderWindow& window)
	{
		window.draw(projectle);
	}

private:
	Borders barriers;
	sf::CircleShape projectle;
	sf::Vector2f which_way;
	bool rebound;
};

// individual Projectles
class Specprojectile
{
public:
	Specprojectile()
	{}

	// firing projectile
	void fire(sf::Vector2f start, sf::Vector2i destination)
	{
		if (projectile.size() <= 4)
		{
			Projectles shot;
			shot.fire(start, destination);
			projectile.push_back(shot);
		}
	}

	// overall flying all projectiles
	void bullet_movement(sf::FloatRect obstacle)
	{
		for (int i = 0; (unsigned)i < projectile.size(); i++)
		{
			if (!projectile[i].getBorders().intersects(obstacle))
			{
				if (!barriers.collision(projectile[i].getBorders()))
				{
					projectile[i].move();
				}
				else
				{
					if (!projectile[i].getRebound())
					{
						projectile[i].ricochet();
						projectile[i].move();
					}
					else
					{
						update(i);
					}
				}
			}
		}
	}

	// check if any projectiles will hit the ghost
	sf::Vector2f will_hit(sf::RectangleShape destination)
	{
		for (int i = 0; (unsigned)i < projectile.size(); i++)
		{
			if (projectile[i].will_hit(destination) != sf::Vector2f(0.0, 0.0))
			{
				return projectile[i].getwhich_way();
			}
		}
		return sf::Vector2f(0.0, 0.0);
	}

	// draws Projectles
	void draw(sf::RenderWindow& window)
	{
		for (auto i : projectile)
		{
			i.draw(window);
		}
	}

	// check if nay projectiles hit barriers
	int collision(sf::FloatRect main)
	{
		for (int i = 0; (unsigned)i < projectile.size(); i++)
		{
			if (main.intersects(projectile[i].getBorders()))
			{
				return i;
			}
		}
		return -1;
	}

	// erase projectle if it hit wall/sprite or ricochetted
	void update(int position)
	{
		if (position >= 0)
		{
			projectile.erase(projectile.begin() + position);
		}
	}

private:
	// static so all objects use the same projectile
	static std::vector<Projectles> projectile;
	Borders barriers;
};

std::vector<Projectles> Specprojectile::projectile = {};

#endif