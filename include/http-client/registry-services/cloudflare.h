/*
*
* This file declares the service to interact with registry using cloudflare
*
* Author:   Elias Lopes
* Date:     02/18/2026
*/
#pragma once
#include "registryService.h"

class cloudflare_service : registryService
{

public:
/** fetches the registry meta data from the registry */
std::string get_reg_meta_data() override;


};