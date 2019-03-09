#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <vector>
#include "primitives.h"
#include "colors.h"
#include "vector3.h"
class Scene
{

public:

	Scene() {}

	bool load_nff(const char* filename) {

		std::ifstream inputFileStream(filename);
		/*
		Possible regex re implementation 

		std::regex rx(R"((?:^|\s)([+-]?[[:digit:]]+(?:\.[[:digit:]]+)?)(?=$|\s))"); // Declare the regex with a raw string literal
		std::smatch m;
		std::string str = buffer;
		while (regex_search(str, m, rx)) {
			std::cout << "Number found: " << m[1] << std::endl; // Get Captured Group 1 text
			str = m.suffix().str(); // Proceed to the next match
		}*/
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
				Vector3 sPos = { std::stof(tokens[1]),std::stof(tokens[2]),std::stof(tokens[3])};
				Sphere* sphere = new Sphere(sPos,
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
				Vector3 from = {std::stof(tokens[1]),std::stof(tokens[2]),std::stof(tokens[3])};
				tokens.clear();

				//AT
				std::getline(inputFileStream, line);
				lineStream.str(line);
				lineStream.clear();
				while (std::getline(lineStream, command, ' ')) {
					tokens.push_back(command);
				}
				Vector3 at = {std::stof(tokens[1]),std::stof(tokens[2]),std::stof(tokens[3])};
				tokens.clear();

				//UP
				std::getline(inputFileStream, line);
				lineStream.str(line);
				lineStream.clear();
				while (std::getline(lineStream, command, ' ')) {
					tokens.push_back(command);
				}
				Vector3 up = { std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3])};
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
				std::cout << "from: " << from.x << ", " << from.y << ", " << from.z << ", " << std::endl;
				std::cout << "at: " << at.x << ", " << at.x << ", " << at.z << ", " << std::endl;
				std::cout << "up: " << up.x << ", " << up.y << ", " << up.z << ", " << std::endl;
				std::cout << "angle: " << angle << std::endl;
				std::cout << "hither: " << hither << std::endl;
				std::cout << "res: " << resx << ", " << resy << std::endl;
			}
			//BACKGROUND
			else if (command == "b") {
				_color *background = new _color();
				background->r = std::stof(tokens[1]);
				background->g = std::stof(tokens[2]);
				background->b = std::stof(tokens[3]);
			}
			//POSITIONAL LIGHT
			else if (command == "l") {
				tokens.clear();
				//pos
				std::getline(inputFileStream, line);
				std::istringstream lineStream(line);
				while (std::getline(lineStream, command, ' ')) {
					tokens.push_back(command);
				}
				Vector3 lPos = { std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]) };
				Vector3 RGB = { 255, 255, 255 };
				if (tokens.size() == 7) {
					float r, g, b;
					tokens[4].erase(0,1);
					tokens[6].erase(tokens[6].end()-2,tokens[6].end());
					r = std::stof(tokens[4]);
					g = std::stof(tokens[5]);
					b = std::stof(tokens[6]);
					RGB = { r, g, b };
				}
				std::cout << "light position: " << lPos.x << ", " << lPos.y << ", " << lPos.z << std::endl;
				std::cout << "light rgb: " << RGB.x << ", " << RGB.y << ", " << RGB.z  << std::endl;
			}
			//FILL COLOR AND SHADING PARAMS
			else if (command == "f") {
				Vector3 rgb  = { std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]) };
				float kd, ks, shine, transm, idxRefr;
				kd = std::stof(tokens[4]);
				ks = std::stof(tokens[5]);
				shine = std::stof(tokens[6]);
				transm = std::stof(tokens[7]);
				idxRefr = std::stof(tokens[8]);
			}
		}

		return true;
	}



private:
};