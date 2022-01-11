#pragma once

class Block
{
	friend struct std::hash<Block>;
	friend bool operator == (const Block& first, const Block& sec);
	friend std::ofstream& operator << (std::ofstream& ofs, const Block& block);
public:
	Block(int x, int y, int z)
	{
		float val1 = (rand() % 10000) / 10000.;
		float val2 = (rand() % 10000) / 10000.;
		float val3 = (rand() % 10000) / 10000.;

		r = -0.5 + 1.5 * val1;
		g = -0.5 + 1.5 * val2;
		b = -0.5 + 1.5 * val3;
		this->x = x;
		this->y = y;
		this->z = z;
	}
protected:
	virtual void print(std::ofstream& ofs) const;
	int x;
	int y;
	int z;
	float r;
	float g;
	float b;
};




enum ORIENTATION {
	UP = 0,
	DOWN,
	RIGHT,
	LEFT
};

class Window : public Block
{
public:
	Window(int x, int y, int z, ORIENTATION or ) :
		Block(x, y, z)
	{
		id = 140322687641976357 + idCount++;
		forward = or ;
		if (or < 2)
			up = UP;
		else
			up = DOWN;
	}
protected:
	static int idCount;
	static const std::string str[4];
	static const std::string str2[4];
	enum ORIENTATION forward;
	enum ORIENTATION up;
	__int64 id;
};

class Window1x1 : public Window
{
public:
	Window1x1(int x, int y, int z, ORIENTATION or ) :
		Window(x, y, z, or ) { }
protected:
	virtual void print(std::ofstream& ofs) const;
};

class Window2x3 : public Window
{
public:
	Window2x3(int x, int y, int z, ORIENTATION or ) :
		Window(x, y, z, or ) { }
protected:
	static const std::string Window2x3::str2x3[4];
	virtual void print(std::ofstream& ofs) const;
};

class Window3x3 : public Window
{
public:
	Window3x3(int x, int y, int z, ORIENTATION or ) :
		Window(x, y, z, or ) { }
protected:
	virtual void print(std::ofstream& ofs) const;
};

bool operator == (const Block& first, const Block& sec);


namespace std
{
template <>
struct hash<Block>
{
	size_t operator () (const Block& b) const { return (b.x * 18397) + (b.y * 20483) + (b.z * 29303); }
};
}
enum CELLTYPE
{
	WALL = 0,
	PASSAGE,
};
enum
{
	W = 0, E, N, S, NW, NE, SW, SE
};
class Cell
{
public:
	Cell()
	{
		type = WALL;
	}
	virtual Cell** get_neig() = 0;
	virtual void MakePassage() = 0;
	CELLTYPE GetType()
	{
		return type;
	}
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& boost::serialization::make_binary_object(&type, sizeof(type));

	}
	int IsCorner()
	{
		if (type != WALL)
			return 0;
		Cell** neig = get_neig();
		int end = 0;
		for (int k = 0; k < 4; k++) {
			if (!neig[k])
				return 0;
			if (neig[k]->GetType() == WALL)
				end++;
		}
		return ((end == 1) || ((neig[N]->GetType() == WALL && (neig[E]->GetType() == WALL || neig[W]->GetType() == WALL)) ||
			(neig[S]->GetType() == WALL && (neig[E]->GetType() == WALL || neig[W]->GetType() == WALL))));
	}
protected:
	CELLTYPE type;
};

class SimpleCell : public Cell
{
public:
	SimpleCell()
	{
		way = 0;
		for (int i = 0; i < 8; i++)
			_neighbors[i] = NULL;
	}
	virtual Cell** get_neig()
	{
		return (Cell**)(_neighbors);
	}
	virtual void MakePassage()
	{
		type = PASSAGE;
	}
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& boost::serialization::base_object<Cell>(*this);
	}
	int x;
	int y;
	COLORREF color;
	int way;
protected:

	SimpleCell* _neighbors[8];
};

class SuperCell : public Cell
{
public:
	SuperCell()
	{
		for (int i = 0; i < 8; i++)
			_neighbors[i] = NULL;
	}
	virtual Cell** get_neig()
	{
		return (Cell**)(_neighbors);
	}
	virtual void MakePassage()
	{
		type = PASSAGE;
		for (auto& i : _cells)
			i->MakePassage();
	}
	void AddCell(SimpleCell* cell)
	{
		_cells.push_back(cell);
	}
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& boost::serialization::base_object<Cell>(*this);
	}
protected:
	SuperCell* _neighbors[8];
	std::vector<SimpleCell*> _cells;
};

class CMaze
{
	friend class boost::serialization::access;
public:
	CMaze() { m_pMaze = NULL; }
	CMaze(int rows, int cols);
	virtual ~CMaze();
	virtual void GeneratePrim();
	void GenerateDFS();
	void Draw(HDC hdc);
	void CreateBlueprint(std::ofstream& ofs);

	template<class Archive>
	void save(Archive& ar, const unsigned int version) const
	{
		for (int i = 0; i < m_rows; i++) {
			for (int j = 0; j < m_cols; j++) {
				ar& m_pMaze[i][j];
			}
		}
	}
	template<class Archive>
	void load(Archive& ar, const unsigned int version)
	{
		m_pMaze = new SimpleCell * [m_rows];
		for (int i = 0; i < m_rows; i++) {
			m_pMaze[i] = new SimpleCell[m_cols];
			for (int j = 0; j < m_cols; j++)
				ar& m_pMaze[i][j];
		}
	}
	template<class Archive>
	void serialize(Archive& ar, const unsigned int file_version)
	{
		ar& m_rows;
		ar& m_cols;
		ar& m_nCell;

		boost::serialization::split_member(ar, *this, file_version);
	}
protected:
	void _GeneratePrim(Cell* first);
	SimpleCell** m_pMaze;
	int m_nCell;
	int m_rows;
	int m_cols;
};

class CSuperMaze : public CMaze
{
	friend class boost::serialization::access;
	typedef CMaze inherited;
public:
	CSuperMaze() { m_supMaze = NULL; }
	CSuperMaze(int rows, int cols, int cell_size);
	virtual ~CSuperMaze();
	virtual void GeneratePrim();

	template<class Archive>
	void save(Archive& ar, const unsigned int version) const
	{
	}
	template<class Archive>
	void load(Archive& ar, const unsigned int version)
	{
	}
	template<class Archive>
	void serialize(Archive& ar, const unsigned int file_version)
	{
		ar& boost::serialization::base_object<CMaze>(*this);
		ar& _m;
		ar& _n;
		boost::serialization::split_member(ar, *this, file_version);
	}
protected:
	int _m;
	int _n;
	SuperCell** m_supMaze;
};