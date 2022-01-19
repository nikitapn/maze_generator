#include "stdafx.h"
#include "Maze.h"

#define CELL_XL 10
#define CELL_YL 10

BOOST_CLASS_EXPORT_GUID(SimpleCell, "simple_cell")
BOOST_CLASS_EXPORT_GUID(SuperCell, "super_cell")
BOOST_CLASS_EXPORT_GUID(CSuperMaze, "CSuperMaze")

const std::string Window::str[4] = { "Up", "Down", "Right", "Left" };

const std::string Window::str2[4] = { "Backward", "Forward", "Backward", "Backward" };

std::string str2x3_[4] = { "Up", "Left", "Down", "Right" };

const std::string Window2x3::str2x3[4] = { "Forward", "Forward", "Forward", "Forward" };



int Window::idCount = 0;

bool operator == (const Block& first, const Block& sec)
{
	return (first.x == sec.x && first.y == sec.y && first.z == sec.z) ? true : false;
}

std::ofstream& operator << (std::ofstream& ofs, const Block& block)
{
	block.print(ofs);
	return ofs;
}

void Block::print(std::ofstream& ofs) const
{
	ofs << "            <MyObjectBuilder_CubeBlock xsi:type=\"MyObjectBuilder_CubeBlock\">\n"
		<< "              <SubtypeName>LargeBlockArmorBlock</SubtypeName>\n"
		<< "              <Min x=\"" << x << "\" y=\"" << y << "\" z=\"" << z << "\" />\n"
		<< "              <ColorMaskHSV x=\"" << r << "\" y=\"" << g << "\" z=\"" << b << "\" />\n"
		<< "            </MyObjectBuilder_CubeBlock>\n";
}

void Window1x1::print(std::ofstream& ofs) const
{
	ofs << "            <MyObjectBuilder_CubeBlock xsi:type=\"MyObjectBuilder_CubeBlock\">\n"
		<< "              <SubtypeName>Window1x1Flat</SubtypeName>\n"
		<< "			  <EntityId>" << id << "</EntityId>\n"
		<< "              <Min x=\"" << x << "\" y=\"" << y << "\" z=\"" << z << "\" />\n"
		<< "			  <BlockOrientation Forward=\"" + Window::str[forward] + "\" Up=\"" + Window::str2[up] + "\" />\n"
		<< "              <ColorMaskHSV x=\"" << r << "\" y=\"" << g << "\" z=\"" << b << "\" />\n"
		<< "            </MyObjectBuilder_CubeBlock>\n";
}

void Window2x3::print(std::ofstream& ofs) const
{
	if (forward > 1)
	ofs << "            <MyObjectBuilder_CubeBlock xsi:type=\"MyObjectBuilder_CubeBlock\">\n"
		<< "              <SubtypeName>Window2x3Flat</SubtypeName>\n"
		<< "			  <EntityId>" << id << "</EntityId>\n"
		<< "              <Min x=\"" << x << "\" y=\"" << y << "\" z=\"" << z << "\" />\n"
		<< "			  <BlockOrientation Forward=\"" + Window2x3::str2x3[forward] + "\" Up=\"" + str2x3_[up] + "\" />\n"
		<< "              <ColorMaskHSV x=\"" << r << "\" y=\"" << g << "\" z=\"" << b << "\" />\n"
		<< "            </MyObjectBuilder_CubeBlock>\n";
	else
	ofs << "            <MyObjectBuilder_CubeBlock xsi:type=\"MyObjectBuilder_CubeBlock\">\n"
		<< "              <SubtypeName>Window2x3Flat</SubtypeName>\n"
		<< "			  <EntityId>" << id << "</EntityId>\n"
	//	<< "			  <BlockOrientation Forward=\"" + Window2x3::str2x3[up] + "\" Up=\"" + str2x3_[forward] + "\" />\n"
		<< "              <Min x=\"" << x << "\" y=\"" << y << "\" z=\"" << z << "\" />\n"
		<< "              <ColorMaskHSV x=\"" << r << "\" y=\"" << g << "\" z=\"" << b << "\" />\n"
		<< "            </MyObjectBuilder_CubeBlock>\n";

}

