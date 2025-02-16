#include <stdio.h>
#include <string.h>
#include <glew.h>
#include <glfw3.h>
#include <windows.h>
//Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 600;
GLuint VAO, VBO, shader;

//Vertex Shader
//recibir color, salida Vcolor
static const char* vShader = "						\n\
#version 330										\n\
layout (location =0) in vec3 pos;					\n\
void main()											\n\
{													\n\
gl_Position=vec4(pos.x,pos.y,pos.z,1.0f); 			\n\
}";
//recibir Vcolor y dar de salida color
static const char* fShader = "						\n\
#version 330										\n\
out vec4 color;										\n\
void main()											\n\
{													\n\
	color = vec4(1.0f,0.0f,1.0f,0.0f);	 			\n\
}";



void CrearTriangulo()
{// TRES COLUMNAS PARA IDENTIFICAR VISUALMENTE QUE TENGO VERTICES CON COORDENADAS DE X,Y,Z
	GLfloat vertices[] = {
		//x       y     z
		-0.9f, -0.5f, 0.0f,
		-0.8f, -0.5f, 0.0f,
		-0.9f,  0.5f, 0.0f,

		-0.9f, 0.5f, 0.0f,
		-0.8f, 0.5f, 0.0f,
		-0.8f,  -0.5f, 0.0f,

		 -0.8f, 0.25f, 0.0f,// izquierda
		 -0.7, 0.25f, 0.0f,// derecha 
		 -0.8f, 0.5f, 0.0f,// central

		 -0.7f, 0.25f, 0.0f,// izquierda
		 -0.6, 0.25f, 0.0f,// derecha 
		 -0.6f, 0.5f, 0.0f,// central
	};
	glGenVertexArrays(1, &VAO); //generar 1 VAO, ahi se almacenan los arreglos de los datos de vertices 
	// se puede generar mas de un VAO a la vez 

	glBindVertexArray(VAO);//asignar VAO
	 // para cada VAO debo generar una VBO 
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// SIEMPRE SE USA LA BANDERA DE GL STATIC DRAW
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //pasarle los datos al VBO asignando tamano, los datos y en este caso es estático pues no se modificarán los valores
		// EL NUMERO DE COLUMNAS ES EL NUMERO DE COLUMNAS QUE YO TENGO ESCRITAS EN EL ARREGLO QUE ES EL TRES QUE TIENE EL SIZE OF 
		// COMO SOLO TENGO TRES DATOS TENGO QUE TENER EL CERO EN EL GL VOID
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);//Stride en caso de haber datos de color por ejemplo, es saltar cierta cantidad de datos
		//LOS TRES DATOS QUE TENGO  Y LOS GUARDA COMO UN VECT 3
		glEnableVertexAttribArray(0);
		//agregar valores a vèrtices y luego declarar un nuevo vertexAttribPointer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

}
void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) //Función para agregar los shaders a la tarjeta gráfica
//the Program recibe los datos de theShader


{
	GLuint theShader = glCreateShader(shaderType);//theShader es un shader que se crea de acuerdo al tipo de shader: vertex o fragment
	const GLchar* theCode[1];
	theCode[0] = shaderCode;//shaderCode es el texto que se le pasa a theCode
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);//longitud del texto
	glShaderSource(theShader,1, theCode, codeLength);//Se le asigna al shader el código
	glCompileShader(theShader);//Se comila el shader
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	//verificaciones y prevención de errores
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al compilar el shader %d es: %s \n",shaderType, eLog);
		return;
	}
	glAttachShader(theProgram, theShader);//Si no hubo problemas se asigna el shader a theProgram el cual asigna el código a la tarjeta gráfica
}

void CompileShaders() {
	shader= glCreateProgram(); //se crea un programa
	if (!shader)
	{
		printf("Error creando el shader");
		return;
	}
	AddShader(shader, vShader, GL_VERTEX_SHADER);//Agregar vertex shader
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);//Agregar fragment shader
	//Para terminar de linkear el programa y ver que no tengamos errores
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glLinkProgram(shader);//se linkean los shaders a la tarjeta gráfica
	 //verificaciones y prevención de errores
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al linkear es: %s \n", eLog);
		return;
	}
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al validar es: %s \n", eLog);
		return;
	}



}
int main()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Practica 1 introduccion a open gl ", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	int BufferWidth, BufferHeight;
	glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Asignar valores de la ventana y coordenadas
	//Asignar Viewport
	glViewport(0, 0, BufferWidth, BufferHeight);

 //Crear tríangulo
	CrearTriangulo();
	CompileShaders();


	//Loop mientras no se cierra la ventana
	while (!glfwWindowShouldClose(mainWindow))
	{
		//Recibir eventos del usuario
		glfwPollEvents();


		// Se define el intervalo de cambio de color en segundos (2 segundos)
		float intervaloCambio = 2.0f;

		// Se obtiene el tiempo actual (en segundos) y se guarda como el tiempo del último cambio
		float ultimoTiempoCambio = glfwGetTime();

		// Se inicializa el índice del color actual; 0 corresponde a azul, 1 a rojo, y 2 a verde
		int indiceColor = 0;

		// Bucle principal que continúa hasta que se cierra la ventana
		while (!glfwWindowShouldClose(mainWindow))
		{
			// Se obtiene el tiempo actual en segundos
			float tiempoActual = (float)glfwGetTime();

			// Se verifica si han pasado al menos 'intervaloCambio' segundos desde el último cambio
			if (tiempoActual - ultimoTiempoCambio >= intervaloCambio) {
				// Se incrementa el índice de color de forma cíclica (0 -> 1 -> 2 -> 0 -> ...)
				indiceColor = (indiceColor + 1) % 3;
				// Se actualiza el tiempo del último cambio al tiempo actual
				ultimoTiempoCambio = tiempoActual;
			}

			// Se selecciona el color de fondo según el valor de 'indiceColor'
			if (indiceColor == 0) {
				// Si 'indiceColor' es 0, se establece el fondo azul
				glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
			}
			else if (indiceColor == 1) {
				// Si 'indiceColor' es 1, se establece el fondo rojo
				glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
			}
			else if (indiceColor == 2) {
				// Si 'indiceColor' es 2, se establece el fondo verde
				glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
			}


			glClear(GL_COLOR_BUFFER_BIT);

			glUseProgram(shader);

			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 12);
			glBindVertexArray(0);

			glUseProgram(0);

			glfwSwapBuffers(mainWindow);

		}

	}
	return 0;
}