/**
*
* This file declares a interface for registry services
*   
* Author:   Elias Lopes
* Date:     02/18/2026
*
*/
#pragma once

#include <iostream>

class registryService
{

public:
/** fetches the registry meta data from the registry */
virtual std::string get_reg_meta_data();


};