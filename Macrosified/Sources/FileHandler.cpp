#include "stdafx.h"
#include "FileHandler.h"
#include <time.h>
#include <assert.h>
#include <sys/stat.h>

const int MAXFILES = 99;

FileHandler::FileHandler()
{}

FileHandler::~FileHandler()
{
	Close();
}

void FileHandler::Close()
{
	if (m_FileWriter)
	{
		m_FileWriter->close();
		delete m_FileWriter;
	}
}
bool FileHandler::CreateNewFile(string i_FileName)
{
	bool fileCreatedSuccessfully = false;

	for (int i = 0; i < MAXFILES; ++i)
	{
		m_LogFileName = i_FileName + "_" + to_string(i) + ".txt";
		if (!CheckExist(m_LogFileName))
		{
			m_FileWriter = new ofstream(m_LogFileName);
			fileCreatedSuccessfully = true;
			break;
		}
	}

	assert(fileCreatedSuccessfully);
	return fileCreatedSuccessfully;
}


bool FileHandler::CheckExist(string i_FileName)
{
	struct stat buffer;
	return (stat(i_FileName.c_str(), &buffer) == 0);
}

bool FileHandler::Log(
	string i_LogText,
	bool i_DisableTimeStamp /*= false*/)
{
	if (m_FileWriter)
	{
		if (i_DisableTimeStamp)
		{
			*m_FileWriter << i_LogText;
		}
		else
		{
			time_t timer;
			*m_FileWriter << time(&timer) << "\t" << i_LogText;
		}
		return true;
	}
	return false;
}