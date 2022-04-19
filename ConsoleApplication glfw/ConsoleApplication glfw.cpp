// ConsoleApplication glfw.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdlib>
#include <math.h>


const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* vertexShaderSource2 = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 transform;"
"void main()\n"
"{\n"
"   gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* vertexShaderSource3 = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 transform3;"
"void main()\n"
"{\n"
"   gl_Position = transform3 * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

const char* fragmentShaderSource2 = "#version 330 core\n"
"out vec4 FragColor;"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.8f, 0.0f, 1.0f);\n"
"}\0";

const char* fragmentShaderSource3 = "#version 330 core\n"
"out vec4 FragColor;"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.1f, 0.1f, 1.0f);\n"
"}\0";


//player class
class Player {       // The class
public:             // Access specifier
    int myNum;        // Attribute (int variable)
    bool isDown;        // Attribute (bool variable)
};

Player player1;

//Obstacle class
class Obstacle {       // The class
public:             // Access specifier
    float xloc;        // Attribute (int variable)
    bool isDown;       // Attribute (bool variable)
    float yloc;
    float thickness;
};

Obstacle obstacle1;

float obsSpeed;
float gameTime = 0.0f;
const int AIpopulation=20;




//This neural network will have 1 hidden layer with 2 nodes, 2 inputs and an output that will be a node class.
// Defining Neural network Nodes
//Node class
class Node {       // The class
public:             // Access specifier
    float input[2];        // Attribute (int variable)
    float output;
    float w[2];


    float CalculateNodeOutput() { // outputs the normalized output of the node
        float x = 0.0;
        for (int i = 0; i < 2; i++) {
            x += input[i] * w[i];
        }
        return x / (1 + exp(-x));
    }

};



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        player1.isDown = false;

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        player1.isDown = true;
}

bool AIoutput(float w[3][2]) {
    //AI Control
    float input[2] = { obstacle1.xloc, obstacle1.yloc };

    Node hiddenLayer[2];
    Node Output;

    for (int i = 0; i < 2; i++) {
        hiddenLayer[i].input[0] = obstacle1.xloc;
        hiddenLayer[i].input[1] = obstacle1.yloc;

        hiddenLayer[i].w[0] = w[i][0];
        hiddenLayer[i].w[1] = w[i][1];

        Output.input[i] = hiddenLayer[i].CalculateNodeOutput();
    }
    Output.w[0] = w[2][0];
    Output.w[1] = w[1][0];

    float out = Output.CalculateNodeOutput();
    if (out > 0.5) {
        return true;
    }
    else {
        return false;
    }

}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);

    void framebuffer_size_callback(GLFWwindow * window, int width, int height);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    float counter = 0.01;
    float timer = 0.0;
    float timeint = 0.0;
    float lastTimer = 0.0;
    obsSpeed = 0.5;

    player1.isDown = true;
    
    obstacle1.xloc = 1.0;
    obstacle1.isDown = true;
    obstacle1.thickness = 0.1;


    float vertices[] = {
    -1.0f, -1.0f, 0.0f,
     1.0f, 0.0f, 0.0f,
     -1.0f,  0.0f, 0.0f,
    };
    float vertices2[] = {
    -0.8f, -0.0f, 0.0f,
     -0.7f, -0.0f, 0.0f,
     -0.75f, 0.2f, 0.0f
    };
    float vertices3[] = {
-0.1f, 0.1f, 0.0f,
 -0.0f, -0.0f, 0.0f,
 -0.0f, 0.2f, 0.0f
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int VBO2;
    glGenBuffers(1, &VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

    unsigned int VBO3;
    glGenBuffers(1, &VBO3);
    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    unsigned int VAO2;
    glGenVertexArrays(1, &VAO2);

    unsigned int VAO3;
    glGenVertexArrays(1, &VAO3);

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glUseProgram(shaderProgram);

    //create yellow vertex shader

    unsigned int vertexShader2;
    vertexShader2 = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader2, 1, &vertexShaderSource2, NULL);
    glCompileShader(vertexShader2);

    //create yellow frag shader
    unsigned int fragmentShader2;
    fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
    glCompileShader(fragmentShader2);



    //create yellow shader program
    unsigned int shaderProgram2;
    shaderProgram2 = glCreateProgram();
    glAttachShader(shaderProgram2, vertexShader2);
    glAttachShader(shaderProgram2, fragmentShader2);
    glLinkProgram(shaderProgram2);
    glUseProgram(shaderProgram2);

    
    
    
    unsigned int vertexShader3;
    vertexShader3 = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader3, 1, &vertexShaderSource3, NULL);
    glCompileShader(vertexShader3);

    //create red frag shader
    unsigned int fragmentShader3;
    fragmentShader3 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader3, 1, &fragmentShaderSource3, NULL);
    glCompileShader(fragmentShader3);



    //create red shader program
    unsigned int shaderProgram3;
    shaderProgram3 = glCreateProgram();
    glAttachShader(shaderProgram3, vertexShader3);
    glAttachShader(shaderProgram3, fragmentShader3);
    glLinkProgram(shaderProgram3);
    glUseProgram(shaderProgram3);

    //delete shaders
    glDeleteShader(vertexShader);
    glDeleteShader(vertexShader2);
    glDeleteShader(vertexShader3);
    glDeleteShader(fragmentShader);
    glDeleteShader(fragmentShader2);
    glDeleteShader(fragmentShader3);




    // ..:: Initialization code (done once (unless your object frequently changes)) :: ..
