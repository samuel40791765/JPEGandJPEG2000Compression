//*****************************************************************************
//
// Image.cpp : Defines the class operations on images
//
// Author - Parag Havaldar
// Code used by students as starter code to display and modify images
//
//*****************************************************************************

#include "Image.h"
#define DWTcompress 16

// Constructor and Desctructors
MyImage::MyImage() 
{
	Data = NULL;
	Width = -1;
	Height = -1;
	ImagePath[0] = 0;
}

MyImage::~MyImage()
{
	if ( Data )
		delete Data;
}


// Copy constructor
MyImage::MyImage( MyImage *otherImage)
{
	Height = otherImage->Height;
	Width  = otherImage->Width;
	Data   = new unsigned char[Width*Height*4];
	strcpy(otherImage->ImagePath, ImagePath );

	for ( int i=0; i<(Height*Width*4); i++ )
	{
		Data[i]	= otherImage->Data[i];
	}


}



// = operator overload
MyImage & MyImage::operator= (const MyImage &otherImage)
{
	Height = otherImage.Height;
	Width  = otherImage.Width;
	Data   = new unsigned  char[Width*Height*4];
	strcpy( (char *)otherImage.ImagePath, ImagePath );

	for ( int i=0; i<(Height*Width*4); i++ )
	{
		Data[i]	= otherImage.Data[i];
	}
	
	return *this;

}


// MyImage::ReadImage
// Function to read the image given a path
bool MyImage::ReadImage()
{

	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Image or Image properties not defined");
		fprintf(stderr, "Usage is `Image.exe Imagefile w h`");
		return false;
	}
	
	// Create a valid output file pointer
	FILE *IN_FILE;
	IN_FILE = fopen(ImagePath, "rb");
	if ( IN_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Reading");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	unsigned char *Rbuf = new unsigned char[Height*Width];
	unsigned char *Gbuf = new unsigned char[Height*Width];
	unsigned char *Bbuf = new unsigned char[Height*Width];

	//fseek(IN_FILE, Width*Height * 3 * 40, SEEK_SET);

	for (i = 0; i < Width*Height; i ++)
	{
		Rbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		Gbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		Bbuf[i] = fgetc(IN_FILE);
	}
	
	// Allocate Data structure and copy
	Data = new unsigned char[Width*Height*4];
	for (i = 0; i < Height*Width; i++)
	{
		Data[4*i]	= Bbuf[i];
		Data[4*i+1]	= Gbuf[i];
		Data[4*i+2]	= Rbuf[i];
		Data[4*i+3] = 255;
	}

	// Clean up and return
	delete Rbuf;
	delete Gbuf;
	delete Bbuf;
	fclose(IN_FILE);

	return true;

}



// MyImage functions defined here
bool MyImage::WriteImage()
{


	// Verify ImagePath
	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Image or Image properties not defined");
		return false;
	}
	
	// Create a valid output file pointer
	FILE *OUT_FILE;
	OUT_FILE = fopen(ImagePath, "wb");
	if ( OUT_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Writing");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char *Rbuf = new char[Height*Width]; 
	char *Gbuf = new char[Height*Width]; 
	char *Bbuf = new char[Height*Width]; 

	for (i = 0; i < Height*Width; i++)
	{
		Bbuf[i] = Data[3*i];
		Gbuf[i] = Data[3*i+1];
		Rbuf[i] = Data[3*i+2];
	}

	
	// Write data to file
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Rbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Gbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Bbuf[i], OUT_FILE);
	}
	
	// Clean up and return
	delete Rbuf;
	delete Gbuf;
	delete Bbuf;
	fclose(OUT_FILE);

	return true;

}

// Deprecated function.
bool MyImage::Modify()
{
	
	// sample operation
	for ( int i=0; i<Width*Height; i++ )
	{
		Data[3*i] = 0;
		Data[3*i+1] = 0;
	}

	return false;
}

