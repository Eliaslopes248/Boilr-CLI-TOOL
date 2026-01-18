/**
BREIF:
    This file will be used to define
    and register project builds to this tool.
    I felt like there needs to be a way to easily add project
    templates as a contributor.

*/
#include <cstddef>
#include <map>
#include <ostream>
#include <iostream>


#pragma once
using namespace std;

/**
    BRIEF: this struct is used to define
            a project build, this build will go in
            the registry
*/
struct build
{
    string          name;
    unsigned char*  header_data;
    size_t          header_size;
    string          path;
};

// Macros to construct variable names from base name
// xxd -i creates: name_zip[] and name_zip_len
// Note: xxd uses the filename (without extension) + "_zip" and "_zip_len"
#define BUILD_DATA(name) name##_zip
#define BUILD_SIZE(name) name##_zip_len

class build_registery
{
public:

// static instance of class so other files can access
static build_registery &Instance() 
{
    // creates singleton
    static build_registery Instance;
    return Instance;
}

// stores each build
map<unsigned int, build> registery;

// Register a build with direct pointers to the byte data
// data_ptr: pointer to the unsigned char array from the header file
// data_size: size_t value from the header file
void register_build(string name, unsigned char* data_ptr, size_t data_size, string path)
{
    // catch invalid input
    if (name.size() == 0 || path.size() == 0){
        printf("Missing build name or build path\n");
        return;
    }
    if (data_ptr == nullptr || data_size == 0){
        printf("Invalid build data: data pointer is null or size is zero\n");
        return;
    }
    // create build
    build b{
        name,
        data_ptr,      // pointer to the byte array
        data_size,     // size of the byte array
        path
    };
    // add to the registry
    int id = registery.size();

    registery.insert({
        id, 
        b
    });
}

void print_registry()
{
    cout << "========================================" << endl;
    cout << "BUILD REGISTRY" << endl;
    cout << "========================================" << endl;
    for (auto build : registery)
    {
        printf("ID: %d  NAME: %s  PATH: %s\n", 
            build.first, 
            build.second.name.c_str(),
            build.second.path.c_str()
        );
    }
}

map<unsigned int, build> getBuilds()
{
    return this->registery;
}

private:

};

// MACRO FOR REGISTERING BUILDS
// Can be used in header files - uses static initialization pattern
// Creates a unique static variable that registers the build when initialized
// 
// Usage: REGISTER_BUILD("build-name", base_name, "path/to/file.h")
// Example: REGISTER_BUILD("test-build", test_build_1, "templates/test_build_1.h")
//          This will use test_build_1_zip[] and test_build_1_len from the header file

#define REGISTER_BUILD(name, base_name, path) \
    namespace { \
        static bool _register_build_##__LINE__ = []() { \
            build_registery::Instance().register_build( \
                name, \
                BUILD_DATA(base_name), \
                BUILD_SIZE(base_name), \
                path \
            ); \
            return true; \
        }(); \
    }










