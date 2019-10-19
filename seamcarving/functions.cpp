#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include "functions.h"

using namespace std;

Pixel** createImage(int width, int height) {
  cout << "Start createImage... " << endl;
  
  // Create a one dimensional array on the heap of pointers to Pixels 
  //    that has width elements (i.e. the number of columns)
  Pixel** image = new Pixel*[width];
  
  bool fail = false;
  
  for (int i=0; i < width; ++i) { // loop through each column
    // assign that column to a one dimensional array on the heap of Pixels
    //  that has height elements (i.e. the number of rows)
    image[i] = new Pixel[height];
    
    if (image[i] == nullptr) { // failed to allocate
      fail = true;
    }
  }
  
  if (fail) { // if any allocation fails, clean up and avoid memory leak
    // deallocate any arrays created in for loop
    for (int i=0; i < width; ++i) {
      delete [] image[i]; // deleting nullptr is not a problem
    }
    delete [] image; // dlete array of pointers
    return nullptr;
  }
  
  // initialize cells
  //cout << "Initializing cells..." << endl;
  for (int row=0; row<height; ++row) {
    for (int col=0; col<width; ++col) {
      //cout << "(" << col << ", " << row << ")" << endl;
      image[col][row] = { 0, 0, 0 };
    }
  }
  cout << "End createImage... " << endl;
  return image;
}

void deleteImage(Pixel** image, int width) {
  cout << "Start deleteImage..." << endl;
  // avoid memory leak by deleting the array
  for (int i=0; i<width; ++i) {
    delete [] image[i]; // delete each individual array placed on the heap
  }
  delete [] image;
  image = nullptr;
}

// implement for part 1

int* createSeam(int length) {
	int* seam = new int[length];
	return seam;
}

void deleteSeam(int* seam) {
	delete[] seam;
}

bool loadImage(string filename, Pixel** image, int width, int height) {
	fstream ifs;
	ifs.open(filename);
	if (!(ifs.is_open()))
	{
		cout << "Error: failed to open input file - " << filename << endl;
		return false;
	}
	string type;
	ifs >> type;
	if (ifs.fail())
	{
		cout << "Error: read non-integer value" << endl;
		return false;
	}
	if (type != "P3")
	{
		cout << "Error: type is " << type << " instead of P3" << endl;
		return false;
	}
	int fileWidth;
	ifs >> fileWidth;
	if (ifs.fail())
	{
		cout << "Error: read non-integer value" << endl;
		return false;
	}
	if (fileWidth != width)
	{
		cout << "Error: input width (" << width << ") does not match value in file (" << fileWidth << ")" << endl;
		return false;
	}
	int fileHeight;
	ifs >> fileHeight;
	if (ifs.fail())
	{
		cout << "Error: read non-integer value" << endl;
		return false;
	}
	if (fileHeight != height)
	{
		cout << "Error: input height (" << height << ") does not match value in file (" << fileHeight << ")" << endl;
		return false;
	}
	int colorMax;
	ifs >> colorMax;
	if (ifs.fail())
	{
		cout << "Error: read non-integer value" << endl;
		return false;
	}
	for (int j = 0; j < height; j++) 
	{
		for (int i = 0; i < width; i++) 
		{
			int r;
			int g;
			int b;
			if (ifs.eof())
			{
				cout << "Error: not enough color values" << endl;
				return false;
			}
			ifs >> r;
			if (ifs.eof())
			{
				cout << "Error: not enough color values" << endl;
				return false;
			}
			ifs >> g;
			if (ifs.eof())
			{
				cout << "Error: not enough color values" << endl;
				return false;
			}
			ifs >> b;
			if (ifs.eof())
			{
				cout << "Error: not enough color values" << endl;
				return false;
			}
			image[i][j].r = r;
			image[i][j].g = g;
			image[i][j].b = b;
			if (ifs.fail())
			{
				cout << "Error: read non-integer value" << endl;
				return false;
			}

			if (image[i][j].r < 0 || image[i][j].r > 255)
			{
				cout << "Error: invalid color value " << image[i][j].r << endl;
				return false;
			}
			if (image[i][j].g < 0 || image[i][j].g > 255)
			{
				cout << "Error: invalid color value " << image[i][j].g << endl;
				return false;
			}
			if (image[i][j].b < 0 || image[i][j].b > 255)
			{
				cout << "Error: invalid color value " << image[i][j].b << endl;
				return false;
			}
		}
	}
	return true;
}

