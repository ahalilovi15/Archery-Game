#include "mbed.h"
#include "stm32f413h_discovery_ts.h"
#include "stm32f413h_discovery_lcd.h"
#include <vector>
#include <string>
#include <iostream>
#include <cstring>
using namespace std;
DigitalIn btnLeft(p5), btnRight(p6), btnUp(p7), btnDown(p8), btnOK(p9);


TS_StateTypeDef TS_State = { 0 };

bool add = false;

void drawShape(short x, short y, uint16_t color)
{
	short length = 6;
	BSP_LCD_SetTextColor(LCD_COLOR_DARKGRAY);
	BSP_LCD_DrawCircle(x, y, length);
	BSP_LCD_SetTextColor(color);
	BSP_LCD_FillCircle(x, y, length-1);
	BSP_LCD_SetTextColor(LCD_COLOR_DARKGRAY);
	BSP_LCD_FillEllipse(x, y+7, 12, length);
}

void removeShape(short x, short y)
{
	short length = 6;
	BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
	BSP_LCD_FillCircle(x, y, length);
	BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
	BSP_LCD_FillCircle(x, y, length-1);
	BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
	BSP_LCD_FillEllipse(x, y+7, 12, length);
}

bool pointInElipse(short x, short y, short a, short b)
{
	return ((a-x)*(a-x)/144 + (b-y-7)*(b-y-7)/36) <= 1;
}

bool pointInCircle(short x, short y, short a, short b)
{
	return ((a-x)*(a-x) + (b-y)*(b-y)) <= 36;
}

void drawClouds()
{
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_FillCircle (194, 8, 6);
	BSP_LCD_FillCircle (184, 9, 4);
	BSP_LCD_FillEllipse (190, 15, 14, 7);
	BSP_LCD_FillCircle (58, 13, 8);
	BSP_LCD_FillCircle (44, 15, 6);
	BSP_LCD_FillEllipse (52, 22, 20, 8);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
}

short score;

void drawBackground()
{
	BSP_LCD_SetBackColor (LCD_COLOR_CYAN);
	BSP_LCD_Clear (LCD_COLOR_CYAN);
	Point Points2[]= {{250, 250}, {0, 250}, {0, 210}, {250,210}};
	BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
	BSP_LCD_FillPolygon(Points2, 4);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
}