void Window3x3::print(std::ofstream& ofs) const
{
	ofs << "            <MyObjectBuilder_CubeBlock xsi:type=\"MyObjectBuilder_CubeBlock\">\n"
		<< "              <SubtypeName>Window3x3Flat</SubtypeName>\n"
		<< "			  <EntityId>" << id << "</EntityId>\n"
		<< "              <Min x=\"" << x << "\" y=\"" << y << "\" z=\"" << z << "\" />\n"
		<< "			  <BlockOrientation Forward=\"" + Window::str[forward] + "\" Up=\"" + Window::str2[up] + "\" />\n"
		<< "              <ColorMaskHSV x=\"" << r << "\" y=\"" << g << "\" z=\"" << b << "\" />\n"
		<< "            </MyObjectBuilder_CubeBlock>\n";
}

CMaze::CMaze(int rows, int cols)
{
	m_rows = rows;
	m_cols = cols;

	m_nCell = m_rows * m_cols;

	m_pMaze = new SimpleCell*[m_rows];
	for (int i = 0; i < m_rows; i++) {
		m_pMaze[i] = new SimpleCell[m_cols];
	}

	for (int i = 0; i < m_rows; i++)
	{
		for (int j = 0; j < m_cols; j++)
		{
			m_pMaze[i][j].x = i;
			m_pMaze[i][j].y = j;
			SimpleCell** neighbors = (SimpleCell**)m_pMaze[i][j].get_neig();
		
			if (i > 0) {
				neighbors[N] = &m_pMaze[i - 1][j];
				if (j > 0)
					neighbors[NW] = &m_pMaze[i - 1][j - 1];
				if (j < m_cols - 1)
					neighbors[NE] = &m_pMaze[i - 1][j + 1];
			}
			if (i < m_rows - 1) {
				neighbors[S] = &m_pMaze[i + 1][j];
				if (j > 0)
					neighbors[SW] = &m_pMaze[i + 1][j - 1];
				if (j < m_cols - 1)
					neighbors[SE] = &m_pMaze[i + 1][j + 1];
			}

			if (j > 0)
				neighbors[W] = &m_pMaze[i][j - 1];

			if (j < m_cols - 1)
				neighbors[E] = &m_pMaze[i][j + 1];
		}
	}
}

CMaze::~CMaze()
{
	for (int i = 0; i < m_rows; i++) {
		delete[]m_pMaze[i];
	}
	delete[]m_pMaze;
}

void CMaze::_GeneratePrim(Cell* first)
{
	std::list<Cell*> walls;
	Cell** first_neig;

	first->MakePassage();
	first_neig = first->get_neig();

	for (int i = 0; i < 4; i++) {
		if (!first_neig[i]) continue;
		walls.push_back(first_neig[i]);
	}

	while (!walls.empty())
	{
		int rnum = rand() % walls.size();
		auto it = walls.begin();
		std::advance(it, rnum);

		Cell* wall = *it;

		int k = 0;
		int n;

		Cell** neighbors = wall->get_neig();

		for (int i = 0; i < 4; i++)
		{
			if (!neighbors[i]) {
				k = 0;
				break;
			}
			if (neighbors[i]->GetType() == PASSAGE) {
				k++;
				n = i;
			}
		}

		if (k == 1)
		{
			int wallbreak = 0;
			if (n == W) {
				if (neighbors[NE]->GetType() == PASSAGE ||
					neighbors[SE]->GetType() == PASSAGE)
					wallbreak = 1;
			}
			else if (n == E) {
				if (neighbors[NW]->GetType() == PASSAGE ||
					neighbors[SW]->GetType() == PASSAGE)
					wallbreak = 1;
			}
			else if (n == S) {
				if (neighbors[NE]->GetType() == PASSAGE ||
					neighbors[NW]->GetType() == PASSAGE)
					wallbreak = 1;
			}
			else {
				if (neighbors[SE]->GetType() == PASSAGE ||
					neighbors[SW]->GetType() == PASSAGE)
					wallbreak = 1;
			}

			if (!wallbreak) {
				wall->MakePassage();
				for (int i = 0; i < 4; i++) {
					Cell* cur = neighbors[i];
					if (!cur) continue;
					if (cur->GetType() == WALL) {
						if (std::find_if(walls.begin(), walls.end(), [cur](Cell* c1) {
							return cur == c1;
						}) == walls.end()) {
							walls.push_back(cur);
						}
					}
				}
			}
		}
		walls.erase(it);
	}
}

