#ifndef SHADER_H
//#define STB_IMAGE_IMPLEMENTATION
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




bool openFunc = false;
bool openLoop = false;
bool autoCast = true;
std::string currentFunc = "END";
std::string lastFunc = "";
std::string path;


using namespace std;
class ShaderBuilder 
{
public:

    int lineNum = 1;
    int layoutSpace = 2;
    int variableSpace = 3;
    int outSpace = 4;
    int numOfVars = 0;

    
    vector<ShaderType> variables;
    vector<Function> functions;
    vector<ShaderType> funcVariables;
    int forloops = 0;
    int ifBodies = 0;
    bool instantiated = false;
    
    ShaderBuilder() {}

    /*
    * The purpose of this function is to write the nescessary 
    * tags and template code of a GLSL file
    */
    void instantiate() {
        if (instantiated == false) {
            ofstream file(path);
            string message = "#version 460 core";
            file << message << endl;
            file.close();
            appendLine("//Layouts\n//Ins\n//Uniforms\n//Outs\n//Functions\nvoid main()\n{\n//END\n}");
            instantiated = true;
        }
    }
    
   
    /*
     * This function is a writes a PBR shader to the current shader objects 
     * fragment path
     */
    void createPBR() {

        createOutput("vec4", "FragColor");
        createInput("vec2", "TexCoords");
        createInput("vec3", "WorldPos");
        createInput("vec3", "Normal");

        createVariable("sampler2D", "albedoMap");
        createVariable("sampler2D", "normalMap");
        createVariable("sampler2D", "metallicMap");
        createVariable("sampler2D", "roughnessMap");
        createVariable("sampler2D", "aoMap");

        createVariable("vec3", "lightPositions[4]");
        createVariable("vec3", "lightColors[4]");

        createVariable("vec3", "camPos");

        addGetNormalFromMap();
        addDistributionGGX();
        addGeometrySchlickGGX();
        addGeometrySmith();
        addFresnelSchlick();
        
        createFunctionDef("vec4", "addPBR");

        func("addPBR").addParameter("vec2", "TexCoords");
        func("addPBR").addParameter("vec3", "WorldPos");
        func("addPBR").addParameter("vec3", "Normal");
       
        closeFunctionDef("addPBR");

        openFunction("addPBR");
       
        newVar("vec3", "albedo") = pow(texture(var("albedoMap"), var("TexCoords"), "rgb"), vec3(2.2f));
        newVar("float", "metallic") = texture(var("metallicMap"), var("TexCoords"), "r");
        newVar("float", "roughness") = texture(var("roughnessMap"), var("TexCoords"), "r");
        newVar("float", "ao") = texture(var("aoMap"), var("TexCoords"), "r");

        newVar("vec3", "N") = call(func("GetNormalFromMap"));
        newVar("vec3", "V") = normalize(var("camPos") - var("WorldPos"));

        newVar("vec3", "F0") = vec3(0.04f);
        var("F0") = mix(var("F0"), var("albedo"), var("metallic"));
        
        newVar("vec3", "Lo") = vec3(0.0);

        newVar("int", "i");

        For(newVar("int","i") = 0, var("i") < 4, preIncrement(var("i")));

        newVar("vec3", "L") = normalize(iter("lightPositions[4]","i") - var("WorldPos"));
        newVar("vec3", "H") = normalize(var("V") + var("L"));
        newVar("float", "distance") = length(iter("lightPositions[4]", "i") - var("WorldPos"));
        newVar("float", "temp") = var("distance") * var("distance");
        newVar("float", "attenuation") = flo(1.0) / var("temp");
        newVar("vec3", "radiance") = iter("lightColors[4]", "i") * var("attenuation");

        newVar("float", "NDF") = call(func("DistributionGGX"), var("N"), var("H"), var("roughness"));
        newVar("float", "G") = call(func("GeometrySmith"), var("N"), var("V"), var("L"), var("roughness"));
        newVar("vec3", "F") = call(func("FresnelSchlick"), max(dot(var("H"), var("V")), 0.0f), var("F0"));

        newVar("vec3", "numerator") = var("NDF") * var("G") * var("F");
        newVar("float", "denominator") = flo(4.0) * max(dot(var("N"), var("V")), 0.0) * max(dot(var("N"), var("L")), 0.0) + flo(0.0001);
        newVar("vec3", "specular") = var("numerator") / var("denominator");

        newVar("vec3", "kD") = vec3(1.0) - var("F");
        var("temp") = flo(1.0) - var("metallic");
        var("kD") = var("kD") * var("temp");

        newVar("float", "NdotL") = max(dot(var("N"), var("L")), 0.0);

        newVar("vec3", "temp2") = var("kD") * var("albedo") / flo(3.14159265359) + var("specular");
        var("temp2") = var("temp2") * var("radiance") * var("NdotL");
        var("Lo") = var("Lo") + var("temp2");

        endFor();

        newVar("vec3", "ambient") = vec3(0.03) * var("albedo") * var("ao");
        newVar("vec3", "color") = var("ambient") + var("Lo");

        newVar("vec3", "temp2") = var("color") + vec3(1.0);
        var("color") = var("color") / var("temp2");

        var("color") = pow(var("color"), vec3(1.0 / 2.2));

        var("FragColor") = var("FragColor") + convertTo("vec4", var("color"));

        stateReturn(var("FragColor"));
        closeFunction();

        
       

        var("FragColor") = call(func("addPBR"), var("TexCoords"), var("WorldPos"), var("Normal"));
 
    }
    /*
     * The purpose of this function is to compute the average value of the red, green,
     * and blue channels of a given vec4 color.
     */
    void addAvg()
    {
        createFunctionDef("float", "avg");

        func("avg").addParameter("vec4", "color");
      
        closeFunctionDef("avg");

        openFunction("avg");
        newVar("float", "rgbSum") = var("color", "r") + var("color", "g") + var("color", "b");
        stateReturn(var("rgbSum") / 3.0);
        closeFunction();
    }
    /*
     * The purpose of this function is to create a shader which accurately simulates an object
     * submerged in water
     */
    void addSubmerge(float speed, float scale, float opacity)
    {
        createVariable("sampler2D", "waterAlbedoMap");
        createVariable("sampler2D", "waterHighlightMap");
        createVariable("float", "iTime");

        addAvg();
        createFunctionDef("vec4", "addSubmerge");

        func("addSubmerge").addParameter("vec2", "fragCoord");

        closeFunctionDef("addSubmerge");


        openFunction("addSubmerge");
        
        newVar("float", "speed") = flo(speed);
        newVar("float", "scale") = flo(scale);
        newVar("float", "opacity") = flo(opacity);
       
        newVar("vec2", "scaledUv") = var("fragCoord") * var("scale");

        newVar("vec4", "water1") = texture(var("waterAlbedoMap"), var("scaledUv") + var("iTime") * 0.02 * var("speed") - 0.1);
        newVar("vec4", "water2") = texture(var("waterAlbedoMap"), var("scaledUv","xy") + var("iTime") *  var("speed")  * vec2(-0.02,-0.02) + 0.1);
        
        newVar("vec4", "highlights1") = texture(var("waterHighlightMap"), var("scaledUv", "xy") + var("iTime") * var("speed") / vec2(-10, 100));
        newVar("vec4", "highlights2") = texture(var("waterHighlightMap"), var("scaledUv", "xy") + var("iTime") * var("speed") / vec2(10, 100));
        
        newVar("vec4", "background") = texture(var("albedoMap"), vec2(var("fragCoord")) + call(func("avg"), var("water1")) * 0.05f);
        
        var("water1", "rgb") = vec3(call(func("avg"), var("water1")));
        var("water2", "rgb") = vec3(call(func("avg"), var("water2")));

        var("highlights1", "rgb") = vec3(call(func("avg"), var("highlights1"))/1.5);
        var("highlights2", "rgb") = vec3(call(func("avg"), var("highlights2")) / 1.5);
      
        newVar("float", "alpha") = var("opacity");

        If(call(func("avg"), var("water1") + var("water2")) > 0.3);
        var("alpha") = flo(0.0);
        endFor();
      
        If(call(func("avg"), var("water1") + var("water2") + var("highlights1") + var("highlights2")) > 1.7);
        var("alpha") = var("opacity") * 0.3;
        endFor();
       
        newVar("vec4", "water") = var("water1") + var("water2");
        var("water") = var("water") * var("alpha") + var("background");
        var("FragColor") = var("FragColor") + var("water") * 0.2;
        
        stateReturn(var("FragColor"));

        closeFunction();

        var("FragColor") = var("FragColor") + call(func("addSubmerge"), var("TexCoords"));

        
    }
    
    
    /* These are helper functions intended to be used within high level shader functions */