void drawKeyboard()
{
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
	BSP_LCD_FillRect (65, 180, 110, 25);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DrawRect (65,180,110,25);
	BSP_LCD_SetBackColor (LCD_COLOR_LIGHTGRAY);
	BSP_LCD_DisplayStringAt(0, 190, (uint8_t *)"ENTER", CENTER_MODE);
	BSP_LCD_SetBackColor (LCD_COLOR_CYAN);
	BSP_LCD_DrawRect (10,25,15,15);
	BSP_LCD_DisplayStringAt(15, 27.5, (uint8_t *)"A", LEFT_MODE);
	BSP_LCD_DrawRect (30,25,15,15);
	BSP_LCD_DisplayStringAt(35, 27.5, (uint8_t *)"B", LEFT_MODE);
	BSP_LCD_DrawRect (50,25,15,15);
	BSP_LCD_DisplayStringAt(55, 27.5, (uint8_t *)"C", LEFT_MODE);
	BSP_LCD_DrawRect (70,25,15,15);
	BSP_LCD_DisplayStringAt(75, 27.5, (uint8_t *)"D", LEFT_MODE);
	BSP_LCD_DrawRect (90,25,15,15);
	BSP_LCD_DisplayStringAt(95, 27.5, (uint8_t *)"E", LEFT_MODE);
	BSP_LCD_DrawRect (110,25,15,15);
	BSP_LCD_DisplayStringAt(115, 27.5, (uint8_t *)"F", LEFT_MODE);
	BSP_LCD_DrawRect (130,25,15,15);
	BSP_LCD_DisplayStringAt(135, 27.5, (uint8_t *)"G", LEFT_MODE);
	BSP_LCD_DrawRect (150,25,15,15);
	BSP_LCD_DisplayStringAt(155, 27.5, (uint8_t *)"H", LEFT_MODE);
	BSP_LCD_DrawRect (170,25,15,15);
	BSP_LCD_DisplayStringAt(175, 27.5, (uint8_t *)"I", LEFT_MODE);
	BSP_LCD_DrawRect (190,25,15,15);
	BSP_LCD_DisplayStringAt(195, 27.5, (uint8_t *)"J", LEFT_MODE);
	BSP_LCD_DrawRect (210,25,15,15);
	BSP_LCD_DisplayStringAt(215, 27.5, (uint8_t *)"K", LEFT_MODE);
	BSP_LCD_DrawRect (10,45,15,15);
	BSP_LCD_DisplayStringAt(15, 47.5, (uint8_t *)"L", LEFT_MODE);
	BSP_LCD_DrawRect (30,45,15,15);
	BSP_LCD_DisplayStringAt(35, 47.5, (uint8_t *)"M", LEFT_MODE);
	BSP_LCD_DrawRect (50,45,15,15);
	BSP_LCD_DisplayStringAt(55, 47.5, (uint8_t *)"N", LEFT_MODE);
	BSP_LCD_DrawRect (70,45,15,15);
	BSP_LCD_DisplayStringAt(75, 47.5, (uint8_t *)"O", LEFT_MODE);
	BSP_LCD_DrawRect (90,45,15,15);
	BSP_LCD_DisplayStringAt(95, 47.5, (uint8_t *)"P", LEFT_MODE);
	BSP_LCD_DrawRect (110,45,15,15);
	BSP_LCD_DisplayStringAt(115, 47.5, (uint8_t *)"Q", LEFT_MODE);
	BSP_LCD_DrawRect (130,45,15,15);
	BSP_LCD_DisplayStringAt(135, 47.5, (uint8_t *)"R", LEFT_MODE);
	BSP_LCD_DrawRect (150,45,15,15);
	BSP_LCD_DisplayStringAt(155, 47.5, (uint8_t *)"S", LEFT_MODE);
	BSP_LCD_DrawRect (170,45,15,15);
	BSP_LCD_DisplayStringAt(175, 47.5, (uint8_t *)"T", LEFT_MODE);
	BSP_LCD_DrawRect (190,45,15,15);
	BSP_LCD_DisplayStringAt(195, 47.5, (uint8_t *)"U", LEFT_MODE);
	BSP_LCD_DrawRect (210,45,15,15);
	BSP_LCD_DisplayStringAt(215, 47.5, (uint8_t *)"V", LEFT_MODE);
	BSP_LCD_DrawRect (80,65,15,15);
	BSP_LCD_DisplayStringAt(85, 67.5, (uint8_t *)"W", LEFT_MODE);
	BSP_LCD_DrawRect (100,65,15,15);
	BSP_LCD_DisplayStringAt(105, 67.5, (uint8_t *)"X", LEFT_MODE);
	BSP_LCD_DrawRect (120,65,15,15);
	BSP_LCD_DisplayStringAt(125, 67.5, (uint8_t *)"Y", LEFT_MODE);
	BSP_LCD_DrawRect (140,65,15,15);
	BSP_LCD_DisplayStringAt(145, 67.5, (uint8_t *)"Z", LEFT_MODE);
}

