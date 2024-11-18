// Include standard headers, GLEW, GLFW, GLM, Shader
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cmath>
#include "dependente/glew/glew.h"
#include "dependente/glfw/glfw3.h"  
#include "dependente/glm/glm.hpp"
#include "dependente/glm/gtc/matrix_transform.hpp"
#include "dependente/glm/gtc/type_ptr.hpp"
#include "shader.hpp"

// will be used to start game on user input
GLuint programID;
bool showStartScreen = true;
// PRESS ENTER TO START THE GAME
void drawStartScreen() {
    unsigned int colorLoc = glGetUniformLocation(programID, "color");

    // Fundal albastru pentru ecranul de start
    glm::vec4 backgroundColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    glUniform4fv(colorLoc, 1, glm::value_ptr(backgroundColor));

    glClear(GL_COLOR_BUFFER_BIT);

    // Poți desena forme sau afișa text aici dacă folosești o librărie pentru fonturi.
    std::cout << "Press ENTER to start the game...\n";
}

// Global Variables
GLFWwindow* window;
const int width = 1024, height = 1024; // Size of the window
GLuint vaoRoad, vboRoad, iboRoad;
GLuint vaoCar, vboCar, iboCar;
float carX = 0.0f; // Initial position of the car on X-axis
glm::mat4 carTrans;
GLuint vaoRand, vboRand, iboRand;

//Green Car Vertices (right road side
GLfloat verticesr[] = {
    0.8f,  1.3f, 0.0f,   //righ top
    0.8f,  1.1f, 0.0f,   //right bottom
    0.6f,  1.1f, 0.0f,   //left bottom
	0.6f,  1.3f, 0.0f    //left top
};
GLuint indicesr[] = {
    0, 1, 2,  // First Triangle 
    2, 3, 0  // Second Triangle 
};
void initRand() {
    glGenVertexArrays(1, &vaoRand);
    glGenBuffers(1, &vboRand);
    glGenBuffers(1, &iboRand);

    glBindVertexArray(vaoRand);

    glBindBuffer(GL_ARRAY_BUFFER, vboRand);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesr), verticesr, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboRand);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesr), indicesr, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}
//Draw Rand (Rand and Rand1 will be traffic cars)
void drawRand() {
    glBindVertexArray(vaoRand);

    unsigned int transformLoc = glGetUniformLocation(programID, "transform");
    unsigned int colorLoc = glGetUniformLocation(programID, "color");

    glm::mat4 transRand(1.0f);
    glm::vec4 color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);  // Green color for rand

    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transRand));
    glUniform4fv(colorLoc, 1, glm::value_ptr(color));

    glDrawElements(GL_TRIANGLES, sizeof(indicesr) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

}

//Vertices for traffic cars
GLuint vaoRand1, vboRand1, iboRand1;
GLfloat verticesr1[] = {
    -0.8f,  0.8f, 0.0f,   // top left
    -0.8f,  0.6f, 0.0f,   // top right
    -0.6f,  0.6f, 0.0f,   // bottom right
    -0.6f,  0.8f, 0.0f    // bottom left
};
GLuint indicesr1[] = {
    0, 1, 2,  // First Triangle 
    2, 3, 0  // Second Triangle 
};
void initRand1() {
    glGenVertexArrays(1, &vaoRand1);
    glGenBuffers(1, &vboRand1);
    glGenBuffers(1, &iboRand1);

    glBindVertexArray(vaoRand1);

    glBindBuffer(GL_ARRAY_BUFFER, vboRand1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesr1), verticesr1, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboRand1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesr1), indicesr1, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}
//Draw Rand (Rand and Rand1 will be traffic cars)
void drawRand1() {
    glBindVertexArray(vaoRand1);

    unsigned int transformLoc1 = glGetUniformLocation(programID, "transform");
    unsigned int colorLoc1 = glGetUniformLocation(programID, "color");

    glm::mat4 transRand1(1.0f);
    glm::vec4 color1 = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);  // Green color for rand

    glUniformMatrix4fv(transformLoc1, 1, GL_FALSE, glm::value_ptr(transRand1));
    glUniform4fv(colorLoc1, 1, glm::value_ptr(color1));

    glDrawElements(GL_TRIANGLES, sizeof(indicesr1) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

}

