#include<SFML/Graphics.hpp>
#include<ctime>
#include<vector>
#include<string>
using namespace sf;
using namespace std;

#define WIDTH 400		
#define HEIGHT 533		
#define BAR_COUNT 5	
static const float GRAVITY = 0.21f;
 
// Player Class
class Player
{
private:
	int x;
	int y;
	int imgWidth;
	int imgHeight;
	int score;
	float dy;
	Sprite* imgReady;
	Texture t2;
	bool jumpFlag;
private:
	const Sprite& GetImg()
	{
		return *imgReady;
	}

public:
	Player() : score(0), dy(0), jumpFlag(true) //constructor
	{
		x = static_cast<int>(WIDTH / 2);
		y = static_cast<int>(HEIGHT / 2);

		t2.loadFromFile("images/doodle.png");	//doodle image

		imgReady = new Sprite(t2);

		imgWidth = static_cast<int>(imgReady->getTexture()->getSize().x);
		imgHeight = static_cast<int>(imgReady->getTexture()->getSize().y);
	}
	~Player() //destructor
	{
		delete(imgReady);
	}

	void SetPosition()	//set position of doodle
	{
		imgReady->setPosition(x, y);
	}
	void Move()	//move doodle left and right
	{
		if (Keyboard::isKeyPressed(Keyboard::Right))	//Right Key
		{
			x += 4;
		}
		if (Keyboard::isKeyPressed(Keyboard::Left)){	//Left Key
			x -= 4;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
            score=0;
		}
		if (x <= 0)
		{
			x = 0;
		}
		if (x >= WIDTH - imgWidth)
		{
			x = WIDTH - imgWidth;
		}

		jumpFlag = true;
		dy += GRAVITY;
		y += static_cast<int>(dy);
		
		if (y > HEIGHT - imgHeight)	//checking condition to jump
		{
			jumpFlag = false;
			dy = -10;
			score++;
		}

	}
	void Draw(RenderWindow& window)	//Rendering images
	{
		window.draw(GetImg());
	}

	float GetDy() const	//get downward y
	{
		return dy;
	}
	int GetY() const	//get y
	{
		return y;
	}
	int GetX() const	//get x
	{
		return x;
	}
	int GetScore()		//get score
	{
		return score;
	}
	int GetWidth() const	//get width of Bar
	{
		return imgWidth;
	}
	int GetHeight() const	//get height
	{
		return imgHeight;
	}
	void SetY(int _y)	//set new y
	{
		y = _y;
	}
	void Jump()	//jump
	{
		jumpFlag = false;
		dy = -10;
		if (y >= 1)
			score++;
		else
		{
			score=0;
		}
		
	}
};


class Bar	//Bar Class
{
private:
	struct Pos	//position of bar
	{
		int x;
		int y;
	};
	vector<Pos> vBar;
	Sprite* imgBar;		//Sprite to Draw Bar, uses SFML
	Texture t;
	int imgWidth;
public:
	Bar()	//constructor
	{
		srand(static_cast<unsigned int>(time(nullptr)));

		t.loadFromFile("images/platform.png");	//load bar image
		imgBar = new Sprite(t);

		imgWidth = imgBar->getTexture()->getSize().x;

		for (int i = 0; i < BAR_COUNT; ++i)
		{
			Pos p;
			p.x = rand() % WIDTH - imgWidth / 2;
			p.y = rand() % HEIGHT;
			vBar.push_back(p);	//making bars at random positions
		}

		vBar[0].y = HEIGHT - 200;
	}
	~Bar()	//destructire
	{
		delete(imgBar);
	}
	
	void Draw(RenderWindow& window)		//Rendering
	{
		int barcount = BAR_COUNT;
		for (int i = 0; i < barcount; ++i)
		{
			imgBar->setPosition(vBar[i].x, vBar[i].y);
			window.draw(*imgBar);
		}
	}
	bool CheckCollision(Player* pPlayer)	//Check for collisions to jump
	{
		//null check.
		if (pPlayer == nullptr)
		{
			return false;
		}

		for (int i = 0; i < BAR_COUNT; ++i)
		{
			if (pPlayer->GetDy() > 0
				&& pPlayer->GetX() + pPlayer->GetWidth() > vBar[i].x
				&& pPlayer->GetX() < vBar[i].x + imgWidth
				&& pPlayer->GetY() + pPlayer->GetHeight() > vBar[i].y
				&& pPlayer->GetY() + pPlayer->GetHeight() < vBar[i].y + 10)
			{
				pPlayer->Jump();
				return true;
			}
		}
		return false;
	}
	void MoveAndReset(Player* pPlayer)	//As we move up, reset the bars, to generate new bars
	{
		static const int limit = HEIGHT / 3;
		if (pPlayer->GetY() < limit)
		{
			for (int i = 0; i < BAR_COUNT; ++i)
			{
				pPlayer->SetY(limit);
				vBar[i].y -= static_cast<int>(pPlayer->GetDy());
				if (vBar[i].y > HEIGHT + 10)
				{
					vBar[i].y = rand() % HEIGHT / 3 + 100;
					vBar[i].x = rand() % WIDTH;
				}
			}
		}
	}

};

int main(void)
{
	Texture t1;

	RenderWindow window(VideoMode(WIDTH, HEIGHT), "Doodle Game by Bhavishya and Aayush");	//Window
	window.setFramerateLimit(60);	//FPS

	t1.loadFromFile("images/background.png");
	
	Sprite sBackground(t1);
	
	Font font;
	font.loadFromFile("images/Tuffy.ttf");
	Text txt("", font);

	//setting
	Player* pPlayer = new Player();		//Player Object
	Bar* pBar = new Bar();	//Bar object
	
	txt.setString(to_string(pPlayer->GetScore()));
	txt.setCharacterSize(30);
	txt.setFillColor(Color::Black);

	while (window.isOpen())		//check if window is open
	{
		Event e;
		if (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
			{
				window.close();
			}
		}
		//logic
		pPlayer->Move();
		pBar->MoveAndReset(pPlayer);
		pBar->CheckCollision(pPlayer);
		pPlayer->SetPosition();
		
		txt.setString(to_string(pPlayer->GetScore()));
		//draw
		window.clear(Color::White);
		window.draw(sBackground);
		pPlayer->Draw(window);
		pBar->Draw(window);
		window.draw(txt);
		window.display();
	}

	delete(pBar);
	delete(pPlayer);
	return 0;
}