void modeSelectionView()
{
	BSP_LCD_SetTextColor(LCD_COLOR_DARKCYAN);
	BSP_LCD_FillRect (90, 152, 55, 25);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DrawRect (90, 152, 55, 25);
	BSP_LCD_SetBackColor (LCD_COLOR_DARKCYAN);
	BSP_LCD_DisplayStringAt(0, 160, (uint8_t *)"HARD", CENTER_MODE);

	BSP_LCD_SetTextColor(LCD_COLOR_DARKCYAN);
	BSP_LCD_FillRect (90, 112, 55, 25);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DrawRect (90, 112, 55, 25);
	BSP_LCD_SetBackColor (LCD_COLOR_DARKCYAN);
	BSP_LCD_DisplayStringAt(0, 120, (uint8_t *)"MEDIUM", CENTER_MODE);

	BSP_LCD_SetTextColor(LCD_COLOR_DARKCYAN);
	BSP_LCD_FillRect (90, 72, 55, 25);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DrawRect (90, 72, 55, 25);
	BSP_LCD_SetBackColor (LCD_COLOR_DARKCYAN);
	BSP_LCD_DisplayStringAt(0, 80, (uint8_t *)"EASY", CENTER_MODE);
}

void winView(short score)
{
	char ispis[10];
	BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
	BSP_LCD_DisplayStringAt(0, 80, (uint8_t *)"CONGRATULATIONS", CENTER_MODE);
	sprintf(ispis, "SCORE: %d", score);
	BSP_LCD_DisplayStringAt(0, 120, (uint8_t *)ispis, CENTER_MODE);
	BSP_LCD_DisplayStringAt(0, 100, (uint8_t *)" YOU PASSED ALL LEVELS", CENTER_MODE);
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
	BSP_LCD_FillRect (65, 180, 110, 25);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DrawRect (65,180,110,25);
	BSP_LCD_SetBackColor (LCD_COLOR_LIGHTGRAY);
	BSP_LCD_DisplayStringAt(0, 190, (uint8_t *)"MAIN MENU", CENTER_MODE);
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
	BSP_LCD_FillRect (65, 147, 110, 25);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DrawRect (65, 147, 110, 25);
	BSP_LCD_SetBackColor (LCD_COLOR_LIGHTGRAY);
	BSP_LCD_DisplayStringAt(0, 157, (uint8_t *)"RANKING", CENTER_MODE);
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
	BSP_LCD_FillRect (65, 213, 110, 25);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DrawRect (65, 213, 110, 25);
	BSP_LCD_SetBackColor (LCD_COLOR_LIGHTGRAY);
	BSP_LCD_DisplayStringAt(0, 223, (uint8_t *)"PLAY AGAIN", CENTER_MODE);
}

void gameOverView(short score, short level)
{
	char ispis[10];
	BSP_LCD_DisplayStringAt(0, 67, (uint8_t *)"GAME OVER", CENTER_MODE);
	sprintf(ispis, "SCORE: %d", score);
	BSP_LCD_DisplayStringAt(0, 87, (uint8_t *)ispis, CENTER_MODE);
	sprintf(ispis, "LEVEL: %d", level);
	BSP_LCD_DisplayStringAt(0, 107, (uint8_t *)ispis, CENTER_MODE);
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
	BSP_LCD_FillRect (65, 147, 110, 25);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DrawRect (65, 147, 110, 25);
	BSP_LCD_SetBackColor (LCD_COLOR_LIGHTGRAY);
	BSP_LCD_DisplayStringAt(0, 157, (uint8_t *)"RANKING", CENTER_MODE);
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
	BSP_LCD_FillRect (65, 180, 110, 25);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DrawRect (65, 180, 110, 25);
	BSP_LCD_DrawRect (65, 47, 110, 90);
	BSP_LCD_SetBackColor (LCD_COLOR_LIGHTGRAY);
	BSP_LCD_DisplayStringAt(0, 190, (uint8_t *)"MAIN MENU", CENTER_MODE);
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
	BSP_LCD_FillRect (65, 213, 110, 25);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DrawRect (65, 213, 110, 25);
	BSP_LCD_SetBackColor (LCD_COLOR_LIGHTGRAY);
	BSP_LCD_DisplayStringAt(0, 223, (uint8_t *)"PLAY AGAIN", CENTER_MODE);
}

