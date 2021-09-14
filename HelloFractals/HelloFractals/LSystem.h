#pragma once

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <stack>
#include <string>
#include <map>

#include "Shader.h"

using namespace std;

class LSystem
{
public:
	LSystem();
	~LSystem();

	void initialize(); //similar ao setupGeometry

	//Para setar os atributos de visualização 
	void setTexture(int texID);
	inline void setShader(Shader* shader) { this->shader = shader; shader->Use(); }
	inline void setPosition(glm::vec3 pos) { this->pos = pos; } //dica: fazer setX e setY individualmente
	inline void setDimension(glm::vec3 scale) { this->scale = scale; }
	inline void setAngle(float angle) { this->angle = angle; } //para 3D precisa angulo por eixo ou quaternion

	//Para setar os atributos do LSystem
	inline void setD(float d) { this->d = d; }
	inline void setAlpha(float alpha) { this->alpha = glm::radians(alpha); }
	inline void setAxiom(string axiom) { this->axiom = axiom; }
	void addRule(string left, string right); 
	inline void setGenerations(int nGenerations) { this->nGenerations = nGenerations; }

	inline int getGenerations() { return nGenerations; }


	//Para controle direto da matriz de transformações
	void setRotation(float angle, glm::vec3 axis, bool reset = true);
	void setTranslation(glm::vec3 displacements, bool reset = true);
	void setScale(glm::vec3 scaleFactors, bool reset = true);

	//Chamados no ciclo do SceneManager
	void draw();
	void update();

	void generateMesh();
	void iterate();

protected:
	//Atributos gerais
	int nGenerations;
	string axiom;
	map<string,string> prodRules;
	string result;
	float d, alpha;
	vector <glm::vec3> geometry;

	GLuint VAO; //ID do buffer de geometria

	glm::mat4 transform; //matriz de transformação

	unsigned int texID; //identificador da textura

	Shader* shader; //ponteiro para o objeto de shader, para fazer a interface

	//Atributos de controle da posição, orientação e dimensões do sprite
	glm::vec3 pos, scale;
	float angle;

};