    /*
     * The purpose of this function is to define the nescesary variables
     * to implement a perspective view
     */
    void createPerspective() {
        createVariable("mat4", "model");
        createVariable("mat4", "view");
        createVariable("mat4", "projection");
    }

    /*
     * The purpose of this function is provide a default template for an objects
     * vertex shader
     * 
     * @param textures: a bool for defining whether the texture coordinates should be included or not
     */
    void initVertexShader(bool textures = false) {

        createLayout("vec3", "aPos", 0);
        createLayout("vec3", "aNormal", 1);

        createOutput("vec3", "FragPos");
        createOutput("vec3", "Normal");

        createPerspective();

        // New Implementation
        var("FragPos") = var("model") * var("aPos");
        var("Normal") = transpose(inverse(var("model"))) * var("aNormal");
        
        if (textures == true) {
            createLayout("vec2", "aTexCoords", 2);
            createOutput("vec2", "TexCoords");
            var("TexCoords") = var("aTexCoords");
        }

        var("gl_Position") = var("projection") * var("view") * var("model") * var("aPos");

    }

    /*
    * The purpose of this function is to generate a random vec2 value
    */
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
    /*
     * The purpose of this function is to compute the geometry term using a
     * BRDF model 
     */
    void addGeometrySmith()
    {

        createFunctionDef("float", "GeometrySmith");

        func("GeometrySmith").addParameter("vec3", "N");
        func("GeometrySmith").addParameter("vec3", "V");
        func("GeometrySmith").addParameter("vec3", "L");
        func("GeometrySmith").addParameter("float", "roughness");
       
        closeFunctionDef("GeometrySmith");

        openFunction("GeometrySmith");

        newVar("float", "ourVar") = var("N") * var("L");
        newVar("float", "NdotV") = max(dot(var("N"), var("V")), 0.0f);
        newVar("float", "NdotL") = max(dot(var("N"), var("L")), 0.0f);

       
        newVar("float", "ggx2") = call(func("GeometrySchlickGGX"), var("NdotV"), var("roughness"));
        newVar("float", "ggx1") = call(func("GeometrySchlickGGX"), var("NdotL"), var("roughness"));

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
        
        newVar("vec3", "temp") = flo(1.0f) - var("F0");
       
        stateReturn(var("F0") + var("temp") * pow(clamp( flo(1.0) - var("cosTheta"), 0.0, 1.0), 5.0));
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
        
        var("temp") = flo(1.0f) - var("k") ;
        newVar("float", "denom") = var("NdotV") * var("temp") + var("k");

        stateReturn(var("NdotV") / var("denom"));

        closeFunction();
    }

