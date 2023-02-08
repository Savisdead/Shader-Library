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

#include <map>
#include <fstream>
#include <sstream>
#include <iostream>


#include<ShaderType.h>
#include <Function.h>
#include <common.h>

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


bool openFunc = false;
std::string currentFunc = "main";

using namespace std;
class ShaderBuilder 
{
public:

    int lineNum = 1;
    int layoutSpace = 2;
    int variableSpace = 3;
    int outSpace = 4;
    int numOfVars = 0;
    //vector<string> variables;
    //vector<string> variableTypes;
    
    vector<ShaderType> variables;
    vector<Function> functions;
    vector<ShaderType> funcVariables;
    
    ShaderBuilder() {

        ofstream file("C:\\OpenGLS\\Project\\Project1\\ShaderWriter\\src\\Tests\\Test.txt");
        string message = "#version 460 core";
        file << message << endl;
        file.close();
        appendLine("//Layouts\n//Ins\n//Uniforms\n//Outs\n//Functions\nvoid main()\n{\n//END\n}");

        initVertexShader(true);
       
        createFunctionDef("int", "add");
        func("add").addParameter("vec3", "direction");
        func("add").addParameter("float", "magnitude");
        func("add").addParameter("vec4", "ortho");
        closeFunctionDef("add");

        openFunction("add");

        newVar("mat4", "longview") = var("view") * 3.0;
        stateReturn(var("model") * var("longview"));

        closeFunction();

        addDistributionGGX();
        
    }

    /* ---------------------- Simple Commands ------------------------------ */
    
    void createPerspective() {
        createVariable("mat4", "model");
        createVariable("mat4", "view");
        createVariable("mat4", "projection");
    }
    
    void initVertexShader(bool textures = false) {

        createLayout("vec3", "aPos", 0);
        createLayout("vec3", "aNormal", 1);

        createOutput("vec3", "FragPos");
        createOutput("vec3", "Normal");

        createPerspective();

        var("FragPos") = var("model") * var("aPos");

        var("Normal") = transpose(inverse(var("model"))) * var("aNormal");

        if (textures == true) {
            createLayout("vec2", "aTexCoords", 2);
            createOutput("vec2", "TexCoords");
            var("TexCoords") = var("aTexCoords");
        }

        var("gl_Position") = var("projection") * var("view") * var("model") * var("aPos");

    }

    void addRand()
    {
        createFunctionDef("float", "Rand");

        func("Rand").addParameter("vec2", "co");
        func("Rand").addParameter("float", "a");
        func("Rand").addParameter("float", "b");
        func("Rand").addParameter("float", "c");

        closeFunctionDef("Rand");

        openFunction("Rand");
        stateReturn(fract(sin(dot(var("co", "xy"), vec2(var("a"), var("b"))) * var("c"))));
        closeFunction();
    }


    void addGeometrySmith()
    {

        // Check if GeometrySchlickGGX Is in function table, if not then add it
        // addGeometrySchlickGGX();

        createFunctionDef("float", "GeometrySmith");

        func("GeometrySmith").addParameter("vec3", "N");
        func("GeometrySmith").addParameter("vec3", "V");
        func("GeometrySmith").addParameter("vec3", "L");
        func("GeometrySmith").addParameter("float", "roughness");
       
        closeFunctionDef("GeometrySmith");

        openFunction("GeometrySmith");

        newVar("float", "NdotV") = max(dot(var("N"), var("V")), 0.0f);
        newVar("float", "NdotL") = max(dot(var("N"), var("L")), 0.0f);

        newVar("float", "ggx2") = call("GeometrySchlickGGX", var("NdotV"), var("roughness"));
        newVar("float", "ggx1") = call("GeometrySchlickGGX", var("NdotL"), var("roughness"));

        stateReturn(var("ggx1") * var("ggx2"));

        closeFunction();
    }

    void addFresnelSchlick()
    {
        createFunctionDef("vec3", "FresnelSchlick");

        func("FresnelSchlick").addParameter("float", "cosTheta");
        func("FresnelSchlick").addParameter("vec3", "F0");

        closeFunctionDef("FresnelSchlick");

        openFunction("FresnelSchlick");
        //1.0-F0
        newVar("vec3", "temp") = var("F0") - 1.0f;
        //1.0 - cosTheta
        stateReturn(var("F0") + var("temp") * pow(clamp(var("cosTheta"), 0.0, 1.0), 5.0));
        closeFunction();
    }

    void addGeometrySchlickGGX()
    {
        createFunctionDef("float", "GeometrySchlickGGX");

        func("GeometrySchlickGGX").addParameter("float", "NdotV");
        func("GeometrySchlickGGX").addParameter("float", "roughness");
       
        closeFunctionDef("GeometrySchlickGGX");

        openFunction("GeometrySchlickGGX");

        newVar("float", "r") = var("roughness") + 1.0f;
        newVar("float", "temp") = var("r") * var("r");
        newVar("float", "k") = var("temp") / 8.0f;
        // reverse this
        var("temp") = var("k") - 1.0f;
        newVar("float", "denom") = var("NdotV") * var("temp") + var("k");

        stateReturn(var("NdotV") / var("denom"));

        closeFunction();
    }

