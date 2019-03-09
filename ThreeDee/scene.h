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
		std::vector<std::string> tokens = getTokensFromLine(inputFileStream);
		while (tokens.size() != 0) {



			command = tokens[0];
			std::cout << command << std::endl;
			//SPHERE
			if (command == "s") {
				Vector3 sPos = { std::stof(tokens[1]),std::stof(tokens[2]),std::stof(tokens[3]) };
				Sphere* sphere = new Sphere(sPos,
					std::stof(tokens[4]));
				tokens.clear();
			}
			//VIEWPORT
			else if (command == "v") {
				tokens = getTokensFromLine(inputFileStream);
				//FROM

				Vector3 from = { std::stof(tokens[1]),std::stof(tokens[2]),std::stof(tokens[3]) };
				//tokens.clear();

				//AT

				tokens = getTokensFromLine(inputFileStream);
				Vector3 at = { std::stof(tokens[1]),std::stof(tokens[2]),std::stof(tokens[3]) };

				//UP
				tokens = getTokensFromLine(inputFileStream);
				Vector3 up = { std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]) };

				//ANGLE
				tokens = getTokensFromLine(inputFileStream);
				float angle = stof(tokens[1]);

				//HITHER
				tokens = getTokensFromLine(inputFileStream);
				float hither = stof(tokens[1]);

				//RES
				tokens = getTokensFromLine(inputFileStream);
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
				std::cout << "Creating background color" << std::endl;
				_color *background = new _color();
				background->r = std::stof(tokens[1]);
				background->g = std::stof(tokens[2]);
				background->b = std::stof(tokens[3]);
			}
			//POSITIONAL LIGHT
			else if (command == "l") {
				std::cout << "Creating positional light" << std::endl;
				//pos
				Vector3 lPos = { std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]) };
				//rgb
				Vector3 RGB = { 255, 255, 255 };

				if (tokens.size() == 7) {//if rgb defined
					float r, g, b;
					r = std::stof(tokens[4]);
					g = std::stof(tokens[5]);
					b = std::stof(tokens[6]);
					RGB = { r, g, b };
				}

				std::cout << "light position: " << lPos.x << ", " << lPos.y << ", " << lPos.z << std::endl;
				std::cout << "light rgb: " << RGB.x << ", " << RGB.y << ", " << RGB.z << std::endl;
			}
			//FILL COLOR AND SHADING PARAMS
			else if (command == "f") {
				std::cout << "Setting fill color:" << std::endl;
				Vector3 rgb = { std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]) };
				float kd, ks, shine, transm, idxRefr;
				kd = std::stof(tokens[4]);
				ks = std::stof(tokens[5]);
				shine = std::stof(tokens[6]);
				transm = std::stof(tokens[7]);
				idxRefr = std::stof(tokens[8]);
				std::cout << "RGB: ";
				printVector(rgb);
				std::cout << "Kd: " << kd << "; Ks: " << ks << "; ";
				std::cout << "Shine:" << shine << "; Transm: " << transm << "; idx Refraction:" << idxRefr;
				//TODO add fill color
			}
			else if (command == "c") {

				//BASE
				std::cout << "Creating cone/cilinder" << std::endl;

				tokens = getTokensFromLine(inputFileStream);
				Vector3 base = { std::stof(tokens[0]),std::stof(tokens[1]),std::stof(tokens[2]) };
				float baseR = std::stof(tokens[3]);
				std::cout << "Base: "; printVector(base);
				std::cout << "Radius: " << baseR << std::endl;

				//APEX
				tokens = getTokensFromLine(inputFileStream);
				Vector3 apex = { std::stof(tokens[0]),std::stof(tokens[1]),std::stof(tokens[2]) };
				float apexR = std::stof(tokens[3]);
				std::cout << "Apex: "; printVector(apex);
				std::cout << "Radius: " << apexR << std::endl;
				//TODO create cone/cilinder
			}
			else if (command == "pl") {
				std::cout << "Creating plane:" << std::endl;
				Vector3 p1 = { std::stof(tokens[1]),std::stof(tokens[2]),std::stof(tokens[3]) };
				Vector3 p2 = { std::stof(tokens[4]),std::stof(tokens[5]),std::stof(tokens[6]) };
				Vector3 p3 = { std::stof(tokens[7]),std::stof(tokens[8]),std::stof(tokens[9]) };
				//TODO create plan
				std::cout << "Point 1 : "; printVector(p1);
				std::cout << "Point 2 : "; printVector(p2);
				std::cout << "Point 3 : "; printVector(p3);

			}
			else if (command == "p") {
				std::cout << "Creating Poligon: " << std::endl;
				int tVertices = std::stoi(tokens[1]);
				std::cout << tVertices << " vertices" << std::endl;
				for (int i = 0; i < tVertices; i++) {
					tokens = getTokensFromLine(inputFileStream);
					Vector3 vert = { std::stof(tokens[0]),std::stof(tokens[1]),std::stof(tokens[2]) };
					std::cout << "Vertice " << i + 1 << ": "; printVector(vert);
					//TODO Create Vertices?!?
				}
				//TODO Create Poligon
			}
			else if (command == "pp") {
				std::cout << "Creating Poligon Patch: " << std::endl;
				int tVertices = std::stoi(tokens[1]);
				std::cout << tVertices << " vertices" << std::endl;

				for (int i = 0; i < tVertices; i++) {
					tokens = getTokensFromLine(inputFileStream);
					Vector3 vert = { std::stof(tokens[0]),std::stof(tokens[1]),std::stof(tokens[2]) };
					Vector3 norm = { std::stof(tokens[3]),std::stof(tokens[4]),std::stof(tokens[5]) };
					std::cout << "Vertice " << i + 1 << ": "; printVector(vert);
					std::cout << "Norm " << i + 1 << ": "; printVector(norm);
					//TODO Create Vertices and norm objects?!?
				}
				//TODO Create Poligonal patch
			}
			tokens = getTokensFromLine(inputFileStream);
		}

		return true;
	}

	std::vector<std::string> getTokensFromLine(std::ifstream& ifStream) {
		std::vector<std::string> tokens;
		std::string s;
		std::getline(ifStream, s);
		std::istringstream lineStream(s);
		std::string temp;
		while (std::getline(lineStream, temp, ' ')) {
			tokens.push_back(temp);
		}
		return tokens;
	}

private:
};