bool outputImage(string filename, Pixel** image, int width, int height) {
	ofstream ofs(filename);
//	ofs.open(filename);
	if (!(ofs.is_open())) 
	{
		cout << "Error: failed to open file- " << filename << endl;
		return false;
	}
	ofs << "P3" << endl << width << " " << height << endl;
	ofs << 255 << endl;
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			ofs << image[i][j].r << " ";
			ofs << image[i][j].g << " ";
			ofs << image[i][j].b << " ";
		}
		ofs << endl;
	}


	return true;
}

int energy(Pixel** image, int x, int y, int width, int height) { 
	
	int sumX;
	int sumY;

	if (x == 0 || y == 0)
	{
		if (x == 0 && y != 0)
		{
			int Rx = abs(image[(x + 1) % width][y % height].r - image[(width - 1)][y % height].r);
			int Gx = abs(image[(x + 1) % width][y % height].g - image[(width - 1)][y % height].g);
			int Bx = abs(image[(x + 1) % width][y % height].b - image[(width - 1)][y % height].b);
			sumX = pow(Rx, 2) + pow(Gx, 2) + pow(Bx, 2);


			int Ry = abs(image[x % width][(y + 1) % height].r - image[x % width][(y - 1) % height].r);
			int Gy = abs(image[x % width][(y + 1) % height].g - image[x % width][(y - 1) % height].g);
			int By = abs(image[x % width][(y + 1) % height].b - image[x % width][(y - 1) % height].b);
			sumY = pow(Ry, 2) + pow(Gy, 2) + pow(By, 2);
		}
		else if (x != 0 && y == 0)
		{
			int Rx = abs(image[(x + 1) % width][y % height].r - image[(x - 1) % width][y % height].r);
			int Gx = abs(image[(x + 1) % width][y % height].g - image[(x - 1) % width][y % height].g);
			int Bx = abs(image[(x + 1) % width][y % height].b - image[(x - 1) % width][y % height].b);
			sumX = pow(Rx, 2) + pow(Gx, 2) + pow(Bx, 2);


			int Ry = abs(image[x % width][(y + 1) % height].r - image[x % width][(height - 1)].r);
			int Gy = abs(image[x % width][(y + 1) % height].g - image[x % width][(height - 1)].g);
			int By = abs(image[x % width][(y + 1) % height].b - image[x % width][(height - 1)].b);
			sumY = pow(Ry, 2) + pow(Gy, 2) + pow(By, 2);
		}
		else
		{
			int Rx = abs(image[(x + 1) % width][y % height].r - image[(width - 1)][y % height].r);
			int Gx = abs(image[(x + 1) % width][y % height].g - image[(width - 1)][y % height].g);
			int Bx = abs(image[(x + 1) % width][y % height].b - image[(width - 1)][y % height].b);
			sumX = pow(Rx, 2) + pow(Gx, 2) + pow(Bx, 2);


			int Ry = abs(image[x % width][(y + 1) % height].r - image[x % width][(height - 1)].r);
			int Gy = abs(image[x % width][(y + 1) % height].g - image[x % width][(height - 1)].g);
			int By = abs(image[x % width][(y + 1) % height].b - image[x % width][(height - 1)].b);
			sumY = pow(Ry, 2) + pow(Gy, 2) + pow(By, 2);
		}
	}
	else
	{
		
		int Rx = abs(image[(x + 1) % width][y % height].r - image[(x - 1) % width][y % height].r);
		int Gx = abs(image[(x + 1) % width][y % height].g - image[(x - 1) % width][y % height].g);
		int Bx = abs(image[(x + 1) % width][y % height].b - image[(x - 1) % width][y % height].b);
		sumX = pow(Rx, 2) + pow(Gx, 2) + pow(Bx, 2);

		
		int Ry = abs(image[x % width][(y + 1) % height].r - image[x % width][(y - 1) % height].r);
		int Gy = abs(image[x % width][(y + 1) % height].g - image[x % width][(y - 1) % height].g);
		int By = abs(image[x % width][(y + 1) % height].b - image[x % width][(y - 1) % height].b);
		sumY = pow(Ry, 2) + pow(Gy, 2) + pow(By, 2);
	}
//	cout << "Energy " << sumX + sumY << endl;//test
	return (sumX + sumY);
}