int showRemainingTime(short vrijeme, short mode )
{
	char ispis_vrijeme[10];
	int vrijeme2 = 20 - vrijeme;
	if(mode == 2) vrijeme2 = 10 - vrijeme;
	if(mode == 3) vrijeme2 = 20 - vrijeme;
	//sprintf(ispis_vrijeme, "%d", vrijeme2);
	char ispis_vremena[20];
	if(vrijeme2 >= 10)
		strcpy(ispis_vremena, "00:");
	else
		strcpy(ispis_vremena, "00:0");
	sprintf(ispis_vrijeme, "%d", vrijeme2);
	strcat(ispis_vremena, ispis_vrijeme);
	BSP_LCD_DisplayStringAt(0, 225, (uint8_t *)ispis_vremena, RIGHT_MODE);
	return vrijeme2;
}

void showCurrentLevel(short level)
{
	char ispis_nivoa[10];
	char trenutni_nivo[20];
	strcpy(trenutni_nivo, "Level: ");
	sprintf(ispis_nivoa, "%d", level);
	strcat(trenutni_nivo, ispis_nivoa);
	BSP_LCD_DisplayStringAt(0, 225, (uint8_t *)trenutni_nivo, LEFT_MODE);
}

void showCurrentScore(short score)
{
	char ispis[10];
	sprintf(ispis, "%d", score);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_SetBackColor (LCD_COLOR_GREEN);
	BSP_LCD_DisplayStringAt(0, 225, (uint8_t *)ispis, CENTER_MODE);
}

void rankingView()
{
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
	BSP_LCD_FillRect (65, 180, 110, 25);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DrawRect (65, 180, 110, 25);
	BSP_LCD_SetBackColor (LCD_COLOR_LIGHTGRAY);
	BSP_LCD_DisplayStringAt(0, 190, (uint8_t *)"MAIN MENU", CENTER_MODE);

	BSP_LCD_SetBackColor (LCD_COLOR_CYAN);
	BSP_LCD_DisplayStringAt(0, 50, (uint8_t *)"SCORE", CENTER_MODE);
	BSP_LCD_DisplayStringAt(160, 50, (uint8_t *)"NAME", LEFT_MODE);
	BSP_LCD_DisplayStringAt(50, 50, (uint8_t *)"RANK", LEFT_MODE);
	BSP_LCD_DisplayStringAt(50, 70, (uint8_t *)"1ST", LEFT_MODE);
	BSP_LCD_DisplayStringAt(50, 90, (uint8_t *)"2ND", LEFT_MODE);
	BSP_LCD_DisplayStringAt(50, 110, (uint8_t *)"3RD", LEFT_MODE);
	BSP_LCD_DisplayStringAt(50, 130, (uint8_t *)"4TH", LEFT_MODE);
	BSP_LCD_DisplayStringAt(50, 150, (uint8_t *)"5TH", LEFT_MODE);
}

void mainMenuView()
{
	BSP_LCD_SetTextColor(LCD_COLOR_DARKCYAN);
	BSP_LCD_FillRect (55, 100, 130, 30);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DrawRect (55, 100, 130, 30);
	BSP_LCD_SetBackColor (LCD_COLOR_DARKCYAN);
	BSP_LCD_DisplayStringAt(0, 110, (uint8_t *)"ENTER YOUR NAME", CENTER_MODE);
}

class Shape
{   
public:
	virtual bool isDead() = 0;
	virtual void move(short displacement) = 0;
	virtual void killIfHit(short a, short b, short points) = 0;
};

class RedUFO : public Shape
{
	short x, y, length;
	bool leftRight = true, dead = false;
public:
	RedUFO(short b, bool direction = true)
	{
		y = b;
		length = 6;
		leftRight = direction;
		if (leftRight)
			x = 0;
		else
			x = 250 - length;
		drawShape(x, y, LCD_COLOR_LIGHTRED);
	}
	bool isDead()
	{
		return dead;
	}
	void move(short displacement)
	{
		removeShape(x,y);
		x += leftRight*displacement - (!leftRight)*displacement;
		drawShape(x,y,LCD_COLOR_LIGHTRED);
		if (leftRight && x >= 255 || !leftRight && x <= 0-length-10)
			dead = true;
	}
	void killIfHit(short a, short b, short points)
	{
		if (pointInElipse(x, y, a, b) || pointInCircle(x, y, a, b)) {
			removeShape(x, y);
			dead = true;
			score += points;
		}
	}
	friend void drawShape(short x, short y, uint16_t color);
	friend void removeShape(short x, short y);
	friend bool pointInElipse(short x, short y, short a, short b);
	friend bool pointInCircle(short x, short y, short a, short b);
};

