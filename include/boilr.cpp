#include "boilr.h"
#include "registerBuilds.h"  // This registers all builds automatically
#include "buildRegistry.h"
#include <climits>
#include <cstring>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <memory>
#include <set>

#ifdef _WIN32
#include <windows.h>
#endif

#define BR boilr
namespace fs =  std::filesystem;

// Cross-platform color support
namespace {
    void init_terminal_colors() {
        #ifdef _WIN32
            // Enable ANSI escape sequences in Windows 10+
            HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
            if (hOut != INVALID_HANDLE_VALUE) {
                DWORD dwMode = 0;
                if (GetConsoleMode(hOut, &dwMode)) {
                    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                    SetConsoleMode(hOut, dwMode);
                }
            }
        #endif
    }
    
    const char* COLOR_GREEN = "\033[32m";
    const char* COLOR_RED = "\033[91m";
    const char* COLOR_RESET = "\033[0m";
}


BR::boilr()
{
    // Initialize terminal colors for cross-platform support
    init_terminal_colors();
    // Builds are automatically registered via registerBuilds.h include above
    this->registry = build_registery::Instance();
    // user_config is automatically initialized with default values from USER_CONFIG struct
}
BR::boilr(USER_CONFIG& config)
{
    // Initialize terminal colors for cross-platform support
    init_terminal_colors();
    // Builds are automatically registered via registerBuilds.h include above
    this->registry      = build_registery::Instance();
    this->user_config   = config;
}



BR::~boilr(){}

void BR::set_user_config(USER_CONFIG& config)
{
    this->user_config = config;
}

/**
    Prints out help menu
    - shows how to use tool
    - shows command args options

    NOTE:
    to make a template yourself and have it compiled in 
    the c++ binary is to run this command on your build 
    zipped folder xxd -i my-build-app.zip > my_build_app_zip.h

*/
void BR::help()
{
    cout << this->help_message() << endl;
}

/**
    holds large help message string
    and returns it for use
*/
string BR::help_message(){
    return R"(boilr - A lightweight CLI tool for quickly scaffolding and building minimal full-stack projects

USAGE:
    boilr [OPTIONS]

DESCRIPTION:
    Boilr is a CLI tool for quickly scaffolding and building minimal full-stack
    projects. Perfect for demos, MVPs, hackathons, or rapid prototyping.
    
    You must specify either a template ID (-I) or template name (-TN) to select
    a build template. Optionally specify a project name and destination directory.

OPTIONS:
    -h, --help             Show this help message and exit
    
    -pr, -print-registry   Print all available build templates in the registry
    
    -I, -ID <id>           Specify the template ID to use
    
    -TN, -TNAME <name>     Specify the template name to use
    
    -N, -NAME <name>       Set the project name (default: boilr-template)
    
    -D, -DESTINATION <path> Set the destination directory for the project
                            (default: current directory)

EXAMPLES:
    boilr -h
        Show this help message
    
    boilr -pr
        List all available templates
    
    boilr -I 1 -N my-project -D ./projects
        Create a project using template ID 1 with name "my-project" in ./projects
    
    boilr -TN my-template -N my-app -D ~/workspace
        Create a project using template named "my-template" with name "my-app"
        in ~/workspace directory

NOTES:
    For convenience, add this tool to your system PATH so you can run it from
    anywhere without specifying the full path.

AUTHOR:
    Elias A. Lopes
)";
}

void BR::print_registry()
{
    this->registry.print_registry();
}

// --------------------------------------------------------
// checks configuration before template is injected
bool BR::verify_config(){
    const USER_CONFIG config = this->user_config;
    // see if config specifies template id and name
    if (config.id < 0 && config.template_name == ""){
        cout << "[ERROR] No -ID and -N provided, you must specify at least one" << endl;
        return false;
    }
    // fetch the builds form registry
    map<unsigned int, build> builds = this->registry.getBuilds();
    build* chosen_build = nullptr;

    // check name
    // loop though map to find a build with matching name
    if (config.id < 0)
    {
        // check to see if there a match by name (return the key)
        int id = verify_template_name(builds, config.template_name);
        // no match found
        if (id == INT_MIN) { return false; }
        // get build by id key
        chosen_build = &builds.at(id);
        
    }
    // check id
    // find build with a matching id
    else if (config.template_name == "")
    {
        if (!verify_id(builds, config.id))
        {
            return false;
        }
        chosen_build = &builds.at(config.id);
    }else 
    {
        // tries to get build by id first
        if (builds.find(config.id) != builds.end())
        {
            chosen_build = &builds.at(config.id);
        }

        // runs if first check failed
        if (!chosen_build)
        {
            // finds a match by name and returns the id
            int id = verify_template_name(builds, config.template_name);
            if (id != INT_MIN)
            {
                chosen_build = &builds.at(id);
            }
        }

    }
    if (!chosen_build)
    { 
        cout << "[PROC]Verifying Configuration... " << COLOR_RED << "FAIL" << COLOR_RESET << "\n";
        return false; 
    }
    cout << "[PROC]Verifying Configuration... " << COLOR_GREEN << "OK" << COLOR_RESET << "\n";
    cout << "[PROC]Attempting Insertion... " << COLOR_GREEN << "OK" << COLOR_RESET << "\n";
    
    // attempt to insert build
    return insert(chosen_build);
}
bool BR::verify_id(map<unsigned int, build>& builds, const int id)
{
    if (builds.find(id) == builds.end()){
        return false;
    }
    return true;
}
int BR::verify_template_name(map<unsigned int, build>& builds, const string name)
{
    // checks each pair
    for (auto& pair : builds)
    {
        if (pair.second.name == name)
        {
            return pair.first;
        }
    }
    return INT_MIN;
}
bool BR::verify_destination(const string name)
{
    if (fs::exists(name)){
        cout << "[PROC]Verifying Destination... " << COLOR_GREEN << "OK" << COLOR_RESET << "\n";
        return true;
    }
    cout << "[PROC]Verifying Destination... " << COLOR_RED << "FAIL" << COLOR_RESET << "\n";
    return false;
}