// Vertices for the central divider (squar) and dashed lane dividers
GLfloat vertices[] = {
    // Central square divider
    -0.05f,  1.0f, 0.0f,   // top left 
     0.05f,  1.0f, 0.0f,   // top right 
     0.05f, -1.0f, 0.0f,   // bottom right 
    -0.05f, -1.0f, 0.0f,   // bottom left 

    // Dashed lane divider left (6 segments)
    -0.55f,  0.85f, 0.0f,   // top left (1st dash)
    -0.50f,  0.85f, 0.0f,   // top right (1st dash)
    -0.50f,  0.75f, 0.0f,   // bottom right (1st dash)
    -0.55f,  0.75f, 0.0f,   // bottom left (1st dash)

    -0.55f,  0.45f, 0.0f,   // top left (2nd dash)
    -0.50f,  0.45f, 0.0f,   // top right (2nd dash)
    -0.50f,  0.35f, 0.0f,   // bottom right (2nd dash)
    -0.55f,  0.35f, 0.0f,   // bottom left (2nd dash)

    -0.55f,  0.15f, 0.0f,   // top left (3rd dash)
    -0.50f,  0.15f, 0.0f,   // top right (3rd dash)
    -0.50f,  0.05f, 0.0f,   // bottom right (3rd dash)
    -0.55f,  0.05f, 0.0f,   // bottom left (3rd dash)

    -0.55f, -0.25f, 0.0f,   // top left (4th dash)
    -0.50f, -0.25f, 0.0f,   // top right (4th dash)
    -0.50f, -0.35f, 0.0f,   // bottom right (4th dash)
    -0.55f, -0.35f, 0.0f,   // bottom left (4th dash)

    -0.55f, -0.55f, 0.0f,   // top left (5th dash)
    -0.50f, -0.55f, 0.0f,   // top right (5th dash)
    -0.50f, -0.65f, 0.0f,   // bottom right (5th dash)
    -0.55f, -0.65f, 0.0f,   // bottom left (5th dash)

    -0.55f, -0.75f, 0.0f,   // top left (6th dash)
    -0.50f, -0.75f, 0.0f,   // top right (6th dash)
    -0.50f, -0.85f, 0.0f,   // bottom right (6th dash)
    -0.55f, -0.85f, 0.0f,   // bottom left (6th dash)

    // Dashed lane divider right (6 segments)
    0.50f,  0.85f, 0.0f,   // top left (1st dash)
    0.55f,  0.85f, 0.0f,   // top right (1st dash)
    0.55f,  0.75f, 0.0f,   // bottom right (1st dash)
    0.50f,  0.75f, 0.0f,   // bottom left (1st dash)

    0.50f,  0.45f, 0.0f,   // top left (2nd dash)
    0.55f,  0.45f, 0.0f,   // top right (2nd dash)
    0.55f,  0.35f, 0.0f,   // bottom right (2nd dash)
    0.50f,  0.35f, 0.0f,   // bottom left (2nd dash)

    0.50f,  0.15f, 0.0f,   // top left (3rd dash)
    0.55f,  0.15f, 0.0f,   // top right (3rd dash)
    0.55f,  0.05f, 0.0f,   // bottom right (3rd dash)
    0.50f,  0.05f, 0.0f,   // bottom left (3rd dash)

    0.50f, -0.25f, 0.0f,   // top left (4th dash)
    0.55f, -0.25f, 0.0f,   // top right (4th dash)
    0.55f, -0.35f, 0.0f,   // bottom right (4th dash)
    0.50f, -0.35f, 0.0f,   // bottom left (4th dash)

    0.50f, -0.55f, 0.0f,   // top left (5th dash)
    0.55f, -0.55f, 0.0f,   // top right (5th dash)
    0.55f, -0.65f, 0.0f,   // bottom right (5th dash)
    0.50f, -0.65f, 0.0f,   // bottom left (5th dash)

    0.50f, -0.75f, 0.0f,   // top left (6th dash)
    0.55f, -0.75f, 0.0f,   // top right (6th dash)
    0.55f, -0.85f, 0.0f,   // bottom right (6th dash)
    0.50f, -0.85f, 0.0f    // bottom left (6th dash)
};
// Update the indices accordingly to reflect the new vertices for dashes
GLuint indices[] = {
    // Central scuar divider
    0, 1, 2,  // First Triangle for center divider
    2, 3, 0,  // Second Triangle for center divider

    // Dashed lane divider left
    4, 5, 6,  // First Triangle for left dash 1
    6, 7, 4,  // Second Triangle for left dash 1

    8, 9, 10, // First Triangle for left dash 2
    10, 11, 8, // Second Triangle for left dash 2

    12, 13, 14, // First Triangle for left dash 3
    14, 15, 12, // Second Triangle for left dash 3

    16, 17, 18, // First Triangle for left dash 4
    18, 19, 16, // Second Triangle for left dash 4

    20, 21, 22, // First Triangle for left dash 5
    22, 23, 20, // Second Triangle for left dash 5

    24, 25, 26, // First Triangle for left dash 6
    26, 27, 24, // Second Triangle for left dash 6

    // Dashed lane divider right
    28, 29, 30, // First Triangle for right dash 1
    30, 31, 28, // Second Triangle for right dash 1

    32, 33, 34, // First Triangle for right dash 2
    34, 35, 32, // Second Triangle for right dash 2

    36, 37, 38, // First Triangle for right dash 3
    38, 39, 36, // Second Triangle for right dash 3

     40, 41, 42, // First Triangle for right dash 4
     42, 43, 40, // Second Triangle for right dash 4

     44, 45, 46, // First Triangle for right dash 5
     46, 47, 44, // Second Triangle for right dash 5

     48, 49, 50, // First Triangle for right dash 6
     50, 51, 48  // Second Triangle for right dash 6
};

