


#include <GL/glew.h>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdio>

#include <>
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

using namespace std;
class ShaderType
{
public:

    string Type;
    string Name;
    ShaderBuilder SB;



    string operator* (const ShaderType& other) {
        stringstream ss;

        if (Type.compare("mat4") == 0 && other.Type.compare("vec3") == 0)
        {
            ss << Name + " * " + "vec4(" + other.Name + ", 1.0)";
        }
        else if (Type.compare("mat4") == 0 && other.Type.compare("vec2") == 0)
        {
            ss << Name + " * " + "vec4(" + other.Name + ", 1.0, 1.0)";
        }
        else if (Type.compare("mat4") == 0 && other.Type.compare("mat3") == 0)
        {
            ss << Name + " * " + "mat4(" + other.Name + ")";
        }
        else if (Type.compare("mat4") == 0 && other.Type.compare("mat2") == 0)
        {
            ss << Name + " * " + "mat4(" + other.Name + ")";
        }
        else if (Type.compare("mat3") == 0 && other.Type.compare("vec4") == 0)
        {
            ss << Name + " * " + "vec3(" + other.Name + ")";
        }
        else if (Type.compare("mat3") == 0 && other.Type.compare("vec2") == 0)
        {
            ss << Name + " * " + "vec3(" + other.Name + ", 1.0)";
        }
        else if (Type.compare("mat3") == 0 && other.Type.compare("mat4") == 0)
        {
            ss << Name + " * " + "mat3(" + other.Name + ")";
        }
        else if (Type.compare("mat3") == 0 && other.Type.compare("mat2") == 0)
        {
            ss << Name + " * " + "mat3(" + other.Name + ")";
        }
        else {
            ss << Name + " * " + other.Name;
        }

        return ss.str();
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

        if (Name == "gl_Position") {
            string line = "\t" + Name + " = " + "vec4" + "(" + other.Name + ")" + ";";
            SB.insertBefore("End", line);
            return;
        }
        //
        string line = "\t" + Name + " = " + Type + "(" + other.Name + ")" + ";";

        SB.insertBefore("End", line);

        return;
    }


    ShaderType(string type, string name, ShaderBuilder shaderBuilder)
    {
        Type = type;
        Name = name;
        SB = shaderBuilder;
    }



};

