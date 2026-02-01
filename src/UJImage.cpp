module;

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

export module UJImage;

import LibUtility;

// Aliases
using Row = UJPixel*;
using Grid = Row*;

export class UJImage {
public:
    // Constructors
    UJImage();
    UJImage(int intRows, int intCols);
    UJImage(int intRows, int intCols, UJPixel recPixel);
    UJImage(const UJImage& objOriginal);

    ~UJImage();

    // Operator overloads
    const UJImage& operator=(const UJImage&);
    const bool operator==(const UJImage&) const;
    const UJImage& operator+(const UJImage&);
    UJPixel& operator()(int intRow, int intCol);
    UJPixel& operator[](int intIndex);
    UJImage& operator--(int);
    friend std::ostream& operator<<(std::ostream&, const UJImage&);

    // Getters
    int getHeight() const;
    int getWidth() const;
    UJPixel getPixel(int intRow, int intCol) const;

    // Setters
    void setPixel(int intRow, int intCol, UJPixel recPixel);
	void resetArray(int rows, int cols);

    // Constants
    static constexpr int DEFAULT_ROWS = 2;
    static constexpr int DEFAULT_COLS = 2;
    static constexpr UJPixel DEFAULT_PIXEL = {255, 255, 255};
    static constexpr int MAX = 256;
	
	std::string toPGM() const;
	std::string toPBM() const;
    std::string toPPM() const;

private:
    void alloc(int intRows, int intCols, UJPixel recPixel);
    void clone(const UJImage& objOriginal);
    void dealloc();
    void enforceRange(int intValue, int intMin, int intMax) const;

    Grid _image;
    int _rows;
    int _cols;
};

// ======================= IMPLEMENTATION =======================

UJImage::UJImage() : UJImage(DEFAULT_ROWS, DEFAULT_COLS) {}

UJImage::UJImage(int intRows, int intCols)
    : UJImage(intRows, intCols, DEFAULT_PIXEL) {}

UJImage::UJImage(int intRows, int intCols, UJPixel recPixel) {
    alloc(intRows, intCols, recPixel);
}

UJImage::UJImage(const UJImage& objOriginal)
    : UJImage(objOriginal._rows, objOriginal._cols) {
    clone(objOriginal);
}

UJImage::~UJImage() {
    dealloc();
}

std::string UJImage::toPGM() const{
	std::stringstream ssPGM;
    ssPGM << "P2" << std::endl
          << _cols << ' ' << _rows << std::endl
          << 255 << std::endl;
    for (int r = 0; r < _rows; r++) {
        for (int c = 0; c < _cols; c++) {
            int average = (_image[r][c].intRed + _image[r][c].intGreen + _image[r][c].intBlue) / 3;
			ssPGM << average << " ";
        }
        ssPGM << std::endl;
    }
    return ssPGM.str();
}
std::string UJImage::toPBM() const{
	std::stringstream ssPBM;
    ssPBM << "P1" << std::endl
          << _cols << ' ' << _rows << std::endl;
    for (int r = 0; r < _rows; r++) {
        for (int c = 0; c < _cols; c++) {
            if(_image[r][c].intRed == 255 && _image[r][c].intGreen == 255 && _image[r][c].intBlue == 255){
				ssPBM << 0 << " ";
			}else{
				ssPBM << 1 << " ";
			}
        }
        ssPBM << std::endl;
    }
    return ssPBM.str();
}

std::string UJImage::toPPM() const {
    std::stringstream ssPPM;
    ssPPM << "P3" << std::endl
          << _cols << ' ' << _rows << std::endl
          << 255 << std::endl;
    for (int r = 0; r < _rows; r++) {
        for (int c = 0; c < _cols; c++) {
            ssPPM << _image[r][c].intRed << ' '
                  << _image[r][c].intGreen << ' '
                  << _image[r][c].intBlue << ' ';
        }
        ssPPM << std::endl;
    }
    return ssPPM.str();
}

void UJImage::alloc(int intRows, int intCols, UJPixel recPixel) {
    _rows = intRows;
    _cols = intCols;

    _image = new Row[_rows];
    for (int r = 0; r < _rows; r++) {
        _image[r] = new UJPixel[_cols];
        for (int c = 0; c < _cols; c++) {
            _image[r][c] = recPixel;
        }
    }
}

void UJImage::clone(const UJImage& objOriginal) {
    assert(_rows == objOriginal._rows);
    assert(_cols == objOriginal._cols);
    for (int r = 0; r < _rows; r++) {
        for (int c = 0; c < _cols; c++) {
            _image[r][c] = objOriginal._image[r][c];
        }
    }
}

