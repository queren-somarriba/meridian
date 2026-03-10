#include <fstream>
#include <sstream>
#include <iostream>
#include "flowfield.hpp"

bool fileExist(const std::string& name)
{
	std::ifstream f(name.c_str());
	return f.good();
}

std::string readFile(const char* fileName)
{
	std::ifstream file(fileName, std::ios::in | std::ios::binary);
	
	if (!file.is_open())
		return ("");
	
	file.seekg(0, file.end);
	std::streampos len = file.tellg();
	file.seekg(0, file.beg);
	std::string outfile;
	outfile.resize(len);
	file.read(&outfile[0], len);
	return (outfile);
}

bool loadConfig(const std::string& filename, otData& data)
{
	std::ifstream file(filename);
	
	if (!file.is_open())
	{
		std::cerr << "Error : Can't open configuration file" << filename << std::endl;
		return false;
	}

	std::string line;
	while (std::getline(file, line)) 
	{
		if (line.empty())
			continue;
		
		size_t commentPos = line.find("//");
		if (commentPos != std::string::npos)
			line = line.substr(0, commentPos);

		std::stringstream ss(line);
		std::string key, valueStr;

		if (std::getline(ss, key, ':') && std::getline(ss, valueStr)) 
		{
			key.erase(remove_if(key.begin(), key.end(), isspace), key.end());
			float value = std::stof(valueStr);

			if (key == "window_width")
				data.width = static_cast<int>(value);
			else if (key == "window_height")
				data.height = static_cast<int>(value);
			else if (key == "perlin_scale") 
				data.scale = value;
			else if (key == "step_size") 
				data.stepSize = value;
			else if (key == "line_padding") 
				data.line_padding = value;
			else if (key == "pds_radius") 
				data.pds_radius = value;
			else if (key == "speed") 
				data.speed = value;
			else if (key == "record") 
				data.record = value;
		}
	}

	if (!data.width || !data.height || !data.stepSize
		|| !data.line_padding || !data.pds_radius || !data.speed)
		return false;
	return true;
}