class BlueUFO : public Shape
{
	short x, y, length, yRange;
	bool leftRight = true, downUp = true, dead = false;
public:
	BlueUFO(short b, bool direction = true)
	{
		y = b;
		yRange = b;
		length = 6;
		leftRight = direction;
		if (leftRight)
			x = 0;
		else
			x = 250 - length;
		drawShape(x, y, LCD_COLOR_LIGHTBLUE);
	}
	bool isDead()
	{
		return dead;
	}
	void move(short displacement)
	{
		removeShape(x, y);
		x += leftRight*displacement - (!leftRight)*displacement;
		if (y >= yRange + 15)
			downUp = true;
		else if (y <= yRange - 15)
			downUp = false;
		y += (!downUp)*displacement - downUp*displacement;
		drawShape(x, y, LCD_COLOR_LIGHTBLUE);
		if (leftRight && x >= 255 || !leftRight && x <= 0 - length - 10)
			dead = true;
	}
	void killIfHit(short a, short b, short points)
	{
		if (pointInElipse(x, y, a, b) || pointInCircle(x, y, a, b)) {
			removeShape(x, y);
			dead = true;
			score += points;
		}
	}
	friend void drawShape(short x, short y, uint16_t color);
	friend void removeShape(short x, short y);
	friend bool pointInElipse(short x, short y, short a, short b);
	friend bool pointInCircle(short x, short y, short a, short b);
};

class GreenUFO : public Shape
{
	short x, y, length;
	bool leftRight = true, down = false, up = false, dead = false;
public:
	GreenUFO(short b, bool direction = true)
	{
		y = b;
		length = 6;
		leftRight = direction;
		if (leftRight)
			x = 0;
		else
			x = 250 - length;
		drawShape(x, y, LCD_COLOR_LIGHTGREEN);
	}
	bool isDead()
	{
		return dead;
	}
	void move(short displacement)
	{
		removeShape(x,y);
		x += leftRight*displacement - (!leftRight)*displacement;
		up = down = false;
		if (btnDown && y - displacement >= 5)
			up = true;
		else if (btnUp && y + displacement < 197)
			down = true;
		y += down*displacement - up*displacement;
		drawShape(x, y, LCD_COLOR_LIGHTGREEN);
		if (leftRight && x >= 255 || !leftRight && x <= 0 - length - 10)
			dead = true;
	}
	void killIfHit(short a, short b, short points)
	{
		if (pointInElipse(x, y, a, b) || pointInCircle(x, y, a, b)) {
			removeShape(x, y);
			dead = true;
			score += points;
		}
	}
	friend void drawShape(short x, short y, uint16_t color);
	friend void removeShape(short x, short y);
	friend bool pointInElipse(short x, short y, short a, short b);
	friend bool pointInCircle(short x, short y, short a, short b);
};

class Crosshair
{
	short xH, yH, xV, yV, length;
public:
	Crosshair(short a, short b, short c, short d, short e)
	{
		xH = a;
		yH = b;
		xV = c;
		yV = d;
		length = e;
		show();
	}
	void setX(short x)
	{
		xH = x-length/2;
		xV = x;
	}
	void setY(short y)
	{
		yH = y;
		yV = y - length/2;
	}
	void move(short speed)
	{
		hide();
		if (btnRight && xH + 9 < 240) {
			xH += speed;
			xV += speed;
		}
		if (btnLeft && xH > 0) {
			xH -= speed;
			xV -= speed;
		}
		if (btnUp && yV > 0) {
			yH -= speed;
			yV -= speed;
		}
		if (btnDown && yV + 9 < 210) {
			yH += speed;
			yV += speed;
		}
		show();
	}
	void crossSection(short &x, short &y)
	{
		x = xV;
		y = yH;
	}
	void hide()
	{
		BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
		BSP_LCD_DrawHLine(xH, yH, length);
		BSP_LCD_DrawVLine(xV, yV, length);
	}
	void show()
	{
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_DrawHLine(xH, yH, length);
		BSP_LCD_DrawVLine(xV, yV, length);
	}
};


