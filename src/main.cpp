#include <iostream>
#include <fstream>

import LibUtility;
import UJImage;

using namespace std;

void compare(const UJImage& objImageLHS, const UJImage& objImageRHS) {
    if (objImageLHS == objImageRHS) {
        cout << "The images are similar" << endl;
    } else {
        cout << "The images are NOT similar" << endl;
    }
}

//export type
enum EXPORT_MODE{PPM, PBM, PGM};

//loadImage: Reads a PPM file and saves the pixels
void loadImage(UJImage& obj, string filePath){
	//create a fileInputStream
	ifstream readFile(filePath);
	
	if(readFile.fail()){
		cerr << "Couldnt open file: " << filePath <<  endl;
		exit(-1);
	}
	
	//read the magic number
	string magicNumber = "";
	readFile >> magicNumber;
	cout << "Magic number: " << magicNumber << endl;
	
	if(magicNumber != "P3"){
		cerr << "Image format not supported" << endl;
		exit(-1);
	}

	//read the cols and rows
	int cols = -1;
	int rows = -1;
	
	readFile >> cols;
	readFile >> rows;
	
	//Reset rows and cols to new values and allocate memory
	obj.resetArray(rows, cols);
	
	//read all the pixel
	int tempPixelValue;
	int pixelCount = 0;
	int counter = 0;
	
	int r, g, b;
	while(readFile >> tempPixelValue){
		pixelCount++;
		if(pixelCount == 1){
			r = tempPixelValue;
		}else if(pixelCount == 2){
			g = tempPixelValue;
		}else if(pixelCount == 3){
			b = tempPixelValue;
			pixelCount = 0;
			
			//set pixel to rbg
			UJPixel readPixel = {r, g, b};
			obj[counter] = readPixel;
			//increment
			counter++;
		}
	}
	readFile.close();
}

//exportImage: Writes pixels to a ppm file
void exportImage(UJImage& obj, EXPORT_MODE modeType, string FolderPath){
	//file output stream
	//Type
	if(modeType == PPM){
		//just write to file
		ofstream writeFile(FolderPath + "/Image.ppm");
		writeFile << obj.toPPM();
		
		writeFile.close();
		
	}else if(modeType == PGM){
		//just write to file
		ofstream writeFile(FolderPath + "/Image.pgm");
		writeFile << obj.toPGM();
		
		writeFile.close();
		
	}else if(modeType == PBM){
		//just write to file
		ofstream writeFile(FolderPath + "/Image.pbm");
		writeFile << obj.toPBM();
		
		writeFile.close();
	}
}

int main() {
    //loadImage example
	UJImage loadedImage;
	string filePath = "../output/image.ppm";
	string folderPath = "../output";
	loadImage(loadedImage, filePath);
	
	//display image to screen
	cout << loadedImage;
	
	//export image example
	exportImage(loadedImage, PBM, folderPath);
	
    return SUCCESS;
}