// implement for part 2
int loadVerticalSeam(Pixel** image, int start_col, int width, int height, int* seam) {
	long totalEnergy = 0;
	int minEnergy = 2147483647;
	int minEnergyPos = 0;
	seam[0] = start_col;

	for (int i = 1; i < height; i++)
	{
		if (seam[i - 1] != 0)
			if (energy(image, seam[i - 1] - 1, i, width, height) <= minEnergy)
			{
				minEnergy = energy(image, seam[i - 1] - 1, i, width, height);
				minEnergyPos = seam[i - 1] - 1;
			}
		if (seam[i - 1] != width - 1)
			if (energy(image, seam[i - 1] + 1, i, width, height) <= minEnergy)
			{
				minEnergy = energy(image, seam[i - 1] + 1, i, width, height);
				minEnergyPos = seam[i - 1] + 1;
			}
		if (energy(image, seam[i - 1], i, width, height) <= minEnergy)
		{
			minEnergy = energy(image, seam[i - 1], i, width, height);
			minEnergyPos = seam[i - 1];
		}
		
		totalEnergy += minEnergy;
//		cout << "PIXEL ENERGY " << minEnergy << endl;//test
//		cout << "SEAM ENERGY " << totalEnergy << endl;//test
		seam[i] = minEnergyPos;
		minEnergy = 2147483647;
	}
	return totalEnergy;
}

int loadHorizontalSeam(Pixel** image, int start_row, int width, int height, int* seam) {
	long totalEnergy = 0;
	int minEnergy = 2147483647;
	int minEnergyPos = 0;
	seam[0] = start_row;

	for (int i = 1; i < width; i++)
	{
		if (seam[i - 1] != 0)
			if (energy(image, i, seam[i - 1] - 1, width, height) < minEnergy)
			{
				minEnergy = energy(image, i, seam[i - 1] - 1, width, height);
				minEnergyPos = seam[i - 1] - 1;
			}
		if (energy(image, i, seam[i - 1], width, height) <= minEnergy)
		{
			minEnergy = energy(image, i, seam[i - 1], width, height);
			minEnergyPos = seam[i - 1];
		}
		if (seam[i - 1] != height - 1)
			if (energy(image, i, seam[i - 1] + 1, width, height) < minEnergy)
			{
				minEnergy = energy(image, i, seam[i - 1] + 1, width, height);
				minEnergyPos = seam[i - 1] + 1;
			}
		totalEnergy += minEnergy;
//		cout << "PIXEL ENERGY " << minEnergy << endl;//test
//		cout << "SEAM ENERGY " << totalEnergy << endl;//test
		seam[i] = minEnergyPos;
		minEnergy = 2147483647;
	}
	return totalEnergy;
}

int* findMinVerticalSeam(Pixel** image, int width, int height) {
	int minStartCol = 0;
	int minEnergy = 2147483647;
	int* seam = createSeam(height);
	int currSeam;
	for (int i = 0; i < width; i++)
	{
		currSeam = loadVerticalSeam(image, i, width, height, seam);
		if (currSeam < minEnergy)
		{
			minEnergy = currSeam;
			minStartCol = i;
		}
	}
	loadVerticalSeam(image, minStartCol, width, height, seam);
	cout << "Found Min Vertical Seam" << endl;
	return seam;
}

int* findMinHorizontalSeam(Pixel** image, int width, int height) {
	int minStartRow = 0;
	int minEnergy = 2147483647;
	int* seam = createSeam(width);
	int currSeam;
	for (int i = 0; i < height; i++)
	{
		currSeam = loadHorizontalSeam(image, i, width, height, seam);
		if (currSeam < minEnergy)
		{
			minEnergy = currSeam;
			minStartRow = i;
		}
	}
	loadHorizontalSeam(image, minStartRow, width, height, seam);
	cout << "Found Min Horizontal Seam" << endl;
	return seam;
}

void removeVerticalSeam(Pixel** image, int width, int height, int* verticalSeam) {
	cout << "Removing Vertical Seam" << endl;
	for (int j = 0; j < height; j++)
		for (int i = verticalSeam[j]; i < width - 1; i++)
			image[j][i] = image[j][i + 1];
}

void removeHorizontalSeam(Pixel** image, int width, int height, int* horizontalSeam) {
	cout << "Removing Horizontal Seam" << endl;
	for (int i = 0; i < width; i++)
		for (int j = horizontalSeam[i]; j < height - 1; j++)
			image[j][i] = image[j + 1][i];
}