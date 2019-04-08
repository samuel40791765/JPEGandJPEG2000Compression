//*****************************************************************************
//
// Image.h : Defines the class operations on images
//
// Author - Parag Havaldar
// Main Image class structure 
//
//*****************************************************************************

#ifndef IMAGE_DISPLAY
#define IMAGE_DISPLAY

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "afxwin.h"

// C RunTime Header Files
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <vector>

#define pie 3.141592653589793

// Class structure of Image 
// Use to encapsulate an RGB image
class MyImage 
{

private:
	struct JPGData {
		std::vector<std::vector<double>> DCTRData;
		std::vector<std::vector<double>> DCTGData;
		std::vector<std::vector<double>> DCTBData;
	};
	struct JPG2000Data {
		double* DWTData;
	};

	int		Width;					// Width of Image
	int		Height;					// Height of Image
	char	ImagePath[_MAX_PATH];	// Image location
	unsigned char*	Data;					// RGB data of the image
	
	JPGData jpgdata;
	JPG2000Data	jpg2000Data;

public:
	// Constructor
	MyImage();
	// Copy Constructor
	MyImage::MyImage( MyImage *otherImage);
	// Destructor
	~MyImage();

	// operator overload
	MyImage & operator= (const MyImage & otherImage);

	// Reader & Writer functions
	void	setWidth( const int w)  { Width = w; }; 
	void	setHeight(const int h) { Height = h; }; 
	void	setImageData( const unsigned char *img ) { Data = (unsigned char *)img; };
	void	setImagePath( const char *path) { strcpy(ImagePath, path); }
	void	compressJPGImage();
	void	compressJPG2000();
	int		getWidth() { return Width; };
	int		getHeight() { return Height; };
	unsigned char*	getImageData() { return Data; };
	char*	getImagePath() { return ImagePath; }
	unsigned char*	decompressJPG(int coefficients);
	unsigned char*	decompressJPG2000(int coefficients);

	// Input Output operations
	bool	ReadImage();
	bool	WriteImage();

	// Modifications
	bool	Modify();

};

#endif //IMAGE_DISPLAY
