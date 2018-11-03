#include "level.h"
#include <iostream>


#define path "./images/"
#define impath "./images/"

using namespace std;
level::level()
{
}

level::level(string levelname)
{
	tmxtag file = tmxtag(string(path) + levelname,0);

	

	tileheight = stoi(file.head("tileheight"));
	tilewidth = stoi(file.head("tilewidth"));
	height = stoi(file.head("height"))*tileheight;
	width = stoi(file.head("width"))*tilewidth;
	
	bg = plate(width, height);
	fg = plate(width, height);


	for (int layers = 0; layers < file.tagcount(); layers++) {
		tmxtag currentlayer = file.get(layers);

		//tilesheets
		if (currentlayer.head("type").compare("tileset")==0) {
			this->handletilesheets(currentlayer);

		}
		//plates / layers
		else if (currentlayer.head("type").compare("layer")==0) {
			this->handleplates(currentlayer);
		}
	}





}


void level::handletilesheets(tmxtag currentlayer)
{

	if (currentlayer.head("type").compare("tileset") == 0) {
		tmxtag img = currentlayer.get(0);

		int tileheight = stoi(currentlayer.head("tileheight"));
		int tilewidth = stoi(currentlayer.head("tilewidth"));
		int count = stoi(currentlayer.head("tilecount"));
		int horizontalwidth = stoi(currentlayer.head("columns"));

		string imgname = img.head("source");
		tilesheets.push_back(sheet(string(string(impath) + imgname).c_str(), 0, tilewidth, tileheight,horizontalwidth, count / horizontalwidth));

	}
}

void level::handleplates(tmxtag currentlayer)
{
	int w = stoi(currentlayer.head("width"));
	int h = stoi(currentlayer.head("height"));
	string initdata = currentlayer.get(0).getData();
	int x = 0;
	int y = 0;
	cout << initdata << endl;
	if (currentlayer.head("name").find("bg") != string::npos) {
		for (int end = 0; end < initdata.size(); end++) {
			if (initdata[end] == ',' || initdata[end] == '\n') {
				x += 1;
				int tile = stoi(initdata.substr(0, end));

				this->addbg(tile,x,y);


				break;
			}

		}

		for (int i = 0; i < initdata.size(); i++) {
			x += 1;
			if (x > w) {
				y += 1;
				x = 0;
				cout << endl;
			}
			int start;
			int end;
			for (start = i; start < initdata.size(); start++) {
				if (initdata[start] == ',' || initdata[start] == '\n') {
					break;
				}

			}
			for (end = start + 1; end < initdata.size(); end++) {
				if (initdata[end] == ',' || initdata[end] == '\n') {
					break;
				}

			}
			i = end - 1;
			if (0!= end - start - 1) {
				int tile = stoi(initdata.substr(start + 1, end - start - 1));

				this->addbg(tile, x, y);
			}
		}
		//decode csv



	}
	else if (currentlayer.head("name").find("fg") != string::npos) {

	}
}

bool level::addbg(int tile, int x, int y)
{
	if (tile > 0) {
		tile -= 1;
		int currentsheet = 0;
		while (currentsheet < tilesheets.size()) {
			if (tile < tilesheets[currentsheet].getcount()) {
				bg.addtile(tilesheets[currentsheet].getar(tile), x*tilewidth, y*tileheight, tilesheets[currentsheet].getw(), tilesheets[currentsheet].geth());
				return true;
			}
			tile -= tilesheets[currentsheet].getcount();
			currentsheet += 1;
		}
		bg.addtile(tilesheets[0].getar(tile), x * 16, y * 16, 16, 16);
	}
	return false;
}
bool level::addfg(int tile, int x, int y)
{
	if (tile > 0) {
		tile -= 1;
		int currentsheet = 0;
		while (currentsheet < tilesheets.size()) {
			if (tile < tilesheets[currentsheet].getcount()) {
				fg.addtile(tilesheets[currentsheet].getar(tile), x*tilewidth, y*tileheight, tilesheets[currentsheet].getw(), tilesheets[currentsheet].geth());
				return true;
			}
			tile -= tilesheets[currentsheet].getcount();
			currentsheet += 1;
		}
	}
	return false;
}

int level::getwidth()
{
	return width;
}

int level::getheight()
{
	return height;
}

plate level::getbg()
{
	return bg;
}