    void addGetNormalFromMap()
    {

        // Create nescessary layouts and uniforms 
        createFunctionDef("vec3", "GetNormalFromMap");

        closeFunctionDef("GetNormalFromMap");

        openFunction("GetNormalFromMap");

        newVar("vec3", "tangentNormal") = texture(var("normalMap"), var("TexCoords"), "xyz") * 2.0f - 1.0f;

        newVar("vec3", "Q1") = dFdx(var("WorldPos"));
        newVar("vec3", "Q2") = dFdy(var("WorldPos"));
        newVar("vec2", "st1") = dFdx(var("TexCoords"));
        newVar("vec2", "st2") = dFdy(var("TexCoords"));

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

   
    vector<ShaderType> callBuilder() {
        vector<ShaderType> empty;
        return empty;
    }
    
    template<typename ShaderType, typename... ShaderTypes>
    vector<ShaderType> callBuilder(ShaderType arg, ShaderTypes... args) {
        vector<ShaderType> vec1;

        ShaderType caller("", arg.Name);
        vec1.push_back(caller);
        
        vector<ShaderType> vec2 = callBuilder(args...);
        
        vec1.insert(vec1.end(), vec2.begin(), vec2.end());

        return vec1;
    }
    // Recursive case
    template<typename Function, typename... ShaderTypes>
    ShaderType call(Function arg, ShaderTypes... args) {
        // error handle if exists 
       
        Function param("", arg.Name);
        
        vector<ShaderType> vec = callBuilder(args...);
       
        //function type needed 
        ShaderType out("Func", param.Name + "( ");

        for (int i = 0; i < vec.size(); i++)
        {
            if (i != 0) 
            {
                out.Name = out.Name + ", ";
            }

            out.Name = out.Name + vec[i].Name;
            
        }
        out.Name = out.Name + " )";

        return out;
    }

    ShaderType call(Function arg) {
        // error handle if exists 

        //function type needed 
        ShaderType out("Func", arg.Name + "()");

        return out;
    }

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
       
        insertBefore("Functions", line);
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
        currentFunc = "END";
    }
    
