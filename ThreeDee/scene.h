#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <limits.h>
#include <math.h>
#include "primitives.h"
#include "grid.h"
#include "colors.h"
#include "vector3.h"
#include "camera.h"
#include "positionallight.h"
#include "materialproperties.h"
#include "ray.h"

class Scene
{
public:
	Camera* camera;
	std::vector<Primitive*> primitives;
	std::vector<Light*> lights;
	Color* background = new Color({ 0,0,0 });

	Grid* grid;
	bool isGridActivated;
	
	Scene(bool gridOptimization): isGridActivated(gridOptimization){}
	Camera* GetCamera() {
		return camera;
	}
	bool load_nff(const char* filename) {
		/*
		Possible regex re implementation

		std::regex rx(R"((?:^|\s)([+-]?[[:digit:]]+(?:\.[[:digit:]]+)?)(?=$|\s))"); // Declare the regex with a raw string literal
		std::smatch m;
		std::string str = buffer;
		while (regex_search(str, m, rx)) {
			// std::cout << "Number found: " << m[1] << std::endl; // Get Captured Group 1 text

			str = m.suffix().str(); // Proceed to the next match
		}*/

		//Material Properties
		MaterialProperties* matProperties = new MaterialProperties({ { 0, 0, 0 },0 ,0 ,0 ,0 ,0 });

		//NFF File
		std::ifstream inputFileStream(filename);


		std::vector<std::string> tokens = getTokensFromLine(inputFileStream);



		while (tokens.size() != 0) {

			std::string command = tokens[0];

			//VIEWPORT
			if (command == "v") {
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
				float angle = (3.1415 /180) * stof(tokens[1]);

				//HITHER
				tokens = getTokensFromLine(inputFileStream);
				float hither = stof(tokens[1]);

				//RES
				tokens = getTokensFromLine(inputFileStream);
				int resx = stoi(tokens[1]);
				int resy = stoi(tokens[2]);

				//LENS
				tokens = getTokensFromLine(inputFileStream);
				float focus = stof(tokens[1]);
				float rad = stof(tokens[2]);

				camera = new Camera(from, at, up, angle, hither, resx, resy, focus, rad);
				// std::cout << "Viewport:" << std::endl;

				// std::cout << "from: " << from.x << ", " << from.y << ", " << from.z << ", " << std::endl;

				// std::cout << "at: " << at.x << ", " << at.x << ", " << at.z << ", " << std::endl;

				// std::cout << "up: " << up.x << ", " << up.y << ", " << up.z << ", " << std::endl;

				// std::cout << "angle: " << angle << std::endl;

				// std::cout << "focus: " << focus << std::endl;

				// std::cout << "hither: " << hither << std::endl;

				// std::cout << "res: " << resx << ", " << resy << std::endl << std::endl;

			}
			//BACKGROUND
			else if (command == "b") {
				background = new Color();
				background->r = std::stof(tokens[1]);
				background->g = std::stof(tokens[2]);
				background->b = std::stof(tokens[3]);
				// std::cout << "Creating background color: " << tokens[1] << "; " << tokens[2] << "; " << tokens[3] << std::endl << std::endl;

			}
			//POSITIONAL LIGHT
			else if (command == "l") {
				// std::cout << "Creating positional light" << std::endl;

				//pos
				Vector3 lPos = { std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]) };
				//rgb
				Color RGB = { 1, 1, 1 };

				if (tokens.size() == 7) {//if rgb defined
					float r, g, b;
					r = std::stof(tokens[4]);
					g = std::stof(tokens[5]);
					b = std::stof(tokens[6]);
					RGB = { r, g, b };
				}
				lights.push_back(new Light(lPos, RGB));
				// std::cout << "light position: " << lPos.x << ", " << lPos.y << ", " << lPos.z << std::endl;

				// std::cout << "light rgb: " << RGB.r << ", " << RGB.g << ", " << RGB.b << std::endl << std::endl;

			}
			else if (command == "al") {
				// std::cout << "Creating Area light" << std::endl;

				//pos
				Vector3 p1, p2, p3;
				Vector3 lPos = { std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]) };
				
				
				//rgb
				Color RGB = { 1, 1, 1 };
				p1 = { std::stof(tokens[4]), std::stof(tokens[5]), std::stof(tokens[6])};
				p2 = { std::stof(tokens[7]), std::stof(tokens[8]), std::stof(tokens[9])};
				p3 = { std::stof(tokens[10]), std::stof(tokens[11]), std::stof(tokens[12])};

				if (tokens.size() == 16) {//if rgb defined
					float r, g, b;
					r = std::stof(tokens[13]);
					g = std::stof(tokens[14]);
					b = std::stof(tokens[15]);
					RGB = { r, g, b };
				}
				lights.push_back(new AreaLight(lPos, RGB, p1, p2, p3));
				// std::cout << "light position: " << lPos.x << ", " << lPos.y << ", " << lPos.z << std::endl;

				// std::cout << "light rgb: " << RGB.r << ", " << RGB.g << ", " << RGB.b << std::endl << std::endl;

			}
			//FILL COLOR AND SHADING PARAMS
			else if (command == "f") {
				// std::cout << "Setting fill color:" << std::endl;

				Color color;
				float kd, ks, shine, transm, idxRefr;
				color = { std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]) };
				kd = std::stof(tokens[4]);
				ks = std::stof(tokens[5]);
				shine = std::stof(tokens[6]);
				transm = std::stof(tokens[7]);
				idxRefr = std::stof(tokens[8]);
				matProperties = new MaterialProperties({ color,kd,ks,shine,transm,idxRefr });
				// std::cout << "RGB: " << color.r << "; " << color.g << "; " << color.b << std::endl;

				// std::cout << "Kd: " << kd << "; Ks: " << ks << "; ";

				// std::cout << "Shine:" << shine << "; Transm: " << transm << "; idx Refraction: " << idxRefr << std::endl << std::endl;

				//TODO add fill color
			}
			//CILINDER/CONE
			else if (command == "c") {

				//BASE
				tokens = getTokensFromLine(inputFileStream);
				Vector3 base = { std::stof(tokens[0]),std::stof(tokens[1]),std::stof(tokens[2]) };
				float baseR = std::stof(tokens[3]);
				//APEX
				tokens = getTokensFromLine(inputFileStream);
				Vector3 apex = { std::stof(tokens[0]),std::stof(tokens[1]),std::stof(tokens[2]) };
				float apexR = std::stof(tokens[3]);

				Cilinder * cilinder = new Cilinder(base, baseR, apex, apexR, matProperties);
				primitives.push_back(cilinder);
				// std::cout << "Creating cone/cilinder" << std::endl;

				// std::cout << "Base: "; printVector(base);

				// std::cout << "Radius: " << baseR << std::endl;

				// std::cout << "Apex: "; printVector(apex);

				// std::cout << "Radius: " << apexR << std::endl;

				printMaterialProperties(*cilinder->_matProps); // std::cout << std::endl;

			}
			//SPHERE
			else if (command == "s") {
				Vector3 sPos = { std::stof(tokens[1]),std::stof(tokens[2]),std::stof(tokens[3]) };
				Sphere* sphere = new Sphere(sPos, std::stof(tokens[4]), matProperties);
				// std::cout << "Creating sphere at: " << sPos.x << ", " << sPos.y << ", " << sPos.z << ";" << std::endl << "with radius: " << tokens[4] << std::endl;

				primitives.push_back(sphere);
				printMaterialProperties(*sphere->_matProps); // std::cout << std::endl;

			}
			//POLIGON
			else if (command == "p") {
				// std::cout << "Creating Poligon: " << std::endl;

				int tVertices = std::stoi(tokens[1]);
				// std::cout << tVertices << " vertices" << std::endl;

				std::vector<Vector3> vertices;
				for (int i = 0; i < tVertices; i++) {
					tokens = getTokensFromLine(inputFileStream);
					Vector3 vert = { std::stof(tokens[0]),std::stof(tokens[1]),std::stof(tokens[2]) };
					vertices.push_back(vert);
					// std::cout << "Vertice " << i + 1 << ": "; printVector(vert);

				}
				Poligon* poligon = new Poligon(vertices, matProperties);
				printMaterialProperties(*poligon->_matProps); // std::cout << std::endl;

				primitives.push_back(poligon);
			}
			//POLIGONAL PATCH
			else if (command == "pp") {
				// std::cout << "Creating Poligon Patch: " << std::endl;

				int tVertices = std::stoi(tokens[1]);
				// std::cout << tVertices << " vertices" << std::endl;

				std::vector<Vector3> vertices;
				std::vector<Vector3> normals;
				for (int i = 0; i < tVertices; i++) {
					tokens = getTokensFromLine(inputFileStream);
					Vector3 vert = { std::stof(tokens[0]),std::stof(tokens[1]),std::stof(tokens[2]) };
					Vector3 normal = { std::stof(tokens[3]),std::stof(tokens[4]),std::stof(tokens[5]) };
					vertices.push_back(vert);
					normals.push_back(normal);
					// std::cout << "Vertice " << i + 1 << ": "; printVector(vert);

					// std::cout << "Normal " << i + 1 << ": "; printVector(normal);

				}
				PoligonalPatch* poligonPatch = new PoligonalPatch(vertices, normals, matProperties);
				printMaterialProperties(*poligonPatch->_matProps); // std::cout << std::endl;

				primitives.push_back(poligonPatch);
			}
			//PLANE
			else if (command == "pl") {
				// std::cout << "Creating plane:" << std::endl;

				Vector3 p1 = { std::stof(tokens[1]),std::stof(tokens[2]),std::stof(tokens[3]) };
				Vector3 p2 = { std::stof(tokens[4]),std::stof(tokens[5]),std::stof(tokens[6]) };
				Vector3 p3 = { std::stof(tokens[7]),std::stof(tokens[8]),std::stof(tokens[9]) };
				std::vector<Vector3> vertices1, vertices2;
				//TODO create plan
				// std::cout << "Point 1 : "; printVector(p1);

				// std::cout << "Point 2 : "; printVector(p2);

				// std::cout << "Point 3 : "; printVector(p3);

				vertices1.push_back(p1); vertices1.push_back(p2); vertices1.push_back(p3);
				Poligon* poligon = new Poligon(vertices1, matProperties);
				printMaterialProperties(*poligon->_matProps); // std::cout << std::endl;

				//primitives.push_back(poligon);

			}
			//AABB
			else if (command == "ab") {
				// std::cout << "Creating AABB:" << std::endl;

				Vector3 p1 = { std::stof(tokens[1]),std::stof(tokens[2]),std::stof(tokens[3]) };
				Vector3 p2 = { std::stof(tokens[4]),std::stof(tokens[5]),std::stof(tokens[6]) };
				//TODO create plan
				// std::cout << "Point 1 : "; printVector(p1);

				// std::cout << "Point 2 : "; printVector(p2);

				AxisAlignedBoundingBox* aabb = new AxisAlignedBoundingBox(p1, p2, matProperties);
				printMaterialProperties(*aabb->_matProps); // std::cout << std::endl;

				primitives.push_back(aabb);

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

	Collision castRay(Ray ray) {
		Collision col;
		col.object = nullptr;
		if (!isGridActivated) {
			for (Primitive* p : primitives) {
				Collision ncol = p->intersect(ray);
				if (col.object == nullptr || vector3Length(subVector(ncol.point, ray.origin)) < vector3Length(subVector(col.point, ray.origin))) {
					col = ncol;
				}
			}
			return col;
		}
		else {
			return grid->traverse(ray);
		}
	}

	void initializeGrid() {
		grid = new Grid(primitives);

	}

private:
};