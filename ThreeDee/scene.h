#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
class Scene
{

public:

	Scene()
	{
	}

	~Scene()
	{
	}
	bool load_nff(const char* filename) {

		std::ifstream inputFileStream(filename);


		std::string line;
		std::string command;
		while (std::getline(inputFileStream, line)) {

			std::istringstream lineStream(line);
			while (std::getline(lineStream, command, ' ')) {
				std::cout << command << std::endl;
			}
		}
		return true;
	}
private:

};