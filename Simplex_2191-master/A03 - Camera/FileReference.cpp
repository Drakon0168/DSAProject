#include "pch.h"
#include "FileReference.h"

//Creates a new File Refernce with a filePath and uniqueID
FileReference::FileReference(string path, string ID)
{
	filePath = path;
	uniqueID = ID;
}

//Returns the filePath of this reference
string FileReference::GetFilePath()
{
	return filePath;
}

//Returns the uniqueID of this reference
string FileReference::GetUniqueID()
{
	return uniqueID;
}