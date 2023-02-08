#ifndef SHADERTYPE_H

#define SHADERTYPE_H

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


#include <iostream>
#include <string>
#include <vector>
#include <variant>

#include <map>
#include <fstream>
#include <sstream>
#include <iostream>


#include <common.h>

using namespace std;
class ShaderType
{
public:

    string Type;
    string Name;
    

    ShaderType operator* (const ShaderType& other) {
        stringstream ss;
        ShaderType st = typeCheck(other, " * ");
        if (st.Type.compare("NONE") != 0) {
            return st;
        }
        else {
            ss << Name + " * " + other.Name;
            return ShaderType(Type, ss.str());
        }
    }

    ShaderType operator+ (const ShaderType& other) {
        stringstream ss;
        ShaderType st = typeCheck(other, " + ");
        if (st.Type.compare("NONE") != 0) {
            return st;
        }
        else {
            ss << Name + " + " + other.Name;
            return ShaderType(Type, ss.str());
        }
    }

    ShaderType operator- (const ShaderType& other) {
        stringstream ss;
        ShaderType st = typeCheck(other, " - ");
        if (st.Type.compare("NONE") != 0) {
            return st;
        }
        else {
            ss << Name + " - " + other.Name;
            return ShaderType(Type, ss.str());
        }
    }

    ShaderType operator/ (const ShaderType& other) {
        stringstream ss;
        ShaderType st = typeCheck(other, " / ");
        if (st.Type.compare("NONE") != 0) {
            return st;
        }
        else {
            ss << Name + " / " + other.Name;
            return ShaderType(Type, ss.str());
        }
    }

    ShaderType operator* (const float& other) {
        stringstream ss;
        ss << Name + " * " + to_string(other);
        return ShaderType(Type, ss.str());
    }

    ShaderType operator+ (const float& other) {
        stringstream ss;
        ss << Name + " * " + to_string(other);
        return ShaderType(Type, ss.str());
    }

    ShaderType operator- (const float& other) {
        stringstream ss;
        ss << Name + " * " + to_string(other);
        return ShaderType(Type, ss.str());
    }

    ShaderType operator/ (const float& other) {
        stringstream ss;
        ss << Name + " * " + to_string(other);
        return ShaderType(Type, ss.str());
    }



    


    

    void operator= (const ShaderType& other) {
        stringstream ss;

        if (Type.compare("mat4") == 0 && other.Type.compare("vec3") == 0)
        {
            ss << Name + " = " + "vec4(" + other.Name + ", 1.0)";
        }
        else if (Type.compare("mat4") == 0 && other.Type.compare("vec2") == 0)
        {
            ss << Name + " = " + "vec4(" + other.Name + ", 1.0, 1.0)";
        }
        else if (Type.compare("mat4") == 0 && other.Type.compare("mat3") == 0)
        {
            ss << Name + " = " + "mat4(" + other.Name + ")";
        }
        else if (Type.compare("mat4") == 0 && other.Type.compare("mat2") == 0)
        {
            ss << Name + " = " + "mat4(" + other.Name + ")";
        }
        else if (Type.compare("mat3") == 0 && other.Type.compare("vec4") == 0)
        {
            ss << Name + " = " + "vec3(" + other.Name + ")";
        }
        else if (Type.compare("mat3") == 0 && other.Type.compare("vec2") == 0)
        {
            ss << Name + " = " + "vec3(" + other.Name + ", 1.0)";
        }
        else if (Type.compare("mat3") == 0 && other.Type.compare("mat4") == 0)
        {
            ss << Name + " = " + "mat3(" + other.Name + ")";
        }
        else if (Type.compare("mat3") == 0 && other.Type.compare("mat2") == 0)
        {
            ss << Name + " = " + "mat3(" + other.Name + ")";
        }
        else {
            ss << Name + " = " + other.Name;
        }

        // Dont do this if functionOpen is true! 
        if (Name == "gl_Position") {
            string line = "\t" + Name + " = " + "vec4" + "(" + other.Name + ")" + ";";
            insertBefore("End", line);
            return;
        }
        //
        string line = "\t" + Name + " = " + Type + "(" + other.Name + ")" + ";\n";

        insertBefore("End", line);

        return;
    }


    ShaderType  typeCheck(const ShaderType& other, string op) {
        stringstream ss;

        if (Type.compare("mat4") == 0 && other.Type.compare("vec3") == 0)
        {
            ss << Name + op + "vec4(" + other.Name + ", 1.0)";
            return ShaderType("mat4", ss.str());
        }
        else if (Type.compare("mat4") == 0 && other.Type.compare("vec2") == 0)
        {
            ss << Name + op + "vec4(" + other.Name + ", 1.0, 1.0)";
            return ShaderType("mat4", ss.str());
        }
        else if (Type.compare("mat4") == 0 && other.Type.compare("mat3") == 0)
        {
            ss << Name + op + "mat4(" + other.Name + ")";
            return ShaderType("mat4", ss.str());
        }
        else if (Type.compare("mat4") == 0 && other.Type.compare("mat2") == 0)
        {
            ss << Name + op + "mat4(" + other.Name + ")";
            return ShaderType("mat4", ss.str());
        }
        else if (Type.compare("mat3") == 0 && other.Type.compare("vec4") == 0)
        {
            ss << Name + op + "vec3(" + other.Name + ")";
            return ShaderType("mat3", ss.str());
        }
        else if (Type.compare("mat3") == 0 && other.Type.compare("vec2") == 0)
        {
            ss << Name + op + "vec3(" + other.Name + ", 1.0)";
            return ShaderType("mat3", ss.str());
        }
        else if (Type.compare("mat3") == 0 && other.Type.compare("mat4") == 0)
        {
            ss << Name + op + "mat3(" + other.Name + ")";
            return ShaderType("mat3", ss.str());
        }
        else if (Type.compare("mat3") == 0 && other.Type.compare("mat2") == 0)
        {
            ss << Name + op + "mat3(" + other.Name + ")";
            return ShaderType("mat3", ss.str());
        }
        else {
            return ShaderType("NONE", "");
        }
    }


    ShaderType(string type, string name)
    {
        Type = type;
        Name = name;
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

    void insertBefore(string word, string insert)
    {

        std::string fileName = "C:\\OpenGLS\\Project\\Project1\\ShaderWriter\\src\\Tests\\Test.txt";
        int lineNumber = 0;
        bool wordFound = false;
        
        if (currentFunc.compare("main") == 0)
        {
            word = "End";
        }
        else
        {
            word = currentFunc;
            cout << word << endl;
        }
        
        // Else include currentFuncTag
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