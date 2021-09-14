/* Hello Triangle - código adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Processamento Gráfico - Jogos Digitais - Unisinos
 * Versão inicial: 7/4/2017
 * Última atualização em 14/09/2021
 *
 * A maioria dos exemplos foram tirados de: http://nomoreretake.net/en/2016/12/29/houdinil_system_sample02/
 */

#include <iostream>
#include <string>
#include <assert.h>
#include <cmath>


using namespace std;

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "LSystem.h"

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Protótipos das funções
int setupGeometry();

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 600;
const int nPoints = 100 + 1 + 1; //+centro +cópia do primeiro
const float pi = 3.14159;

double xmin = -10.0, xmax = 10.0, ymin = -10.0, ymax = 10.0;
double zoom = 1.0;
const float sensivity = 0.5;

int currGenerations;
int lsID = 0;
vector <LSystem*> lSystems;

// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();

	//Muita atenção aqui: alguns ambientes não aceitam essas configurações
	//Você deve adaptar para a versão do OpenGL suportada por sua placa
	//Sugestão: comente essas linhas de código para desobrir a versão e
	//depois atualize (por exemplo: 4.5 com 4 e 5)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Essencial para computadores da Apple
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Triangulo!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Compilando e buildando o programa de shader
	Shader* shader = new Shader("../../Common/shaders/ortho.vs", "../../Common/shaders/ortho.fs");

	// Gerando um buffer simples, com a geometria de um triângulo
	GLuint VAO = setupGeometry();

	//Ativando o shader que será usado
	shader->Use();

	// Enviando a cor desejada (vec4) para o fragment shader
	// Utilizamos a variáveis do tipo uniform em GLSL para armazenar esse tipo de info
	// que não está nos buffers
	GLint colorLoc = glGetUniformLocation(shader->ID, "inputColor");
	assert(colorLoc > -1);

	GLint projLoc = glGetUniformLocation(shader->ID, "projection");
	assert(projLoc > -1);

	

	LSystem kochSnow1;
	kochSnow1.initialize();
	kochSnow1.setGenerations(4);
	kochSnow1.setAlpha(60.0);
	kochSnow1.setD(1.0);
	kochSnow1.setAxiom("F--F--F");
	kochSnow1.addRule("F", "F+F--F+F");
	kochSnow1.iterate();
	kochSnow1.generateMesh();
	kochSnow1.setShader(shader);

	lSystems.push_back(&kochSnow1);
	//-----------------------------------
	LSystem snow1;
	snow1.initialize();
	snow1.setGenerations(4);
	snow1.setAlpha(72.0);
	snow1.setD(1.0);
	snow1.setAxiom("F-F-F-F-F");
	snow1.addRule("F", "F-F++F+F-F-F");
	snow1.iterate();
	snow1.generateMesh();
	snow1.setShader(shader);

	lSystems.push_back(&snow1);
	//-----------------------------------
	LSystem snow2;
	snow2.initialize();
	snow2.setGenerations(4);
	snow2.setAlpha(60.0);
	snow2.setD(1.0);
	snow2.setAxiom("F+F+F+F+F+F");
	snow2.addRule("F", "F++F-F-F-F-F++F");
	snow2.iterate();
	snow2.generateMesh();
	snow2.setShader(shader);

	lSystems.push_back(&snow2);
	//-----------------------------------
	LSystem dragonCurve;
	dragonCurve.setGenerations(10);
	dragonCurve.initialize();
	dragonCurve.setAlpha(90.0);
	dragonCurve.setD(1.0);
	dragonCurve.setAxiom("FX");
	dragonCurve.addRule("X", "X+YF");
	dragonCurve.addRule("Y", "FX-Y");
	dragonCurve.iterate();
	dragonCurve.generateMesh();
	dragonCurve.setShader(shader);

	lSystems.push_back(&dragonCurve);

	//-----------------------------------
	LSystem sierpinski;
	sierpinski.setGenerations(6);
	sierpinski.initialize();
	sierpinski.setAlpha(60.0);
	sierpinski.setD(1.0);
	sierpinski.setAxiom("A");
	sierpinski.addRule("A", "B-A-B");
	sierpinski.addRule("B", "A+B+A");
	sierpinski.iterate();
	sierpinski.generateMesh();
	sierpinski.setShader(shader);

	lSystems.push_back(&sierpinski);
	//-----------------------------------
	LSystem snow3;
	snow3.setGenerations(3);
	snow3.initialize();
	snow3.setAlpha(60.0);
	snow3.setD(1.0);
	snow3.setAxiom("fff+fff–F");
	snow3.addRule("F", "F[+F][-F]FG[+F][-F]FG");
	snow3.iterate();
	snow3.generateMesh();
	snow3.setShader(shader);

	lSystems.push_back(&snow3);
	//-----------------------------------
	LSystem snow4;
	snow4.setGenerations(6);
	snow4.initialize();
	snow4.setAlpha(60.0);
	snow4.setD(0.5);
	snow4.setAxiom("F");
	snow4.addRule("F","FF[+F][-F]");
	snow4.iterate();
	snow4.generateMesh();
	snow4.setShader(shader);

	lSystems.push_back(&snow4);
	//-----------------------------------
	LSystem snow5;
	snow5.setGenerations(7);
	snow5.initialize();
	snow5.setAlpha(60.0);
	snow5.setD(0.5);
	snow5.setAxiom("F++F++F");
	snow5.addRule("F", "F+F- -F+F");
	snow5.iterate();
	snow5.generateMesh();
	snow5.setShader(shader);

	lSystems.push_back(&snow5);
	//-----------------------------------
	LSystem snow6;
	snow6.setGenerations(7);
	snow6.initialize();
	snow6.setAlpha(90.0);
	snow6.setD(0.5);
	snow6.setAxiom("F");
	snow6.addRule("F", "FF[+F][-F]");
	snow6.iterate();
	snow6.generateMesh();
	snow6.setShader(shader);

	lSystems.push_back(&snow6);
	//-----------------------------------
	LSystem snow7;
	snow7.setGenerations(3);
	snow7.initialize();
	snow7.setAlpha(122.0);
	snow7.setD(0.5);
	snow7.setAxiom("GF");
	snow7.addRule("F", "F[+F][-F]FG[+F][-F]FG");
	snow7.iterate();
	snow7.generateMesh();
	snow7.setShader(shader);

	lSystems.push_back(&snow7);
	//-----------------------------------
	LSystem snow8;
	snow8.setGenerations(3);
	snow8.initialize();
	snow8.setAlpha(60.0);
	snow8.setD(0.5);
	snow8.setAxiom("fff+fff-F");
	snow8.addRule("F", "F[+F][-F]FG[+F][-F]FG");
	snow8.iterate();
	snow8.generateMesh();
	snow8.setShader(shader);

	lSystems.push_back(&snow8);
	//-----------------------------------
	LSystem snow9;
	snow9.setGenerations(3);
	snow9.initialize();
	snow9.setAlpha(60.0);
	snow9.setD(0.5);
	snow9.setAxiom("ffffffffffffff+++F");
	snow9.addRule("F", "F[+F][-F]FG[+F][-F]FG");
	snow9.iterate();
	snow9.generateMesh();
	snow9.setShader(shader);
	lSystems.push_back(&snow9);

	glm::mat4 ortho = glm::mat4(1); //inicializa com a matriz identidade
	
	

	//shader->setMat4("projection", glm::value_ptr(ortho)),

	

	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		//Aqui o código de correção do aspecto 
		double ratio = width / (float) height;
		if (width >= height)
		{
			ortho = glm::ortho(xmin*ratio*zoom, xmax*ratio*zoom, ymin*zoom, ymax*zoom, -1.0, 1.0);
		}
		else
		{
			ratio = height / (float) width;
			ortho = glm::ortho(xmin, xmax, ymin*ratio, ymax*ratio, -1.0, 1.0);
		}
		glUniformMatrix4fv(projLoc, 1, FALSE, glm::value_ptr(ortho));

		// Limpa o buffer de cor
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(1);
		glPointSize(2);

		// Chamada de desenho - drawcall
		// 
		currGenerations = lSystems[lsID]->getGenerations();
		lSystems[lsID]->update();
		lSystems[lsID]->draw();
		
		//glBindVertexArray(0);

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Função de callback de teclado - só pode ter uma instância (deve ser estática se
// estiver dentro de uma classe) - É chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		lsID = (lsID + 1) % lSystems.size();
	}
}