void MyImage::compressJPGImage() {
	//int* data = YUVConvert();
	double* data = new double[4 * Width*Height];
	for (int i = 0; i < 4 * Width*Height; i++)
		data[i] = static_cast<int>(Data[i]) - 128;
	//DCT
	double* RBlockData = new double[64];
	double* GBlockData = new double[64];
	double* BBlockData = new double[64];
	for (int h = 0; h < Height; h+=8) {
		for (int w = 0; w < Width; w+=8) {
			int currentblockh = h / 8;
			int currentblockw = w / 8;
			for (int v = 0; v < 8; v++) {
				for (int u = 0; u < 8; u++) {
					double rsum = 0.0f;
					double gsum = 0.0f;
					double bsum = 0.0f;
					for (int y = 0; y < 8; y++) {
						for (int x = 0; x < 8; x++) {
							rsum += data[4 * ((h + y)*Width + (w + x))] * cos(((double)(2 * x + 1)*(double)u*pie) / 16.0f) * cos(((double)(2 * (double)y + 1)*v*pie) / 16.0f);
							gsum += data[4 * ((h + y)*Width + (w + x)) + 1] * cos(((double)(2 * x + 1)*(double)u*pie) / 16.0f) * cos(((double)(2 * (double)y + 1)*v*pie) / 16.0f);
							bsum += data[4 * ((h + y)*Width + (w + x)) + 2] * cos(((double)(2 * x + 1)*(double)u*pie) / 16.0f) * cos(((double)(2 * (double)y + 1)*v*pie) / 16.0f);
						}
					}
					double cu = (u == 0) ? (1.0 / sqrt(2)) : 1.0, cv = (v == 0) ? (1.0 / sqrt(2)) : 1.0;
					RBlockData[v * 8 + u] = 0.25 * cu * cv * rsum;
					GBlockData[v * 8 + u] = 0.25 * cu * cv * gsum;
					BBlockData[v * 8 + u] = 0.25 * cu * cv * bsum;
				}
			}
			std::vector<double> dummy;
			jpgdata.DCTRData.push_back(dummy);
			jpgdata.DCTGData.push_back(dummy);
			jpgdata.DCTBData.push_back(dummy);
			for (int i = 0; i < 64; i++) {
				jpgdata.DCTRData[currentblockh * Width / 8 + currentblockw].push_back(RBlockData[i]);
				jpgdata.DCTGData[currentblockh * Width / 8 + currentblockw].push_back(GBlockData[i]);
				jpgdata.DCTBData[currentblockh * Width / 8 + currentblockw].push_back(BBlockData[i]);
			}
		}
	}
}

