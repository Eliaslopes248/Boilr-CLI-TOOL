#pragma once

/**
BRIEF:
    This is a CLI tool use for quick project builds,
    like fast demos, start ups MVP, hackathons, etc.
    I allows users to specify a build type, a project name,
    and a destination for it to be made in.

ADVICE:
    This tool is best when added to your systems PATH for easy use

AUTHOR: Elias A. Lopes
DATE:   01/16/2026

*/
#include "buildRegistry.h"
#include <filesystem>
using namespace std;
namespace fs = filesystem;

// user command config with default values
struct USER_CONFIG {
    int    id                   = -1;
    string template_name        = "";
    string project_name         = "boilr-template";
    string project_destination  = ".";
};

class boilr
{
public:
//-------------------------------------------------------
build_registery registry;       // holds pairs of (project-build -> file_path_to_build.h)
USER_CONFIG     user_config;    // holds specs the user selects through cli
//-------------------------------------------------------
boilr();
boilr(USER_CONFIG& config);
~boilr();
//-------------------------------------------------------
void    help();           // prints help message to standard output
string  help_message();   // holds help message and returns it

// main cli tool operations
void    name_project(const string name); // gives project build a name
void    print_registry();
void    print_build(build* b);
void    set_user_config(USER_CONFIG& conig);


// command checkers
bool    verify_config();
bool    verify_id(map<unsigned int, build>& builds, const int id);
int     verify_template_name(map<unsigned int, build>& builds, const string name);
bool    verify_destination(const string name);
bool    insert(build* b);
bool    write_zip(build* b);
bool    unzip(const fs::path& zip_file, const fs::path& dest_dir);
bool    clean_up(const fs::path& zip_file);


private:
};