// Esta função está bastante harcoded - objetivo é criar os buffers que armazenam a 
// geometria de um triângulo
// Apenas atributo coordenada nos vértices
// 1 VBO com as coordenadas, VAO com apenas 1 ponteiro para atributo
// A função retorna o identificador do VAO
int setupGeometry()
{
	// Aqui setamos as coordenadas x, y e z do triângulo e as armazenamos de forma
	// sequencial, já visando mandar para o VBO (Vertex Buffer Objects)
	// Cada atributo do vértice (coordenada, cores, coordenadas de textura, normal, etc)
	// Pode ser arazenado em um VBO único ou em VBOs separados
	GLfloat* vertices;

	vertices = new GLfloat[nPoints * 3];

	float angle = 0.0;
	float deltaAngle = 2 * pi / (float)(nPoints - 2);
	float radius = 0.5;

	//Adicionar o centro
	vertices[0] = 0.0; // x
	vertices[1] = 0.0; // y
	vertices[2] = 0.0; // z sempre zero 

	for (int i = 3; i < nPoints * 3; i += 3)
	{
		vertices[i] = radius * cos(angle);
		vertices[i + 1] = radius * sin(angle);
		vertices[i + 2] = 0.0;

		angle += deltaAngle;
	}

	GLuint VBO, VAO;

	//Geração do identificador do VBO
	glGenBuffers(1, &VBO);
	//Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, (nPoints * 3) * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	//Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se está normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice 
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	
	//zoom += yoffset * sensivity;
	if (yoffset == 1 && zoom > 0.01)
	{
		zoom -= 0.1 * currGenerations;
	}
	else if (yoffset == -1)
	{
		zoom += 0.1 * currGenerations;
	}

	//cout << xmin * zoom << " " << xmax * zoom << endl;
	//cout << yoffset << " " << zoom << endl;
}