unsigned char* MyImage::decompressJPG(int coefficients) {
	double* RBlockData = new double[64];
	double* GBlockData = new double[64];
	double* BBlockData = new double[64];
	printf("start\n");
	int* decompress = new int[4 * Height*Width];
	unsigned char* output = new unsigned char[4 * Height*Width];
	if (coefficients > Height*Width)
		coefficients = Height * Width;
	int m = round(coefficients / (Height / 8 * Width / 8));
	printf("%d",m);

	for (int k = 0; k < jpgdata.DCTRData.size(); k++) {

		//zigzag reform
		{
			int row = 0, col = 0;
			int count = 0;
			// Boolean variable that will true if we 
			// need to increment 'row' value otherwise 
			// false- if increment 'col' value 
			bool row_inc = 0;

			// Print matrix of upper half zig-zag pattern 
			int mn = min(8, 8);
			for (int len = 1; len <= mn; ++len) {
				for (int i = 0; i < len; ++i) {
					if (count < m) {
						RBlockData[row * 8 + col] = jpgdata.DCTRData[k][row * 8 + col];
						GBlockData[row * 8 + col] = jpgdata.DCTGData[k][row * 8 + col];
						BBlockData[row * 8 + col] = jpgdata.DCTBData[k][row * 8 + col];
					}
					else {
						RBlockData[row * 8 + col] = 0.0f;
						GBlockData[row * 8 + col] = 0.0f;
						BBlockData[row * 8 + col] = 0.0f;
					}
					count++;

					if (i + 1 == len)
						break;
					// If row_increment value is true 
					// increment row and decrement col 
					// else decrement row and increment 
					// col 
					if (row_inc)
						++row, --col;
					else
						--row, ++col;
				}

				if (len == mn)
					break;

				// Update row or col vlaue according 
				// to the last increment 
				if (row_inc)
					++row, row_inc = false;
				else
					++col, row_inc = true;
			}

			// Update the indexes of row and col variable 
			if (row == 0) {
				if (col == 8 - 1)
					++row;
				else
					++col;
				row_inc = 1;
			}
			else {
				if (row == 8 - 1)
					++col;
				else
					++row;
				row_inc = 0;
			}

			// Print the next half zig-zag pattern 
			int MAX = max(8, 8) - 1;
			for (int len, diag = MAX; diag > 0; --diag) {

				if (diag > mn)
					len = mn;
				else
					len = diag;

				for (int i = 0; i < len; ++i) {
					if (count < m) {
						RBlockData[row * 8 + col] = jpgdata.DCTRData[k][row * 8 + col];
						GBlockData[row * 8 + col] = jpgdata.DCTGData[k][row * 8 + col];
						BBlockData[row * 8 + col] = jpgdata.DCTBData[k][row * 8 + col];
					}
					else {
						RBlockData[row * 8 + col] = 0.0f;
						GBlockData[row * 8 + col] = 0.0f;
						BBlockData[row * 8 + col] = 0.0f;
					}
					count++;

					if (i + 1 == len)
						break;

					// Update row or col vlaue according 
					// to the last increment 
					if (row_inc)
						++row, --col;
					else
						++col, --row;
				}

				// Update the indexes of row and col variable 
				if (row == 0 || col == 8 - 1) {
					if (col == 8 - 1)
						++row;
					else
						++col;

					row_inc = true;
				}

				else if (col == 0 || row == 8 - 1) {
					if (row == 8 - 1)
						++col;
					else
						++row;

					row_inc = false;
				}
			}
		}

		//reverse DCT
		for (int y = 0; y < 8; y++) {
			for (int x = 0; x < 8; x++) {
				double rsum = 0.0f;
				double gsum = 0.0f;
				double bsum = 0.0f;
				for (int v = 0; v < 8; v++) {
					for (int u = 0; u < 8; u++) {
						double cu = (u == 0) ? (1.0 / sqrt(2)) : 1.0, cv = (v == 0) ? (1.0 / sqrt(2)) : 1.0;
						rsum += cu * cv*RBlockData[v * 8 + u] * cos(((double)(2 * x + 1)*(double)u*pie) / 16.0f) * cos(((double)(2 * y + 1)*(double)v*pie) / 16.0f);
						gsum += cu * cv*GBlockData[v * 8 + u] * cos(((double)(2 * x + 1)*(double)u*pie) / 16.0f) * cos(((double)(2 * y + 1)*(double)v*pie) / 16.0f);
						bsum += cu * cv*BBlockData[v * 8 + u] * cos(((double)(2 * x + 1)*(double)u*pie) / 16.0f) * cos(((double)(2 * y + 1)*(double)v*pie) / 16.0f);
					}
				}
				
				int r = round(0.25 * rsum) + 128;
				int g = round(0.25 * gsum) + 128;
				int b = round(0.25 * bsum) + 128;
				r = 255 < r ? 255 : r;
				g = 255 < g ? 255 : g;
				b = 255 < b ? 255 : b;
				r = r < 0 ? 0 : r;
				g = g < 0 ? 0 : g;
				b = b < 0 ? 0 : b;
				output[4 * ((((k / (Height / 8)) * 8 + y) * Width + (k % (Width / 8)) * 8) + x)] = static_cast<unsigned char>(r);
				output[4 * ((((k / (Height / 8)) * 8 + y) * Width + (k % (Width / 8)) * 8) + x) + 1] = static_cast<unsigned char>(g);
				output[4 * ((((k / (Height / 8)) * 8 + y) * Width + (k % (Width / 8)) * 8) + x) + 2] = static_cast<unsigned char>(b);
				output[4 * ((((k / (Height / 8)) * 8 + y) * Width + (k % (Width / 8)) * 8) + x) + 3] = 255;
			}
		}
	}

	return output;
}

