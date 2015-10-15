#include <stdlib.h>	// ��� atexit
#include "glut.h"	// ����������� glut
#include "game.h"	// ����������� ������ ������
// ������� ������
int width = 1280;	// ������
int height = 800;	// ������
// ������� ���� �������� ����� ������������ ���
int angleX;			// X
int angleY;			// Y
// ���������� �� ����� ����� �� ���
int distX;			// X
int distY;			// Y
int distZ = -150;	// Z
// �������������
int mouseX, mouseY;	// ���������� �������
int on_button;		// �������� ����, ������� ������ (-1 - ��� �������, 0 - �����, 1 - �������, 2 - ������)
int action;			// �������� ����
int k = 1;			// ��� �������� ����� (1 - ������, -1 �����)
cGames game;		// ��� �����

void draw()
{
	// ��������� ������
	glViewport(0, 0, width, height);
	// ������� ����
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	// ��������� ������ � �� �������
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (GLfloat)width/height, 1, 200);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// �������� �������������� ���������
	glEnable(GL_DEPTH_TEST);				// �������� ����� �������
	glEnable(GL_CULL_FACE);					// ������� (�� �������) ���������� �� ��� ������������
	glEnable(GL_LIGHTING);					// �������� ����, �� ������� ��� ����� "�������"
	glEnable(GL_LIGHT0);					// ��������� �������� ����� �0, ������ �� ������ �� "����"
	glEnable(GL_COLOR_MATERIAL);			// �������� ���� � ���������
	glEnableClientState(GL_VERTEX_ARRAY);	// �������� ������ ��������� ������
	glEnableClientState(GL_COLOR_ARRAY);	// �������� ������ ����� ������
	glEnableClientState(GL_NORMAL_ARRAY);	// �������� ������ ��������
	// ������ ���� �����, ���� action = {1, 2, 3, 4}
	switch (action)
	{
		// ���� �������� ���, ��������� ���� ����� ����� �� � ���� �����, �� �������� ������ ������
		case 0:
			for (int i = 0 ; i < 2 ; i++)
				if ((game.ball[i].x == game.pocket[1 - i].x) && (game.ball[i].y == game.pocket[1 - i].y))
				{
					game.load();
					break;
				}
			break;
		// ��������� ����� ������
		case 1:
			angleX -= 5 * k;
			if (angleX == -45 || angleX == 0) k *= -1;
			break;
		// �����
		case 2:
			angleX += 5  * k;
			if (angleX == 45 || angleX == 0) k *= -1;
			break;
		// �����
		case 3:
			angleY -= 5  * k;
			if (angleY == -45 || angleY == 0) k *= -1;
			break;
		// ������
		case 4:
			angleY += 5  * k;
			if (angleY == 45 || angleY == 0) k *= -1;
			break;
	}
	// ���������� �������
	// ���������� ����� �� ��� X, Y � Z
	glTranslatef(distX, distY, distZ);
	// ������� ����� ������������ ���X
	glRotatef(angleX, 1, 0, 0);
	// ������� ����� ������������ ���Y
	glRotatef(angleY, 0, 1, 0);
	// ������ ����� ���, ����� ����� ����� �������� � ������� ���������
	glTranslatef(-(float)(game.width * 10 + 2) / 2, -(float)(game.height * 10 + 2) / 2, 0);
	// ��������� �����
	game.draw(&action);
	// ��������� ���, ��� ��������
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	// ��������� ��������
	glFlush();
	// ���� return
	glutSwapBuffers();
}
void on_keyboard(unsigned char key, int x, int y)
{
	// escape
	if (key == 27) exit(0);
	// ���� ����� ���������, �� ��������� �� ����������
	if (action != 0) return;
	// ����� ������������
	switch (key)
	{
	case 'w':			// ������ ����� ������
		action = 1;
		break;
	case 's':			// ������ ����� �����
		action = 2;
		break;
	case 'a':			// ������ ����� �����
		action = 3;
		break;
	case 'd':			// ������ ����� ������
		action = 4;
		break;
	case 'r':			// ����� ����
		game.load();
		distX = 0;
		distY = 0;
		distZ = -150;
		break;
	case '+':			// ���������� ������
		if (distZ <= -70)
			distZ += 2;
		break;
	case '-':			// �������� ������
		if (distZ >= -190)
			distZ -= 2;
		break;
	}
}
void on_mouse(int button, int state, int x, int y)
{
	// ���� ����� ���������, �� ��������� �� ����
	if (action != 0) return;
	// ����� ������������
	on_button = button;
	if (state == 0)
	{
		// ����� ������ �� ���������
		if (button == 1)
		{
			distX = 0;
			distY = 0;
			distZ = -150;
		}
		// ���������� ���������� �������
		if (button == 2)
		{
			mouseX = x;
			mouseY = y;
		}
	}
}
void motion_mouse(int x, int y)
{
	// ���� ����� ���������, �� ��������� �� ����
	if (action != 0) return;
	// ����� ������� ����� �� ��������� XY
	if (on_button == 2)
	{
		distX += (x - mouseX) / 2;
		distY += (mouseY - y) / 2;
		mouseX = x;
		mouseY = y;
	}
}
void on_timer(int value)
{
	draw();													// ���������� �����
	glutTimerFunc(33, on_timer, 0);							// ����� 33�� ��������� ��� �������
}
void on_exit(void) 
{ 
}
int main(int argc, char* argv[])							// ���� � ���������
{
	atexit(on_exit);										// ������, ����� ������� �������� �� ������� ��� ������
	glutInit(&argc, argv);									// ������������� GLUT
	glutInitWindowSize(width, height);						// ��������� �������� ����
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);	// ��������� OpenGL ���������
	glutCreateWindow("���� ��������");						// �������� ����
	glutFullScreen();										// ������ ���� �������������
	glutDisplayFunc(draw);									// ��������� ������� ���������� �� �����������
	glutKeyboardFunc(on_keyboard);							// --//--//--//-- �� ������� �� �������
	glutMouseFunc(on_mouse);								// --//--//--//-- �� ������� �� ������ ����
	glutMotionFunc(motion_mouse);							// --//--//--//-- �� ����������� ���� � ������� �������
	glutTimerFunc(33, on_timer, 0);							// ����� 33�� ��������� ��� �������
	glutMainLoop();											// ���� ��������� ���������
	return (0);
}