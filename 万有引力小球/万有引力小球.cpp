#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<graphics.h>
#include<math.h>

#define G 50
#define T 1

struct particle {
	int x;
	int y;
	int m;
}; 
particle p1, p2;  //两个质点

double p1_v_xy[4] = {0,10,80,0};   //分别保存y正半轴、x正半轴、y负半轴、x负半轴的分解速度
double p2_v_xy[4] = {0,0,30,40};   //分别保存y正半轴、x正半轴、y负半轴、x负半轴的分解速度
double p1_a_xy[4];	 //分别保存y正半轴、x正半轴、y负半轴、x负半轴的分解引力
double p2_a_xy[4];	 //分别保存y正半轴、x正半轴、y负半轴、x负半轴的分解引力

double tx1;
double tx2;
double ty1;
double ty2;



void drawParticle();  //绘制质点
void changeV();     //改变方向
void changeXY(); //通过更新完的速度  改变质点的坐标


//绘制质点
void drawParticle() {
	cleardevice();
	setfillcolor(RED);
	fillcircle(p1.x, p1.y, 5);
	setfillcolor(GREEN);
	fillcircle(p2.x, p2.y, 5);
}

void changeV() {  //改变方向
	for (int i = 0; i < 4; i++) {   //两个加速度清零
		p1_a_xy[i] = 0;
		p2_a_xy[i] = 0;
	}

	double tx = abs(p2.x - p1.x);  //p1和p2的左右距离
	double ty = abs(p2.y - p1.y);  //p1和p2的上下距离
	double r = pow(tx * tx + ty * ty, 0.5);  //p1和p2的直线距离
	double a1 = p2.m * G / r * r;
	double a2 = p1.m * G / r * r;

	//x、y轴的分加速度
	double ax = a1 * tx / r;
	double ay = a1 * ty / r;

	//p1在p2的左侧时   p1加速度分解的一部分肯定在x正半轴上 p2在x的负半轴上
	if (p1.x < p2.x) {   
		p1_a_xy[1] = ax;
		p2_a_xy[3] = ax;
	}
	else {
		p1_a_xy[3] = ax;
		p2_a_xy[1] = ax;
	}

	//p1在p2上侧时   p1加速度分解的一部分肯定在y负半轴上  p2在y正半轴上
	if (p1.y < p2.y) {  
		p1_a_xy[2] = ay;
		p2_a_xy[0] = ay;
	}
	else {
		p1_a_xy[0] = ay;
		p2_a_xy[2] = ay;
	}

	//循环加速度  从而改变速度
	for (int i = 0; i < 4; i++) {
		if (p1_a_xy[i] != 0) {
			if (p1_v_xy[i] != 0) {
				p1_v_xy[i] = p1_v_xy[i] * T + p1_a_xy[i] * T * T;
			}
			else if (p1_v_xy[(i+2)%4] != 0) {
				int tv = -p1_v_xy[(i + 2) % 4] * T + p1_a_xy[i] * T * T;
				if (tv > 0) {
					p1_v_xy[i] = tv;
					p1_v_xy[(i + 2) % 4] = 0;
				}
				else {
					p1_v_xy[(i + 2) % 4] = tv;
				}
			}
			else {
				p1_v_xy[i] = p1_a_xy[i] * T * T;
			}
		}
	}

	for (int i = 0; i < 4; i++) {
		if (p2_a_xy[i] != 0) {
			if (p2_v_xy[i] != 0) {
				p2_v_xy[i] = p2_v_xy[i] * T + p2_a_xy[i] * T * T;
			}
			else if (p2_v_xy[(i + 2) % 4] != 0) {
				int tv = -p2_v_xy[(i + 2) % 4] * T + p2_a_xy[i] * T * T;
				if (tv > 0) {
					p2_v_xy[i] = tv;
					p2_v_xy[(i + 2) % 4] = 0;
				}
				else {
					p2_v_xy[(i + 2) % 4] = tv;
				}
			}
			else {
				p2_v_xy[i] = p2_a_xy[i] * T * T;
			}
		}
	}
}

void changeXY() {  //通过更新完的速度  改变质点的坐标
	tx1 = (p1_v_xy[1] - p1_v_xy[3]) * T;
	tx2 = (p2_v_xy[1] - p2_v_xy[3]) * T;

	ty1 = (p1_v_xy[2] - p1_v_xy[0]) * T;
	ty2 = (p2_v_xy[2] - p2_v_xy[0]) * T;
}




int main() {
	//初始化两个质点
	p1.x = 200;
	p1.y = 200;
	p2.x = 300;
	p2.y = 200;
	p1.m = 4;
	p2.m = 3;
	initgraph(600, 450);  //初始化画布
	while (1) {
		drawParticle();  //绘制质点
		changeV();
		changeXY();
		for (int i = 1; i <= 10; i++) {
			p1.x += tx1/10;
			p2.x += tx2/10;
			p1.y += ty1/10;
			p2.y += ty2/10;
			drawParticle();  //绘制质点
			Sleep(100);
		}
	}

}