void CMaze::GeneratePrim()
{
	_GeneratePrim(&m_pMaze[rand() % m_rows][rand() % m_cols]);
}


void CMaze::GenerateDFS()
{

}

void CMaze::Draw(HDC hdc)
{
	HBRUSH hBrushWhite, hBrushBlack, hGreenBlack, hBrush;
	hBrushWhite = CreateSolidBrush(RGB(255, 255, 255));
	hBrushBlack = CreateSolidBrush(RGB(0, 0, 0));
	hGreenBlack = CreateSolidBrush(RGB(0, 255, 0));

	for (int i = 0; i < m_rows; i++) {
		for (int j = 0; j < m_cols; j++) {
			RECT rc;
			rc.left = i * CELL_XL;
			rc.right = rc.left + CELL_XL;
			rc.top = j * CELL_YL;
			rc.bottom = rc.top + CELL_YL;
			if (m_pMaze[i][j].IsCorner()) {
				FillRect(hdc, &rc, hGreenBlack);
			}
			else if (m_pMaze[i][j].GetType() == PASSAGE) {
				FillRect(hdc, &rc, hBrushWhite);
			}
			else {
				hBrush = CreateSolidBrush(m_pMaze[i][j].color);
				FillRect(hdc, &rc, hBrush);
				DeleteObject(hBrush);
			}
		}
	}
	DeleteObject(hGreenBlack);
	DeleteObject(hBrushBlack);
	DeleteObject(hBrushWhite);
}

const char bp_begin[] =
"<?xml version=\"1.0\"?>\n"
"<Definitions xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">\n"
"  <ShipBlueprints>\n"
"    <ShipBlueprint xsi:type=\"MyObjectBuilder_ShipBlueprintDefinition\">\n"
"      <Id Type=\"MyObjectBuilder_ShipBlueprintDefinition\" Subtype=\"Generated Stuff\" />\n"
"      <DisplayName>###</DisplayName>\n"
"      <CubeGrids>\n"
"        <CubeGrid>\n"
"          <SubtypeName />\n"
"          <EntityId>100975725513175282</EntityId>\n"
"          <PersistentFlags>CastShadows InScene</PersistentFlags>\n"
"          <PositionAndOrientation>\n"
"            <Position x=\"0\" y=\"0\" z=\"0\" />\n"
"		     <Forward x=\"-0.9809575\" y=\"0.15086703\" z=\"0.122317448\" />\n"
"		     <Up x=\"0.122376107\" y=\"-0.008931095\" z=\"0.9924436\" />\n"
"	         <Orientation>\n"
"		       <X>0.5661422</X>\n"
"			   <Y>0.490552455</Y>\n"
"			   <Z>0.428892225</Z>\n"
"			   <W>0.504869044</W>\n"
"	         </Orientation>\n"
"		   </PositionAndOrientation>\n"
"          <GridSizeEnum>Large</GridSizeEnum>\n"
"		   <CubeBlocks>\n";

