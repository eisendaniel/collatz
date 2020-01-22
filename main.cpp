#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <cmath>
#define GOLDEN 1.61803398875

std::vector<int> collatz(int n)
{
	std::vector<int> seq = {};
	while (n > 1) {
		if (n % 2) {
			n = 3 * n + 1;
			seq.push_back(0);
		} else {
			n /= 2;
			seq.push_back(1);
		}
	}
	std::reverse(seq.begin(), seq.end());
	return seq;
}

int main()
{
	int width = 1080;
	int height = 1000;
	int border = (width - (width / GOLDEN)) / 4;
	int seg = 5;
	float d_theta[] = {M_PI / 15.0, -(M_PI / 30.0)};
	float x, y;
	double theta;
	std::vector<int> parity;
	sf::Color background(0, 0, 0, 255);
	sf::Color line(255, 255, 255, 255);
	sf::Color clear(0, 0, 0, 0);
	sf::VertexArray path(sf::LineStrip);
	std::vector<sf::VertexArray> paths;

	for (int i = 0; i < 10000; ++i) {
		theta = M_PI_2;
		parity = collatz(i);
		x = 1.0 / GOLDEN * width;
		y = height - border;
		path.append(sf::Vertex(sf::Vector2f(x, y), clear));
		path.append(sf::Vertex(sf::Vector2f(x, y), line));
		for (auto &p : parity) {
			theta += d_theta[p];
			x += (seg * cos(theta));
			y -= (seg * sin(theta));
			path.append(sf::Vertex(sf::Vector2f(x, y), line));
		}
		path.append(sf::Vertex(sf::Vector2f(x, y), clear));
		paths.push_back(path);
		path.clear();
	}


	/*Setup window and frame-rate
	 * init default colors
	 * */
	sf::RenderWindow window(sf::VideoMode(width, height), "Collatz", sf::Style::Close);
	window.setFramerateLimit(1);
	while (window.isOpen()) { //lifetime of program
		sf::Event event = {};
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) { window.close(); }
		}
		window.clear(background);
		for (auto &p : paths) {
			window.draw(p);
		}
		window.display();
	}
	return 0;
}