//DRIVEABLE CAR - Red Car Vertices
GLfloat carVertices[] = {
    // Car body
0.6f,  -0.5f, 0.0f,   // top left 
0.95f,  -0.5f, 0.0f,   // top right 
0.95f, -0.7f, 0.0f,   // bottom right 
0.6f, -0.7f, 0.0f,   // bottom left 

//// Car wheels
//-0.6f, -0.1f, 0.0f,   // top left 
//-0.5f, -0.1f, 0.0f,   // top right 
//-0.5f, -0.2f, 0.0f,   // bottom right 
//-0.6f, -0.2f, 0.0f,   // bottom left 
//
// 0.6f, -0.1f, 0.0f,   // top left 
// 0.5f, -0.1f, 0.0f,   // top right 
// 0.5f, -0.2f, 0.0f,   // bottom right 
// 0.6f, -0.2f,
};
GLuint carIndices[] = {
    // Car body
    0, 1, 2,  // First Triangle for car body
    2, 3, 0,  // Second Triangle for car body

    // Car wheels
    4, 5, 6,  // First Triangle for left wheel
    6, 7, 4,  // Second Triangle for left wheel

    8, 9, 10, // First Triangle for right wheel
    10, 11, 8 // Second Triangle for right wheel
};
void initRoad() {
    glGenVertexArrays(1, &vaoRoad);
    glGenBuffers(1, &vboRoad);
    glGenBuffers(1, &iboRoad);

    glBindVertexArray(vaoRoad);

    glBindBuffer(GL_ARRAY_BUFFER, vboRoad);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboRoad);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}
