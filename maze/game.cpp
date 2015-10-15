#include "game.h"	// подключение нашего класса
#include "glut.h"	// подключение glut

cGames::cGames()
{
	load();			// загружаем всю сцену
}
cGames::~cGames()
{
}
void cGames::draw(int *action)
{
	// движение шариков 
	// 0 - нет движения
	// 1 - в одну сторону, либо вверх, либо вправо
	// -1 - соответственно в другую, либо вниз, либо влево
	int k[2] = {0, 0};
	// для отрисовки примитивов
	GLUquadricObj* obj = gluNewQuadric();
	// отрисовка доски
	glNormalPointer(GL_INT, 0, &normals[0]);	// задаем массив нормалей
	glColorPointer(3, GL_FLOAT, 0, &colors[0]);	// массив цвета
	glVertexPointer(3, GL_INT, 0, &coords[0]);	// массив координат вершин
	glDrawArrays(GL_QUADS, 0, colSpace * 4);	// отрисовка массивов
	// если доска наклоняется, то увеличиваем шаг (span)
	if (*action != 0) span++;
	// проверка могут ли шарики двигаться
	switch (*action)
	{
		// вверх
		case 1:
			// проверка, если ли стенка в данном направлении
			for (int i = 0 ; i < 2 ; i++)
				if ((ball[i].y != height - 1) && (ver[ball[i].x][height - 2 - ball[i].y] != 1))
					k[i] = 1;
			// попал ли шарик в свою лунку, если да, то он больше не двигается
			for (int i = 0 ; i < 2 ; i++)
				if ((ball[i].x == pocket[i].x) && (ball[i].y == pocket[i].y))
					k[i] = 0;
			// проверка, нет ли соседнего шарика, который не может двигаться, по направлению движения
			for (int i = 0 ; i < 2 ; i++)
				if ((k[i] == 1) && (ball[i].y + 1 == ball[1 - i].y) && (ball[i].x == ball[1 - i].x) && (k[1 - i] == 0))
					k[i] = 0;
			// если доска "встала на место", то меняем координаты у шарика
			if (span > 17)
				for (int i = 0 ; i < 2 ; i++)
					if (k[i] == 1) ball[i].y++;
			break;
		// вниз
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
		// влево
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
		// вправо
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
	// отрисовка первого шарика
	glPushMatrix();				// запоминаем текущую матрицу преобразований
	// сбрасываем счетчик шагов
	if (span > 17) span = 0;
	// стбрасываем действие доски
	if (span == 0) *action = 0;
	// перемещаем шарик, в зависимости от направления
	if (*action < 3)
		glTranslatef(ball[0].x * 10 + 6, ball[0].y * 10 + 6 + span * ((float)10 / 17 * k[0]), 3);
	else
		glTranslatef(ball[0].x * 10 + 6 + span * ((float)10 / 17 * k[0]), ball[0].y * 10 + 6, 3);
	// задаем цвет у шарика:
	// свободно катается, то красный
	// попал в свою лунку, то зеленый
	if ((ball[0].x == pocket[0].x) && (ball[0].y == pocket[0].y))
		glColor3f(0, 1, 0);
	else
		glColor3f(0, 0, 1);
	gluSphere(obj, 4, 20, 20);	// отрисовка сферы
	glPopMatrix();				// восстанавливаем матрицу преобразований
	// отрисовка второго шарика
	glPushMatrix();				// запоминаем текущую матрицу преобразований
	if (*action < 3)
		glTranslatef(ball[1].x * 10 + 6, ball[1].y * 10 + 6 + span * ((float)10 / 17 * k[1]), 3);
	else
		glTranslatef(ball[1].x * 10 + 6 + span * ((float)10 / 17 * k[1]), ball[1].y * 10 + 6, 3);
	// задаем цвет у шарика:
	// свободно катается, то синий
	// попал в свою лунку, то зеленый
	if ((ball[1].x == pocket[1].x) && (ball[1].y == pocket[1].y))
		glColor3f(0, 1, 0);
	else
		glColor3f(1, 0, 0);
	gluSphere(obj, 4, 20, 20);
	glPopMatrix();				// восстанавливаем матрицу преобразований
	// отрисовка первой лунки
	glPushMatrix();				// запоминаем текущую матрицу преобразований
	glTranslatef(pocket[0].x * 10 + 6, pocket[0].y * 10 + 6, 0.5);
	// задаем цвет у лунки:
	// без шарика, то синий
	// попал "правильный" шарик, то зеленый
	if ((ball[0].x == pocket[0].x) && (ball[0].y == pocket[0].y))
		glColor3f(0, 1, 0);
	else
		glColor3f(0, 0, 1);
	gluDisk(obj, 3, 4, 30, 30);
	glColor3f(0, 0, 0);
	gluDisk(obj, 0, 3, 30, 30);
	glPopMatrix();				// восстанавливаем матрицу преобразований
	// отрисовка второй лунки
	glPushMatrix();				// запоминаем текущую матрицу преобразований
	glTranslatef(pocket[1].x * 10 + 6, pocket[1].y * 10 + 6, 0.5);
	// задаем цвет у лунки:
	// без шарика, то красный
	// попал "правильный" шарик, то зеленый
	if ((ball[1].x == pocket[1].x) && (ball[1].y == pocket[1].y))
		glColor3f(0, 1, 0);
	else
		glColor3f(1, 0, 0);
	gluDisk(obj, 3, 4, 30, 30);
	glColor3f(0, 0, 0);
	gluDisk(obj, 0, 3, 30, 30);
	glPopMatrix();				// восстанавливаем матрицу преобразований
}
void cGames::load()
{
	// инициализация переменных
	width = 8;			// ширина доски в ячейках
	height = 8;			// ширина доски в ячейках
	colSpace = 0;		// обнуляем количество плоскостей
	// координаты первого шарика
	ball[0].x = 0;
	ball[0].y = 7;
	// координаты второго шарика
	ball[1].x = 7;
	ball[1].y = 7;
	// координаты первой лунки
	pocket[0].x = 7;
	pocket[0].y = 0;
	// координаты второй лунки
	pocket[1].x = 0;
	pocket[1].y = 0;
	// инициализация горизонтальных стенок
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
	// инициализация вертикальных стенок
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
	// пол
	colSpace++;
	coords.push_back(0);				coords.push_back(0);				coords.push_back(0);
	coords.push_back(width * 10 + 2);	coords.push_back(0);				coords.push_back(0);
	coords.push_back(width * 10 + 2);	coords.push_back(height * 10 + 2);	coords.push_back(0);
	coords.push_back(0);				coords.push_back(height * 10 + 2);	coords.push_back(0);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	// крышка нижнего левого столбика
	colSpace++;
	coords.push_back(0);				coords.push_back(0);				coords.push_back(5);
	coords.push_back(2);				coords.push_back(0);				coords.push_back(5);
	coords.push_back(2);				coords.push_back(2);				coords.push_back(5);
	coords.push_back(0);				coords.push_back(2);				coords.push_back(5);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	// крышка нижнего правого столбика
	colSpace++;
	coords.push_back(width * 10);		coords.push_back(0);				coords.push_back(5);
	coords.push_back(width * 10 + 2);	coords.push_back(0);				coords.push_back(5);
	coords.push_back(width * 10 + 2);	coords.push_back(2);				coords.push_back(5);
	coords.push_back(width * 10);		coords.push_back(2);				coords.push_back(5);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	// крышка верхнего правого столбика
	colSpace++;
	coords.push_back(width * 10);		coords.push_back(height * 10);		coords.push_back(5);
	coords.push_back(width * 10 + 2);	coords.push_back(height * 10);		coords.push_back(5);
	coords.push_back(width * 10 + 2);	coords.push_back(height * 10 + 2);	coords.push_back(5);
	coords.push_back(width * 10);		coords.push_back(height * 10 + 2);	coords.push_back(5);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	// крышка верхнего левого столбика
	colSpace++;
	coords.push_back(0);				coords.push_back(height * 10);		coords.push_back(5);
	coords.push_back(2);				coords.push_back(height * 10);		coords.push_back(5);
	coords.push_back(2);				coords.push_back(height * 10 + 2);	coords.push_back(5);
	coords.push_back(0);				coords.push_back(height * 10 + 2);	coords.push_back(5);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	// нижняя крышка
	colSpace++;
	coords.push_back(2);				coords.push_back(0);				coords.push_back(5);
	coords.push_back(width * 10);		coords.push_back(0);				coords.push_back(5);
	coords.push_back(width * 10);		coords.push_back(2);				coords.push_back(5);
	coords.push_back(2);				coords.push_back(2);				coords.push_back(5);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	// верхняя крышка
	colSpace++;
	coords.push_back(2);				coords.push_back(height * 10);		coords.push_back(5);
	coords.push_back(width * 10);		coords.push_back(height * 10);		coords.push_back(5);
	coords.push_back(width * 10);		coords.push_back(height * 10 + 2);	coords.push_back(5);
	coords.push_back(2);				coords.push_back(height * 10 + 2);	coords.push_back(5);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	// левая крышка
	colSpace++;
	coords.push_back(0);				coords.push_back(2);				coords.push_back(5);
	coords.push_back(2);				coords.push_back(2);				coords.push_back(5);
	coords.push_back(2);				coords.push_back(height * 10);		coords.push_back(5);
	coords.push_back(0);				coords.push_back(height * 10);		coords.push_back(5);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	// правая крышка
	colSpace++;
	coords.push_back(width * 10);		coords.push_back(2);				coords.push_back(5);
	coords.push_back(width * 10 + 2);	coords.push_back(2);				coords.push_back(5);
	coords.push_back(width * 10 + 2);	coords.push_back(height * 10);		coords.push_back(5);
	coords.push_back(width * 10);		coords.push_back(height * 10);		coords.push_back(5);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	normals.push_back(0);				normals.push_back(0);				normals.push_back(1);
	// внешняя нижняя стенка
	colSpace++;
	coords.push_back(0);				coords.push_back(0);				coords.push_back(0);
	coords.push_back(width * 10 + 2);	coords.push_back(0);				coords.push_back(0);
	coords.push_back(width * 10 + 2);	coords.push_back(0);				coords.push_back(5);
	coords.push_back(0);				coords.push_back(0);				coords.push_back(5);
	normals.push_back(0);				normals.push_back(-1);				normals.push_back(0);
	normals.push_back(0);				normals.push_back(-1);				normals.push_back(0);
	normals.push_back(0);				normals.push_back(-1);				normals.push_back(0);
	normals.push_back(0);				normals.push_back(-1);				normals.push_back(0);
	// внутренняя нижняя стенка
	colSpace++;
	coords.push_back(2);				coords.push_back(2);				coords.push_back(5);
	coords.push_back(width * 10);		coords.push_back(2);				coords.push_back(5);
	coords.push_back(width * 10);		coords.push_back(2);				coords.push_back(0);
	coords.push_back(2);				coords.push_back(2);				coords.push_back(0);
	normals.push_back(0);				normals.push_back(1);				normals.push_back(0);
	normals.push_back(0);				normals.push_back(1);				normals.push_back(0);
	normals.push_back(0);				normals.push_back(1);				normals.push_back(0);
	normals.push_back(0);				normals.push_back(1);				normals.push_back(0);
	// внешняя верхняя стенка
	colSpace++;
	coords.push_back(0);				coords.push_back(height * 10 + 2);	coords.push_back(5);
	coords.push_back(width * 10 + 2);	coords.push_back(height * 10 + 2);	coords.push_back(5);
	coords.push_back(width * 10 + 2);	coords.push_back(height * 10 + 2);	coords.push_back(0);
	coords.push_back(0);				coords.push_back(height * 10 + 2);	coords.push_back(0);
	normals.push_back(0);				normals.push_back(1);				normals.push_back(0);
	normals.push_back(0);				normals.push_back(1);				normals.push_back(0);
	normals.push_back(0);				normals.push_back(1);				normals.push_back(0);
	normals.push_back(0);				normals.push_back(1);				normals.push_back(0);
	// внутренняя верхняя стенка
	colSpace++;
	coords.push_back(2);				coords.push_back(height * 10);		coords.push_back(0);
	coords.push_back(width * 10);		coords.push_back(height * 10);		coords.push_back(0);
	coords.push_back(width * 10);		coords.push_back(height * 10);		coords.push_back(5);
	coords.push_back(2);				coords.push_back(height * 10);		coords.push_back(5);
	normals.push_back(0);				normals.push_back(-1);				normals.push_back(0);
	normals.push_back(0);				normals.push_back(-1);				normals.push_back(0);
	normals.push_back(0);				normals.push_back(-1);				normals.push_back(0);
	normals.push_back(0);				normals.push_back(-1);				normals.push_back(0);
	// внешняя левая стенка
	colSpace++;
	coords.push_back(0);				coords.push_back(height * 10 + 2);	coords.push_back(0);
	coords.push_back(0);				coords.push_back(0);				coords.push_back(0);
	coords.push_back(0);				coords.push_back(0);				coords.push_back(5);
	coords.push_back(0);				coords.push_back(height * 10 + 2);	coords.push_back(5);
	normals.push_back(-1);				normals.push_back(0);				normals.push_back(0);
	normals.push_back(-1);				normals.push_back(0);				normals.push_back(0);
	normals.push_back(-1);				normals.push_back(0);				normals.push_back(0);
	normals.push_back(-1);				normals.push_back(0);				normals.push_back(0);
	// внутренняя левая стенка
	colSpace++;
	coords.push_back(2);				coords.push_back(2);				coords.push_back(0);
	coords.push_back(2);				coords.push_back(height * 10);		coords.push_back(0);
	coords.push_back(2);				coords.push_back(height * 10);		coords.push_back(5);
	coords.push_back(2);				coords.push_back(2);				coords.push_back(5);
	normals.push_back(1);				normals.push_back(0);				normals.push_back(0);
	normals.push_back(1);				normals.push_back(0);				normals.push_back(0);
	normals.push_back(1);				normals.push_back(0);				normals.push_back(0);
	normals.push_back(1);				normals.push_back(0);				normals.push_back(0);
	// внешняя правая стенка
	colSpace++;
	coords.push_back(width * 10 + 2);	coords.push_back(0);				coords.push_back(0);
	coords.push_back(width * 10 + 2);	coords.push_back(height * 10 + 2);	coords.push_back(0);
	coords.push_back(width * 10 + 2);	coords.push_back(height * 10 + 2);	coords.push_back(5);
	coords.push_back(width * 10 + 2);	coords.push_back(0);				coords.push_back(5);
	normals.push_back(1);				normals.push_back(0);				normals.push_back(0);
	normals.push_back(1);				normals.push_back(0);				normals.push_back(0);
	normals.push_back(1);				normals.push_back(0);				normals.push_back(0);
	normals.push_back(1);				normals.push_back(0);				normals.push_back(0);
	// внутренняя правая стенка
	colSpace++;
	coords.push_back(width * 10);		coords.push_back(height * 10);		coords.push_back(0);
	coords.push_back(width * 10);		coords.push_back(2);				coords.push_back(0);
	coords.push_back(width * 10);		coords.push_back(2);				coords.push_back(5);
	coords.push_back(width * 10);		coords.push_back(height * 10);		coords.push_back(5);
	normals.push_back(-1);				normals.push_back(0);				normals.push_back(0);
	normals.push_back(-1);				normals.push_back(0);				normals.push_back(0);
	normals.push_back(-1);				normals.push_back(0);				normals.push_back(0);
	normals.push_back(-1);				normals.push_back(0);				normals.push_back(0);
	// столбики
	for (int i = 0 ; i < height - 1 ; i++)
		for (int j = 0 ; j < width - 1 ; j++)
		{
			// крышка столбика
			colSpace++;
			coords.push_back(j * 10 + 10);	coords.push_back((height - i - 1) * 10);	coords.push_back(5);
			coords.push_back(j * 10 + 12);	coords.push_back((height - i - 1) * 10);	coords.push_back(5);
			coords.push_back(j * 10 + 12);	coords.push_back((height - i - 1) * 10 + 2);coords.push_back(5);
			coords.push_back(j * 10 + 10);	coords.push_back((height - i - 1) * 10 + 2);coords.push_back(5);
			normals.push_back(0);			normals.push_back(0);						normals.push_back(1);
			normals.push_back(0);			normals.push_back(0);						normals.push_back(1);
			normals.push_back(0);			normals.push_back(0);						normals.push_back(1);
			normals.push_back(0);			normals.push_back(0);						normals.push_back(1);
			// верхняя стенка столбика
			colSpace++;
			coords.push_back(j * 10 + 10);	coords.push_back((height - i - 1) * 10 + 2);coords.push_back(5);
			coords.push_back(j * 10 + 12);	coords.push_back((height - i - 1) * 10 + 2);coords.push_back(5);
			coords.push_back(j * 10 + 12);	coords.push_back((height - i - 1) * 10 + 2);coords.push_back(0);
			coords.push_back(j * 10 + 10);	coords.push_back((height - i - 1) * 10 + 2);coords.push_back(0);
			normals.push_back(0);			normals.push_back(1);						normals.push_back(0);
			normals.push_back(0);			normals.push_back(1);						normals.push_back(0);
			normals.push_back(0);			normals.push_back(1);						normals.push_back(0);
			normals.push_back(0);			normals.push_back(1);						normals.push_back(0);
			// нижняя стенка столбика
			colSpace++;
			coords.push_back(j * 10 + 10);	coords.push_back((height - i - 1) * 10);	coords.push_back(0);
			coords.push_back(j * 10 + 12);	coords.push_back((height - i - 1) * 10);	coords.push_back(0);
			coords.push_back(j * 10 + 12);	coords.push_back((height - i - 1) * 10);	coords.push_back(5);
			coords.push_back(j * 10 + 10);	coords.push_back((height - i - 1) * 10);	coords.push_back(5);
			normals.push_back(0);			normals.push_back(-1);						normals.push_back(0);
			normals.push_back(0);			normals.push_back(-1);						normals.push_back(0);
			normals.push_back(0);			normals.push_back(-1);						normals.push_back(0);
			normals.push_back(0);			normals.push_back(-1);						normals.push_back(0);
			// левая стенка столбика
			colSpace++;
			coords.push_back(j * 10 + 10);	coords.push_back((height - i - 1) * 10 + 2);coords.push_back(0);
			coords.push_back(j * 10 + 10);	coords.push_back((height - i - 1) * 10);	coords.push_back(0);
			coords.push_back(j * 10 + 10);	coords.push_back((height - i - 1) * 10);	coords.push_back(5);
			coords.push_back(j * 10 + 10);	coords.push_back((height - i - 1) * 10 + 2);coords.push_back(5);
			normals.push_back(-1);			normals.push_back(0);						normals.push_back(0);
			normals.push_back(-1);			normals.push_back(0);						normals.push_back(0);
			normals.push_back(-1);			normals.push_back(0);						normals.push_back(0);
			normals.push_back(-1);			normals.push_back(0);						normals.push_back(0);
			// правая стенка столбика
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
	// горизонтальные стенки
	for(int i = 7 ; i >= 0 ; i--)
		for(int j = 0 ; j < 7 ; j++)
			if (hor[i][j] == 1)
			{
				//крышка стенки
				colSpace++;
				coords.push_back(10 + j * 10);	coords.push_back(height * 10 - 8 - i * 10);		coords.push_back(5);
				coords.push_back(12 + j * 10);	coords.push_back(height * 10 - 8 - i * 10);		coords.push_back(5);
				coords.push_back(12 + j * 10);	coords.push_back(height * 10 - i * 10);			coords.push_back(5);
				coords.push_back(10 + j * 10);	coords.push_back(height * 10 - i * 10);			coords.push_back(5);
				normals.push_back(0);			normals.push_back(0);							normals.push_back(1);
				normals.push_back(0);			normals.push_back(0);							normals.push_back(1);
				normals.push_back(0);			normals.push_back(0);							normals.push_back(1);
				normals.push_back(0);			normals.push_back(0);							normals.push_back(1);
				//левая грань стенки
				colSpace++;
				coords.push_back(10 + j * 10);	coords.push_back(height * 10 - 8 - i * 10);		coords.push_back(5);
				coords.push_back(10 + j * 10);	coords.push_back(height * 10 - i * 10);			coords.push_back(5);
				coords.push_back(10 + j * 10);	coords.push_back(height * 10 - i * 10);			coords.push_back(0);
				coords.push_back(10 + j * 10);	coords.push_back(height * 10 - 8 - i * 10);		coords.push_back(0);
				normals.push_back(-1);			normals.push_back(0);							normals.push_back(0);
				normals.push_back(-1);			normals.push_back(0);							normals.push_back(0);
				normals.push_back(-1);			normals.push_back(0);							normals.push_back(0);
				normals.push_back(-1);			normals.push_back(0);							normals.push_back(0);
				//правая грань стенки
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
	// вертикальные стенки
	for(int i = 6 ; i >= 0 ; i--)
		for(int j = 0 ; j < 8 ; j++)
			if(ver[j][i] == 1)
			{
				//крышка стенки
				colSpace++;
				coords.push_back(10 + j * 10);	coords.push_back(height * 10 - 10 - i * 10);	coords.push_back(5);
				coords.push_back(10 + j * 10);	coords.push_back(height * 10 - 8 - i * 10);		coords.push_back(5);
				coords.push_back(2 + j * 10);	coords.push_back(height * 10 - 8 - i * 10);		coords.push_back(5);
				coords.push_back(2 + j * 10);	coords.push_back(height * 10 - 10 - i * 10);	coords.push_back(5);
				normals.push_back(0);			normals.push_back(0);							normals.push_back(1);
				normals.push_back(0);			normals.push_back(0);							normals.push_back(1);
				normals.push_back(0);			normals.push_back(0);							normals.push_back(1);
				normals.push_back(0);			normals.push_back(0);							normals.push_back(1);
				//верхняя грань стенки
				colSpace++;
				coords.push_back(10 + j * 10);	coords.push_back(height * 10 - 8 - i * 10);		coords.push_back(5);
				coords.push_back(10 + j * 10);	coords.push_back(height * 10 - 8 - i * 10);		coords.push_back(0);
				coords.push_back(2 + j * 10);	coords.push_back(height * 10 - 8 - i * 10);		coords.push_back(0);
				coords.push_back(2 + j * 10);	coords.push_back(height * 10 - 8 - i * 10);		coords.push_back(5);
				normals.push_back(0);			normals.push_back(1);							normals.push_back(0);
				normals.push_back(0);			normals.push_back(1);							normals.push_back(0);
				normals.push_back(0);			normals.push_back(1);							normals.push_back(0);
				normals.push_back(0);			normals.push_back(1);							normals.push_back(0);
				//нижняя грань стенки
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
	// инициализация массива цвета
	colors.resize(colSpace * 4 * 3);
	for (int i = 0 ; i < 12 ; i++)
		colors[i] = 0.9;
	for (int i = 12 ; i < colSpace * 12 ; i++)
		colors[i] = 0.5;
}