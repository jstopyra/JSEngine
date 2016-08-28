#include "ZLibLoader.h"

#include <stdio.h>
#include "zlib.h"

#include <errno.h>
//#define _CRT_SECURE_NO_WARNINGS
//#define _CRT_SECURE_NO_DEPRECATE

const int ZLibLoader::k_bufferSize = 1024;

ZLibLoader::ZLibLoader()
{

}

ZLibLoader::~ZLibLoader()
{
}
//decompress a given inFile and save it into a outFile
bool ZLibLoader::DecompressFile(char* inFileName, char* outFileName)
{
	errno_t error;

	//this is the file we will be decompressing
 	gzFile inFile = gzopen(inFileName, "rb");
	if (inFile == NULL)
		return false;
	//this is the file we will be writing into
	FILE *outFile;

	//open the out file so we can write into it
	if ((error = fopen_s(&outFile, outFileName, "wb")) != 0)
		return false;
	
	if (outFile == NULL)
		return false;

	char buffer[k_bufferSize];

	int currentRead = 0;

	//keep decompressing untikll we reach the end of the file
	while ((currentRead = gzread(inFile, buffer, sizeof(buffer))) > 0)
	{
		fwrite(buffer, 1, currentRead, outFile);
	}

	//close both of the files 
	gzclose(inFile);
	fclose(outFile);
	return true;
}


//compress a given inFile and save it into a outFile
bool ZLibLoader::CompressFile(char* inFileName, char* outFileName)
{
	errno_t error;
	//this is the file that we will be compressing
	FILE *inFile;
	//open the file that we will be compressing
	if ((error = fopen_s(&inFile, inFileName, "rb")) != 0)
		return false;

	//this is the file we will be writing into
	gzFile outFile = gzopen(outFileName, "wb");
	if (!inFile || !outFile)
		return false;

	//out buffersize
	char buffer[k_bufferSize];
	int currentRead = 0;

	//keep compressing untill we hit the end of the file
	while ((currentRead = (int)fread(buffer, 1, sizeof(buffer), inFile)) > 0)
	{
		gzwrite(outFile, buffer, currentRead);

	}

	//close both of the files
	fclose(inFile);
	gzclose(outFile);
	return true;
}

//Check the size of the tile.
unsigned long ZLibLoader::FileSize(char* fileName)
{

	errno_t error;
	FILE *pFile;

	if ((error = fopen_s(&pFile, fileName, "rb")) != 0)
		return false;
	
	fseek(pFile, 0, SEEK_END);
	unsigned long returnSize = ftell(pFile);
	fclose(pFile);
	return returnSize;
}