#ifndef SHADER_H
#define STB_IMAGE_IMPLEMENTATION
#define SHADER_H

#include <GL/glew.h>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdio>

#include <mouse.h>
#include <glm/glm.hpp>

#include <stb_image.h>
#include <iostream>
#include <string>
#include <vector>
#include <variant>

#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    unsigned int ID;
   
    Shader(const char* vertexPath, const char* fragmentPath)
    {
       
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        

        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            
            vShaderFile.close();
            fShaderFile.close();
            
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        
        unsigned int vertex, fragment;
       
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        
        glDeleteShader(vertex);
        glDeleteShader(fragment);

    }
   
    void use() const
    {
        glUseProgram(ID);
    }
    // ------------------------------------------------------------------------
    void setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string& name, const glm::vec2& value) const
    {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec2(const std::string& name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string& name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec3(const std::string& name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string& name, const glm::vec4& value) const
    {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string& name, float x, float y, float z, float w) const
    {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string& name, const glm::mat2& mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string& name, const glm::mat3& mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setSample2D(const std::string& name, const char* image, int loc)
    {
        int width, height, nrChannels;
        unsigned char* data = stbi_load(image, &width, &height, &nrChannels, 0);
        if (data) {

      
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Load image data into texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        //glGenerateMipmap(GL_TEXTURE_2D);
        // Bind texture to uniform
        //GLint textureUniform = glGetUniformLocation(shaderProgram, "sampler2D");
        glUniform1i(glGetUniformLocation(ID, name.c_str()), 0); // 0 is the texture unit that the texture is bound to
        glActiveTexture(GL_TEXTURE0 + loc);
        glBindTexture(GL_TEXTURE_2D, texture);
        }
        else {
            std::cout << "Failed to load Texture" << std::endl;
        }
        // Free image data from memory
        stbi_image_free(data);

        
        
       
    }
    // ------------------------------------------------------------------------
    // Model Transforms
    // ------------------------------------------------------------------------
    // Projection Transforms
    // ------------------------------------------------------------------------
    // PBR

private:
   
    void checkCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};



using namespace std;
class ShaderBuilder 
{
public:

    int lineNum = 1;
    int layoutSpace = 2;
    int variableSpace = 3;
    int outSpace = 4;
    int numOfVars = 0;
    vector<string> variables;
    vector<string> variableTypes;
    
    ShaderBuilder() {
        ofstream file("C:\\OpenGLS\\Project\\Project1\\ShaderWriter\\src\\Tests\\Test.txt");
        string message = "#version 460 core";
        file << message << endl;
        file.close();
        appendLine("//Layouts\n//Ins\n//Uniforms\n//Outs\nvoid main()\n{\n//END\n}");
        initVertexShader(true);
        //FragPos = vec3(model * vec4(aPos, 1.0));
        //setVariable("FragPos", multiply("model", "aPos"));
        //setVariable("gl_Position", multiply("projection", "model", "view","aPos"));
        //setVariable("gl_Position", add("projection", "model", "view", "aPos"));
        //setVariable("gl_Position", multiply("projection", "model", "view", 1));
       
        
    }
    
    // Add operator Override  would make this more user-friendly
   

    void createPerspective() {
        createVariable("mat4", "model");
        createVariable("mat4", "view");
        createVariable("mat4", "projection");
    }

    void addDistributionGGX() {

    }

    void initVertexShader(bool textures = false) {

        createLayout("vec3", "aPos", 0);
        createLayout("vec3", "aNormal", 1);

        createOutput("vec3", "FragPos");
        createOutput("vec3", "Normal");

        createPerspective();
        setVariable("FragPos", multiply("model", "aPos"));
        setVariable("Normal", multiply(convertTo("mat3", transpose(inverse("model"))), "aNormal"));
        if (textures == true) {
            createLayout("vec2", "aTexCoords", 2);
            createOutput("vec2", "TexCoords");
            setVariable("TexCoords", "aTexCoords");
        }
        setVariable("gl_Position", multiply("projection", "view", "model", "aPos"));
        
    }

    string convertTo(string type, string command) {
        if (type.compare("vec4") == 0) {
            return "vec4(" + command + " 1.0)";
        }
        else if (type.compare("mat3") == 0) {
            return "mat3(" + command + ")";
        }
    }

    string transpose(string variable) {
        return "transpose(" + variable + ")";
    }

    string inverse(string variable) {
        return "inverse(" + variable + ")";
    }


    template<typename ... Args>
    string multiply(Args ... args) 
    {
        
        bool mat4 = false;
        stringstream ss;
        string op;
        vector<variant<float, string>> argsList{ args... };
        vector<int> location;


        for (const auto& i : argsList) {
            location.push_back(findVar(i));
        }

        for (int i = 0; i < argsList.size(); i++) {
            std::cout << get<string>(argsList[i]) << endl;
            std::cout << location[i] << endl;
            // Data type conversions

            if (location[i] >= 0) {
                if (variableTypes[location[i]].compare("mat4") == 0) {
                    mat4 = true;
                    std::cout << "true" << endl;
                }
            }

        }

        for (int i = 0; i < argsList.size(); i++) {
            if (i == 0) {
                op = " ";
            }
            else {
                op = " * ";
            }
            if (holds_alternative<string>(argsList[i])) {

                if (location[i] >= 0) {
                    if (variableTypes[location[i]] == "vec3" && mat4 == true) {

                        ss << op;
                        ss << "vec4(" + get<string>(argsList[i]) + ", 1.0)";
                    }
                    else {

                        ss << op;
                        ss << get<string>(argsList[i]);
                    }
                }
                else {
                    //something variable not found
                    ss << op;
                    ss << get<string>(argsList[i]);
                }
            }
            else {
                ss << op;
                ss << to_string(get<float>(argsList[i]));
            }
        }

        return ss.str();
    }

    template<typename ... Args>
    string add(Args ... args)
    {

        bool mat4 = false;
        stringstream ss;
        string op;
        vector<variant<float, string>> argsList{ args... };
        vector<int> location;


        for (const auto& i : argsList) {
            location.push_back(findVar(i));
        }

        for (int i = 0; i < argsList.size(); i++) {
            std::cout << get<string>(argsList[i]) << endl;
            std::cout << location[i] << endl;
            // Data type conversions

            if (location[i] >= 0) {
                if (variableTypes[location[i]].compare("mat4") == 0) {
                    mat4 = true;
                    std::cout << "true" << endl;
                }
            }
           
        }
       
        for (int i = 0; i < argsList.size(); i++) {
            if (i == 0) {
                op = " ";
            }
            else {
                op = " + ";
            }
            if (holds_alternative<string>(argsList[i])) {
                
                if (location[i] >= 0) {
                    if (variableTypes[location[i]] == "vec3" && mat4 == true) {

                        ss << op;
                        ss << "vec4(" + get<string>(argsList[i]) + ", 1.0)";
                    }
                    else {

                        ss << op;
                        ss << get<string>(argsList[i]);
                    }
                }
                else {
                    // variable not found?
                    ss << op;
                    ss << get<string>(argsList[i]);
                }
            }
            else {
                ss << op;
                ss << to_string(get<float>(argsList[i]));
            }
        }

        return ss.str();
    }
    template<typename ... Args>
    string divide(Args ... args)
    {

        bool mat4 = false;
        stringstream ss;
        string op;
        vector<variant<float, string>> argsList{ args... };
        vector<int> location;


        for (const auto& i : argsList) {
            location.push_back(findVar(i));
        }

        for (int i = 0; i < argsList.size(); i++) {
            std::cout << get<string>(argsList[i]) << endl;
            std::cout << location[i] << endl;
            // Data type conversions

            if (location[i] >= 0) {
                if (variableTypes[location[i]].compare("mat4") == 0) {
                    mat4 = true;
                    std::cout << "true" << endl;
                }
            }

        }

        for (int i = 0; i < argsList.size(); i++) {
            if (i == 0) {
                op = " ";
            }
            else {
                op = " / ";
            }
            if (holds_alternative<string>(argsList[i])) {

                if (location[i] >= 0) {
                    if (variableTypes[location[i]] == "vec3" && mat4 == true) {

                        ss << op;
                        ss << "vec4(" + get<string>(argsList[i]) + ", 1.0)";
                    }
                    else {

                        ss << op;
                        ss << get<string>(argsList[i]);
                    }
                }
                else {
                    // variable not found?
                    ss << op;
                    ss << get<string>(argsList[i]);
                }
            }
            else {
                ss << op;
                ss << to_string(get<float>(argsList[i]));
            }
        }

        return ss.str();
    }

    template<typename ... Args>
    string minus(Args ... args)
    {

        bool mat4 = false;
        stringstream ss;
        string op;
        vector<variant<float, string>> argsList{ args... };
        vector<int> location;


        for (const auto& i : argsList) {
            location.push_back(findVar(i));
        }

        for (int i = 0; i < argsList.size(); i++) {
            std::cout << get<string>(argsList[i]) << endl;
            std::cout << location[i] << endl;
            // Data type conversions

            if (location[i] >= 0) {
                if (variableTypes[location[i]].compare("mat4") == 0) {
                    mat4 = true;
                    std::cout << "true" << endl;
                }
            }

        }

        for (int i = 0; i < argsList.size(); i++) {
            if (i == 0) {
                op = " ";
            }
            else {
                op = " - ";
            }
            if (holds_alternative<string>(argsList[i])) {

                if (location[i] >= 0) {
                    if (variableTypes[location[i]] == "vec3" && mat4 == true) {

                        ss << op;
                        ss << "vec4(" + get<string>(argsList[i]) + ", 1.0)";
                    }
                    else {

                        ss << op;
                        ss << get<string>(argsList[i]);
                    }
                }
                else {
                    // variable not found?
                    ss << op;
                    ss << get<string>(argsList[i]);
                }
            }
            else {
                ss << op;
                ss << to_string(get<float>(argsList[i]));
            }
        }

        return ss.str();
    }

    template<typename T>
    int findVar(T variable) {

        if (variable.index() == 1) {
            string type = get<string>(variable);
            std::vector<string>::iterator it = std::find(variables.begin(), variables.end(), type);
            if (it != variables.end()) {
                return distance(variables.begin(), it);
            }
            return -52;
        }
        else if (variable.index() == 0) {
            return -1;
        }
        else {
            return 100;
        }
        
        
      
       
    }
    //template<typename T>
    int findVar(string variable)
    {

        std::vector<string>::iterator it = std::find(variables.begin(), variables.end(), variable);
        if (it != variables.end()) {
            return distance(variables.begin(), it);
        }
        else {
            return -1;
        }
    }
    int findVar(float variable) {return -1;}
    

    void setVariable(string name, string operand)
    {
        if (name == "gl_Position") {
            string line = "\t" + name + " = " + "vec4" + "(" + operand + ")" + ";";
            insertBefore("End", line);
            return;
        }
        int location0 = findVar(name);
        // Add error for unfound var
        string line = "\t" + name + " = " + variableTypes[location0] + "(" + operand + ")" + ";";
        
        insertBefore("End", line);
    }

    void setNewVariable(string type, string name, string operand)
    {
        int location0 = findVar(name);
        string line = "\t" + type + " " + name + " = " + variableTypes[location0] + "(" + operand + ")" + ";";
        // Add conversion to variable type
        insertBefore("End", line);
    }
    

    void createLayout(string type, string name, int location)
    {
        string line = "layout (location = " + to_string(location) + ") in " + type + " " + name + ";";
        
        variables.push_back(name);
        variableTypes.push_back(type);
        insertAfter("Layouts", line);
    }

    void createVariable(string type,string name)
    {
        string line = "uniform " + type + " " + name + ";";
        variables.push_back(name);
        variableTypes.push_back(type);
        insertAfter("Uniforms", line);
    }

    void createOutput(string type, string name)
    {
        string line = "out " + type + " " + name + ";";
        variables.push_back(name);
        variableTypes.push_back(type);
        insertAfter("Outs", line);
    }

    void createInput(string type, string name)
    {
        string line = "in " + type + " " + name + ";";
        variables.push_back(name);
        variableTypes.push_back(type);
        insertAfter("Ins", line);
    }
    

    void appendLine(string command)
    {
        lineNum += 1;
        ofstream file("C:\\OpenGLS\\Project\\Project1\\ShaderWriter\\src\\Tests\\Test.txt", ios::app); //open the file in append mode
        string message = command;
        file << message << endl;
        file.close();
    }

    void insertLine(string command, int line_number)
    {
        fstream file("C:\\OpenGLS\\Project\\Project1\\ShaderWriter\\src\\Tests\\Test.txt");
        stringstream buffer;
        string line;
        int current_line = 0; // variable to keep track of the current line number

        while (getline(file, line)) {
            current_line++; // increment the current line number
            if (current_line == line_number) {
                buffer << command << endl; // insert the new line at the specified line number
            }
            buffer << line << endl; // add the existing lines to the buffer
        }

        file.close();

        // truncate the original file and open it in output mode
        file.open("C:\\OpenGLS\\Project\\Project1\\ShaderWriter\\src\\Tests\\Test.txt", ios::out | ios::trunc);

        // write the contents of the buffer to the file
        file << buffer.rdbuf();
        file.close();
    }

    void insertAfter(string word, string insert)
    {
        string targetWord = word;
      
        ifstream file("C:\\OpenGLS\\Project\\Project1\\ShaderWriter\\src\\Tests\\Test.txt");
        ofstream tempFile("C:\\OpenGLS\\Project\\Project1\\ShaderWriter\\src\\Tests\\temp.txt");

        string line;
        while (getline(file, line)) {
            tempFile << line << endl;
            if (line.find(targetWord) != string::npos) {
                tempFile << insert << endl;
            }
        }

        file.close();
        tempFile.close();
        remove("C:\\OpenGLS\\Project\\Project1\\ShaderWriter\\src\\Tests\\Test.txt");
        rename("C:\\OpenGLS\\Project\\Project1\\ShaderWriter\\src\\Tests\\temp.txt", "C:\\OpenGLS\\Project\\Project1\\ShaderWriter\\src\\Tests\\Test.txt");
    }

    void insertBefore(string word, string insert)
    {
        
        std::string fileName = "C:\\OpenGLS\\Project\\Project1\\ShaderWriter\\src\\Tests\\Test.txt";
        int lineNumber = 0;
        bool wordFound = false;

        std::ifstream file(fileName);
        std::string line;
        while (std::getline(file, line)) {
            lineNumber++;
            if (line.find(word) != std::string::npos) {
                std::cout << "The word '" << word << "' was found on line " << lineNumber << std::endl;
                wordFound = true;
                break;
            }
        }
        lineNumber--;
        insertLine(insert, lineNumber);

    }
};


class ShaderType
{
public:
    string Type;
    string Name;

    ShaderType(string type, string name) {
        Type = type;
        Name = name;
    }
};

/*
class BPShader 
{
public:

    unsigned int ID;
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;

    glm::vec3 Normal;
    glm::vec3 WorldPos;
    glm::vec2 TexCoords;

    BPShader(const char* vertexPath, const char* fragmentPath)
    {
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;


        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {

            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;

            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            vShaderFile.close();
            fShaderFile.close();

            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        unsigned int vertex, fragment;

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");


        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");


        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void VertexShader(glm::vec3 aNormal, glm::vec3 aPos, glm::vec2 aTexCoords)
    {
        Normal = glm::mat3(glm::transpose(glm::inverse(model))) * aNormal;
        WorldPos = glm::vec3(model * glm::vec4(aPos, 1.0));
        TexCoords = aTexCoords;

        glm::vec4 gl_pos = projection * view * model * glm::vec4(aPos, 1.0);

        GLint gl_PositionLocation = glGetUniformLocation(ID, "gl_Position");
        glUniform4fv(gl_PositionLocation, 1, &gl_pos[0]);
    }

    void FragmentShader(glm::vec3 ambientColor, glm::vec3 objectColor, glm::vec3 lightPos,glm::vec3 cameraPos, glm::vec3 lightColor )
    {
        // Ambient term
        glm::vec3 ambient = ambientColor * objectColor;
        // Calculate lighting
        glm::vec3 lightDir = glm::normalize(lightPos - WorldPos);
        glm::vec3 viewDir = glm::normalize(cameraPos - WorldPos);
        glm::vec3 halfwayDir = normalize(lightDir + viewDir);
        
        // Diffuse term
        float diff = glm::max(glm::dot(Normal, lightDir), 0.0f);
        glm::vec3 diffuse = diff * lightColor * objectColor;

        // Specular term
        float spec = glm::pow(glm::max(glm::dot(Normal, halfwayDir), 0.0f), 32.0f);
        glm::vec3 specular = spec * lightColor;

        // Final color output
        glm::vec4 FragColor = glm::vec4(ambientColor * objectColor + diffuse + specular, 1.0);

        GLint gl_PositionLocation = glGetUniformLocation(ID, "FragColor");
        glUniform4fv(gl_PositionLocation, 1, &FragColor[0]);
    }

private:

    void checkCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }

};
*/
#endif