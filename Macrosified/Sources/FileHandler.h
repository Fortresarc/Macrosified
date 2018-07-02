#pragma once

#include <string>
#include <fstream>

using namespace std;

class FileHandler
{
public:
	FileHandler();
	~FileHandler();

	// Close file and delete
	void Close();

	// Create new file
	bool CreateNewFile(string i_FileName);

	bool Log(string i_LogText, bool i_DisableTimeStamp = false);

	static bool CheckExist(string i_FileName);

private:
	string m_LogFileName;
	ofstream* m_FileWriter;
};