const char bp_end[] =
"		   </CubeBlocks>\n"
"		   <DisplayName>Generated Stuff</DisplayName>\n"
"		   <DestructibleBlocks>true</DestructibleBlocks>\n"
"		   <CreatePhysics>false</CreatePhysics>\n"
"		   <EnableSmallToLargeConnections>false</EnableSmallToLargeConnections>\n"
"		   <IsRespawnGrid>false</IsRespawnGrid>\n"
"	       <LocalCoordSys>0</LocalCoordSys>\n"
"	     </CubeGrid>\n"
"      </CubeGrids>\n"
"	   <WorkshopId>0</WorkshopId>\n"
"	   <OwnerSteamId>0</OwnerSteamId>\n"
"      <Points>0</Points>\n"
"    </ShipBlueprint>\n"
"  </ShipBlueprints>\n"
"</Definitions>";


void CMaze::CreateBlueprint(std::ofstream& ofs)
{
	ofs << bp_begin;
	std::stack<SimpleCell*> corners;

	for (int i = 0; i < m_rows; i++) 
	{
		for (int j = 0; j < m_cols; j++) 
		{
			if (m_pMaze[i][j].IsCorner())
				corners.push(&m_pMaze[i][j]);
		}
	}


	while(!corners.empty())
	{
		SimpleCell* cur = corners.top();
		corners.pop();
		SimpleCell** neig = (SimpleCell**)cur->get_neig();

		int x, y;
		x = cur->x;
		y = cur->y;

		ofs << Block(x, y, 0);
		ofs << Block(x, y, 1);
		ofs << Block(x, y, 2);

		for (int i = 0; i < 4; i++)
		{
			std::vector<SimpleCell*> glass;
			SimpleCell* next = neig[i];

			ORIENTATION or = (ORIENTATION)i;
			if (next->GetType() != WALL)
				continue;
			if (next->IsCorner())
				continue;
			if (!next->x || !next->y || next->x == m_rows - 1 || next->y == m_cols - 1) 
				continue;
			if (next->way == 1)
				continue;
			do {
				next->way = 1;
				glass.push_back(next);
				next = (SimpleCell*)next->get_neig()[i];
				if (!next->x || !next->y || next->x == m_rows - 1 || next->y == m_cols - 1)
					break;
			}
			while (!next->IsCorner());

			for (auto& g : glass) {
				g->color = RGB(255, 0, 0);
			}

			int gl_size = glass.size();

			if (gl_size == 4) {
				x = glass[gl_size - 4]->x;
				y = glass[gl_size - 4]->y;
				x = (or == RIGHT) ? x - 1 : x;
				y = (or == UP) ? y - 1 : y;
				ofs << Window2x3(x, y, 0, i < 2 ? ORIENTATION::UP : ORIENTATION::LEFT);

				x = glass[gl_size - 2]->x;
				y = glass[gl_size - 2]->y;
				x = (or == RIGHT) ? x - 1 : x;
				y = (or == UP) ? y - 1 : y;
				ofs << Window2x3(x, y, 0, i < 2 ? ORIENTATION::UP : ORIENTATION::LEFT);
				continue;
			}

			if (gl_size >= 3)
			for (int g = 0; g < gl_size; g += 3)
			{
				if (g + 2 >= gl_size)
					break;
				x = glass[g]->x;
				y = glass[g]->y;

				x = (or == RIGHT) ? x - 2 : x;
				y = (or == UP) ? y - 2 : y;

				ofs << Window3x3(x, y, 0, i < 2 ? ORIENTATION::UP : ORIENTATION::RIGHT);
/*
				ofs << Window1x1(glass[g]->x, glass[g]->y, 0, i < 2 ? ORIENTATION::UP : ORIENTATION::RIGHT);
				ofs << Window1x1(glass[g + 1]->x, glass[g + 1]->y, 0, i < 2 ? ORIENTATION::UP : ORIENTATION::RIGHT);
				ofs << Window1x1(glass[g + 2]->x, glass[g + 2]->y, 0, i < 2 ? ORIENTATION::UP : ORIENTATION::RIGHT);
				
				ofs << Window1x1(glass[g]->x, glass[g]->y, 1, i < 2 ? ORIENTATION::UP : ORIENTATION::RIGHT);
				ofs << Window1x1(glass[g + 1]->x, glass[g + 1]->y, 1, i < 2 ? ORIENTATION::UP : ORIENTATION::RIGHT);
				ofs << Window1x1(glass[g + 2]->x, glass[g + 2]->y, 1, i < 2 ? ORIENTATION::UP : ORIENTATION::RIGHT);
				
				ofs << Window1x1(glass[g]->x, glass[g]->y, 2, i < 2 ? ORIENTATION::UP : ORIENTATION::RIGHT);
				ofs << Window1x1(glass[g + 1]->x, glass[g + 1]->y, 2, i < 2 ? ORIENTATION::UP : ORIENTATION::RIGHT);
				ofs << Window1x1(glass[g + 2]->x, glass[g + 2]->y, 2, i < 2 ? ORIENTATION::UP : ORIENTATION::RIGHT);
	*/
/*
				ofs << Block(glass[g]->x, glass[g]->y, 0);
				ofs << Block(glass[g + 1]->x, glass[g + 1]->y, 0);
				ofs << Block(glass[g + 2]->x, glass[g + 2]->y, 0);

				ofs << Block(glass[g]->x, glass[g]->y, 1);
				ofs << Block(glass[g + 1]->x, glass[g + 1]->y, 1);
				ofs << Block(glass[g + 2]->x, glass[g + 2]->y, 1);

				ofs << Block(glass[g]->x, glass[g]->y, 2);
				ofs << Block(glass[g + 1]->x, glass[g + 1]->y, 2);
				ofs << Block(glass[g + 2]->x, glass[g + 2]->y, 2);
*/
			}
			if (gl_size == 2 || gl_size % 3 == 2)
			{
				x = glass[gl_size - 2]->x;
				y = glass[gl_size - 2]->y;
		/*		ofs << Window1x1(x, y, 0, i < 2 ? ORIENTATION::UP : ORIENTATION::RIGHT);
				ofs << Window1x1(x, y, 1, i < 2 ? ORIENTATION::UP : ORIENTATION::RIGHT);
				ofs << Window1x1(x, y, 2, i < 2 ? ORIENTATION::UP : ORIENTATION::RIGHT);
				
				x = glass[gl_size - 1]->x;
				y = glass[gl_size - 1]->y;
				
				ofs << Window1x1(x, y, 0, i < 2 ? ORIENTATION::UP : ORIENTATION::RIGHT);
				ofs << Window1x1(x, y, 1, i < 2 ? ORIENTATION::UP : ORIENTATION::RIGHT);
				ofs << Window1x1(x, y, 2, i < 2 ? ORIENTATION::UP : ORIENTATION::RIGHT);
		*/
				x = (or == RIGHT) ? x - 1 : x;
				y = (or == UP) ? y - 1 : y;
				ofs << Window2x3(x, y, 0, i < 2 ? ORIENTATION::UP : ORIENTATION::LEFT);
			}
			if (gl_size == 1 || gl_size % 3 == 1)
			{
				x = glass[gl_size - 1]->x;
				y = glass[gl_size - 1]->y;
				ofs << Window1x1(x, y, 0, i < 2 ? ORIENTATION::UP : ORIENTATION::RIGHT);
				ofs << Window1x1(x, y, 1, i < 2 ? ORIENTATION::UP : ORIENTATION::RIGHT);
				ofs << Window1x1(x, y, 2, i < 2 ? ORIENTATION::UP : ORIENTATION::RIGHT);
			}
		}
	}

	for (int i = 0; i < m_rows; i++)
	{
		ofs << Block(i, 0, 0);
		ofs << Block(i, 0, 1);
		ofs << Block(i, 0, 2);

		ofs << Block(i, m_cols - 1, 0);
		ofs << Block(i, m_cols - 1, 1);
		ofs << Block(i, m_cols - 1, 2);
	}
	for (int i = 0; i < m_cols; i++)
	{
		ofs << Block(0, i, 0);
		ofs << Block(0, i, 1);
		ofs << Block(0, i, 2);

		ofs << Block(m_rows - 1, i, 0);
		ofs << Block(m_rows - 1, i, 1);
		ofs << Block(m_rows - 1, i, 2);
	}
	ofs << bp_end;
}