bool BR::insert(build* b)
{
    const USER_CONFIG config = this->user_config;
    // verify valid destination args
    if (!verify_destination(config.project_destination))
    {
        return false;
    }
    // reconstruct byte .h file into destination
    if (!write_zip(b))
    {
        return false;
    }
    // unzip ZIP file
    fs::path dest_dir = fs::path(config.project_destination);
    fs::path zip_path = dest_dir / (config.project_name + ".zip");
    
    // Get list of directories before extraction to find what was extracted
    std::set<fs::path> dirs_before;
    if (fs::exists(dest_dir) && fs::is_directory(dest_dir))
    {
        for (const auto& entry : fs::directory_iterator(dest_dir))
        {
            if (fs::is_directory(entry.path()))
            {
                dirs_before.insert(entry.path());
            }
        }
    }
    
    if (!this->unzip(zip_path, dest_dir))
    {
        cout << "[PROC]Extracting Template... " << COLOR_RED << "FAIL" << COLOR_RESET << "\n";
        return false;
    }
    cout << "[PROC]Extracting Template... " << COLOR_GREEN << "OK" << COLOR_RESET << "\n";
    
    // Find the newly extracted folder and rename it to project name
    fs::path extracted_folder;
    for (const auto& entry : fs::directory_iterator(dest_dir))
    {
        if (fs::is_directory(entry.path()) && dirs_before.find(entry.path()) == dirs_before.end())
        {
            extracted_folder = entry.path();
            break;
        }
    }
    
    // Rename extracted folder to project name
    if (!extracted_folder.empty())
    {
        fs::path project_folder = dest_dir / config.project_name;
        if (fs::exists(project_folder))
        {
            fs::remove_all(project_folder);
        }
        fs::rename(extracted_folder, project_folder);
    }
    
    if (!clean_up(zip_path))
    {
        cout << "[PROC]Removing ZIP... " << COLOR_RED << "FAIL" << COLOR_RESET << "\n";
        return false;
    }
    cout << "[PROC]Removing ZIP... " << COLOR_GREEN << "OK" << COLOR_RESET << "\n";
    return true;
}


bool BR::write_zip(build* b)
{
    const USER_CONFIG config = this->user_config;
    // Explicit destination directory
    fs::path dest_dir = fs::path(config.project_destination);

    // Explicit zip file path
    fs::path zip_path = dest_dir / (config.project_name + ".zip");

    // Ensure destination directory exists
    fs::create_directories(config.project_destination);

    // Write ZIP file
    std::ofstream out(zip_path, std::ios::binary);
    if (!out)
    {
        cout << "[PROC]Writing Zip Template... " << COLOR_RED << "FAIL" << COLOR_RESET << "\n";
        return false;
    } 

    // Write bytes to ZIP file
    out.write(reinterpret_cast<const char*>(b->header_data), b->header_size);
    out.close();
    cout << "[PROC]Writing Zip Template... " << COLOR_GREEN << "OK" << COLOR_RESET << "\n";
    return true;
}

bool BR::unzip(const fs::path& zip_file, const fs::path& dest_dir) 
{
    fs::create_directories(dest_dir);

    #ifdef _WIN32
        // Windows 10+ has tar built-in, use it for cross-compatibility
        // PowerShell Expand-Archive is also available but tar works better
        std::string cmd = "tar -xf \"" + zip_file.string() +
                        "\" -C \"" + dest_dir.string() + "\"";
    #elif defined(__APPLE__) || defined(__linux__)
        std::string cmd = "unzip -o \"" + zip_file.string() +
                        "\" -d \"" + dest_dir.string() + "\"";
    #else
        // Fallback: try unzip command
        std::string cmd = "unzip -o \"" + zip_file.string() +
                        "\" -d \"" + dest_dir.string() + "\"";
    #endif
        if (std::system(cmd.c_str()) != 0)
            return false;
    return true;
}

bool BR::clean_up(const fs::path& zip_file)
{
    // Use filesystem library for cross-platform file deletion
    try {
        if (fs::exists(zip_file)) {
            fs::remove(zip_file);
            return true;
        }
    } catch (const fs::filesystem_error& e) {
        // Fallback to system command if filesystem library fails
        #ifdef _WIN32
            std::string cmd = "del /f /q \"" + zip_file.string() + "\"";
        #else
            std::string cmd = "rm -f \"" + zip_file.string() + "\"";
        #endif
        if (std::system(cmd.c_str()) != 0)
            return false;
    }
    return true;
}

//----------------------------------------------------------------------
void BR::print_build(build* b)
{
    cout << "BUILD NAME:" << b->name<< endl;
    cout << "BUILD PATH:" << b->path<< endl;
    cout << "BUILD SIZE:" << b->header_size<< endl;
}