    void addGetNormalFromMap()
    {

        // Create nescessary layouts and uniforms 
        createFunctionDef("vec", "GetNormalFromMap");

        closeFunctionDef("GetNormalFromMap");

        openFunction("GetNormalFromMap");

        newVar("vec3", "tangentNormal") = texture(var("normalMap"), var("TexCoords"), "xyz") * 2.0f - 1.0f;

        newVar("vec3", "Q1") = dFdx(var("WorldPos"));
        newVar("vec3", "Q2") = dFdy(var("WorldPos"));
        newVar("vec3", "st1") = dFdx(var("TexCoords"));
        newVar("vec3", "st2") = dFdy(var("TexCoords"));

        newVar("vec3", "N") = normalize(var("Normal"));
        newVar("vec3", "T") = normalize(var("Q1") * var("st2", "t") - var("Q2") * var("st1", "t"));
        newVar("vec3", "B") = neg(normalize(cross(var("N"), var("T"))));
        newVar("mat3", "TBN") = mat3(var("T"), var("B"), var("N"));

        stateReturn(normalize(var("TBN") * var("tangentNormal")));

        closeFunction();

    }

    void addDistributionGGX() {

        createFunctionDef("float", "DistributionGGX");

        func("DistributionGGX").addParameter("vec3", "N");
        func("DistributionGGX").addParameter("vec3", "H");
        func("DistributionGGX").addParameter("float", "roughness");

        closeFunctionDef("DistributionGGX");


        openFunction("DistributionGGX");

        newVar("float", "a") = var("roughness") * var("roughness");
        newVar("float", "a2") = var("a") * var("a");
        newVar("float", "NdotH") = max(dot(var("N"), var("H")), 0.0);
        newVar("float", "NdotH2") = var("NdotH") * var("NdotH");

        newVar("float", "nom") = var("a2");
        newVar("float", "temp") = var("a2") - 1.0;
        newVar("float", "denom") = var("NdotH2") * var("temp") + 1.0;
        
        var("denom") = var("denom") * var("denom") * 3.141592f;

        stateReturn(var("nom") / var("denom"));

        closeFunction();
    }

    /* ---------------------- Function Logic ------------------------------ */

    void createFunctionDef(string type, string name) {
        
        Function function(type, name);
        //currentFunc = function;
        functions.push_back(function);
        //insert after 
        
    }

    void closeFunctionDef(string name)
    {
        Function function = func(name);

        string params;
        for (int i = 0; i < size(function.Parameters); i++)
        {
            ShaderType variable = function.Parameters[i];
            if (i != 0)
            {
                params = params + ", ";
            }
            // change this to local function variable
            variables.push_back(variable);
            params = params + variable.Type + " " + variable.Name;
        }

        string line = "\n" + function.Type + " " + function.Name + "(" + params + ")" + "\n{\n" + "\n//" + function.Tag + "\n}\n";
       
        insertAfter("Functions", line);
        // Write the function in
    }

    void openFunction(string name)
    {
        // Error handle for unfound function
        openFunc = true;
        currentFunc = func(name).Tag;
    }

    void stateReturn(ShaderType arguement)
    {
        string line = "\treturn " + arguement.Name + ";";
        insertBefore(currentFunc, line);
    }
    void closeFunction()
    {
        // Error handle for unfound function and unopened function
        openFunc = false;
        currentFunc = "main";
    }
    

    Function& func(string name)
    {
        for (int i = 0; i < size(functions); i++)
        {
            if (functions[i].Name.compare(name) == 0)
            {
                return functions[i];
            }
        }
    }

    
   

    ShaderType convertTo(string type, ShaderType variable) {
        if (type.compare("vec4") == 0) {
            return ShaderType("vec4", "vec4(" + variable.Name + ", 1.0)");
            //return "vec4(" + command + " 1.0)";
        }
        else if (type.compare("mat3") == 0) {
            return ShaderType("mat3", "mat3(" + variable.Name + ")");
            //return "mat3(" + command + ")";
        }
    }

    /* ----------------------   In-built Functions ------------------------------ */

    ShaderType transpose(ShaderType variable) {
        return ShaderType(variable.Type,"transpose(" + variable.Name + ")");
    }

    ShaderType normalize(ShaderType variable) {
        return ShaderType(variable.Type, "normalize(" + variable.Name + ")");
    }

    ShaderType dFdx(ShaderType variable) {
        return ShaderType(variable.Type, "dFdx(" + variable.Name + ")");
    }

    ShaderType dFdy(ShaderType variable) {
        return ShaderType(variable.Type, "dFdy(" + variable.Name + ")");
    }

    ShaderType neg(ShaderType variable) {
        return ShaderType(variable.Type, "-" + variable.Name);
    }

    ShaderType texture(ShaderType variable,ShaderType variable1, const char* attribute = "") {
        return ShaderType(variable.Type, "texture(" + variable.Name + ", " + variable1.Name + ")" + attribute);
    }