void initCar() {
    glGenVertexArrays(1, &vaoCar);
    glGenBuffers(1, &vboCar);
    glGenBuffers(1, &iboCar);

    glBindVertexArray(vaoCar);

    glBindBuffer(GL_ARRAY_BUFFER, vboCar);
    glBufferData(GL_ARRAY_BUFFER, sizeof(carVertices), carVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboCar);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(carIndices), carIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

//Drawing of the road
void drawRoad() {
    glBindVertexArray(vaoRoad);

    unsigned int transformLoc = glGetUniformLocation(programID, "transform");
    unsigned int colorLoc = glGetUniformLocation(programID, "color");

    glm::mat4 trans(1.0f);
    glm::vec4 color = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);  // Grey color for road

    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
    glUniform4fv(colorLoc, 1, glm::value_ptr(color));

    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

}
//Drawing of car
void drawCar() {
    glBindVertexArray(vaoCar);

    unsigned int transformLoc = glGetUniformLocation(programID, "transform");
    unsigned int colorLoc = glGetUniformLocation(programID, "color");

    glm::mat4 carTrans = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    glm::vec4 carColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);  // Red color for car

    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(carTrans));
    glUniform4fv(colorLoc, 1, glm::value_ptr(carColor));

    glDrawElements(GL_TRIANGLES, sizeof(carIndices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);


}
//Mouse button callback
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        carTrans = glm::translate(carTrans, glm::vec3(0.1f, 0.0f, 0.0f));
		//Includes showing of center of the red car
        std::cout << "Red Car center: " << carTrans[3][0] + 0.775f << "    "<< carTrans[3][1] - 0.6f <<std::endl;
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        carTrans = glm::translate(carTrans, glm::vec3(-0.1f, 0.0f, 0.0f));
		//Includes showing of center of the red car
        std::cout << "Red Car center: " << carTrans[3][0] + 0.775f << "    " << carTrans[3][1] - 0.6f << std::endl;

    }

}

//Functions to show the center of the red car (X center, Y center)
float red_car_X_center(GLFWwindow* window, int button, int action, int mods)
{
        carTrans = glm::translate(carTrans, glm::vec3(0.0f, 0.0f, 0.0f));
        //std::cout << "Red Car center: " << carTrans[3][0] + 0.775f << "    " << carTrans[3][1] - 0.6f << std::endl;
        return carTrans[3][0] + 0.775f;
}
float red_car_Y_center(GLFWwindow* window, int button, int action, int mods)
{
    carTrans = glm::translate(carTrans, glm::vec3(0.0f, 0.0f, 0.0f));
    //std::cout << "Red Car center: " << carTrans[3][0] + 0.775f << "    " << carTrans[3][1] - 0.6f << std::endl;
    return carTrans[3][1] - 0.6f;
}

// Initialize transformation matrix
glm::mat4 trans(1.0f);
// Window resize callback
void window_callback(GLFWwindow* window, int new_width, int new_height) {
    glViewport(0, 0, new_width, new_height);
}

