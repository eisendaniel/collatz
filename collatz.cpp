#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <map>
#include <algorithm>
#include <cstdio>
#include <cmath>
#include <sstream>

//CONSTANTS for windows and canvas sizes
#define WIDTH 1000
#define HEIGHT 1000
#define BORDER 64

enum dir { left, right };
sf::Clock timer;
std::map<unsigned int, std::vector<int>> computed;
sf::Music m;

/* Implementation of Collatz Conjecture calculator
 * for given n, runs through sequence
 * producing sequence, left for odd, right for even
 */
void collatz(unsigned int start, std::vector<int> &parity)
{
	unsigned int n = start;
	while (n > 1) {
		if (computed.count(n)) {
			parity.insert(parity.end(), computed[n].begin(), computed[n].end());
			break;
		}
		if (n % 2) { //if odd
			n = 3 * n + 1;
			parity.push_back(left);
		} else { //if even
			n /= 2;
			parity.push_back(right);
		}
	}
	computed.insert(std::pair<unsigned int, std::vector<int>>(start, parity));
	std::reverse(parity.begin(), parity.end());
}

/* Generator of the Vertex Array of lines
 * */
void genPath(unsigned int n, float d_left, float d_right, sf::VertexArray &path, sf::RenderWindow &window)
{
	float d_theta[] = {d_left, d_right};
	std::vector<int> parity;
	float x, y;
	double theta;
	int seg = ceil(window.getSize().y / 200.0), c;

	path.clear();
	for (unsigned int i = 0; i < n; ++i) {
		parity.clear();
		collatz(i, parity);
		theta = M_PI_2;
		x = window.getSize().x / 2.0;
		y = window.getSize().y - BORDER;
		path.append(sf::Vertex(sf::Vector2f(x, y), sf::Color(0, 0, 0, 0)));
		path.append(sf::Vertex(sf::Vector2f(x, y), sf::Color(0, 0, 0)));
		c = 1;
		for (auto &p : parity) {
			theta += d_theta[p];
			x += (seg * cos(theta));
			y -= (seg * sin(theta));
			path.append(sf::Vertex(sf::Vector2f(x, y), sf::Color(0, 0, 0,
									     255 - (255 * c++ / parity.size()))));
		}
		path.append(sf::Vertex(sf::Vector2f(x, y), sf::Color(0, 0, 0, 0)));
	}
}

void screencapture(sf::RenderWindow &window)
{
	sf::Vector2u windowSize = window.getSize();
	sf::Texture texture;
	texture.create(windowSize.x, windowSize.y);
	texture.update(window);
	sf::Image screenshot = texture.copyToImage();
	char filename[128];
	sprintf(filename, "screenshots/screenshot_%d.png", timer.getElapsedTime().asMilliseconds());
	screenshot.saveToFile(filename);
	m.openFromFile("resouces/click.wav");
	m.play();
	timer.restart();
}

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Collatz", sf::Style::Default, settings);
	window.setFramerateLimit(60);

	sf::Font font;
	font.loadFromFile("resouces/inconsolata.ttf");
	std::ostringstream s;
	sf::Text count;
	count.setFont(font);
	count.setFillColor(sf::Color::Black);
	count.setPosition(window.getSize().x / 2, window.getSize().y - BORDER);

	float d_left = (M_PI / 15);
	float d_right = -(M_PI / 30);
	unsigned int n = 1000;
	sf::VertexArray path(sf::LineStrip);

	genPath(n, d_left, d_right, path, window);

	while (window.isOpen()) { //lifetime of program
		sf::Event event = {};
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
				break;
			} else if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					window.close();
					break;
				} else if (event.key.code == sf::Keyboard::Space) { screencapture(window); }
			} // catch the resize events
			else if (event.type == sf::Event::Resized) {
				// update the view to the new size of the window
				sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
				window.setView(sf::View(visibleArea));
				count.setPosition(window.getSize().x / 2, window.getSize().y - BORDER);
				genPath(n, d_left, d_right, path, window);
			}
		}
		window.clear(sf::Color(200, 200, 200));

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			d_left -= 0.0005;
			d_right -= 0.0005;
			genPath(n, d_left, d_right, path, window);
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			d_left += 0.0005;
			d_right += 0.0005;
			genPath(n, d_left, d_right, path, window);
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			if (n < 100) {
				n += 1;
			} else {
				n += 100;
			}
			genPath(n, d_left, d_right, path, window);
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			if (n <= 0) {
				n = 0;
			} else if (n < 100) {
				n -= 1;
			} else {
				n -= 100;
			}
			genPath(n, d_left, d_right, path, window);
		}
		s.str("");
		s << n;
		count.setString(s.str());
		window.draw(path);
		window.draw(count);
		if (timer.getElapsedTime().asMilliseconds() < 200) {
			sf::RectangleShape rectangle;
			rectangle.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
			rectangle.setFillColor(sf::Color(0, 0, 0, 64));
			rectangle.setPosition(0, 0);
			window.draw(rectangle);
		}
		window.display();
	}
	return 0;
}