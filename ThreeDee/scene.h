#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <vector>
#include "primitives.h"
class Scene
{

public:

	Scene(){}
	
	bool load_nff(const char* filename) {

		std::ifstream inputFileStream(filename);


		std::string line;
		std::string command;
		while (std::getline(inputFileStream, line)) {
			
			std::istringstream lineStream(line);
			std::vector<std::string> tokens;
			while (std::getline(lineStream, command, ' ')) {
				tokens.push_back(command);
			}
			command = tokens[0];
			if (command=="s") {
				Sphere* sphere = new Sphere(std::stof(tokens[1]), 
											std::stof(tokens[2]), 
											std::stof(tokens[3]), 
											std::stof(tokens[4]));
			}
		}
		return true;
	}
private:

};