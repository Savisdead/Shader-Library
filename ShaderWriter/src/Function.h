#ifndef FUNC_H

#define FUNC_H

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

#include<ShaderType.h>
#include <common.h>

using namespace std;

class Function
{
public:
    string Type;
    string Name;
    string Tag;
    vector<string> Body;
    vector<ShaderType> Parameters;

    Function(string type, string name)
    {
        Type = type;
        Name = name;
        Tag = name + "Body";
        //Parameters = parameters;
    }

    void addParameter(string type, string name)
    {

        Parameters.push_back(ShaderType(type, name));
    }

};


#endif