    Function& func(string name)
    {
        // Add proper error handle 
        for (int i = 0; i < size(functions); i++)
        {
            if (functions[i].Name.compare(name) == 0)
            {
                return functions[i];
            }
        }
    }

    void For(ShaderType counter, ShaderType limit, ShaderType step) {

        openLoop = true;
        lastFunc = currentFunc;
        forloops += 1;
        currentFunc = "ForBody" + to_string(forloops);
        
        //string line = "\n" + "for " + "( " + counter.Name + "; " + limit.Name + ";" + step.Name + ")" + "\n{\n" + "\n//" + "ForBody" + to_string(forloops); + "\n}\n";
        string line = "\tfor( " + counter.Name + "; " + limit.Name + ";" + step.Name + ")" + "\n\t{\n" + "\n\t//" + "ForBody" + to_string(forloops) +"\n\t}\n";
        insertBefore(lastFunc, line);
        
    }

    void endFor()
    {
        openLoop = false;
        currentFunc = lastFunc;
    }

    void If(ShaderType statement) {

        openLoop = true;
        lastFunc = currentFunc;
        ifBodies += 1;
        currentFunc = "IfBody" + to_string(ifBodies);
        
        //string line = "\n" + "for " + "( " + counter.Name + "; " + limit.Name + ";" + step.Name + ")" + "\n{\n" + "\n//" + "ForBody" + to_string(forloops); + "\n}\n";
        string line = "\tif(" + statement.Name + ")" + "\n\t{\n" + "\n\t//" + "IfBody" + to_string(ifBodies) + "\n\t}\n";
        insertBefore(lastFunc, line);

    }

