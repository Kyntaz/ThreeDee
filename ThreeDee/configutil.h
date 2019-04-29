#pragma once

#include <fstream>
#include <sstream>
#include <vector>

#define CONFIG "config/rayTracer.conf"

float EPSILON = 0.0002f;
int SAMPLE_MODE = 1;  //0 for regular sampling, 1 for Jittering sampling, 2 for Stochastic sampling
int SAMPLE_MODE_LIGTH = 1; //0 for regular sampling, 1 for Jittering sampling, 2 for Stochastic sampling
int DEPTH_TRACE_LIMIT = 4;
bool ANTI_ALIASING = true;
int SAMPLING_RATE = 8; //16
bool GRID_OPTIMIZATION = true;
float M = 2;
std::string NFF_FILE = "nffs/bunny.nff";

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

void loadConfig() {
	std::ifstream file(CONFIG);

	std::vector<std::string> tokens = getTokensFromLine(file);
	SAMPLE_MODE = std::stoi(tokens[0]);

	tokens = getTokensFromLine(file);
	SAMPLE_MODE_LIGTH = std::stoi(tokens[0]);

	tokens = getTokensFromLine(file);
	ANTI_ALIASING = tokens[0] == "true";

	tokens = getTokensFromLine(file);
	GRID_OPTIMIZATION = tokens[0] == "true";

	tokens = getTokensFromLine(file);
	DEPTH_TRACE_LIMIT = std::stoi(tokens[0]);

	tokens = getTokensFromLine(file);
	SAMPLING_RATE = std::stoi(tokens[0]);

	tokens = getTokensFromLine(file);
	EPSILON = std::stof(tokens[0]);

	tokens = getTokensFromLine(file);
	M = std::stof(tokens[0]);

	tokens = getTokensFromLine(file);
	NFF_FILE = tokens[0];
}
