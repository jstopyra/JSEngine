#ifndef __ZLIBLOADER_H__
#define __ZLIBLOADER_H__

class ZLibLoader
{
public:
	ZLibLoader();
	~ZLibLoader();

	bool DecompressFile(char* inFileName, char* outFileName);
	bool CompressFile(char* inFileName, char* outFileName);
	unsigned long FileSize(char* fileName);


private:
	static const int k_bufferSize;
};

#endif // !__ZLIBLOADER_H__