    void endIf()
    {
        openLoop = false;
        currentFunc = lastFunc;
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

    /* ----------------------   In-built GLSL Functions ------------------------------ */

    ShaderType integer(int i) {
        return ShaderType("int", to_string(i));
    }
    
    ShaderType flo(float i) {
        return ShaderType("float", to_string(i));
    }
    
    ShaderType postIncrement(ShaderType variable) {
        return ShaderType(variable.Type, variable.Name + "++");
    }
    
    ShaderType preIncrement(ShaderType variable) {
        return ShaderType(variable.Type, "++" + variable.Name);
    }
    
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

    ShaderType length(ShaderType variable) {
        return ShaderType(variable.Type, "length(" + variable.Name + ")");
    }

    ShaderType neg(ShaderType variable) {
        return ShaderType(variable.Type, "-" + variable.Name);
    }

    ShaderType texture(ShaderType variable,ShaderType variable1, const char* attribute = "") {
        if (attribute == "") {
            return ShaderType(variable.Type, "texture(" + variable.Name + ", " + variable1.Name + ")");
        }else{
            return ShaderType(variable.Type, "texture(" + variable.Name + ", " + variable1.Name + ")" + "." + attribute);
        }
        
    }

    ShaderType clamp(ShaderType variable, float x, float y) {
        return ShaderType(variable.Type, "clamp(" + variable.Name + ", " + to_string(x) + ", " + to_string(y) + ")");
    }

    ShaderType pow(ShaderType variable, float indicie) {
        return ShaderType(variable.Type, "pow(" + variable.Name + ", " + to_string(indicie) + ")");
    }

    ShaderType pow(ShaderType variable, ShaderType indicie) {
        return ShaderType(variable.Type, "pow(" + variable.Name + ", " + indicie.Name + ")");
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

    ShaderType vec2(ShaderType variable) {
        return ShaderType(variable.Type, "vec2(" + variable.Name + ")");
    }

    ShaderType vec2(float variable, float variable1) {
        return ShaderType("vec2", "vec2(" + to_string(variable) + ", " + to_string(variable1) + ")");
    }

    ShaderType vec3(float i) {
        return ShaderType("vec3", "vec3(" + to_string(i) + ")");
    }
    
    ShaderType vec3(ShaderType i) {
        return ShaderType("vec3", "vec3(" + i.Name + ")");
    }

    /* ---------------------- Primitive methods ------------------------------ */

    ShaderType var(string name)
    {
        
        for (int i = 0; i < variables.size(); i++) {
           
            if (name.compare(variables[i].Name) == 0) {
                
                return variables[i];
            }
        }

        if (name.compare("gl_Position") == 0) {
            return ShaderType("vec4", "gl_Position");
        }
        return ShaderType("NULL","NOTFOUND");
    }

    ShaderType var(string name, const char* attribute)
    {
        // check if var is in scope and search for corectness
        for (int i = 0; i < variables.size(); i++) {

            if (name.compare(variables[i].Name) == 0) {
                string type = "NULL";
                if (attribute == "rgb") {
                    type = "vec3";
                }
                else {
                    type = variables[i].Type;
                }
                return ShaderType(type, variables[i].Name + "." + attribute);
            }
        }

        if (name.compare("gl_Position") == 0) {
            return ShaderType("vec4", "gl_Position");
        }
        return ShaderType("FALSE", "NULL");
    }

    ShaderType iter(string name,string item)
    {
        // check if var is in scope and search for corectness
        for (int i = 0; i < variables.size(); i++) {
            
            if (name.compare(variables[i].Name) == 0) {
                string name = variables[i].Name;
                //.erase(variables[i].Name.find("["), variables[i].Name.end());
                name.erase(name.find("[",1), name.find("[", 1)+5);
                return ShaderType(variables[i].Type, name + "[" + item + "]");
            }
        }
        return ShaderType("FALSE", "NULL");
    }

    ShaderType newVar(string type, string name)
    {
        // add func vars to func tables 
        ShaderType variable(type, name);
        variables.push_back(variable);
        string line;
        
        line = type + " ";
       
        variable.Name = line + variable.Name;
        //insertBefore(currentFunc, line);
        return variable;
    }

    void setAutoCast(bool set) {
        autoCast = set;
    }

    void setVariable(ShaderType variable, string operand)
    {
        if (variable.Name == "gl_Position") {
            string line = "\t" + variable.Name + " = " + "vec4" + "(" + operand + ")" + ";";
            insertBefore("END", line);
            return;
        }
        //
        string line = "\t" + variable.Name + " = " + variable.Type + "(" + operand + ")" + ";";
        
        insertBefore("END", line);
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
        string line = "out " + type + " " + name + ";" + "\n";
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
        ofstream file(path, ios::app); //open the file in append mode
        string message = command;
        file << message << endl;
        file.close();
    }

    void insertLine(string command, int line_number)
    {
        fstream file(path);
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
        file.open(path, ios::out | ios::trunc);

        // write the contents of the buffer to the file
        file << buffer.rdbuf();
        file.close();
    }

    void insertAfter(string word, string insert)
    {
        string targetWord = word;
      
        ifstream file(path);
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
        remove(path.c_str());
        rename("C:\\OpenGLS\\Project\\Project1\\ShaderWriter\\src\\Tests\\temp.txt", path.c_str());
    }

    void insertBefore(string word, string insert)
    {
        
        std::string fileName = path;
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







class Shader
{
public:
    unsigned int ID;
    int data = -1;
    ShaderBuilder shBuild;
    string fPath;
    string vPath;

    map<string, map<string, unsigned int>> shaderLib;

    Shader(const char* vertexPath, const char* fragmentPath)
    {
        
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;


        fPath = fragmentPath;
        vPath = vertexPath;

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
            std::cout << "ERROR SHADER FILE FAILED TO BE READ: " << e.what() << std::endl;
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
    /* These functions are responsible for creating and updating the PBR shader */
    void createMaterial(glm::vec3 lightPositions[], glm::vec3 lightColors[], int lightNo)
    {
        
        editFragment();
        
        shBuild.instantiate();
        shBuild.createPBR();
        
        // Set model
        glm::mat4 model = glm::mat4(1.0f);
        setMat4("model", model);
        
        // Set lighting
        for (unsigned int i = 0; i < lightNo; ++i)
        {
            glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
            newPos = lightPositions[i];

            setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
           
            setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);

        }
        // Set projection matrix using default values 
        setProjection();
        
    }

    void setMaps(string albedoPath, string normalPath, string metallicPath, string roughnessPath, string aoPath)
    {
        map<string, unsigned int> PBR;
        unsigned int albedo = setMap("albedoMap", albedoPath.c_str());
        unsigned int normal = setMap("normalMap", normalPath.c_str());
        unsigned int metallic = setMap("metallicMap", metallicPath.c_str());
        unsigned int roughness = setMap("roughnessMap", roughnessPath.c_str());
        unsigned int ao = setMap("aoMap", aoPath.c_str());

        PBR["albedo"] = albedo;
        PBR["normal"] = normal;
        PBR["metallic"] = metallic;
        PBR["roughness"] = roughness;
        PBR["ao"] = ao;

        shaderLib["PBR"] = PBR;
    }

    void updateMaterial(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp)
    {
        setMat4("view", glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp));
        setVec3("camPos", cameraPos);
       
        setMat4("model", glm::mat4(1.0f));
        
        map<string, unsigned int> PBR = shaderLib["PBR"];

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, PBR["albedo"]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, PBR["normal"]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, PBR["metallic"]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, PBR["roughness"]);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, PBR["ao"]);
        
    }

    /* These functions are responsible for creating and updating the submerged shader */
    void makeSubmerged()
    {
        editFragment();
        shBuild.instantiate();
        shBuild.addSubmerge(1.0,0.9,0.5);

        map<string, unsigned int> waterSh0;

        unsigned int waterAlbedoMap = setMap("waterAlbedoMap", "C:\\OpenGLS\\Project\\Project1\\ShaderWriter\\src\\textures\\waternoise.png");
        unsigned int waterHighlightMap = setMap("waterHighlightMap", "C:\\OpenGLS\\Project\\Project1\\ShaderWriter\\src\\textures\\waterMap.png");

        waterSh0["waterAlbedoMap"] = waterAlbedoMap;
        waterSh0["waterHighlightMap"] = waterHighlightMap;

        shaderLib["waterSh0"] = waterSh0;
        //reload();
    }

    void updateSubmerged()
    {
        setFloat("iTime", glfwGetTime());

        map<string, unsigned int> water = shaderLib["waterSh0"];

        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, water["waterAlbedoMap"]);
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, water["waterHighlightMap"]);
    }

    void updateView(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp) 
    {
        setMat4("view", glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp));
    }

    void updateCamPos(glm::vec3 cameraPos)
    {
        setVec3("camPos",cameraPos);
    }

    /* These functions are responsible for managing the current shader context */

    void editVertex() 
    {
        path = vPath;
    }

    void editFragment() 
    {
        path = fPath;
      
    }

    void use() 
    {
        
        glUseProgram(ID);
        
    }

    void reload()
    {

        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        const char* fragmentPath = fPath.c_str();
        const char* vertexPath = vPath.c_str();

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

        glDeleteProgram(ID);
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    unsigned int setMap(const std::string& name, char const* path) 
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), ++data);
        return loadTexture(path);
    }

    void setProjection(float fov, int screenHeight, int screenWidth, float nearPlane, float farPlane) 
    {
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)screenWidth / (float)screenHeight, nearPlane, farPlane);
        setMat4("projection", projection);
    }

    void setProjection()
    {
        glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)800 / (float)600, 0.1f, 300.0f);
        setMat4("projection", projection);
    }

    /* These functions are responsible for updating shader variables currently in memory */

    void setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
   
    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
   
    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    
    void setVec2(const std::string& name, const glm::vec2& value) const
    {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    
    void setVec2(const std::string& name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
    }
    
    void setVec3(const std::string& name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    
    void setVec3(const std::string& name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }
    
    void setVec4(const std::string& name, const glm::vec4& value) const
    {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    
    void setVec4(const std::string& name, float x, float y, float z, float w) const
    {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
    } 
    
    void setMat2(const std::string& name, const glm::mat2& mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    
    void setMat3(const std::string& name, const glm::mat3& mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    
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

   
    unsigned int loadTexture(char const* path)
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << path << std::endl;
            stbi_image_free(data);
        }

        return textureID;
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
                std::cout << "SHADER COMPILATION ERROR : " << type << "\n" << infoLog  << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "PROGRAM LINKING ERROR : " << type << "\n" << infoLog  << std::endl;
            }
        }
    }
};


#endif