short vrijeme = 0;


void addSecond()
{
	vrijeme++;
}

void addShape()
{
	add = true;
}


int main()
{
    char name[10] = "";
	BSP_LCD_Init();
	/* Touchscreen initialization */
	if (BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize()) == TS_ERROR) {
		printf("BSP_TS_Init error\n");
	}
	std::vector<std::pair<int, std::string> > rankingList;
	int pocetak = 0;
	int playAgain = 0;
	while(1) {
		if(pocetak != 0) {
			while (1) {
				BSP_TS_GetState(&TS_State);
				if(TS_State.touchDetected) {
					uint16_t x = TS_State.touchX[0];
					uint16_t y = TS_State.touchY[0];

					if(x > 65 && x < 175 && y > 180 && y < 205) {
						score = 0;
						break;
					} else if(x > 65 && x < 175 && y > 147 && y < 172) {
						drawBackground();
						drawClouds();
						rankingView();
						char opa[10];
						for (int i = 0; i < 5; i++) {
							std::string s = "";
							if (i >= rankingList.size()) {
								sprintf(opa, "%d", 0);
							} else {
								sprintf(opa, "%d", rankingList[i].first);
								s = rankingList[i].second;
							}
							BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
							BSP_LCD_SetBackColor (LCD_COLOR_CYAN);
							BSP_LCD_DisplayStringAt(0, 70 + i*20, (uint8_t *)opa, CENTER_MODE);
							BSP_LCD_DisplayStringAt(160, 70 + i*20, (uint8_t *)s.c_str(), LEFT_MODE);
						}
					} else if(x > 65 && x < 175 && y > 213 && y < 238) {
					    playAgain = 1;
					    score = 0;
					    break;
					}
					wait_ms(10);
				}
			}
		}
		int mode = 1;
		int speed = 1;
		int level = 1;
        
	

		BSP_LCD_SetFont(&Font12);
		drawBackground();
		drawClouds();
		char ispis[10];
		
		if(playAgain == 0) {
		while(1) {
			mainMenuView();
			BSP_TS_GetState(&TS_State);
			if(TS_State.touchDetected) {
				uint16_t x = TS_State.touchX[0];
				uint16_t y = TS_State.touchY[0];
				if(x > 55 && x < 185 && y > 100 && y < 130) {

					break;
				}

			}
		}
		drawBackground();
		drawClouds();
		drawKeyboard();
		int redni_broj = 0;
		char selectedLetter[1];
	    
        for(int i = 0; i < 10; i++){
            name[i] = ' ';
        }
		wait(1);
		while(1) {
			BSP_TS_GetState(&TS_State);
			if(TS_State.touchDetected) {
				uint16_t xx = TS_State.touchX[0];
				uint16_t yy = TS_State.touchY[0];
				if(xx>10 && yy>25 && xx<25 && yy<40) {
					selectedLetter[0]='A';
					name[redni_broj]='A';
					redni_broj++;
				} else if(xx>30 && yy>25 && xx<45 && yy<40) {
					selectedLetter[0]='B';
					name[redni_broj]='B';
					redni_broj++;
				} else if(xx>50 && yy>25 && xx<65 && yy<40) {
					selectedLetter[0]='C';
					name[redni_broj]='C';
					redni_broj++;
				} else if(xx>70 && yy>25 && xx<85 && yy<40) {
					selectedLetter[0]='D';
					name[redni_broj]='D';
					redni_broj++;
				} else if(xx>90 && yy>25 && xx<105 && yy<40) {
					selectedLetter[0]='E';
					name[redni_broj]='E';
					redni_broj++;
				} else if(xx>110 && yy>25 && xx<125 && yy<40) {
					selectedLetter[0]='F';
					name[redni_broj]='F';
					redni_broj++;
				} else if(xx>130 && yy>25 && xx<145 && yy<40) {
					selectedLetter[0]='G';
					name[redni_broj]='G';
					redni_broj++;
				} else if(xx>150 && yy>25 && xx<165 && yy<40) {
					selectedLetter[0]='H';
					name[redni_broj]='H';
					redni_broj++;
				} else if(xx>170 && yy>25 && xx<185 && yy<40) {
					selectedLetter[0]='I';
					name[redni_broj]='I';
					redni_broj++;
				} else if(xx>190 && yy>25 && xx<205 && yy<40) {
					selectedLetter[0]='J';
					name[redni_broj]='J';
					redni_broj++;
				} else if(xx>210 && yy>25 && xx<225 && yy<40) {
					selectedLetter[0]='K';
					name[redni_broj]='K';
					redni_broj++;
				} else if(xx>10 && yy>45 && xx<25 && yy<60) {
					selectedLetter[0]='L';
					name[redni_broj]='L';
					redni_broj++;
				} else if(xx>30 && yy>45 && xx<45 && yy<60) {
					selectedLetter[0]='M';
					name[redni_broj]='M';
					redni_broj++;
				} else if(xx>50 && yy>45 && xx<65 && yy<60) {
					selectedLetter[0]='N';
					name[redni_broj]='N';
					redni_broj++;
				} else if(xx>70 && yy>45 && xx<85 && yy<60) {
					selectedLetter[0]='O';
					name[redni_broj]='O';
					redni_broj++;
				} else if(xx>90 && yy>45 && xx<105 && yy<60) {
					selectedLetter[0]='P';
					name[redni_broj]='P';
					redni_broj++;
				} else if(xx>110 && yy>45 && xx<125 && yy<60) {
					selectedLetter[0]='Q';
					name[redni_broj]='Q';
					redni_broj++;
				} else if(xx>130 && yy>45 && xx<145 && yy<60) {
					selectedLetter[0]='R';
					name[redni_broj]='R';
					redni_broj++;
				} else if(xx>150 && yy>45 && xx<165 && yy<60) {
					selectedLetter[0]='S';
					name[redni_broj]='S';
					redni_broj++;
				} else if(xx>170 && yy>45 && xx<185 && yy<60) {
					selectedLetter[0]='T';
					name[redni_broj]='T';
					redni_broj++;
				} else if(xx>190 && yy>45 && xx<205 && yy<60) {
					selectedLetter[0]='U';
					name[redni_broj]='U';
					redni_broj++;
				} else if(xx>210 && yy>45 && xx<225 && yy<60) {
					selectedLetter[0]='V';
					name[redni_broj]='V';
					redni_broj++;
				} else if(xx>80 && yy>65 && xx<95 && yy<80) {
					selectedLetter[0]='W';
					name[redni_broj]='W';
					redni_broj++;
				} else if(xx>100 && yy>65 && xx<115 && yy<80) {
					selectedLetter[0]='X';
					name[redni_broj]='X';
					redni_broj++;
				} else if(xx>120 && yy>65 && xx<135 && yy<80) {
					selectedLetter[0]='Y';
					name[redni_broj]='Y';
					redni_broj++;
				} else if(xx>140 && yy>65 && xx<155 && yy<80) {
					selectedLetter[0]='Z';
					name[redni_broj]='Z';
					redni_broj++;
				}

				BSP_LCD_DisplayStringAt(60 + redni_broj*10, 130, (uint8_t *)selectedLetter, LEFT_MODE);

				if(xx>65 && xx<175 && yy>180 && yy<205) {
					score = 0;
					break;
				}
				wait(1);
			}
		}
}
        playAgain = 0;
		drawBackground();
		drawClouds();
		char message[]="CHOOSE MODE, ";
		BSP_LCD_DisplayStringAt(145, 45, (uint8_t *)name, LEFT_MODE);
		BSP_LCD_DisplayStringAt(55, 45, (uint8_t *)message, LEFT_MODE);
		modeSelectionView();
		while(1) {

			BSP_TS_GetState(&TS_State);
			if(TS_State.touchDetected) {
				uint16_t x = TS_State.touchX[0];
				uint16_t y = TS_State.touchY[0];

				if(x > 90 && x < 145 && y > 152 && y < 177) {
					mode = 3;
					vrijeme = 0;
					break;
				} else if(x > 90 && x < 145 && y > 112 && y < 137) {
					mode = 2;
					vrijeme = 0;
					break;
				} else if(x >90 && x < 145 && y > 72 && y < 97) {
					vrijeme = 0;
					break;
				}
				wait_ms(1);
			}
		}

		std::vector<Shape*> shapes;
		shapes.push_back(new RedUFO(100, false));

		if(mode == 2) speed = 2;
		if(mode == 3) speed = 3;
		short x1 = -1, y1 = -1;

		Crosshair ch(120, 124, 124, 120, 9);
		Ticker t, t1;
		t.attach(&addSecond, 1);
		t1.attach(&addShape, 2);
		drawBackground();
		while (1) {
			for (int i = 0; i < shapes.size(); i++) {
				shapes[i] -> killIfHit(x1, y1, mode+level-1);
				if (shapes[i] -> isDead()) {
					delete shapes[i];
					shapes.erase(shapes.begin() + i--);
				}
			}
			x1 = -1;
			y1 = -1;
			if (add && shapes.size() <= 2) {
				short type = rand() % 3;
				short yAxis = 25 + rand() % 150;
				short direction = rand() % 2;
				if (type == 0) {
					shapes.push_back(new RedUFO(yAxis, direction));
				} else if(type == 1) {
					shapes.push_back(new BlueUFO(yAxis, direction));
				} else {
					shapes.push_back(new GreenUFO(yAxis, direction));
				}
				add = false;
			}


			showCurrentScore(score);

			int vrijeme2 = showRemainingTime(vrijeme, mode);

			ch.move(speed + 2);

			showCurrentLevel(level);

			if (btnOK) {
				ch.crossSection(x1, y1);
			}

			BSP_TS_GetState(&TS_State);
			if(mode != 3 && TS_State.touchDetected) {
				x1 = TS_State.touchX[0];
				y1 = TS_State.touchY[0];
				ch.hide();
				ch.setX(x1);
				ch.setY(y1);
				ch.show();
			}
			drawClouds();
			for (int i = 0; i < shapes.size(); i++)
				shapes[i] -> move(speed);

			if (vrijeme2 == 0) {
				if(score < 5*level*mode) {
					for (int i = 0; i < 5; i++) {
						if (i == rankingList.size()) {
							rankingList.push_back(std::make_pair(score, name));
							break;
						} else if (score > rankingList[i].first) {
							rankingList.insert(rankingList.begin() + i, std::make_pair(score, name));
							break;
						}
					}
					if (rankingList.size() > 5)
						rankingList.pop_back();
					drawBackground();
					drawClouds();
					gameOverView(score, level);
					pocetak++;
					break;
				} else {
					vrijeme = 0;
					level++;
					speed++;
				}
				if(level == 6) {
					for (int i = 0; i < 5; i++) {
						if (i == rankingList.size()) {
							rankingList.push_back(std::make_pair(score, name));
							break;
						} else if (score > rankingList[i].first) {
							rankingList.insert(rankingList.begin() + i, std::make_pair(score, name));
							break;
						}
					}
					if (rankingList.size() > 5)
						rankingList.pop_back();
					drawBackground();
					drawClouds();
					winView(score);
					pocetak++;
					break;
				}

			}

		}

		wait(0.1);
	}
}