// CSuperMaze

CSuperMaze::CSuperMaze(int rows, int cols, int cell_size) :
	CMaze(rows, cols)
{
	_m = m_rows / cell_size;
	_n = m_cols / cell_size;

	m_supMaze = new SuperCell*[_m];
	for (int i = 0; i < _m; i++) {
		m_supMaze[i] = new SuperCell[_n];
	}

	for (int i = 0, ix = 0; i < m_rows; i += cell_size, ix++) {
		for (int j = 0, iy = 0; j < m_cols; j += cell_size, iy++) {
			for (int k = 0; k < cell_size; k++) {
				for (int l = 0; l < cell_size; l++) {
					m_supMaze[ix][iy].AddCell(&m_pMaze[i + k][j + l]);
				}
			}
		}
	}

	for (int i = 0; i < _m; i++)
	{
		for (int j = 0; j < _n; j++)
		{
			SuperCell** neighbors = (SuperCell**)m_supMaze[i][j].get_neig();

			if (i > 0) {
				neighbors[N] = &m_supMaze[i - 1][j];
				if (j > 0)
					neighbors[NW] = &m_supMaze[i - 1][j - 1];
				if (j < _n - 1)
					neighbors[NE] = &m_supMaze[i - 1][j + 1];
			}
			if (i < _m - 1) {
				neighbors[S] = &m_supMaze[i + 1][j];
				if (j > 0)
					neighbors[SW] = &m_supMaze[i + 1][j - 1];
				if (j < _n - 1)
					neighbors[SE] = &m_supMaze[i + 1][j + 1];
			}

			if (j > 0)
				neighbors[W] = &m_supMaze[i][j - 1];

			if (j < _n - 1)
				neighbors[E] = &m_supMaze[i][j + 1];
		}
	}
}

