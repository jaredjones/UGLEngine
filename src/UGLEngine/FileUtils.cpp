#include "FileUtils.h"

bool readFileIntoString(std::string path, std::string &data)
{
	std::ifstream stream(path, std::ios::in);
	if (stream)
	{
		stream.seekg(0, std::ios::end);
		data.resize(stream.tellg()); //pre-allocate room for the data to go in to
		stream.seekg(0, std::ios::beg);
		stream.read(&data[0], data.size());
		stream.close();
		return true;
	}
	else
	{
		printf("ERROR: Failed to open: %s\n", path.c_str());
		return false;
	}
};