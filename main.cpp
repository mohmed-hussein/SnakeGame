#include <bits/stdc++.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
using namespace std;

class Snake{
	private :
		int HeadX , HeadY , len;
		char dir;
		deque<pair<int,int>> body;

	public :
	Snake(int Xpos = 10 , int Ypos = 25){
		HeadX = Xpos;
		HeadY = Ypos;
		body.push_front({HeadX , HeadY});
		len = 1;
		dir = '&';
	}
	bool reverseDir(char _dir){
		string dirRev;
		dirRev.push_back(_dir);
		dirRev.push_back(dir);
		if(dirRev[0] > dirRev[1]) swap(dirRev[0] , dirRev[1]);
		
		if(dirRev == "ad" || dirRev == "sw") return true;
		return false;
	}
	void setDir(char _dir){
		if((!reverseDir(_dir) || len == 1) && (_dir == 'w' || _dir == 's' || _dir == 'a' || _dir == 'd'))
			dir = _dir;
	}
	pair<int,int> delta(){
		int dx = 0 , dy = 0;
		if(dir == 'w') dx = -1;
		if(dir == 's') dx =  1;
		if(dir == 'a') dy = -1;
		if(dir == 'd') dy =  1;
		return {dx , dy};
	}
	pair<int,int>getHeadPos(){
			return {HeadX , HeadY};
	}
	void move(){
		body.pop_back();
		HeadX += delta().first;
		HeadY += delta().second;
		body.push_front({HeadX, HeadY});
	}
	
	void catchFruit(){
		HeadX += delta().first;
		HeadY += delta().second;
		body.push_front({HeadX, HeadY});
		len++;
	}
	int getLen(){
			return len;
	}
	deque<pair<int,int>> getBodyPos(){
			return body;
	}
	char getDir(){
		return dir;
	}
};


class Fruit{
	private :
		int PosX , PosY;
	public :
	Fruit(int h = 20 , int w = 50){
		ChangePos(h , w);
		srand(static_cast<unsigned int>(time(0)));
	}
	pair<int,int> getPos(){
			return {PosX , PosY};
	}
	void ChangePos(int mapHight , int mapWidth){
		
		PosX = 2 + rand() % (mapHight - 3);
		PosY = 2 + rand() % (mapWidth - 3);
	}
};

class Gamemap{
	private :
		int width  , hight ;
	public :
	Gamemap(int h = 20 , int w = 50){
		width = w;
		hight = h;
	}
	void setWidth(int _w){
		width = _w;
	}
	void setHight(int _h){
		hight = _h;
	}
	int getWidth(){
		return width;
	}
	int getHight(){
		return hight;
	}
};

class SnakeGame{
	private :
		Snake snake;
		Gamemap mp;
		Fruit fruit;
		int score , speed;
		bool Lose;
	public :
		SnakeGame(int mapHight = 20 , int mapWidth = 50 , int _speed = 56){
			mp.setHight(mapHight);
			mp.setWidth(mapWidth);
			speed = _speed;
			score = 0;
		}
	
	void drow(){
		int h = mp.getHight();
		int w = mp.getWidth();
		pair<int,int> SnakeHeadPos = snake.getHeadPos();
		deque<pair<int,int>> body = snake.getBodyPos();
		for(int i = 1 ; i <= h ; i++){
			for(int j = 1 ; j <= w ; j++){
				if(i == 1 || j == 1 || i == h || j == w) cout<<'*';
				else if(SnakeHeadPos == make_pair(i , j)) cout<<'@';
				else{
					
					bool printBody = false;
					
					for(int k = 1 ; k < snake.getLen();k++)
						if(body[k] == make_pair(i , j)){
							cout<<'o';
							printBody = true;
							break;
						}
						
					if(!printBody && fruit.getPos() == make_pair(i , j)){
						cout<<'$';
					}else if(!printBody)
						cout<<' ';
				}
					
			}
			cout<<endl;
		}
		cout<<"Score : " << score<<endl<<endl;
		cout<<"\t \t Make by Eng : Mohamed Hussein."<<endl;
    }
    
    inline void sleep(int n){
        usleep(n*1000);
    }
    
    int getkey(){
		termios term;
		
		tcgetattr(0, &term);

		termios term2 = term;

		term2.c_lflag &= ~(ICANON | ECHO);

		tcsetattr(0, TCSANOW, &term2);

		int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
		fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

		int key = getchar();

		tcsetattr(0, TCSANOW, &term);

		return key;
    }
    bool collison(){
		auto [Headx , Heady] = snake.getHeadPos();
		if(Headx == 1 || Headx == mp.getHight() || Heady == 1 || Heady == mp.getWidth())
			return true;
			
		deque<pair<int,int>> body = snake.getBodyPos();
		for(int i = 1 ; i < snake.getLen(); i++){
				if(snake.getHeadPos() == body[i]) return true;
		}
		
		return false;
	}
    void run(){
		int key;
		while(!this->collison()){
			if ((key = getkey()))
				snake.setDir(key);
			snake.move();
			drow();
			if(snake.getHeadPos() == fruit.getPos()){
				snake.catchFruit();
				fruit.ChangePos(mp.getHight() , mp.getWidth());
				score++;	
			}
			sleep(speed);
			system("clear");
		}
	}
		
};


int main()
{
	SnakeGame game;
	game.run();
	
    return 0;
}
