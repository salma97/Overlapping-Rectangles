#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <utility>
#include <algorithm>
#include <string>
#include <filesystem>
#include <chrono>
using namespace std::chrono;
using namespace std;

class point {

	long x;
	long y;

public:

	point()
	{
		x = 0;
		y = 0;
	}

	point(long l, long r)
	{
		x = l;
		y = r;
	}

	long getx()
	{
		return x;
	}

	long gety()
	{
		return y;
	}

	void setx(long i)
	{
		x = i;
	}

	void sety(long i)
	{
		y = i;
	}
	friend class rectangle;
};

class rectangle {

	point LowerLeft, UpperRight;

public:

	rectangle()
	{
		LowerLeft.setx(0);
		LowerLeft.sety(0);
		UpperRight.setx(0);
		UpperRight.sety(0);
	}

	rectangle(point x, point y)
	{
		LowerLeft.setx(x.getx());
		LowerLeft.sety(x.gety());
		UpperRight.setx(y.getx());
		UpperRight.sety(y.gety());
	}

	point get_LowerLeft(void)
	{
		return LowerLeft;
	}

	point get_UpperRight(void)
	{
		return UpperRight;
	}

	bool overlap(rectangle x)
	{
		if (LowerLeft.getx() >= x.get_UpperRight().getx() || x.get_LowerLeft().getx() >= UpperRight.getx())
		{
			return false;
		}
		if (LowerLeft.gety() >= x.get_UpperRight().gety() || x.get_LowerLeft().gety() >= UpperRight.gety())
		{
			return false;
		}

		return true;
	}
};

bool check_set(vector<rectangle> temp, rectangle m)
{
	for (int k = 0; k < temp.size(); k++)
	{
		if (m.overlap(temp[k]))
		{
			return false;
		}
	}
	return true;
}

int divide_into_sets(vector<pair<rectangle, bool>> rectangles)
{
	int count = 1;
	int i = 0;
	for (i = 0; i < rectangles.size() - 1; i++)
	{
		ofstream file;
		vector <rectangle> temp;

		if (rectangles[i].second == false)
		{
			//create a new set/group
			file.open("group_" + to_string(count) + ".txt");
			file << rectangles[i].first.get_LowerLeft().getx() << " " << rectangles[i].first.get_LowerLeft().gety() << " " << rectangles[i].first.get_UpperRight().getx() << " " << rectangles[i].first.get_UpperRight().gety() << endl;
			count++;
			rectangles[i].second = true;
			temp.push_back(rectangles[i].first);

			for (int j = i + 1; j < rectangles.size(); j++)
			{
				if (rectangles[j].second == false)
				{
					//check if it's overlapping with any of the rectangles ALREADY in the set
					if (check_set(temp, rectangles[j].first))
					{
						rectangles[j].second = true;
						file << rectangles[j].first.get_LowerLeft().getx() << " " << rectangles[j].first.get_LowerLeft().gety() << " " << rectangles[j].first.get_UpperRight().getx() << " " << rectangles[j].first.get_UpperRight().gety() << endl;
						temp.push_back(rectangles[j].first);
					}
				}

			}
		}
		file.close();
	}

	//check the last rectangle in the file
	if (rectangles[rectangles.size() - 1].second == false)
	{
		ofstream file;
		file.open("group_" + to_string(count) + ".txt");
		file << rectangles[i].first.get_LowerLeft().getx() << " " << rectangles[i].first.get_LowerLeft().gety() << " " << rectangles[i].first.get_UpperRight().getx() << " " << rectangles[i].first.get_UpperRight().gety() << endl;
		rectangles[i].second = true;
		file.close();
	}

	return count-1;
}

int main()
{
	char done = 'C';

	do {

		//x1,y1,x2,y2 are the points given in one line of the file
		//Vector rectangles has a pair, pair.first stores the rectangle, pair.second indicated whether it belongs to a set or not
		long x1, y1, x2, y2;
		vector <pair<rectangle, bool>> rectangles;
		string file_path;

		cout << "Enter the correct and full file path (including .txt extension):" << endl;
		cin >> file_path;

		//if .txt extension isn't present
		/*if (file_path.find(".txt") == NULL)
		{
			cout << "Path not correct, please ensert .txt extension file" << endl;
			continue;
		}*/

		ifstream file(file_path);

		//if the file can't be found
		if (file.fail())
		{
			cout << "Can't find the file, please insert the correct path" << endl;
			continue;
		}

		auto start = high_resolution_clock::now();

		//Adding the given Rectangles in a Vector
		while (file >> x1 >> y1 >> x2 >> y2)
		{
			point lower_left(x1, y1);
			point upper_right(x2, y2);

			rectangle rec(lower_left, upper_right);
			rectangles.emplace_back(rec, false);
		}

		if (!file.eof())
		{
			cout << "Wrong file format!" << endl;
			continue;
		}

		//close the file
		file.close();

		cout << "Number of Inputs = " << rectangles.size() << endl;

		int count = divide_into_sets(rectangles);

		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(stop - start);
		cout << duration.count() << "millisecond" << endl;

		cout << "Number of Output groups = " << count << endl;
		cout << "To continue press C, to exit press any character:" << endl;
		cin >> done;

	} while (done == 'C' || done == 'c');

	return 0;
}