CSuperMaze::~CSuperMaze()
{
	if (!m_supMaze)
		return;
	for (int i = 0; i < _m; i++) {
		delete[]m_supMaze[i];
	}
	delete[]m_supMaze;
}

void CSuperMaze::GeneratePrim()
{
	_GeneratePrim(&m_supMaze[rand() % _m][rand() % _n]);

	for (int i = 0; i < m_rows; i++)
	{
		for (int j = 0; j < m_cols; j++)
		{
			if (m_pMaze[i][j].GetType() == WALL)
			{
				int k = 0;
				int n;
				Cell** neighbors = m_pMaze[i][j].get_neig();

				for (int i = 0; i < 4; i++)
				{
					if (!neighbors[i]) {
						k = 0;
						break;
					}
					if (neighbors[i]->GetType() == PASSAGE) {
						k++;
						n = i;
					}
				}
				if (k == 1)
				{
					int wallbreak = 0;
					if (n == W) {
						if (neighbors[NE]->GetType() == PASSAGE ||
							neighbors[SE]->GetType() == PASSAGE)
							wallbreak = 1;
					}
					else if (n == E) {
						if (neighbors[NW]->GetType() == PASSAGE ||
							neighbors[SW]->GetType() == PASSAGE)
							wallbreak = 1;
					}
					else if (n == S) {
						if (neighbors[NE]->GetType() == PASSAGE ||
							neighbors[NW]->GetType() == PASSAGE)
							wallbreak = 1;
					}
					else {
						if (neighbors[SE]->GetType() == PASSAGE ||
							neighbors[SW]->GetType() == PASSAGE)
							wallbreak = 1;
					}
					if (!wallbreak)
						_GeneratePrim(&m_pMaze[i][j]);
				}
			}
		}
	}

}