void UJImage::dealloc() {
    assert(_image != nullptr);
    for (int r = 0; r < _rows; r++)
        delete[] _image[r];
    delete[] _image;
}

int UJImage::getHeight() const { return _rows; }
int UJImage::getWidth() const { return _cols; }

UJPixel UJImage::getPixel(int intRow, int intCol) const {
    enforceRange(intRow, 0, _rows - 1);
    enforceRange(intCol, 0, _cols - 1);
    return _image[intRow][intCol];
}

void UJImage::setPixel(int intRow, int intCol, UJPixel recPixel) {
    enforceRange(intRow, 0, _rows - 1);
    enforceRange(intCol, 0, _cols - 1);
    enforceRange(recPixel.intRed, 0, 255);
    enforceRange(recPixel.intGreen, 0, 255);
    enforceRange(recPixel.intBlue, 0, 255);
    _image[intRow][intCol] = recPixel;
}

void UJImage::enforceRange(int intValue, int intMin, int intMax) const {
    if (intValue < intMin || intValue > intMax) {
        std::cerr << "ERROR! " << intValue << " must be within ["
                  << intMin << ", " << intMax << "]. Terminating." << std::endl;
        exit(ERROR_RANGE);
    }
}

const UJImage& UJImage::operator=(const UJImage& objRHS) {
    if (this != &objRHS) {
        dealloc();
        alloc(objRHS._rows, objRHS._cols, DEFAULT_PIXEL);
        clone(objRHS);
    }
    return *this;
}

const bool UJImage::operator==(const UJImage& objRHS) const {
    if (_rows != objRHS._rows || _cols != objRHS._cols) {
        return false;
    }
    for (int r = 0; r < _rows; r++) {
        for (int c = 0; c < _cols; c++) {
            if ((_image[r][c].intRed != objRHS._image[r][c].intRed) ||
                (_image[r][c].intGreen != objRHS._image[r][c].intGreen) ||
                (_image[r][c].intBlue != objRHS._image[r][c].intBlue)) {
                return false;
            }
        }
    }
    return true;
}

const UJImage& UJImage::operator+(const UJImage& objRHS) {
    if (_rows != objRHS._rows || _cols != objRHS._cols) {
        exit(ERROR);
    }
    UJImage* objImage = new UJImage(_rows, _cols);
    for (int r = 0; r < _rows; r++) {
        for (int c = 0; c < _cols; c++) {
            objImage->_image[r][c].intRed =
                (_image[r][c].intRed + objRHS._image[r][c].intRed) % MAX;
            objImage->_image[r][c].intGreen =
                (_image[r][c].intGreen + objRHS._image[r][c].intGreen) % MAX;
            objImage->_image[r][c].intBlue =
                (_image[r][c].intBlue + objRHS._image[r][c].intBlue) % MAX;
        }
    }
    return *objImage;
}

UJPixel& UJImage::operator()(int intRow, int intCol) {
    enforceRange(intRow, 0, _rows - 1);
    enforceRange(intCol, 0, _cols - 1);
    return _image[intRow][intCol];
}

UJPixel& UJImage::operator[](int intIndex) {
    enforceRange(intIndex, 0, _rows * _cols - 1);
    int intRow = intIndex / _cols;
    int intCol = intIndex % _cols;
    return _image[intRow][intCol];
}

UJImage& UJImage::operator--(int) {
    UJImage* temp = new UJImage(*this);
    for (int r = 0; r < _rows; r++) {
        for (int c = 0; c < _cols; c++) {
            _image[r][c].intRed = --_image[r][c].intRed < 0 ? 255 : _image[r][c].intRed;
            _image[r][c].intGreen = --_image[r][c].intGreen < 0 ? 255 : _image[r][c].intGreen;
            _image[r][c].intBlue = --_image[r][c].intBlue < 0 ? 255 : _image[r][c].intBlue;
        }
    }
    return *temp;
}

std::ostream& operator<<(std::ostream& osLHS, const UJImage& objRHS) {
    osLHS << objRHS.toPPM() << std::endl;
    return osLHS;
}

void UJImage::resetArray(int rows, int cols){
	//Deallocate memory 
	if (_image == nullptr){
		return;
	} 
	dealloc();
	
	//reset array size
	_cols = cols;
	_rows = rows;
	
	//reallocate memory
	alloc(rows, cols, DEFAULT_PIXEL);
}