void MyImage::compressJPG2000() {
	jpg2000Data.DWTData = new double[4 * Width*Height];
	double* DWTData = new double[4 * Width*Height];
	double* DWTprogress = new double[4 * Width*Height]; //The last DWTData that was parsed
	for (int i = 0; i < 4 * Width*Height; i++) 
		DWTprogress[i] = static_cast<double>(Data[i]);
	int compress_width = Width;
	int compress_height = Height;
	while (DWTcompress < compress_width && DWTcompress < compress_height) {
		for (int i = 0; i < compress_height; i++) {
			for (int j = 0; j < compress_width; j += 2) {
				double ravg = (DWTprogress[4 * (i*Width + j)] + DWTprogress[4 * (i*Width + j + 1)]) / 2.0;
				double gavg = (DWTprogress[4 * (i*Width + j) + 1] + DWTprogress[4 * (i*Width + j + 1) + 1]) / 2.0;
				double bavg = (DWTprogress[4 * (i*Width + j) + 2] + DWTprogress[4 * (i*Width + j + 1) + 2]) / 2.0;
				DWTData[4 * (i * Width + j / 2 + compress_width / 2)] = DWTprogress[4 * (i*Width + j)] - ravg;
				DWTData[4 * (i * Width + j / 2)] = ravg;
				DWTData[4 * (i * Width + j / 2 + compress_width / 2) + 1] = DWTprogress[4 * (i*Width + j) + 1] - gavg;
				DWTData[4 * (i * Width + j / 2) + 1] = gavg;
				DWTData[4 * (i * Width + j / 2 + compress_width / 2) + 2] = DWTprogress[4 * (i*Width + j) + 2] - bavg;
				DWTData[4 * (i * Width + j / 2) + 2] = bavg;
			}
		}
		for (int i = 0; i < 4 * Width*Height; i++)
			DWTprogress[i] = DWTData[i];
		for (int i = 0; i < compress_height; i += 2) {
			for (int j = 0; j < compress_width; j++) {
				double ravg = (DWTprogress[4 * (i*Width + j)] + DWTprogress[4 * ((i + 1)*Width + j)]) / 2.0;
				double gavg = (DWTprogress[4 * (i*Width + j) + 1] + DWTprogress[4 * ((i + 1)*Width + j) + 1]) / 2.0;
				double bavg = (DWTprogress[4 * (i*Width + j) + 2] + DWTprogress[4 * ((i + 1)*Width + j) + 2]) / 2.0;
				DWTData[4 * ((i / 2 + compress_height / 2) * Width + j)] = DWTprogress[4 * (i*Width + j)] - ravg;
				DWTData[4 * (i / 2 * Width + j)] = ravg;
				DWTData[4 * ((i / 2 + compress_height / 2) * Width + j) + 1] = DWTprogress[4 * (i*Width + j) + 1] - gavg;
				DWTData[4 * (i / 2 * Width + j) + 1] = gavg;
				DWTData[4 * ((i / 2 + compress_height / 2) * Width + j) + 2] = DWTprogress[4 * (i*Width + j) + 2] - bavg;
				DWTData[4 * (i / 2 * Width + j) + 2] = bavg;
			}
		}
		compress_width = compress_width / 2;
		compress_height = compress_height / 2;
		for (int i = 0; i < 4 * Width*Height; i++)
			DWTprogress[i] = DWTData[i];
	}

	for (int i = 0; i < 4 * Width*Height; i++)
		jpg2000Data.DWTData[i] = DWTData[i];
	for (int i = 0; i < Height; i++) {
		for (int j = 0; j < Width; j++) {
			jpg2000Data.DWTData[4 * (i*Width + j) + 3] = 255;
		}
	}
}

