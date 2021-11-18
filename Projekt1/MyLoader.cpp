#include "MyLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>


int countSpaces(const string& s) {
	int cnt = 0;
	for (int i = 0; i < s.length(); i++)
		if (s[i] == ' ') cnt++;
	return cnt;
}

int* seePattern(fstream& source) {
	string line;
	int *wsk = new int[4];
	wsk[0] = wsk[1] = wsk[2] = wsk[3] = 0;
	while (getline(source, line)) {
		if (!line.compare(0, 2, "v ")) ++wsk[0];
		else if (!line.compare(0, 3, "vt ")) ++wsk[1];
		else if (!line.compare(0, 3, "vn ")) ++wsk[2];
		else if (!line.compare(0, 1, "f")) ++wsk[3];
		else line.replace(line.find(line), line.length(), "");
	}
	cout << wsk[0] << " " << wsk[1] << " " << wsk[2] << " " << wsk[3] << endl;
	return wsk;
}

MyLoader::MyLoader(const string& filepath) {
	fstream source(filepath, ios::in);
	fstream gource(filepath, ios::in);
	string line;
	int* tab = seePattern(gource);
	getline(source, line);
	while (line.compare(0, 2, "v ")) getline(source, line);
	float x, y, z;
	verts.resize(tab[0]);
	for (int i = 0; i < tab[0]; i++) {
		line.erase(0, 2);
		stringstream ss(line);
		ss >> x >> y >> z;
		verts[i] = glm::vec3(x, y, z);
		getline(source, line);
	}
	tcoords.resize(tab[1]);
	for (int i = 0; i < tab[1]; i++) {
		line.erase(0, 3);
		stringstream ss(line);
		ss >> x >> y;
		tcoords[i] = glm::vec2(x, y);
		getline(source, line);
	}
	normals.resize(tab[2]);
	for (int i = 0; i < tab[2]; i++) {
		line.erase(0, 3);
		stringstream ss(line);
		ss >> x >> y >> z;
		normals[i] = glm::vec3(x, y, z);
		getline(source, line);
	}

	if(line.compare(0, 2, "f ")) getline(source, line);
	indices.resize(tab[3] * 3*3);
	texturec.resize(tab[3] * 2*3);
	normalc.resize(tab[3] * 3*3);
	
	for(int i=0, k = 0; i<tab[3]; i++){
		if (line[0] != 'f') {
			getline(source, line);
			continue;
		}
		line.erase(0, 2);
		stringstream ss(line);
		string a, b, c;
		for (int j = 0; j < 3; j++, k++) {
			getline(ss, a, '/');
			getline(ss, b, '/');
			getline(ss, c, ' ');
			//cout <<"a: "<< a << " b: " << b << " c: " << c << endl;
			x = stoi(a) - 1;
			y = stoi(b) - 1;
			z = stoi(c) - 1;
			int p = 3 * k;
			//indices.push_back(verts[x].x); indices.push_back(verts[x].y); indices.push_back(verts[x].z);
			indices[p] = verts[x].x; indices[p+1] = verts[x].y; indices[p+2] = verts[x].z;
			//texturec.push_back(tcoords[y].x); texturec.push_back(tcoords[y].y);
			texturec[2*k] = tcoords[y].x; texturec[2*k+1] = tcoords[y].y;
			//normalc.push_back(normals[z].x); normalc.push_back(normals[z].y); normalc.push_back(normals[z].z);
			normalc[p] = normals[z].x; normalc[p + 1] = normals[z].y; normalc[p + 2] = normals[z].z;
		}
		getline(source, line);
	}

}

MyLoader::MyLoader(const MyLoader& obj) {
	verts = obj.verts;
	tcoords = obj.tcoords;
	normals = obj.normals;

	indices = obj.indices;
	texturec = obj.texturec;
	normalc = obj.normalc;
}

void MyLoader::print() const {
	/*for (int i = 0; i < verts.size(); i++)
		cout << verts[i].x << ' ' << verts[i].y << ' ' << verts[i].z << endl;
	cout << endl;
	for (int i = 0; i < tcoords.size(); i++)
		cout << tcoords[i].x << ' ' << tcoords[i].y  << endl;
	cout << endl;*/
	for (int i = 0; i < normals.size(); i++)
		cout << normals[i].x << ' ' << normals[i].y << ' ' << normals[i].z << endl;
	cout << endl;
	//cout << "WIERCHY" << endl;
	//for (int i = 0; i < indices.size()/3; i++)
	//	cout << indices[i] <<'/'<< texturec[i] << '/' << normalc[i] << endl;
	//cout << endl;
}

//
//MyLoader::MyLoader(const string& filepath) {
//	fstream source(filepath, ios::in);
//	string line;
//	while (getline(source, line)) {
//		float x, y, z;
//		if (!line.compare(0, 2, "v ")) {
//			line.erase(0, 2);
//			stringstream ss(line);
//			ss >> x >> y >> z;
//			verts.push_back(glm::vec3(x, y, z));
//		}
//		else if (!line.compare(0, 3, "vt ")) {
//			line.erase(0, 3);
//			stringstream ss(line);
//			ss >> x >> y;
//			tcoords.push_back(glm::vec2(x, y));
//		}
//		else if (!line.compare(0, 3, "vn ")) {
//			line.erase(0, 3);
//			stringstream ss(line);
//			ss >> x >> y >> z;
//			normals.push_back(glm::vec3(x, y, z));
//		}
//		else if (!line.compare(0, 1, "f")) {
//			line.erase(0, 2);
//			stringstream ss(line);
//			string a, b, c;
//			for (int i = 0; i < 3; i++) {
//				getline(ss, a, '/');
//				getline(ss, b, '/');
//				getline(ss, c, ' ');
//				x = stoi(a)-1;
//				y = stoi(b)-1;
//				z = stoi(c)-1;
//				indices.push_back(verts[x].x); indices.push_back(verts[x].y); indices.push_back(verts[x].z);
//				texturec.push_back(tcoords[y].x); texturec.push_back(tcoords[y].y);
//				normalc.push_back(normals[z].x); normalc.push_back(normals[z].y); normalc.push_back(normals[z].z);
//			}
//		}
//	}
//}