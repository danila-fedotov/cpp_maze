#include <stdlib.h>	// для atexit
#include "glut.h"	// подключение glut
#include "game.h"	// подключение нашего класса
// размеры экрана
int width = 1280;	// ширина
int height = 800;	// высота
// текущий угол поворота сцены относительно оси
int angleX;			// X
int angleY;			// Y
// расстояние до нашей сцены по оси
int distX;			// X
int distY;			// Y
int distZ = -150;	// Z
// дополнительно
int mouseX, mouseY;	// координаты курсора
int on_button;		// кнопочка мыши, которая нажата (-1 - нет нажатых, 0 - левая, 1 - средняя, 2 - правая)
int action;			// действия игры
int k = 1;			// для поворота сцены (1 - вперед, -1 назад)
cGames game;		// наш класс

void draw()
{
	// установка экрана
	glViewport(0, 0, width, height);
	// очищаем окно
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	// настройка камеры и ее позиции
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (GLfloat)width/height, 1, 200);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// включаем дополнительные параметры
	glEnable(GL_DEPTH_TEST);				// включаем буфер глубины
	glEnable(GL_CULL_FACE);					// срезаем (не выводим) отвернутые от нас треугольники
	glEnable(GL_LIGHTING);					// включаем свет, не сделать все будет "плоским"
	glEnable(GL_LIGHT0);					// добавляем источник света №0, сейчас он светит из "глаз"
	glEnable(GL_COLOR_MATERIAL);			// включаем цвет у материала
	glEnableClientState(GL_VERTEX_ARRAY);	// включаем массив координат вершин
	glEnableClientState(GL_COLOR_ARRAY);	// включаем массив цвета вершин
	glEnableClientState(GL_NORMAL_ARRAY);	// включаем массив нормалей
	// задаем угол сцены, если action = {1, 2, 3, 4}
	switch (action)
	{
		// если действий нет, проверяем если шарик попал не в свою лунку, то начинаем играть заново
		case 0:
			for (int i = 0 ; i < 2 ; i++)
				if ((game.ball[i].x == game.pocket[1 - i].x) && (game.ball[i].y == game.pocket[1 - i].y))
				{
					game.load();
					break;
				}
			break;
		// наклоняем доску вперед
		case 1:
			angleX -= 5 * k;
			if (angleX == -45 || angleX == 0) k *= -1;
			break;
		// назад
		case 2:
			angleX += 5  * k;
			if (angleX == 45 || angleX == 0) k *= -1;
			break;
		// влево
		case 3:
			angleY -= 5  * k;
			if (angleY == -45 || angleY == 0) k *= -1;
			break;
		// вправо
		case 4:
			angleY += 5  * k;
			if (angleY == 45 || angleY == 0) k *= -1;
			break;
	}
	// управление камерой
	// перемещаем сцену по оси X, Y и Z
	glTranslatef(distX, distY, distZ);
	// поворот сцены относительно осиX
	glRotatef(angleX, 1, 0, 0);
	// поворот сцены относительно осиY
	glRotatef(angleY, 0, 1, 0);
	// ставим сцену так, чтобы центр сцены совпадал с началом координат
	glTranslatef(-(float)(game.width * 10 + 2) / 2, -(float)(game.height * 10 + 2) / 2, 0);
	// отрисовка сцены
	game.draw(&action);
	// отключаем все, что включали
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	// закончили рисовать
	glFlush();
	// типа return
	glutSwapBuffers();
}
void on_keyboard(unsigned char key, int x, int y)
{
	// escape
	if (key == 27) exit(0);
	// если сцена двигается, не реагируем на клавиатуру
	if (action != 0) return;
	// иначе обрабатываем
	switch (key)
	{
	case 'w':			// наклон доски вперед
		action = 1;
		break;
	case 's':			// наклон доски назад
		action = 2;
		break;
	case 'a':			// наклон доски влево
		action = 3;
		break;
	case 'd':			// наклон доски вправо
		action = 4;
		break;
	case 'r':			// сброс игры
		game.load();
		distX = 0;
		distY = 0;
		distZ = -150;
		break;
	case '+':			// приблизить камеру
		if (distZ <= -70)
			distZ += 2;
		break;
	case '-':			// отдалить камеру
		if (distZ >= -190)
			distZ -= 2;
		break;
	}
}
void on_mouse(int button, int state, int x, int y)
{
	// если сцена двигается, не реагируем на мышь
	if (action != 0) return;
	// иначе обрабатываем
	on_button = button;
	if (state == 0)
	{
		// сброс камеры по умолчанию
		if (button == 1)
		{
			distX = 0;
			distY = 0;
			distZ = -150;
		}
		// запоминаем координаты курсора
		if (button == 2)
		{
			mouseX = x;
			mouseY = y;
		}
	}
}
void motion_mouse(int x, int y)
{
	// если сцена двигается, не реагируем на мышь
	if (action != 0) return;
	// иначе двигаем сцену на плоскости XY
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
	draw();													// перерисуем экран
	glutTimerFunc(33, on_timer, 0);							// через 33мс вызовется эта функция
}
void on_exit(void) 
{ 
}
int main(int argc, char* argv[])							// вход в программу
{
	atexit(on_exit);										// укажем, какая функция отвечает за очистку при выходе
	glutInit(&argc, argv);									// инициализация GLUT
	glutInitWindowSize(width, height);						// установка размеров окна
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);	// настройка OpenGL контекста
	glutCreateWindow("Игра Лабиринт");						// создание окна
	glutFullScreen();										// делаем окно полноэкранным
	glutDisplayFunc(draw);									// указываем функцию отвечающую за перерисовку
	glutKeyboardFunc(on_keyboard);							// --//--//--//-- за нажатие на клавишу
	glutMouseFunc(on_mouse);								// --//--//--//-- за нажатие на кнопку мыши
	glutMotionFunc(motion_mouse);							// --//--//--//-- за перемещение мыши с нажатой кнопкой
	glutTimerFunc(33, on_timer, 0);							// через 33мс вызовется эта функция
	glutMainLoop();											// цикл обработки сообщений
	return (0);
}