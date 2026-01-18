#pragma once

/**
 * BUILD REGISTRATION FILE
 * 
 * This file registers all available template builds.
 * Simply include this file in your code, and all builds will be
 * automatically registered when the program starts.
 * 
 * To add a new build:
 * 1. Create your template zip file
 * 2. Convert it to .h: xxd -i your-template.zip > your-template.h
 * 3. Add a REGISTER_BUILD line below
 */

#include "buildRegistry.h"
#include "../templates/test_build_1.h"

// Register all available builds
// Paths are relative to project root (where templates/ directory exists)
// 
// Format: REGISTER_BUILD("display-name", base_variable_name, "path/to/file.h")
// The base_variable_name should match the prefix used by xxd -i
// For example: xxd -i myfile.zip creates myfile_zip[] and myfile_len
// So use: REGISTER_BUILD("my-build", myfile, "templates/myfile.h")

REGISTER_BUILD("test-build", test_build_1, "../templates/test_build_1.h")
// Add more builds here as you create them:
// REGISTER_BUILD("another-build", "templates/another-build.h")