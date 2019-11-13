#pragma once
#include <string>
using namespace std;

class FileReference
{
	private:
		string filePath;
		string uniqueID;

	public:
		FileReference(string path, string ID);
		string GetFilePath();
		string GetUniqueID();
};