unsigned char* MyImage::decompressJPG2000(int coefficients) {
	unsigned char* output = new unsigned char[4 * Height*Width];
	double* DWTData = new double[4 * Width*Height];
	double* DWTprogress = new double[4 * Width*Height]; //The last DWTData that was parsed

	//start zigzag
	{
		int count = 0;
		//top left block(orignal image)
		{
			int row = 0, col = 0;
			// Boolean variable that will true if we 
			// need to increment 'row' value otherwise 
			// false- if increment 'col' value 
			bool row_inc = 0;
			// Print matrix of upper half zig-zag pattern
			int mn = min(DWTcompress, DWTcompress);
			for (int len = 1; len <= mn; ++len) {
				for (int k = 0; k < len; ++k) {
					if (count <= coefficients) {
						DWTprogress[4 * (row * Width + col)] = jpg2000Data.DWTData[4 * (row * Width + col)];
						DWTprogress[4 * (row * Width + col) + 1] = jpg2000Data.DWTData[4 * (row * Width + col) + 1];
						DWTprogress[4 * (row * Width + col) + 2] = jpg2000Data.DWTData[4 * (row * Width + col) + 2];
					}
					else {
						DWTprogress[4 * (row * Width + col)] = 0.0f;
						DWTprogress[4 * (row * Width + col) + 1] = 0.0f;
						DWTprogress[4 * (row * Width + col) + 2] = 0.0f;
					}
					count++;
					if (k + 1 == len)
						break;
					// If row_increment value is true 
					// increment row and decrement col 
					// else decrement row and increment 
					// col 
					if (row_inc)
						++row, --col;
					else
						--row, ++col;
				}
				if (len == mn)
					break;
				// Update row or col vlaue according 
				// to the last increment 
				if (row_inc)
					++row, row_inc = false;
				else
					++col, row_inc = true;
			}
			// Update the indexes of row and col variable 
			if (row == 0) {
				if (col == DWTcompress - 1)
					++row;
				else
					++col;
				row_inc = 1;
			}
			else {
				if (row == DWTcompress - 1)
					++col;
				else
					++row;
				row_inc = 0;
			}
			// Print the next half zig-zag pattern 
			int MAX = max(DWTcompress, DWTcompress) - 1;
			for (int len, diag = MAX; diag > 0; --diag) {
				if (diag > mn)
					len = mn;
				else
					len = diag;
				for (int k = 0; k < len; ++k) {
					if (count <= coefficients) {
						DWTprogress[4 * (row * Width + col)] = jpg2000Data.DWTData[4 * (row * Width + col)];
						DWTprogress[4 * (row * Width + col) + 1] = jpg2000Data.DWTData[4 * (row * Width + col) + 1];
						DWTprogress[4 * (row * Width + col) + 2] = jpg2000Data.DWTData[4 * (row * Width + col) + 2];
					}
					else {
						DWTprogress[4 * (row * Width + col)] = 0.0f;
						DWTprogress[4 * (row * Width + col) + 1] = 0.0f;
						DWTprogress[4 * (row * Width + col) + 2] = 0.0f;
					}
					count++;
					if (k + 1 == len)
						break;
					// Update row or col vlaue according 
					// to the last increment 
					if (row_inc)
						++row, --col;
					else
						++col, --row;
				}
				// Update the indexes of row and col variable 
				if (row == 0 || col == DWTcompress - 1) {
					if (col == DWTcompress - 1)
						++row;
					else
						++col;

					row_inc = true;
				}
				else if (col == 0 || row == DWTcompress - 1) {
					if (row == DWTcompress - 1)
						++col;
					else
						++row;

					row_inc = false;
				}
			}
		}

		int h = DWTcompress, w = DWTcompress;
		int i = 0, j = 0;
		int iter_count = 0;
		while (h < Height && w < Width) {
			/*if (j != DWTcompress)
				w = w * 2;*/
				/*if (i != DWTcompress && h != Height / 2)
					h = h * 2;*/
			
			// Print matrix of upper half zig-zag pattern
			for (int iter = 0; iter < 3; iter++) {
				switch (iter) {
				case 0:
					i = 0, j = w;
					break;
				case 1:
					i = h, j = 0;
					break;
				case 2:
					i = h, j = w;
					break;
				default:
					break;
				}
				{
					int row = 0, col = 0;
					// Boolean variable that will true if we 
					// need to increment 'row' value otherwise 
					// false- if increment 'col' value 
					bool row_inc = 0;
					int mn = min(w, h);
					for (int len = 1; len <= mn; ++len) {
						for (int k = 0; k < len; ++k) {
							if (count <= coefficients) {
								DWTprogress[4 * ((i + row) * Width + (j + col))] = jpg2000Data.DWTData[4 * ((i + row) * Width + (j + col))];
								DWTprogress[4 * ((i + row) * Width + (j + col)) + 1] = jpg2000Data.DWTData[4 * ((i + row) * Width + (j + col)) + 1];
								DWTprogress[4 * ((i + row) * Width + (j + col)) + 2] = jpg2000Data.DWTData[4 * ((i + row) * Width + (j + col)) + 2];
							}
							else {
								DWTprogress[4 * ((i + row) * Width + (j + col))] = 0.0f;
								DWTprogress[4 * ((i + row) * Width + (j + col)) + 1] = 0.0f;
								DWTprogress[4 * ((i + row) * Width + (j + col)) + 2] = 0.0f;
							}
							count++;
							if (k + 1 == len)
								break;
							// If row_increment value is true 
							// increment row and decrement col 
							// else decrement row and increment 
							// col 
							if (row_inc)
								++row, --col;
							else
								--row, ++col;
						}
						if (len == mn)
							break;
						// Update row or col vlaue according 
						// to the last increment 
						if (row_inc)
							++row, row_inc = false;
						else
							++col, row_inc = true;
					}
					// Update the indexes of row and col variable 
					if (row == 0) {
						if (col == w - 1)
							++row;
						else
							++col;
						row_inc = 1;
					}
					else {
						if (row == h - 1)
							++col;
						else
							++row;
						row_inc = 0;
					}
					// Print the next half zig-zag pattern 
					int MAX = max(w, h) - 1;
					for (int len, diag = MAX; diag > 0; --diag) {
						if (diag > mn)
							len = mn;
						else
							len = diag;
						for (int k = 0; k < len; ++k) {
							if (count <= coefficients) {
								DWTprogress[4 * ((i + row) * Width + (j + col))] = jpg2000Data.DWTData[4 * ((i + row) * Width + (j + col))];
								DWTprogress[4 * ((i + row) * Width + (j + col)) + 1] = jpg2000Data.DWTData[4 * ((i + row) * Width + (j + col)) + 1];
								DWTprogress[4 * ((i + row) * Width + (j + col)) + 2] = jpg2000Data.DWTData[4 * ((i + row) * Width + (j + col)) + 2];
							}
							else {
								DWTprogress[4 * ((i + row) * Width + (j + col))] = 0.0f;
								DWTprogress[4 * ((i + row) * Width + (j + col)) + 1] = 0.0f;
								DWTprogress[4 * ((i + row) * Width + (j + col)) + 2] = 0.0f;
							}
							count++;
							if (k + 1 == len)
								break;
							// Update row or col vlaue according 
							// to the last increment 
							if (row_inc)
								++row, --col;
							else
								++col, --row;
						}
						// Update the indexes of row and col variable 
						if (row == 0 || col == w - 1) {
							if (col == w - 1)
								++row;
							else
								++col;

							row_inc = true;
						}
						else if (col == 0 || row == h - 1) {
							if (row == h - 1)
								++col;
							else
								++row;

							row_inc = false;
						}
					}
				}
			}
			h = h * 2;
			w = w * 2;
		}
	}
	for (int i = 0; i < 4 * Width*Height; i++)
		DWTData[i] = DWTprogress[i];

	//decompressing
	int decompress_width = DWTcompress * 2;
	int decompress_height = DWTcompress * 2;
	while (decompress_height < Height * 2  && decompress_width <  Width * 2) {
		for (int i = 0; i < decompress_height / 2; i++) {
			for (int j = 0; j < decompress_width; j++) {
				DWTData[4 * ((i * 2) * Width + j)] = DWTprogress[4 * (i*Width + j)] + DWTprogress[4 * ((i + decompress_height / 2) * Width + j)];
				DWTData[4 * ((i * 2 + 1) * Width + j)] = DWTprogress[4 * (i*Width + j)] - DWTprogress[4 * ((i + decompress_height / 2) * Width + j)];
				DWTData[4 * ((i * 2) * Width + j) + 1] = DWTprogress[4 * (i*Width + j) + 1] + DWTprogress[4 * ((i + decompress_height / 2) * Width + j) + 1];
				DWTData[4 * ((i * 2 + 1) * Width + j) + 1] = DWTprogress[4 * (i*Width + j) + 1] - DWTprogress[4 * ((i + decompress_height / 2) * Width + j) + 1];
				DWTData[4 * ((i * 2) * Width + j) + 2] = DWTprogress[4 * (i*Width + j) + 2] + DWTprogress[4 * ((i + decompress_height / 2) * Width + j) + 2];
				DWTData[4 * ((i * 2 + 1) * Width + j) + 2] = DWTprogress[4 * (i*Width + j) + 2] - DWTprogress[4 * ((i + decompress_height / 2) * Width + j) + 2];

		/*		DWTData[4 * ((i * 2) * Width + j)] = 255 < DWTData[4 * ((i * 2) * Width + j)] ? 255 : DWTData[4 * ((i * 2) * Width + j)];
				DWTData[4 * ((i * 2 + 1) * Width + j)] = 255 < DWTData[4 * ((i * 2 + 1) * Width + j)] ? 255 : DWTData[4 * ((i * 2 + 1) * Width + j)];
				DWTData[4 * ((i * 2) * Width + j) + 1] = 255 < DWTData[4 * ((i * 2) * Width + j) + 1] ? 255 : DWTData[4 * ((i * 2) * Width + j) + 1];
				DWTData[4 * ((i * 2 + 1) * Width + j) + 1] = 255 < DWTData[4 * ((i * 2 + 1) * Width + j) + 1] ? 255 : DWTData[4 * ((i * 2 + 1) * Width + j) + 1];
				DWTData[4 * ((i * 2) * Width + j) + 2] = 255 < DWTData[4 * ((i * 2) * Width + j) + 2] ? 255 : DWTData[4 * ((i * 2) * Width + j) + 2];
				DWTData[4 * ((i * 2 + 1) * Width + j) + 2] = 255 < DWTData[4 * ((i * 2 + 1) * Width + j) + 2] ? 255 : DWTData[4 * ((i * 2 + 1) * Width + j) + 2];
				DWTData[4 * ((i * 2) * Width + j)] = DWTData[4 * ((i * 2) * Width + j)] < 0 ? 0 : DWTData[4 * ((i * 2) * Width + j)];
				DWTData[4 * ((i * 2 + 1) * Width + j)] = DWTData[4 * ((i * 2 + 1) * Width + j)] < 0 ? 0 : DWTData[4 * ((i * 2 + 1) * Width + j)];
				DWTData[4 * ((i * 2) * Width + j) + 1] = DWTData[4 * ((i * 2) * Width + j) + 1] < 0 ? 0 : DWTData[4 * ((i * 2) * Width + j) + 1];
				DWTData[4 * ((i * 2 + 1) * Width + j) + 1] = DWTData[4 * ((i * 2 + 1) * Width + j) + 1] < 0 ? 0 : DWTData[4 * ((i * 2 + 1) * Width + j) + 1];
				DWTData[4 * ((i * 2) * Width + j) + 2] = DWTData[4 * ((i * 2) * Width + j) + 2] < 0 ? 0 : DWTData[4 * ((i * 2) * Width + j) + 2];
				DWTData[4 * ((i * 2 + 1) * Width + j) + 2] = DWTData[4 * ((i * 2 + 1) * Width + j) + 2] < 0 ? 0 : DWTData[4 * ((i * 2 + 1) * Width + j) + 2];*/
			}
		}
		for (int i = 0; i < 4 * Width*Height; i++)
			DWTprogress[i] = DWTData[i];
		
		for (int i = 0; i < decompress_height; i++) {
			for (int j = 0; j < decompress_width / 2; j++) {
				DWTData[4 * (i * Width + j * 2)] = DWTprogress[4 * (i*Width + j)] + DWTprogress[4 * (i * Width + j + decompress_width / 2)];
				DWTData[4 * (i * Width + j * 2 + 1)] = DWTprogress[4 * (i*Width + j)] - DWTprogress[4 * (i * Width + j + decompress_width / 2)];
				DWTData[4 * (i * Width + j * 2) + 1] = DWTprogress[4 * (i*Width + j) + 1] + DWTprogress[4 * (i * Width + j + decompress_width / 2) + 1];
				DWTData[4 * (i * Width + j * 2 + 1) + 1] = DWTprogress[4 * (i*Width + j) + 1] - DWTprogress[4 * (i * Width + j + decompress_width / 2) + 1];
				DWTData[4 * (i * Width + j * 2) + 2] = DWTprogress[4 * (i*Width + j) + 2] + DWTprogress[4 * (i * Width + j + decompress_width / 2) + 2];
				DWTData[4 * (i * Width + j * 2 + 1) + 2] = DWTprogress[4 * (i*Width + j) + 2] - DWTprogress[4 * (i * Width + j + decompress_width / 2) + 2];

				//DWTData[4 * (i * Width + j * 2)] = 255 < DWTData[4 * (i * Width + j * 2)] ? 255 : DWTData[4 * (i * Width + j * 2)];
				//DWTData[4 * (i * Width + j * 2 + 1)] = 255 < DWTData[4 * (i * Width + j * 2 + 1)] ? 255 : DWTData[4 * (i * Width + j * 2 + 1)];
				//DWTData[4 * (i * Width + j * 2) + 1] = 255 < DWTData[4 * (i * Width + j * 2) + 1] ? 255 : DWTData[4 * (i * Width + j * 2) + 1];
				//DWTData[4 * (i * Width + j * 2 + 1) + 1] = 255 < DWTData[4 * (i * Width + j * 2 + 1) + 1] ? 255 : DWTData[4 * (i * Width + j * 2 + 1) + 1];
				//DWTData[4 * (i * Width + j * 2) + 2] = 255 < DWTData[4 * (i * Width + j * 2) + 2] ? 255 : DWTData[4 * (i * Width + j * 2) + 2];
				//DWTData[4 * (i * Width + j * 2 + 1) + 2] = 255 < DWTData[4 * (i * Width + j * 2 + 1) + 2] ? 255 : DWTData[4 * (i * Width + j * 2 + 1) + 2];
				//DWTData[4 * (i * Width + j * 2)] = DWTData[4 * (i * Width + j * 2)] < 0 ? 0 : DWTData[4 * (i * Width + j * 2)];
				//DWTData[4 * (i * Width + j * 2 + 1)] = DWTData[4 * (i * Width + j * 2 + 1)] < 0 ? 0 : DWTData[4 * (i * Width + j * 2 + 1)];
				//DWTData[4 * (i * Width + j * 2) + 1] = DWTData[4 * (i * Width + j * 2) + 1] < 0 ? 0 : DWTData[4 * (i * Width + j * 2) + 1];
				//DWTData[4 * (i * Width + j * 2 + 1) + 1] = DWTData[4 * (i * Width + j * 2 + 1) + 1] < 0 ? 0 : DWTData[4 * (i * Width + j * 2 + 1) + 1];
				//DWTData[4 * (i * Width + j * 2) + 2] = DWTData[4 * (i * Width + j * 2) + 2] < 0 ? 0 : DWTData[4 * (i * Width + j * 2) + 2];
				//DWTData[4 * (i * Width + j * 2 + 1) + 2] = DWTData[4 * (i * Width + j * 2 + 1) + 2] < 0 ? 0 : DWTData[4 * (i * Width + j * 2 + 1) + 2];
			}
		}
		decompress_height = decompress_height * 2;
		decompress_width = decompress_width * 2;
		for (int i = 0; i < 4 * Width*Height; i++)
			DWTprogress[i] = DWTData[i];
	}
	for (int i = 0; i < Height; i++) {
		for (int j = 0; j < Width; j++) {
			DWTData[4 * (i * Width + j)] = 255 < DWTData[4 * (i * Width + j)] ? 255 : DWTData[4 * (i * Width + j)];
			DWTData[4 * (i * Width + j) + 1] = 255 < DWTData[4 * (i * Width + j) + 1] ? 255 : DWTData[4 * (i * Width + j) + 1];
			DWTData[4 * (i * Width + j) + 2] = 255 < DWTData[4 * (i * Width + j) + 2] ? 255 : DWTData[4 * (i * Width + j) + 2];
			DWTData[4 * (i * Width + j)] = DWTData[4 * (i * Width + j)] < 0 ? 0 : DWTData[4 * (i * Width + j)];
			DWTData[4 * (i * Width + j) + 1] = DWTData[4 * (i * Width + j) + 1] < 0 ? 0 : DWTData[4 * (i * Width + j) + 1];
			DWTData[4 * (i * Width + j) + 2] = DWTData[4 * (i * Width + j) + 2] < 0 ? 0 : DWTData[4 * (i * Width + j) + 2];
		}
	}


	for (int i = 0; i < 4*Width*Height; i++) 
		output[i] = static_cast<char>(round(DWTData[i]));

	//reassign alpha 
	for (int i = 0; i < Height; i++) {
		for (int j = 0; j < Width; j++) {
			output[4 * (i*Width + j) + 3] = 255;
		}
	}
	return output;
}
