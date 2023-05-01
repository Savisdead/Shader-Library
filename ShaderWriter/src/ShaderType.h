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

        // If the types are incompatible, return a ShaderType with the "NONE" type
        if (st.Type.compare("NONE") != 0) {
            return st;
        }
        // Otherwise, combine the two ShaderType names with a " * " separator and return the result
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

    ShaderType operator< (const ShaderType& other) {
        stringstream ss;
        ShaderType st = typeCheck(other, " < ");
        if (st.Type.compare("NONE") != 0) {
            return st;
        }
        else {
            ss << Name + " < " + other.Name;
            return ShaderType(Type, ss.str());
        }
    }

    ShaderType operator> (const ShaderType& other) {
        stringstream ss;
        ShaderType st = typeCheck(other, " > ");
        if (st.Type.compare("NONE") != 0) {
            return st;
        }
        else {
            ss << Name + " > " + other.Name;
            return ShaderType(Type, ss.str());
        }
    }

    ShaderType operator>= (const ShaderType& other) {
        stringstream ss;
        ShaderType st = typeCheck(other, " >= ");
        if (st.Type.compare("NONE") != 0) {
            return st;
        }
        else {
            ss << Name + " >= " + other.Name;
            return ShaderType(Type, ss.str());
        }
    }

    ShaderType operator<= (const ShaderType& other) {
        stringstream ss;
        ShaderType st = typeCheck(other, " <= ");
        if (st.Type.compare("NONE") != 0) {
            return st;
        }
        else {
            ss << Name + " <= " + other.Name;
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
        ss << Name + " + " + to_string(other);
        return ShaderType(Type, ss.str());
    }

    ShaderType operator- (const float& other) {
        stringstream ss;
        ss << Name + " - " + to_string(other);
        return ShaderType(Type, ss.str());
    }

    ShaderType operator/ (const float& other) {
        stringstream ss;
        ss << Name + " / " + to_string(other);
        return ShaderType(Type, ss.str());
    }

    ShaderType operator< (const float& other) {
        stringstream ss;
        ss << Name + " < " + to_string(other);
        return ShaderType(Type, ss.str());
    }

    ShaderType operator> (const float& other) {
        stringstream ss;
        ss << Name + " > " + to_string(other);
        return ShaderType(Type, ss.str());
    }

    ShaderType operator<= (const float& other) {
        stringstream ss;
        ss << Name + " <= " + to_string(other);
        return ShaderType(Type, ss.str());
    }

    ShaderType operator>= (const float& other) {
        stringstream ss;
        ss << Name + " >= " + to_string(other);
        return ShaderType(Type, ss.str());
    }
 
    ShaderType operator= (const int& other) {
        stringstream ss;
        ss << Name + " = " + to_string(other);
        string line = "\t" + Name + " = " + Type + "(" + to_string(other) + ")" + ";\n";

        //insertBefore("END", line);
        return ShaderType(Type, ss.str());
    }

    void operator= (const ShaderType& other) {
        stringstream ss;
       
        ss << Name + " = " + other.Name;

        if (Name == "gl_Position") {
            string line = "\t" + Name + " = " + "vec4" + "(" + other.Name + ")" + ";";
            insertBefore("END", line);
            return;
        }
        
        string line = Name + " = ";

        if (openLoop == true) {
            line = "\t\t" + line;
        }
        else {
            line = "\t" + line;
        }

        if (autoCast == true) {
            line = line + Type + "(" + other.Name + ")" + ";\n";
        }
        else {
            line = line + other.Name + ";\n";
        }

        insertBefore("END", line);

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
        fstream file(path);
        stringstream buffer;
        string line;
        int current_line = 0; 

        while (getline(file, line)) {
            current_line++;
            if (current_line == line_number) {
                buffer << command << endl; 
            }
            buffer << line << endl; 
        }

        file.close();

        file.open(path, ios::out | ios::trunc);

        file << buffer.rdbuf();
        file.close();
    }

    void insertBefore(string word, string insert)
    {

        std::string fileName = path;
        int lineNumber = 0;
        bool wordFound = false;
        
        if (currentFunc.compare("END") == 0)
        {
            word = "End";
        }
        else
        {
            word = currentFunc;
            cout << word << endl;
        }
        
        std::ifstream file(fileName);
        std::string line;
        while (std::getline(file, line)) {
            lineNumber++;
            if (line.find(word) != std::string::npos) {
                wordFound = true;
                break;
            }
        }
        lineNumber--;
        insertLine(insert, lineNumber);

    }



};

#endif