#pragma once
#include <GL/glew.h>
#include "MyLoader.h"
#include "shader_s.h"
class Object {
	unsigned int VAO, VBO;
    unsigned int size;
    Shader sh;
	vector<float> vertices, normals, textureCoords;
public:
	Object(Shader shader, const string& objSource) {
		MyLoader obj(objSource);
		vertices = obj.indices;
		normals = obj.normalc;
        textureCoords = obj.texturec;
        size = obj.sizev();

		textureCoords = obj.texturec;
        sh.ID = shader.ID;

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        int size = obj.sizev();

        glBufferData(GL_ARRAY_BUFFER, 8 * size * sizeof(float), nullptr, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * size * sizeof(float), &vertices[0]);
        glBufferSubData(GL_ARRAY_BUFFER, 3 * size * sizeof(float), 3 * size * sizeof(float), &normals[0]);
        glBufferSubData(GL_ARRAY_BUFFER, 2 * 3 * size * sizeof(float), 2*size*sizeof(float), &textureCoords[0]);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(3 * size * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(6 * size * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
    void draw() {
        glBindVertexArray(VAO);
        sh.use();
        glDrawArrays(GL_TRIANGLES, 0, size);
    }
};