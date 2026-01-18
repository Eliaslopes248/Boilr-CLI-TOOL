#include <cstdlib>
#include <cstring>
#include "include/boilr.h"

using namespace std;

void config_to_string(USER_CONFIG config);
int handle_commands(int argc, char* argv[]);
/**
------------------------------------------------------------------
MAIN: program entry point
------------------------------------------------------------------
*/
int main(int argc, char* argv[]) { return handle_commands(argc, argv); }

/**
------------------------------------------------------------------
HELPERS: main implementation
------------------------------------------------------------------
**/
int handle_commands(int argc, char* argv[])
{
    // user config
    USER_CONFIG user_config;
    // boilr command line tool
    boilr br;
    // parse command line args
    for(int i=0;i<argc;i++)
    {
        // handle help command
        if (strcmp(argv[i], "-help") == 0 || strcmp(argv[i], "-h") == 0){
            br.help();
            exit(0);   
        }
        // handle print registry
        else if (strcmp(argv[i], "-pr") == 0 || strcmp(argv[i], "-print-registry") == 0) {
            br.print_registry();
            exit(0);
        }
        // handle selecting project template id
        else if (strcmp(argv[i], "-ID") == 0 || strcmp(argv[i], "-I") == 0) {
            if (i+1 < argc)
            {
                user_config.id = stoi(argv[++i]);
                continue;
            }
            cout << "[ERROR] invalid number of command args: " << argv[i] << endl;
            exit(-1);
        }
        // handle selecting project build custom name
        else if (strcmp(argv[i], "-N") == 0 || strcmp(argv[i], "-NAME") == 0) {
            if (i+1 < argc)
            {
                user_config.project_name = argv[++i];
                continue;
            }
            cout << "[ERROR] invalid number of command args: " << argv[i] << endl;
            exit(-1);
        }
        // handle selecting project destination file path
        else if (strcmp(argv[i], "-D") == 0 || strcmp(argv[i], "-DESTINATION") == 0) {
            if (i+1 < argc)
            {
                user_config.project_destination = argv[++i];
                continue;
            }
            cout << "[ERROR] invalid number of command args: " << argv[i] << endl;
            exit(-1);
        }
        // handle selecting project template name
        else if (strcmp(argv[i], "-TN") == 0 || strcmp(argv[i], "-TNAME") == 0) {
            if (i+1 < argc)
            {
                user_config.template_name = argv[++i];
                continue;
            }
            cout << "[ERROR] invalid number of command args: " << argv[i] << endl;
            exit(-1);
        }
    }
    cout << "[PROC]Parsing Arguments... " << "\033[32mOK\033[0m\n";
    /**
        INJECTS:
    */
    br.set_user_config(user_config);
    cout << "[PROC]Building Configuration... " << "\033[32mOK\033[0m\n";
    bool result = br.verify_config();

    return result ? 0 : -1;
}

void config_to_string(USER_CONFIG config)
{
    cout << "ID: " << config.id << endl;
    cout << "TEMPLATE NAME: " << config.template_name << endl;
    cout << "PROJECT NAME: " << config.project_name << endl;
    cout << "DESTINATION: " << config.project_destination << endl;
}
