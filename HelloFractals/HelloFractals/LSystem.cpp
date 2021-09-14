#include "LSystem.h"

LSystem::LSystem()
{
}

LSystem::~LSystem()
{
}

void LSystem::initialize()
{
	d = 1.0;
	alpha = glm::radians(45.0);
	result = "F+F-F-FF+F+F-​";
}

void LSystem::setTexture(int texID)
{
	this->texID = texID;
}

void LSystem::addRule(string left, string right)
{
	prodRules.insert(pair<string, string>(left, right));
}

void LSystem::setRotation(float angle, glm::vec3 axis, bool reset)
{
	if (reset) transform = glm::mat4(1);
	transform = glm::rotate(transform, angle, axis);
}

void LSystem::setTranslation(glm::vec3 displacements, bool reset)
{
	if (reset) transform = glm::mat4(1);
	transform = glm::translate(transform, displacements);
}

void LSystem::setScale(glm::vec3 scaleFactors, bool reset)
{
	if (reset) transform = glm::mat4(1);
	transform = glm::scale(transform, scaleFactors);
	scale = scaleFactors;
}

void LSystem::draw()
{
	//glBindTexture(GL_TEXTURE_2D, texID);
	//glUniform1i(glGetUniformLocation(shader->ID, "ourTexture1"), 0);
	shader->setVec4("inputColor", 1.0f, 0.0f, 0.0f, 1.0f);
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, geometry.size());

	shader->setVec4("inputColor", 0.0f, 1.0f, 1.0f, 1.0f);
	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, geometry.size());
	glBindVertexArray(0);
}

void LSystem::update()
{
	//Por enquanto o update é basicamente pedir para atualizar no shader 
	//a matriz de transformação

	setTranslation(pos);
	setRotation(angle, glm::vec3(0.0f, 0.0f, 1.0f), false);
	setScale(scale, false);

	GLint transformLoc = glGetUniformLocation(shader->ID, "model");
	// Pass them to the shaders
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	//TO DO: quando tiver animação por LSystemsheet, passar os deslocamentos
	//nas coords de textura
}

void LSystem::generateMesh()
{
	glm::mat4 matrix = glm::mat4(1); //sempre matriz identidade
	stack <glm::mat4> matrixStack;
	glm::vec4 v0 = glm::vec4(0.0, 0.0, 0.0, 1.0);
	glm::vec4 v = v0;

	bool push = false, pop = false;

	bool inicio = true;
	int count = 0;

	for (int i = 0; i < result.size(); i++)
	{
		v = v0;
				
		if (result[i] == '[')
		{
			matrixStack.push(matrix);
		}
		if (result[i] == ']')
		{
			matrix = matrixStack.top();
			matrixStack.pop();
		}
		if (result[i] == '+')
		{
			matrix = glm::rotate(matrix, alpha, glm::vec3(0.0, 0.0, 1.0));
			
		}
		if (result[i] == '-')
		{
			matrix = glm::rotate(matrix, -alpha, glm::vec3(0.0, 0.0, 1.0));
			
		}
		if (result[i] == 'F' || result[i] == 'A' || result[i] == 'B' || result[i] == 'G')
		{
			glm::vec3 v1, v2;
			v = matrix * v;
			v1.x = v.x; v1.y = v.y; v1.z = v.z;
			
			matrix = glm::translate(matrix, glm::vec3(d, 0.0, 0.0));
			//aqui desacumula?
			v = v0;
			v= matrix * v;
			v2.x = v.x; v2.y = v.y; v2.z = v.z;

		
			geometry.push_back(v1);
			geometry.push_back(v2);
		}
		if (result[i] == 'f')
		{
			matrix = glm::translate(matrix, glm::vec3(d, 0.0, 0.0));
			//aqui desacumula?
		}

		

	}

	//Agora que geramos a geometria, vamos mandá-la para os buffers

	unsigned int VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, geometry.size() * 3 *sizeof(GLfloat), geometry.data(), GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	transform = glm::mat4(1); //matriz identidade
	texID = -1; //ainda não temos
	shader = NULL; //ainda não temos
	pos = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	angle = 0.0f;

}

void LSystem::iterate()
{
	string aux1 = axiom, aux2 = "";

	for (int i = 0; i < nGenerations; i++)
	{	
		for (int j = 0; j < aux1.size(); j++)
		{
			std::map<string,string>::iterator it;
			string symbol = aux1.substr(j,1);
			it = prodRules.find(symbol);
			if (it != prodRules.end())
			{
				string prod = it->second;
				aux2 += prod;
			}
			else
			{
				aux2 += symbol;
			}
		}
		aux1 = aux2;
		aux2.clear();
	}

	result = aux1;
	//cout << result << endl;

}