    ShaderType clamp(ShaderType variable, float x, float y) {
        return ShaderType(variable.Type, "clamp(" + variable.Name + ", " + to_string(x) + ", " + to_string(y));
    }

    ShaderType pow(ShaderType variable, float indicie) {
        return ShaderType(variable.Type, "clamp(" + variable.Name + ", " + to_string(indicie));
    }

    ShaderType mix(ShaderType variable, ShaderType variable1, ShaderType variable2) {
        return ShaderType(variable.Type, "mix(" + variable.Name + ", " + variable1.Name + ", " + variable2.Name + ")");
    }

    ShaderType inverse(ShaderType variable) {
        return ShaderType(variable.Type, "inverse(" + variable.Name + ")");
    }
    
    ShaderType fract(ShaderType variable) {
        return ShaderType(variable.Type, "fract(" + variable.Name + ")");
    }

    ShaderType floor(ShaderType variable) {
        return ShaderType(variable.Type, "floor(" + variable.Name + ")");
    }

    ShaderType sin(ShaderType variable) {
        return ShaderType(variable.Type, "sin(" + variable.Name + ")");
    }

    ShaderType max(ShaderType variable,float val) {
        return ShaderType(variable.Type, "max(" + variable.Name +  ", " + to_string(val) + ")");
    }

    ShaderType max(ShaderType variable, ShaderType variable1) {
        return ShaderType(variable.Type, "max(" + variable.Name + ", " + variable1.Name + ")");
    }

    ShaderType step(ShaderType variable, ShaderType variable1) {
        return ShaderType(variable.Type, "step(" + variable.Name + ", " + variable1.Name + ")");
    }

    ShaderType dot(ShaderType variable, ShaderType variable1) {
        return ShaderType(variable.Type, "dot(" + variable.Name + ", " + variable1.Name + ")");
    }

    ShaderType cross(ShaderType variable, ShaderType variable1) {
        return ShaderType(variable.Type, "cross(" + variable.Name + ", " + variable1.Name + ")");
    }

    ShaderType mat3(ShaderType variable, ShaderType variable1, ShaderType variable2) {
        return ShaderType(variable.Type, "mat3(" + variable.Name + ", " + variable1.Name + ", " + variable2.Name + ")");
    }

    ShaderType vec2(ShaderType variable, ShaderType variable1) {
        return ShaderType(variable.Type, "vec2(" + variable.Name + ", " + variable1.Name + ")");
    }

    /* ---------------------- Primitive methods ------------------------------ */

    ShaderType var(string name)
    {
        // check if var is in scope and search for corectness
        for (int i = 0; i < variables.size(); i++) {
            
            if (name.compare(variables[i].Name) == 0) {
                
                return variables[i];
            }
        }

        if (name.compare("gl_Position") == 0) {
            return ShaderType("vec4", "gl_Position");
        }
        return ShaderType("FALSE","NULL");
    }

    ShaderType var(string name, const char* attribute)
    {
        // check if var is in scope and search for corectness
        for (int i = 0; i < variables.size(); i++) {

            if (name.compare(variables[i].Name) == 0) {

                return ShaderType(variables[i].Type, variables[i].Name + "." + attribute);
            }
        }

        if (name.compare("gl_Position") == 0) {
            return ShaderType("vec4", "gl_Position");
        }
        return ShaderType("FALSE", "NULL");
    }

    ShaderType newVar(string type, string name)
    {
        // add func vars to func tables 
        ShaderType variable(type, name);
        variables.push_back(variable);
        string line = "\t" + type + " " + name + ";";
        insertBefore(currentFunc, line);
        return variable;
    }



    void setVariable(ShaderType variable, string operand)
    {
        if (variable.Name == "gl_Position") {
            string line = "\t" + variable.Name + " = " + "vec4" + "(" + operand + ")" + ";";
            insertBefore("End", line);
            return;
        }
        //
        string line = "\t" + variable.Name + " = " + variable.Type + "(" + operand + ")" + ";";
        
        insertBefore("End", line);
    }

    void createLayout(string type, string name, int location)
    {
        string line = "layout (location = " + to_string(location) + ") in " + type + " " + name + ";";
        ShaderType var(type, name);
        variables.push_back(var);
        //variableTypes.push_back(type);
        insertAfter("Layouts", line);
    }

    void createVariable(string type,string name)
    {
        string line = "uniform " + type + " " + name + ";";
        ShaderType var(type, name);
        variables.push_back(var);
        //variableTypes.push_back(type);
        insertAfter("Uniforms", line);
    }

    void createOutput(string type, string name)
    {
        string line = "out " + type + " " + name + ";";
        ShaderType var(type, name);
        variables.push_back(var);
        //variableTypes.push_back(type);
        insertAfter("Outs", line);
    }

    void createInput(string type, string name)
    {
        string line = "in " + type + " " + name + ";";
        ShaderType var(type, name);
        variables.push_back(var);
        //variableTypes.push_back(type);
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




#endif