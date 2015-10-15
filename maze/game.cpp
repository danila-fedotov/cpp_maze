#include "game.h"	// ����������� ������ ������
#include "glut.h"	// ����������� glut

cGames::cGames()
{
	load();			// ��������� ��� �����
}
cGames::~cGames()
{
}
void cGames::draw(int *action)
{
	// �������� ������� 
	// 0 - ��� ��������
	// 1 - � ���� �������, ���� �����, ���� ������
	// -1 - �������������� � ������, ���� ����, ���� �����
	int k[2] = {0, 0};
	// ��� ��������� ����������
	GLUquadricObj* obj = gluNewQuadric();
	// ��������� �����
	glNormalPointer(GL_INT, 0, &normals[0]);	// ������ ������ ��������
	glColorPointer(3, GL_FLOAT, 0, &colors[0]);	// ������ �����
	glVertexPointer(3, GL_INT, 0, &coords[0]);	// ������ ��������� ������
	glDrawArrays(GL_QUADS, 0, colSpace * 4);	// ��������� ��������
	// ���� ����� �����������, �� ����������� ��� (span)
	if (*action != 0) span++;
	// �������� ����� �� ������ ���������
	switch (*action)
	{
		// �����
		case 1:
			// ��������, ���� �� ������ � ������ �����������
			for (int i = 0 ; i < 2 ; i++)
				if ((ball[i].y != height - 1) && (ver[ball[i].x][height - 2 - ball[i].y] != 1))
					k[i] = 1;
			// ����� �� ����� � ���� �����, ���� ��, �� �� ������ �� ���������
			for (int i = 0 ; i < 2 ; i++)
				if ((ball[i].x == pocket[i].x) && (ball[i].y == pocket[i].y))
					k[i] = 0;
			// ��������, ��� �� ��������� ������, ������� �� ����� ���������, �� ����������� ��������
			for (int i = 0 ; i < 2 ; i++)
				if ((k[i] == 1) && (ball[i].y + 1 == ball[1 - i].y) && (ball[i].x == ball[1 - i].x) && (k[1 - i] == 0))
					k[i] = 0;
			// ���� ����� "������ �� �����", �� ������ ���������� � ������
			if (span > 17)
				for (int i = 0 ; i < 2 ; i++)
					if (k[i] == 1) ball[i].y++;
			break;
		// ����
		case 2:
			for (int i = 0 ; i < 2 ; i++)
				if ((ball[i].y != 0) && (ver[ball[i].x][height - 1 - ball[i].y] != 1))
					k[i] = -1;
			for (int i = 0 ; i < 2 ; i++)
				if ((ball[i].x == pocket[i].x) && (ball[i].y == pocket[i].y))
					k[i] = 0;
			for (int i = 0 ; i < 2 ; i++)
				if ((k[i] == -1) && (ball[i].y - 1 == ball[1 - i].y) && (ball[i].x == ball[1 - i].x) && (k[1 - i] == 0))
					k[i] = 0;
			if (span > 17)
				for (int i = 0 ; i < 2 ; i++)
					if (k[i] == -1) ball[i].y--;
			break;
		// �����
		case 3:
			for (int i = 0 ; i < 2 ; i++)
				if ((ball[i].x != 0) && (hor[height - 1 - ball[i].y][ball[i].x - 1] != 1))
					k[i] = -1;
			for (int i = 0 ; i < 2 ; i++)
				if ((ball[i].x == pocket[i].x) && (ball[i].y == pocket[i].y))
					k[i] = 0;
			for (int i = 0 ; i < 2 ; i++)
				if ((k[i] == -1) && (ball[i].y == ball[1 - i].y) && (ball[i].x - 1 == ball[1 - i].x) && (k[1 - i] == 0))
					k[i] = 0;
			if (span > 17)
				for (int i = 0 ; i < 2 ; i++)
					if (k[i] == -1) ball[i].x--;
			break;
		// ������
		case 4:
			for (int i = 0 ; i < 2 ; i++)
				if ((ball[i].x != width - 1) && (hor[height - 1 - ball[i].y][ball[i].x] != 1))
					k[i] = 1;
			for (int i = 0 ; i < 2 ; i++)
				if ((ball[i].x == pocket[i].x) && (ball[i].y == pocket[i].y))
					k[i] = 0;
			for (int i = 0 ; i < 2 ; i++)
				if ((k[i] == 1) && (ball[i].x + 1 == ball[1 - i].x) && (ball[i].y == ball[1 - i].y) && (k[1 - i] == 0))
					k[i] = 0;
			if (span > 17)
				for (int i = 0 ; i < 2 ; i++)
					if (k[i] == 1) ball[i].x++;
			break;
	}
	// ��������� ������� ������
	glPushMatrix();				// ���������� ������� ������� ��������������
	// ���������� ������� �����
	if (span > 17) span = 0;
	// ����������� �������� �����
	if (span == 0) *action = 0;
	// ���������� �����, � ����������� �� �����������
	if (*action < 3)
		glTranslatef(ball[0].x * 10 + 6, ball[0].y * 10 + 6 + span * ((float)10 / 17 * k[0]), 3);
	else
		glTranslatef(ball[0].x * 10 + 6 + span * ((float)10 / 17 * k[0]), ball[0].y * 10 + 6, 3);
	// ������ ���� � ������:
	// �������� ��������, �� �������
	// ����� � ���� �����, �� �������
	if ((ball[0].x == pocket[0].x) && (ball[0].y == pocket[0].y))
		glColor3f(0, 1, 0);
	else
		glColor3f(0, 0, 1);
	gluSphere(obj, 4, 20, 20);	// ��������� �����
	glPopMatrix();				// ��������������� ������� ��������������
	// ��������� ������� ������
	glPushMatrix();				// ���������� ������� ������� ��������������
	if (*action < 3)
		glTranslatef(ball[1].x * 10 + 6, ball[1].y * 10 + 6 + span * ((float)10 / 17 * k[1]), 3);
	else
		glTranslatef(ball[1].x * 10 + 6 + span * ((float)10 / 17 * k[1]), ball[1].y * 10 + 6, 3);
	// ������ ���� � ������:
	// �������� ��������, �� �����
	// ����� � ���� �����, �� �������
	if ((ball[1].x == pocket[1].x) && (ball[1].y == pocket[1].y))
		glColor3f(0, 1, 0);
	else
		glColor3f(1, 0, 0);
	gluSphere(obj, 4, 20, 20);
	glPopMatrix();				// ��������������� ������� ��������������
	// ��������� ������ �����
	glPushMatrix();				// ���������� ������� ������� ��������������
	glTranslatef(pocket[0].x * 10 + 6, pocket[0].y * 10 + 6, 0.5);
	// ������ ���� � �����:
	// ��� ������, �� �����
	// ����� "����������" �����, �� �������
	if ((ball[0].x == pocket[0].x) && (ball[0].y == pocket[0].y))
		glColor3f(0, 1, 0);
	else
		glColor3f(0, 0, 1);
	gluDisk(obj, 3, 4, 30, 30);
	glColor3f(0, 0, 0);
	gluDisk(obj, 0, 3, 30, 30);
	glPopMatrix();				// ��������������� ������� ��������������
	// ��������� ������ �����
	glPushMatrix();				// ���������� ������� ������� ��������������
	glTranslatef(pocket[1].x * 10 + 6, pocket[1].y * 10 + 6, 0.5);
	// ������ ���� � �����:
	// ��� ������, �� �������
	// ����� "����������" �����, �� �������
	if ((ball[1].x == pocket[1].x) && (ball[1].y == pocket[1].y))
		glColor3f(0, 1, 0);
	else
		glColor3f(1, 0, 0);
	gluDisk(obj, 3, 4, 30, 30);
	glColor3f(0, 0, 0);
	gluDisk(obj, 0, 3, 30, 30);
	glPopMatrix();				// ��������������� ������� ��������������
}
void cGames::load()
{
	// ������������� ����������
	width = 8;			// ������ ����� � �������
	height = 8;			// ������ ����� � �������
	colSpace = 0;		// �������� ���������� ����������
	// ���������� ������� ������
	ball[0].x = 0;
	ball[0].y = 7;
	// ���������� ������� ������
	ball[1].x = 7;
	ball[1].y = 7;
	// ���������� ������ �����
	pocket[0].x = 7;
	pocket[0].y = 0;
	// ���������� ������ �����
	pocket[1].x = 0;
	pocket[1].y = 0;
	// ������������� �������������� ������
	hor.resize(height);
	for (int i = 0 ; i < height ; i++)
		hor[i].resize(width - 1);
	hor[0][0] = 0; hor[0][1] = 0; hor[0][2] = 1; hor[0][3] = 0; hor[0][4] = 0; hor[0][5] = 0; hor[0][6] = 1;
	hor[1][0] = 0; hor[1][1] = 0; hor[1][2] = 1; hor[1][3] = 0; hor[1][4] = 1; hor[1][5] = 1; hor[1][6] = 1;
	hor[2][0] = 0; hor[2][1] = 0; hor[2][2] = 0; hor[2][3] = 0; hor[2][4] = 0; hor[2][5] = 1; hor[2][6] = 0;
	hor[3][0] = 0; hor[3][1] = 0; hor[3][2] = 1; hor[3][3] = 1; hor[3][4] = 0; hor[3][5] = 0; hor[3][6] = 1;
	hor[4][0] = 0; hor[4][1] = 1; hor[4][2] = 1; hor[4][3] = 1; hor[4][4] = 1; hor[4][5] = 0; hor[4][6] = 0;
	hor[5][0] = 1; hor[5][1] = 0; hor[5][2] = 1; hor[5][3] = 1; hor[5][4] = 0; hor[5][5] = 0; hor[5][6] = 0;
	hor[6][0] = 1; hor[6][1] = 1; hor[6][2] = 0; hor[6][3] = 0; hor[6][4] = 0; hor[6][5] = 1; hor[6][6] = 0;
	hor[7][0] = 0; hor[7][1] = 1; hor[7][2] = 0; hor[7][3] = 0; hor[7][4] = 1; hor[7][5] = 0; hor[7][6] = 0;
	// ������������� ������������ ������
	ver.resize(width);
	for (int i = 0 ; i < width ; i++)
		ver[i].resize(height - 1);
	ver[0][0] = 1; ver[1][0] = 1; ver[2][0] = 0; ver[3][0] = 0; ver[4][0] = 1; ver[5][0] = 0; ver[6][0] = 0; ver[7][0] = 0;
	ver[0][1] = 0; ver[1][1] = 1; ver[2][1] = 1; ver[3][1] = 1; ver[4][1] = 1; ver[5][1] = 0; ver[6][1] = 0; ver[7][1] = 0;
	ver[0][2] = 0; ver[1][2] = 1; ver[2][2] = 1; ver[3][2] = 0; ver[4][2] = 1; ver[5][2] = 1; ver[6][2] = 1; ver[7][2] = 0;
	ver[0][3] = 1; ver[1][3] = 0; ver[2][3] = 0; ver[3][3] = 0; ver[4][3] = 0; ver[5][3] = 1; ver[6][3] = 1; ver[7][3] = 0;
	ver[0][4] = 0; ver[1][4] = 1; ver[2][4] = 0; ver[3][4] = 0; ver[4][4] = 0; ver[5][4] = 0; ver[6][4] = 1; ver[7][4] = 1;
	ver[0][5] = 0; ver[1][5] = 0; ver[2][5] = 1; ver[3][5] = 0; ver[4][5] = 1; ver[5][5] = 1; ver[6][5] = 1; ver[7][5] = 0;
	ver[0][6] = 1; ver[1][6] = 0; ver[2][6] = 0; ver[3][6] = 1; ver[4][6] = 1; ver[5][6] = 0; ver[6][6] = 1; ver[7][6] = 1;
	// ���
	colSpace++;
	coords.push_back(0);				coords.push_back(0);				coords.push_back(0);
	coords.push_back(width * 10 + 2);	coords.push_back(0);				coords.push_back(0);
	coords.push_back(width * 10 + 2);	coords.push_back(height * 10 + 2);	coords.push_back(0);
	coords.push_back(0);				coords.push_back(height * 10 + 2);	coords.push_back(0);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	// ������ ������� ������ ��������
	colSpace++;
	coords.push_back(0);				coords.push_back(0);				coords.push_back(5);
	coords.push_back(2);				coords.push_back(0);				coords.push_back(5);
	coords.push_back(2);				coords.push_back(2);				coords.push_back(5);
	coords.push_back(0);				coords.push_back(2);				coords.push_back(5);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	// ������ ������� ������� ��������
	colSpace++;
	coords.push_back(width * 10);		coords.push_back(0);				coords.push_back(5);
	coords.push_back(width * 10 + 2);	coords.push_back(0);				coords.push_back(5);
	coords.push_back(width * 10 + 2);	coords.push_back(2);				coords.push_back(5);
	coords.push_back(width * 10);		coords.push_back(2);				coords.push_back(5);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	// ������ �������� ������� ��������
	colSpace++;
	coords.push_back(width * 10);		coords.push_back(height * 10);		coords.push_back(5);
	coords.push_back(width * 10 + 2);	coords.push_back(height * 10);		coords.push_back(5);
	coords.push_back(width * 10 + 2);	coords.push_back(height * 10 + 2);	coords.push_back(5);
	coords.push_back(width * 10);		coords.push_back(height * 10 + 2);	coords.push_back(5);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	// ������ �������� ������ ��������
	colSpace++;
	coords.push_back(0);				coords.push_back(height * 10);		coords.push_back(5);
	coords.push_back(2);				coords.push_back(height * 10);		coords.push_back(5);
	coords.push_back(2);				coords.push_back(height * 10 + 2);	coords.push_back(5);
	coords.push_back(0);				coords.push_back(height * 10 + 2);	coords.push_back(5);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	// ������ ������
	colSpace++;
	coords.push_back(2);				coords.push_back(0);				coords.push_back(5);
	coords.push_back(width * 10);		coords.push_back(0);				coords.push_back(5);
	coords.push_back(width * 10);		coords.push_back(2);				coords.push_back(5);
	coords.push_back(2);				coords.push_back(2);				coords.push_back(5);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	// ������� ������
	colSpace++;
	coords.push_back(2);				coords.push_back(height * 10);		coords.push_back(5);
	coords.push_back(width * 10);		coords.push_back(height * 10);		coords.push_back(5);
	coords.push_back(width * 10);		coords.push_back(height * 10 + 2);	coords.push_back(5);
	coords.push_back(2);				coords.push_back(height * 10 + 2);	coords.push_back(5);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	// ����� ������
	colSpace++;
	coords.push_back(0);				coords.push_back(2);				coords.push_back(5);
	coords.push_back(2);				coords.push_back(2);				coords.push_back(5);
	coords.push_back(2);				coords.push_back(height * 10);		coords.push_back(5);
	coords.push_back(0);				coords.push_back(height * 10);		coords.push_back(5);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	// ������ ������
	colSpace++;
	coords.push_back(width * 10);		coords.push_back(2);				coords.push_back(5);
	coords.push_back(width * 10 + 2);	coords.push_back(2);				coords.push_back(5);
	coords.push_back(width * 10 + 2);	coords.push_back(height * 10);		coords.push_back(5);
	coords.push_back(width * 10);		coords.push_back(height * 10);		coords.push_back(5);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	// ������� ������ ������
	colSpace++;
	coords.push_back(0);				coords.push_back(0);				coords.push_back(0);
	coords.push_back(width * 10 + 2);	coords.push_back(0);				coords.push_back(0);
	coords.push_back(width * 10 + 2);	coords.push_back(0);				coords.push_back(5);
	coords.push_back(0);				coords.push_back(0);				coords.push_back(5);
	normals.push_back(0);				normals.push_back(-1);				normals.push_back(0);
	normals.push_back(0);				normals.push_back(-1);				normals.push_back(0);
	normals.push_back(0);				normals.push_back(-1);				normals.push_back(0);
	normals.push_back(0);				normals.push_back(-1);				normals.push_back(0);
	// ���������� ������ ������
	colSpace++;
	coords.push_back(2);				coords.push_back(2);				coords.push_back(5);
	coords.push_back(width * 10);		coords.push_back(2);				coords.push_back(5);
	coords.push_back(width * 10);		coords.push_back(2);				coords.push_back(0);
	coords.push_back(2);				coords.push_back(2);				coords.push_back(0);
	normals.push_back(0);				normals.push_back(1);				normals.push_back(0);
	normals.push_back(0);				normals.push_back(1);				normals.push_back(0);
	normals.push_back(0);				normals.push_back(1);				normals.push_back(0);
	normals.push_back(0);				normals.push_back(1);				normals.push_back(0);
	// ������� ������� ������
	colSpace++;
	coords.push_back(0);				coords.push_back(height * 10 + 2);	coords.push_back(5);
	coords.push_back(width * 10 + 2);	coords.push_back(height * 10 + 2);	coords.push_back(5);
	coords.push_back(width * 10 + 2);	coords.push_back(height * 10 + 2);	coords.push_back(0);
	coords.push_back(0);				coords.push_back(height * 10 + 2);	coords.push_back(0);
	normals.push_back(0);				normals.push_back(1);				normals.push_back(0);
	normals.push_back(0);				normals.push_back(1);				normals.push_back(0);
	normals.push_back(0);				normals.push_back(1);				normals.push_back(0);
	normals.push_back(0);				normals.push_back(1);				normals.push_back(0);
	// ���������� ������� ������
	colSpace++;
	coords.push_back(2);				coords.push_back(height * 10);		coords.push_back(0);
	coords.push_back(width * 10);		coords.push_back(height * 10);		coords.push_back(0);
	coords.push_back(width * 10);		coords.push_back(height * 10);		coords.push_back(5);
	coords.push_back(2);				coords.push_back(height * 10);		coords.push_back(5);
	normals.push_back(0);				normals.push_back(-1);				normals.push_back(0);
	normals.push_back(0);				normals.push_back(-1);				normals.push_back(0);
	normals.push_back(0);				normals.push_back(-1);				normals.push_back(0);
	normals.push_back(0);				normals.push_back(-1);				normals.push_back(0);
	// ������� ����� ������
	colSpace++;
	coords.push_back(0);				coords.push_back(height * 10 + 2);	coords.push_back(0);
	coords.push_back(0);				coords.push_back(0);				coords.push_back(0);
	coords.push_back(0);				coords.push_back(0);				coords.push_back(5);
	coords.push_back(0);				coords.push_back(height * 10 + 2);	coords.push_back(5);
	normals.push_back(-1);				normals.push_back(0);				normals.push_back(0);
	normals.push_back(-1);				normals.push_back(0);				normals.push_back(0);
	normals.push_back(-1);				normals.push_back(0);				normals.push_back(0);
	normals.push_back(-1);				normals.push_back(0);				normals.push_back(0);
	// ���������� ����� ������
	colSpace++;
	coords.push_back(2);				coords.push_back(2);				coords.push_back(0);
	coords.push_back(2);				coords.push_back(height * 10);		coords.push_back(0);
	coords.push_back(2);				coords.push_back(height * 10);		coords.push_back(5);
	coords.push_back(2);				coords.push_back(2);				coords.push_back(5);
	normals.push_back(1);				normals.push_back(0);				normals.push_back(0);
	normals.push_back(1);				normals.push_back(0);				normals.push_back(0);
	normals.push_back(1);				normals.push_back(0);				normals.push_back(0);
	normals.push_back(1);				normals.push_back(0);				normals.push_back(0);
	// ������� ������ ������
	colSpace++;
	coords.push_back(width * 10 + 2);	coords.push_back(0);				coords.push_back(0);
	coords.push_back(width * 10 + 2);	coords.push_back(height * 10 + 2);	coords.push_back(0);
	coords.push_back(width * 10 + 2);	coords.push_back(height * 10 + 2);	coords.push_back(5);
	coords.push_back(width * 10 + 2);	coords.push_back(0);				coords.push_back(5);
	normals.push_back(1);				normals.push_back(0);				normals.push_back(0);
	normals.push_back(1);				normals.push_back(0);				normals.push_back(0);
	normals.push_back(1);				normals.push_back(0);				normals.push_back(0);
	normals.push_back(1);				normals.push_back(0);				normals.push_back(0);
	// ���������� ������ ������
	colSpace++;
	coords.push_back(width * 10);		coords.push_back(height * 10);		coords.push_back(0);
	coords.push_back(width * 10);		coords.push_back(2);				coords.push_back(0);
	coords.push_back(width * 10);		coords.push_back(2);				coords.push_back(5);
	coords.push_back(width * 10);		coords.push_back(height * 10);		coords.push_back(5);
	normals.push_back(-1);				normals.push_back(0);				normals.push_back(0);
	normals.push_back(-1);				normals.push_back(0);				normals.push_back(0);
	normals.push_back(-1);				normals.push_back(0);				normals.push_back(0);
	normals.push_back(-1);				normals.push_back(0);				normals.push_back(0);
	// ��������
	for (int i = 0 ; i < height - 1 ; i++)
		for (int j = 0 ; j < width - 1 ; j++)
		{
			// ������ ��������
			colSpace++;
			coords.push_back(j * 10 + 10);	coords.push_back((height - i - 1) * 10);	coords.push_back(5);
			coords.push_back(j * 10 + 12);	coords.push_back((height - i - 1) * 10);	coords.push_back(5);
			coords.push_back(j * 10 + 12);	coords.push_back((height - i - 1) * 10 + 2);coords.push_back(5);
			coords.push_back(j * 10 + 10);	coords.push_back((height - i - 1) * 10 + 2);coords.push_back(5);
			normals.push_back(0);			normals.push_back(0);						normals.push_back(1);
			normals.push_back(0);			normals.push_back(0);						normals.push_back(1);
			normals.push_back(0);			normals.push_back(0);						normals.push_back(1);
			normals.push_back(0);			normals.push_back(0);						normals.push_back(1);
			// ������� ������ ��������
			colSpace++;
			coords.push_back(j * 10 + 10);	coords.push_back((height - i - 1) * 10 + 2);coords.push_back(5);
			coords.push_back(j * 10 + 12);	coords.push_back((height - i - 1) * 10 + 2);coords.push_back(5);
			coords.push_back(j * 10 + 12);	coords.push_back((height - i - 1) * 10 + 2);coords.push_back(0);
			coords.push_back(j * 10 + 10);	coords.push_back((height - i - 1) * 10 + 2);coords.push_back(0);
			normals.push_back(0);			normals.push_back(1);						normals.push_back(0);
			normals.push_back(0);			normals.push_back(1);						normals.push_back(0);
			normals.push_back(0);			normals.push_back(1);						normals.push_back(0);
			normals.push_back(0);			normals.push_back(1);						normals.push_back(0);
			// ������ ������ ��������
			colSpace++;
			coords.push_back(j * 10 + 10);	coords.push_back((height - i - 1) * 10);	coords.push_back(0);
			coords.push_back(j * 10 + 12);	coords.push_back((height - i - 1) * 10);	coords.push_back(0);
			coords.push_back(j * 10 + 12);	coords.push_back((height - i - 1) * 10);	coords.push_back(5);
			coords.push_back(j * 10 + 10);	coords.push_back((height - i - 1) * 10);	coords.push_back(5);
			normals.push_back(0);			normals.push_back(-1);						normals.push_back(0);
			normals.push_back(0);			normals.push_back(-1);						normals.push_back(0);
			normals.push_back(0);			normals.push_back(-1);						normals.push_back(0);
			normals.push_back(0);			normals.push_back(-1);						normals.push_back(0);
			// ����� ������ ��������
			colSpace++;
			coords.push_back(j * 10 + 10);	coords.push_back((height - i - 1) * 10 + 2);coords.push_back(0);
			coords.push_back(j * 10 + 10);	coords.push_back((height - i - 1) * 10);	coords.push_back(0);
			coords.push_back(j * 10 + 10);	coords.push_back((height - i - 1) * 10);	coords.push_back(5);
			coords.push_back(j * 10 + 10);	coords.push_back((height - i - 1) * 10 + 2);coords.push_back(5);
			normals.push_back(-1);			normals.push_back(0);						normals.push_back(0);
			normals.push_back(-1);			normals.push_back(0);						normals.push_back(0);
			normals.push_back(-1);			normals.push_back(0);						normals.push_back(0);
			normals.push_back(-1);			normals.push_back(0);						normals.push_back(0);
			// ������ ������ ��������
			colSpace++;
			coords.push_back(j * 10 + 12);	coords.push_back((height - i - 1) * 10);	coords.push_back(0);
			coords.push_back(j * 10 + 12);	coords.push_back((height - i - 1) * 10 + 2);coords.push_back(0);
			coords.push_back(j * 10 + 12);	coords.push_back((height - i - 1) * 10 + 2);coords.push_back(5);
			coords.push_back(j * 10 + 12);	coords.push_back((height - i - 1) * 10);	coords.push_back(5);
			normals.push_back(1);			normals.push_back(0);						normals.push_back(0);
			normals.push_back(1);			normals.push_back(0);						normals.push_back(0);
			normals.push_back(1);			normals.push_back(0);						normals.push_back(0);
			normals.push_back(1);			normals.push_back(0);						normals.push_back(0);
		}
	// �������������� ������
	for(int i = 7 ; i >= 0 ; i--)
		for(int j = 0 ; j < 7 ; j++)
			if (hor[i][j] == 1)
			{
				//������ ������
				colSpace++;
				coords.push_back(10 + j * 10);	coords.push_back(height * 10 - 8 - i * 10);		coords.push_back(5);
				coords.push_back(12 + j * 10);	coords.push_back(height * 10 - 8 - i * 10);		coords.push_back(5);
				coords.push_back(12 + j * 10);	coords.push_back(height * 10 - i * 10);			coords.push_back(5);
				coords.push_back(10 + j * 10);	coords.push_back(height * 10 - i * 10);			coords.push_back(5);
				normals.push_back(0);			normals.push_back(0);							normals.push_back(1);
				normals.push_back(0);			normals.push_back(0);							normals.push_back(1);
				normals.push_back(0);			normals.push_back(0);							normals.push_back(1);
				normals.push_back(0);			normals.push_back(0);							normals.push_back(1);
				//����� ����� ������
				colSpace++;
				coords.push_back(10 + j * 10);	coords.push_back(height * 10 - 8 - i * 10);		coords.push_back(5);
				coords.push_back(10 + j * 10);	coords.push_back(height * 10 - i * 10);			coords.push_back(5);
				coords.push_back(10 + j * 10);	coords.push_back(height * 10 - i * 10);			coords.push_back(0);
				coords.push_back(10 + j * 10);	coords.push_back(height * 10 - 8 - i * 10);		coords.push_back(0);
				normals.push_back(-1);			normals.push_back(0);							normals.push_back(0);
				normals.push_back(-1);			normals.push_back(0);							normals.push_back(0);
				normals.push_back(-1);			normals.push_back(0);							normals.push_back(0);
				normals.push_back(-1);			normals.push_back(0);							normals.push_back(0);
				//������ ����� ������
				colSpace++;
				coords.push_back(12 + j * 10);	coords.push_back(height * 10 - i * 10);			coords.push_back(5);
				coords.push_back(12 + j * 10);	coords.push_back(height * 10 - 8 - i * 10);		coords.push_back(5);
				coords.push_back(12 + j * 10);	coords.push_back(height * 10 - 8 - i * 10);		coords.push_back(0);
				coords.push_back(12 + j * 10);	coords.push_back(height * 10 - i * 10);			coords.push_back(0);
				normals.push_back(1);			normals.push_back(0);							normals.push_back(0);
				normals.push_back(1);			normals.push_back(0);							normals.push_back(0);
				normals.push_back(1);			normals.push_back(0);							normals.push_back(0);
				normals.push_back(1);			normals.push_back(0);							normals.push_back(0);
 			}
	// ������������ ������
	for(int i = 6 ; i >= 0 ; i--)
		for(int j = 0 ; j < 8 ; j++)
			if(ver[j][i] == 1)
			{
				//������ ������
				colSpace++;
				coords.push_back(10 + j * 10);	coords.push_back(height * 10 - 10 - i * 10);	coords.push_back(5);
				coords.push_back(10 + j * 10);	coords.push_back(height * 10 - 8 - i * 10);		coords.push_back(5);
				coords.push_back(2 + j * 10);	coords.push_back(height * 10 - 8 - i * 10);		coords.push_back(5);
				coords.push_back(2 + j * 10);	coords.push_back(height * 10 - 10 - i * 10);	coords.push_back(5);
				normals.push_back(0);			normals.push_back(0);							normals.push_back(1);
				normals.push_back(0);			normals.push_back(0);							normals.push_back(1);
				normals.push_back(0);			normals.push_back(0);							normals.push_back(1);
				normals.push_back(0);			normals.push_back(0);							normals.push_back(1);
				//������� ����� ������
				colSpace++;
				coords.push_back(10 + j * 10);	coords.push_back(height * 10 - 8 - i * 10);		coords.push_back(5);
				coords.push_back(10 + j * 10);	coords.push_back(height * 10 - 8 - i * 10);		coords.push_back(0);
				coords.push_back(2 + j * 10);	coords.push_back(height * 10 - 8 - i * 10);		coords.push_back(0);
				coords.push_back(2 + j * 10);	coords.push_back(height * 10 - 8 - i * 10);		coords.push_back(5);
				normals.push_back(0);			normals.push_back(1);							normals.push_back(0);
				normals.push_back(0);			normals.push_back(1);							normals.push_back(0);
				normals.push_back(0);			normals.push_back(1);							normals.push_back(0);
				normals.push_back(0);			normals.push_back(1);							normals.push_back(0);
				//������ ����� ������
				colSpace++;
				coords.push_back(10 + j * 10);	coords.push_back(height * 10 - 10 - i * 10);	coords.push_back(5);
				coords.push_back(2 + j * 10);	coords.push_back(height * 10 - 10 - i * 10);	coords.push_back(5);
				coords.push_back(2 + j * 10);	coords.push_back(height * 10 - 10 - i * 10);	coords.push_back(0);
				coords.push_back(10 + j * 10);	coords.push_back(height * 10 - 10 - i * 10);	coords.push_back(0);	
				normals.push_back(0);			normals.push_back(-1);							normals.push_back(0);
				normals.push_back(0);			normals.push_back(-1);							normals.push_back(0);
				normals.push_back(0);			normals.push_back(-1);							normals.push_back(0);
				normals.push_back(0);			normals.push_back(-1);							normals.push_back(0);
			}
	// ������������� ������� �����
	colors.resize(colSpace * 4 * 3);
	for (int i = 0 ; i < 12 ; i++)
		colors[i] = 0.9;
	for (int i = 12 ; i < colSpace * 12 ; i++)
		colors[i] = 0.5;
}