// 1. bind Vertex Array Object
    glBindVertexArray(VAO);
    // 2. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. then set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);// explica como ler os valored do vertices[] no vertexshader
    glEnableVertexAttribArray(0);


    glm::mat4 trans = glm::mat4(1.0f);
    //trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    //trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
    trans = glm::translate(trans, glm::vec3(0.0, 0.2, 0.0));




    unsigned int transformLoc = glGetUniformLocation(shaderProgram2, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

    //unsigned int transformLoc3 = glGetUniformLocation(shaderProgram3, "transform3");
    //glUniformMatrix4fv(transformLoc3, 1, GL_FALSE, glm::value_ptr(trans3));



    srand(time(NULL));

    float r = (static_cast <float>(rand()) / static_cast <float> (RAND_MAX / 2)) - 1;
    float w[AIpopulation][3][2];
    bool isDown[AIpopulation];
    bool isDead[AIpopulation];
    int lastdeathindex;
    int seclastdeathindex;
    // for loop to assign weights
    for (int m = 0; m < AIpopulation; m++) {
        isDead[m] = false;
        isDown[m] = false;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 2; j++) {
                w[m][i][j] = (static_cast <float>(rand()) / static_cast <float> (RAND_MAX / 20)) - 10;
            }
        }
    }


    while (!glfwWindowShouldClose(window))
    {
        timer = (float)glfwGetTime();
        
        processInput(window);

        for (int m = 0; m < AIpopulation; m++) {
            isDown[m] = AIoutput(w[m]);
            if (!isDead[m]) {
                player1.isDown = isDown[m];
            }
        }



        counter = counter + 0.00001;
        timeint = timer - lastTimer;

        gameTime += timeint;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        obsSpeed = gameTime / 40 + 0.5;
        //process transformations


        obstacle1.xloc -= (obsSpeed * timeint);

        if (obstacle1.xloc < -1.0) {
            obstacle1.xloc = 1.0f;
            int randint = (rand() % 10) + 1;
            if (randint > 5) {
                obstacle1.isDown = false;
                obstacle1.yloc = 0.2;
            }
            else {
                obstacle1.isDown = true;
                obstacle1.yloc = 0.0;
            }
        }

        /*
        //Player reset case.
        if (obstacle1.xloc - obstacle1.thickness < -0.8f && obstacle1.xloc + obstacle1.thickness > -0.8f && ((obstacle1.isDown && player1.isDown) || (!obstacle1.isDown && !player1.isDown))) {
            gameTime = 0;
            obstacle1.xloc = 1.0f;
            std::cout << obstacle1.isDown << std::endl;
            std::cout << player1.isDown << std::endl;
        }

        */

        //Population reset case

        bool iseveryonedead = true;
        int deadCount = 0;
        for (int m = 0; m < AIpopulation; m++) {
            if (obstacle1.xloc - obstacle1.thickness < -0.8f && obstacle1.xloc + obstacle1.thickness > -0.8f && ((obstacle1.isDown && isDown[m]) || (!obstacle1.isDown && !isDown[m]))) {
                isDead[m] = true;
            }
            if (!isDead[m]) {
                iseveryonedead = false;
            }
            if(isDead[m])
            {
                deadCount += 1;
                std::cout << deadCount << std::endl;
            }
            //get the survivor's index
            if (deadCount > AIpopulation-2 && deadCount< AIpopulation) {
                seclastdeathindex = m;
            }
            if (deadCount > AIpopulation-1) {
                lastdeathindex = m;
            }
        }
        //reset game
        if (iseveryonedead) {
            gameTime = 0;
            obstacle1.xloc = 1.0f;
            iseveryonedead = false;
            for (int m = 0; m < AIpopulation; m++) {
                isDead[m] = false;
                isDown[m] = false;
                //reassign the weights and mutate
                if (m < AIpopulation/3 || m == AIpopulation) {
                    for (int i = 0; i < 3; i++) {
                        for (int j = 0; j < 2; j++) {
                            w[m][i][j] = w[seclastdeathindex][i][j] + ((static_cast <float>(rand()) / static_cast <float> (RAND_MAX / 4)) - 2);
                        }
                    }
                }
                else
                {
                    for (int i = 0; i < 3; i++) {
                        for (int j = 0; j < 2; j++) {
                            w[m][i][j] = w[seclastdeathindex][i][j] + ((static_cast <float>(rand()) / static_cast <float> (RAND_MAX / 4)) - 2);
                        }
                    }
                }
            }
        }


        //Shader processing

        glUseProgram(shaderProgram);


        // ..:: Initialization code (done once (unless your object frequently changes)) :: ..
// 1. bind Vertex Array Object
        glBindVertexArray(VAO);
        // 2. copy our vertices array in a buffer for OpenGL to use
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // 3. then set our vertex attributes pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);// explica como ler os valored do vertices[] no vertexshader
        glEnableVertexAttribArray(0);



        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderProgram2);

        // ..:: Initialization code (done once (unless your object frequently changes)) :: ..
