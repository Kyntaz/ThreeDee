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

	Scene() {}

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
			//SPHERE
			if (command == "s") {
				Sphere* sphere = new Sphere(std::stof(tokens[1]),
					std::stof(tokens[2]),
					std::stof(tokens[3]),
					std::stof(tokens[4]));
				tokens.clear();
			}
			//VIEWPORT
			else if (command == "v") {

				tokens.clear();
				//FROM
				std::getline(inputFileStream, line);
				std::istringstream lineStream(line);
				while (std::getline(lineStream, command, ' ')) {
					tokens.push_back(command);
				}
				float fromx = std::stof(tokens[1]);
				float fromy = std::stof(tokens[2]);
				float fromz = std::stof(tokens[3]);
				tokens.clear();
				//AT
				std::getline(inputFileStream, line);
				lineStream.str(line);
				lineStream.clear();
				while (std::getline(lineStream, command, ' ')) {
					tokens.push_back(command);
				}
				float atx = std::stof(tokens[1]);
				float aty = std::stof(tokens[2]);
				float atz = std::stof(tokens[3]);
				tokens.clear();
				//UP
				std::getline(inputFileStream, line);
				lineStream.str(line);
				lineStream.clear();
				while (std::getline(lineStream, command, ' ')) {
					tokens.push_back(command);
				}

				float upx = std::stof(tokens[1]);
				float upy = std::stof(tokens[2]);
				float upz = std::stof(tokens[3]);
				tokens.clear();
				//ANGLE
				std::getline(inputFileStream, line);
				lineStream.str(line);
				lineStream.clear();
				std::getline(lineStream, command, ' ');
				std::getline(lineStream, command, ' ');
				float angle = stof(command);
				//HITHER
				std::getline(inputFileStream, line);
				lineStream.str(line);
				lineStream.clear();
				std::getline(lineStream, command, ' ');
				std::getline(lineStream, command, ' ');
				float hither = stof(command);
				//RES
				std::getline(inputFileStream, line);
				lineStream.str(line);
				lineStream.clear();
				while (std::getline(lineStream, command, ' ')) {
					tokens.push_back(command);
				}
				float resx = stof(tokens[1]);
				float resy = stof(tokens[2]);
				std::cout << "Viewport:" << std::endl;
				std::cout << "from: " << fromx << ", " << fromy << ", " << fromz << ", " << std::endl;
				std::cout << "at: " << atx << ", " << atx << ", " << atz << ", " << std::endl;
				std::cout << "up: " << upx << ", " << upy << ", " << upz << ", " << std::endl;
				std::cout << "angle: " << angle << std::endl;
				std::cout << "hither: " << hither << std::endl;
				std::cout << "res: " << resx << ", " << resy << std::endl;
			}
		}

		return true;
	}



private:
};