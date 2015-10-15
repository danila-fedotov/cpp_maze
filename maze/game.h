#include <vector>	// ����������� ��������

// ��������� ��������� ��� ������� � �����
struct sCoords
{
	int x;
	int y;
};
// �����, � ������� ������� ��� �������� ����
class cGames
{
public:
	sCoords ball[2];							// ��� ������
	sCoords pocket[2];							// ��� �����
	int width;									// ������ ����� � �������
	int height;									// ������ ����� � �������

	cGames();									// �����������
	~cGames();									// ����������
	void draw(int *action);						// ��������� �����
	void load();								// ������������� ���� ����������, �������� �����
private:
	int span;									// ���������� ����� ��� ����������� �������
	int colSpace;								// ���������� ����������, ��� ��������� ���������
	std::vector <int> coords;					// ������ ���������
	std::vector <float> colors;					// ������ ������
	std::vector <float> normals;				// ������ ��������
	std::vector < std::vector <bool> > hor;		// ������ �������������� ������ |
	std::vector < std::vector <bool> > ver;		// ������ ������������ ������ _
};