// Main function
int main(void) {
    // Initialize GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(width, height, "Lab 4", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window.");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        glfwTerminate();
        return -1;
    }

    // Specify the size of the rendering window
    glViewport(0, 0, width, height);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

    initRoad();
    initCar();

    initRand();
    initRand1();

    // Set callbacks
    glfwSetFramebufferSizeCallback(window, window_callback);

    // Variables for positions
    const int numSegments = 6; // Number of segments for continuous effect
    std::vector<float> positions(numSegments);
    float speed = 0.0005f; // Speed of movement

    // Initialize positions
    for (int i = 0; i < numSegments; ++i) {
        positions[i] = -1.0f + i * (2.0f / numSegments); // Distribute segments vertically
    }
    int key = GLFW_KEY_ESCAPE;
    const int numSegmentsR = 2;
    std::vector<float> positionsR(numSegmentsR);
    float speedR = 0.00005f; // Viteza de mișcare în jos a paritlor de drum
    

    // Inițializează pozițiile segmentelor pentru drum
    for (int j = 0; j < numSegmentsR; ++j) {
        positionsR[j] = -1.0f + j * (2.0f / numSegmentsR);
    }

    const int numSegmentsR1 = 2;
    std::vector<float> positionsR1(numSegmentsR1);
    float speedR1 = 0.0009f; // Viteza de mișcare în jos a paritlor de drum

    // Inițializează pozițiile segmentelor paritlor de drum
    for (int k = 0; k < numSegmentsR1; ++k) {
        positionsR1[k] = -1.0f + k * (2.0f / numSegmentsR1);
    }

    glfwSetMouseButtonCallback(window, mouse_button_callback);

	//Declare multiple  variables for the program
	//all variables are initialising with precalculated values
    //that suit our needs
    glm::mediump_float someFloat = 0.0f;
    glm::mediump_float someFloat0 = 0.0f;
    int random_var;
    int random_var0;
    glm::mediump_float y_counter_downwords = -0.00f;
    glm::mediump_float y_counter_downwords_blue = 0.30f; 
	glm::mediump_float x_random_green_car = 0.1f;
	glm::mediump_float x_random_blue_car = 0.1f;
	int game_over = 0;

    //WHILE (running game)
    while (!glfwGetKey(window, GLFW_KEY_ESCAPE) && !glfwWindowShouldClose(window)) {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);
        //start game on ENTER pressed
        if (showStartScreen) {
            drawStartScreen();
            glfwSwapBuffers(window);
            glfwPollEvents();
            if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
                showStartScreen = false;
            }
            continue;
        }
        // Use our shader
        glUseProgram(programID);

        // Draw the road (stationary)
        for (int i = 0; i < numSegments; ++i) {
            // Create transformation matrix for each segment (no movement for road)
            glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, positions[i], 0.0f));
            // Move upwards
            positions[i] += speed;

            // Wrap-around logic
            if (positions[i] > 1.0f) {
                positions[i] -= 2.0f; // Reset position to the bottom for continuous effect
            }

            // Send the transformation matrix to the shader
            unsigned int transformLoc = glGetUniformLocation(programID, "transform");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

            // Set the color for each segment
            unsigned int colorLoc = glGetUniformLocation(programID, "color");
            glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // White color for the road
            glUniform4fv(colorLoc, 1, glm::value_ptr(color));

            // Draw the road
            drawRoad();

        }

        // GREEN CAR LOGIC -  right road side   
        {
            glm::mat4 transRand = glm::translate(glm::mat4(1.0f), glm::vec3(x_random_green_car, y_counter_downwords, 0.0f));
            
            unsigned int transformLoc1 = glGetUniformLocation(programID, "transform");
            glUniformMatrix4fv(transformLoc1, 1, GL_FALSE, glm::value_ptr(transRand));
            unsigned int colorLoc1 = glGetUniformLocation(programID, "color");
            glm::vec4 color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f); // Green color for the car
            glUniform4fv(colorLoc1, 1, glm::value_ptr(color));

            drawRand();

			//Save X,Y green car center coordonates
            float green_car_center[2] = { transRand[3][0] + 0.7f, (transRand[3][1] - 0.1f)+1.2 };
            //Save X,Y reed car center coordonates
            float x_center_red = red_car_X_center(window, key, 0, 0);
            float y_center_red = red_car_Y_center(window, key, 0, 0);

			//can be  used for  debugging
           // std::cout << green_car_center[0] << "    " << green_car_center[1] << std::endl;

			//CHECK IF CENTERS ARE TOO CLOSE (COLLISION DETECTED)
            if (abs(x_center_red - green_car_center[0]) <= 0.3 && abs(y_center_red - green_car_center[1]) <= 0.1)
            {
				std::cout << "X difference: "<< x_center_red - green_car_center[0] << " Y difference:   " << y_center_red - green_car_center[1] << std::endl;
                std::cout << "COLLISION DETECTED" << std::endl;
                std::cout << x_center_red << "    " << y_center_red << std::endl;
                std::cout << green_car_center[0] << "    " << green_car_center[1] << std::endl;
             
                // Replace sleep(1000) with the following code
                std::this_thread::sleep_for(std::chrono::milliseconds(3000));
                break;

            }
			//standard car speed
            y_counter_downwords -= 0.004f;
            //speed up, slow down green car
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            {
                y_counter_downwords -= 0.0041f;
            }
            else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            {
                y_counter_downwords += 0.00048f;
            }

			//respawn green car (after quitting the screen)
            if (y_counter_downwords < -2.6f) {
                y_counter_downwords = 0.0f;
                int random_var = rand() % 10;
                if (random_var % 2 == 0)
                    x_random_green_car = -0.42f;
                else
					x_random_green_car = 0.1f;
            }
        }

		//BLUE CAR LOGIC - left road side
        {
            glm::mat4 transRand1 = glm::translate(glm::mat4(1.0f), glm::vec3(x_random_blue_car, y_counter_downwords_blue, 0.0f));
         
            unsigned int transformLoc1 = glGetUniformLocation(programID, "transform");
            glUniformMatrix4fv(transformLoc1, 1, GL_FALSE, glm::value_ptr(transRand1));
            unsigned int colorLoc1 = glGetUniformLocation(programID, "color");
            glm::vec4 color = glm::vec4(0.0f, 1.0f, 1.0f, 0.5f); // bluecolor for the car
            glUniform4fv(colorLoc1, 1, glm::value_ptr(color));

            drawRand1();

            //CENTRU BLUE CAR
            float blue_car_center[2] = { transRand1[3][0] + -0.7f, (transRand1[3][1] - 0.1f) + 0.8 };
            //CENTRU RED CAR
            float x_center_red = red_car_X_center(window, key, 0, 0);
            float y_center_red = red_car_Y_center(window, key, 0, 0);

            std::cout << blue_car_center[0] << "    " << blue_car_center[1] << std::endl;
			//COLIZIUNE (based on center of the cars)
            if (abs(x_center_red - blue_car_center[0]) <= 0.3 && abs(y_center_red - blue_car_center[1]) <= 0.1)
            {
                std::cout << "X difference BLUE: " << x_center_red - blue_car_center[0] << " Y difference BLUE:   " << y_center_red -blue_car_center[1] << std::endl;
                std::cout << "COLLISION DETECTED BLUE CAR" << std::endl;
                std::cout << x_center_red << "    " << y_center_red << std::endl;
                std::cout << blue_car_center[0] << "    " << blue_car_center[1] << std::endl;

                // Replace sleep(1000) with the following code
                std::this_thread::sleep_for(std::chrono::milliseconds(3000));
                break;

            }
			//SPEED OF BLUE CAR
            y_counter_downwords_blue -= 0.013f;
            //speed up, slow down blue car
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            {
                y_counter_downwords_blue -= 0.0041f;
            }
            else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            {
                y_counter_downwords_blue += 0.00048f;
            }

            //respawn masina ALBASTRA
            if (y_counter_downwords_blue < -2.6f) {
                y_counter_downwords_blue = 0.3f;
                int random_var = rand() % 10;
                if (random_var % 2 == 0)
                    x_random_blue_car = 0.45f;
                else
                    x_random_blue_car = 0.0f;
                
            }
        }

        // Transmite matricea de transformare la shader pentru mașină
        {
        unsigned int transformLoc = glGetUniformLocation(programID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(carTrans));
        unsigned int colorLoc = glGetUniformLocation(programID, "color");
        glm::vec4 carColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); // Red color for the car
        glUniform4fv(colorLoc, 1, glm::value_ptr(carColor));
        drawCar();
    }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // Cleanup
    {
        glDeleteBuffers(1, &vboRoad);
        glDeleteBuffers(1, &iboRoad);
        glDeleteVertexArrays(1, &vaoRoad);
        glDeleteProgram(programID);

        glDeleteBuffers(1, &vboCar);
        glDeleteBuffers(1, &iboCar);
        glDeleteVertexArrays(1, &vaoCar);
    }

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}