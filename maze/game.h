#include <vector>	// подключение векторов

// структура координат для шариков и лунок
struct sCoords
{
	int x;
	int y;
};
// класс, в котором описаны все принципы игры
class cGames
{
public:
	sCoords ball[2];							// два шарика
	sCoords pocket[2];							// две лунки
	int width;									// ширина доски в ячейках
	int height;									// высота доски в ячейках

	cGames();									// конструктор
	~cGames();									// деструктор
	void draw(int *action);						// отрисовка сцены
	void load();								// инициализация всех переменных, загрузка карты
private:
	int span;									// количество шагов при перемещении шариков
	int colSpace;								// количество плоскостей, для отрисовки массивами
	std::vector <int> coords;					// массив координат
	std::vector <float> colors;					// массив цветов
	std::vector <float> normals;				// массив нормалей
	std::vector < std::vector <bool> > hor;		// массив горизонтальных стенок |
	std::vector < std::vector <bool> > ver;		// массив вертикальных стенок _
};