// 1. bind Vertex Array Object
        glBindVertexArray(VAO2);
        // 2. copy our vertices array in a buffer for OpenGL to use
        glBindBuffer(GL_ARRAY_BUFFER, VBO2);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
        // 3. then set our vertex attributes pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);// explica como ler os valored do vertices[] no vertexshader
        glEnableVertexAttribArray(0);



        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLES, 0, 3);


        //transform yellow uniform
        glm::mat4 trans = glm::mat4(1.0f);
        //trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
        //trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
        if (player1.isDown) {
            trans = glm::translate(trans, glm::vec3(0.0, 0.0, 0.0));
        }
        else {
            trans = glm::translate(trans, glm::vec3(0.0, 0.2, 0.0));
        }
        unsigned int transformLoc = glGetUniformLocation(shaderProgram2, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        trans = glm::mat4(1.0f);



        glUseProgram(shaderProgram3);

        // ..:: Initialization code (done once (unless your object frequently changes)) :: ..
// 1. bind Vertex Array Object
        glBindVertexArray(VAO3);
        // 2. copy our vertices array in a buffer for OpenGL to use
        glBindBuffer(GL_ARRAY_BUFFER, VBO3);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);
        // 3. then set our vertex attributes pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);// explica como ler os valored do vertices[] no vertexshader
        glEnableVertexAttribArray(0);



        glBindVertexArray(VAO3);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glm::mat4 trans3 = glm::mat4(1.0f);
        trans3 = glm::translate(trans, glm::vec3(obstacle1.xloc, obstacle1.yloc, 0.0));
        unsigned int transformLoc3 = glGetUniformLocation(shaderProgram3, "transform3");
        glUniformMatrix4fv(transformLoc3, 1, GL_FALSE, glm::value_ptr(trans3));

        lastTimer = timer;
        // final lines of the loop (swap the buffers)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

// Executar programa: Ctrl + F5 ou Menu Depurar > Iniciar Sem Depuração
// Depurar programa: F5 ou menu Depurar > Iniciar Depuração

// Dicas para Começar: 
//   1. Use a janela do Gerenciador de Soluções para adicionar/gerenciar arquivos
//   2. Use a janela do Team Explorer para conectar-se ao controle do código-fonte
//   3. Use a janela de Saída para ver mensagens de saída do build e outras mensagens
//   4. Use a janela Lista de Erros para exibir erros
//   5. Ir Para o Projeto > Adicionar Novo Item para criar novos arquivos de código, ou Projeto > Adicionar Item Existente para adicionar arquivos de código existentes ao projeto
//   6. No futuro, para abrir este projeto novamente, vá para Arquivo > Abrir > Projeto e selecione o arquivo. sln
