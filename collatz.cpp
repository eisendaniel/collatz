#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <cmath>

//CONSTANTS for windows and canvas sizes
#define GOLDEN 1.61803398875
#define WIDTH 1000
#define HEIGHT 1000
#define BORDER (WIDTH-(WIDTH/GOLDEN))/8

enum dir { left, right };

/* Implementation of Collatz Conjecture calculator
 * for given n, runs through sequence
 * producing sequence, left for odd, right for even
 */
void collatz(unsigned int n, std::vector<int> &seq)
{
	while (n > 1) {
		if (n % 2) { //if odd
			n = 3 * n + 1;
			seq.push_back(left);
		} else { //if even
			n /= 2;
			seq.push_back(right);
		}
	}
	std::reverse(seq.begin(), seq.end());
}

/* Generator of the Vertex Array of lines
 * */
void genPaths(unsigned int n, float d_left, float d_right, std::vector<sf::VertexArray> &paths)
{
	float d_theta[] = {d_left, d_right};
	sf::VertexArray path(sf::LineStrip);
	std::vector<int> parity;
	float x, y;
	double theta;
	int seg = 5, c;

	paths.clear();
	for (unsigned int i = 0; i < n; ++i) {
		path.clear();
		parity.clear();
		collatz(i, parity);
		theta = M_PI_2;
		x = WIDTH / 2.0;
		y = HEIGHT - BORDER;
		path.append(sf::Vertex(sf::Vector2f(x, y), sf::Color(0, 0, 0, 0)));
		path.append(sf::Vertex(sf::Vector2f(x, y), sf::Color::Black));
		c = 1;
		for (auto &p : parity) {
			theta += d_theta[p];
			x += (seg * cos(theta));
			y -= (seg * sin(theta));
			path.append(sf::Vertex(sf::Vector2f(x, y), sf::Color(0, 0, 0,
									     255 - (255 * c++ / parity.size()))));
		}
		path.append(sf::Vertex(sf::Vector2f(x, y), sf::Color(0, 0, 0, 0)));
		paths.push_back(path);
	}
}

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Collatz", sf::Style::Close, settings);
	window.setFramerateLimit(30);

	float d_left = M_PI / 15;
	float d_right = -(M_PI / 30);
	unsigned int n = 11;
	std::vector<sf::VertexArray> paths;
	genPaths(n, d_left, d_right, paths);

	while (window.isOpen()) { //lifetime of program
		sf::Event event = {};
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) { window.close(); }
			else if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) { window.close(); }
				else if (event.key.code == sf::Keyboard::Right) {
					d_left -= 0.0005;
					d_right -= 0.0005;
					genPaths(n, d_left, d_right, paths);
				} else if (event.key.code == sf::Keyboard::Left) {
					d_left += 0.0005;
					d_right += 0.0005;
					genPaths(n, d_left, d_right, paths);
				} else if (event.key.code == sf::Keyboard::Up) {
					n += 100;
					genPaths(n, d_left, d_right, paths);
				} else if (event.key.code == sf::Keyboard::Down) {
					n = (n - 100 > 0) ? n - 100 : n;
					genPaths(n, d_left, d_right, paths);
				}
			}
		}
		window.clear(sf::Color(200, 200, 200));
		for (auto &p : paths) {
			window.draw(p);
		}
		window.display();
	}
	return 0;
}