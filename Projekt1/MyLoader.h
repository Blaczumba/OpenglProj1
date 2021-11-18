#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
using namespace std;

class MyLoader {
public:
	vector<glm::vec3> verts;
	vector<glm::vec2> tcoords;
	vector<glm::vec3> normals;

	vector<float> indices;
	vector<float> texturec;
	vector<float> normalc;

	MyLoader(const string& filepath);
	MyLoader(const MyLoader& obj);
	inline vector<float> getv() const { return indices; }
	inline vector<float> getc() const { return texturec; }
	inline vector<float> getn() const { return normalc; }
	inline int sizev() const { return indices.size() / 3; }
	void print() const;
};