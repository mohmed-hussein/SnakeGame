#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <iostream>
#include <deque>
#include <cstdlib> 
#include <time.h> 
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
		dir = '&';
	}
	void setDir(char _dir){
		if(_dir == 'w' || _dir == 's' || _dir == 'a' || _dir == 'd'){
		
				dir = _dir;
			
		}
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
		HeadX += delta().first;
		HeadY += delta().second;
		body.push_front({HeadX, HeadY});
		body.pop_back();
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
	}
	pair<int,int> getPos(){
			return {PosX , PosY};
	}
	void ChangePos(int mapHight , int mapWidth){
		mapHight--;
		mapWidth--;
		PosX = (rand() % mapHight) + 1;
		PosY = (rand() % mapWidth) + 1;
	}
};

class map{
	private :
		int width  , hight ;
	public :
	map(int h = 20 , int w = 50){
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
		map mp;
		Fruit fruit;
		int score , speed;
	public :
		SnakeGame(int mapHight = 20 , int mapWidth = 50 , int _speed = 52){
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
					for(int k = 0 ; k < snake.getLen();k++)
						if(body[k] == make_pair(i , j)){
							cout<<'o';
							printBody = true;
							break;
						}
					if(!printBody && fruit.getPos() == make_pair(i , j)){
						cout<<'$';
						continue;
					}
					if(!printBody)
						cout<<' ';
				}
			}
			cout<<endl;
		}
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
    void run(){
		int key;
		while(true){
			if ((key = getkey()))
				snake.setDir(key);
			if(snake.getHeadPos() == fruit.getPos()){
				snake.catchFruit();
				fruit.ChangePos(mp.getHight() , mp.getWidth());
				score++;	
			}
			snake.move();
			drow();
			
			cout<<"Score : " << score<<endl;
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
