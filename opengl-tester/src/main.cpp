#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <chrono>
#include <math.h>

using namespace std;

const char* vertexShaderSrc = R"glsl(
    #version 330 core
    layout (location = 0) in vec2 aPos;

    void main() {
        gl_Position = vec4(aPos, 0.0, 1.0);
    }
)glsl";

const char* fragmentShaderSrc = R"glsl(
    #version 330 core
    out vec4 FragColor;

    precision highp int;
    precision highp float;

    uniform int width;
    uniform int height;
    uniform int iops;
    uniform int flops;
    
    uniform int currentTest;

    void main() {
        int x = int(gl_FragCoord.x);
        int y = int(gl_FragCoord.y);
        
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);

        // test performance of iops
        if(currentTest == 0){
            int a = 1345751839;
            int c = -899462753;
            for(int i=0;i<iops;i++){
                a *= c;
            }
            FragColor = vec4(float(a), float(a), float(a), 1.0);
            return;
        }
        
        // test performance of flops
        if(currentTest == 1){
            float a = 0.4;
            float c = (x / width) * (y / height);
            for(int i=0;i<flops;i++){
                a *= c;
            }
            FragColor = vec4(a, a, a, 1.0);
            return;
        }
        
        // test precision of ints
        if(x < 400 && y < 400 && x % 50 < 40 && y % 50 < 40){
            int t = (y / 50) * 8 + (x / 50);
            int num = 1;
            bool flag = true;
            for(int i=0;i<t-1;i++){
                int n = num;
                num *= 2;
                // check if num*2 is not greater than num (if so, we've reached last bit of num)
                if(n >= num){
                    flag = false; break;
                }
            }
            if(flag){
                FragColor = vec4(0.0, 1.0, 0.0, 1.0);
            }else{
                FragColor = vec4(1.0, 0.0, 0.0, 1.0);
            }
        }
        
        // test precision of floats
        if(x >= 500 && x < 900 && y < 400 && x % 50 < 40 && y % 50 < 40){
            int t = (y / 50) * 8 + ((x - 500) / 50);
            float num = 1.0;
            float m = 0.5;
            bool flag = true;
            for(int i=0;i<=t;i++){
                num += m;
                // check if the difference in consecutive values rounds to 0 (if so, we've reached last bit of mantissa)
                if(num >= num + m || num <= num - m){
                    flag = false; break;
                }
                m /= 2.0;
            }
            if(flag){
                FragColor = vec4(0.0, 1.0, 0.0, 1.0);
            }else{
                FragColor = vec4(1.0, 0.0, 0.0, 1.0);
            }
        }
    }
)glsl";

unsigned int compileShader(GLenum type, const char* src) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        cerr << "Shader compilation failed: " << infoLog << "\n";
    }
    return shader;
}

GLFWwindow* window;
int width = 1000, height = 1600;

unsigned int VBO, VAO = 0;
unsigned int vertexShader = 0;
unsigned int fragmentShader = 0;
unsigned int shaderProgram = 0;

double mouseX = 0.0;
double mouseY = 0.0;

int iops = 4000;
int flops = 4000;

long long frameDuration = 0;
long long frame = 0;

int currentTest = 0;
int previousTest = 0;

bool previousClick = 0;

void setup(){
    width = 1000, height = 600;

    if (!glfwInit()) {
        cerr << "Failed to initialize GLFW.\n";
        exit(-1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Window", nullptr, nullptr);
    if (!window) {
        cerr << "Failed to create window.\n";
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cerr << "Failed to initialize GLAD.\n";
        exit(-1);
    }

    // Compile shaders
    vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSrc);
    fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);

    // Link shader program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check link status
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        cerr << "Program linking failed: " << infoLog << "\n";
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void processInput(){
    glfwGetCursorPos(window, &mouseX, &mouseY);

    if(glfwGetKey(window, 87)){
        
    }
    if(glfwGetKey(window, 65)){
        
    }
    if(glfwGetKey(window, 83)){
        
    }
    if(glfwGetKey(window, 68)){
        
    }
    if(glfwGetMouseButton(window, 0)){
        if(!previousClick){
            currentTest += 1;
            currentTest %= 3;
        }
        previousClick = 1;
    }else{
        previousClick = 0;
    }
    if(glfwGetMouseButton(window, 1)){
        
    }
}

void appLoop(){

    processInput();

    glClear(GL_COLOR_BUFFER_BIT);

    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);


    glUseProgram(shaderProgram);
    glUniform1f(glGetUniformLocation(shaderProgram, "width"), (float)width);
    glUniform1f(glGetUniformLocation(shaderProgram, "height"), (float)height);
    glUniform1i(glGetUniformLocation(shaderProgram, "iops"), iops);
    glUniform1i(glGetUniformLocation(shaderProgram, "flops"), flops);
    glUniform1i(glGetUniformLocation(shaderProgram, "currentTest"), currentTest);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

int main() {

    setup();

    // Fullscreen quad
    float vertices[] = {
        -1.0f,  1.0f,
        -1.0f, -1.0f,
         1.0f, -1.0f,
        -1.0f,  1.0f,
         1.0f, -1.0f,
         1.0f,  1.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    long long testFrames = 0;
    long long testDuration = 0;
    auto testMarker = chrono::high_resolution_clock::now();

    while (!glfwWindowShouldClose(window)) {
        
        auto frameStart = chrono::high_resolution_clock::now();
        
        appLoop();

        auto frameEnd = chrono::high_resolution_clock::now();

        chrono::nanoseconds frameDiff = frameEnd - frameStart;
        frameDuration = (long long)frameDiff.count();

        if(currentTest != previousTest){

            chrono::nanoseconds testDiff = chrono::high_resolution_clock::now() - testMarker;
            testDuration = (long long)testDiff.count();
            
            double seconds = (double)testDuration / 1000000000.0;
            double fps = (double)testFrames / seconds;
            double pixels = (double)width * (double)height;
            double pps = fps * pixels;

            switch(currentTest){
                case 1:{
                double ops = pps * (double)iops;
                cout << "Integer operation test results: " << testFrames << " frames, " << testDuration << " ns, "
                << seconds << " seconds, " << fps << " fps, " << pixels << " pixels, " << pps << " pixels/s, " << ops << " integer operations/s.\n";
                }
                break;
                case 2:{
                double ops = pps * (double)flops;
                cout << "Float operation test results: " << testFrames << " frames, " << testDuration << " ns, "
                << seconds << " seconds, " << fps << " fps, " << pixels << " pixels, " << pps << " pixels/s, " << ops << " float operations/s.\n";
                }
                break;
            }

            testFrames = 0;
            testDuration = 0;
        
            testMarker = chrono::high_resolution_clock::now();
        }

        previousTest = currentTest;
        frame++;
        testFrames++;
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}