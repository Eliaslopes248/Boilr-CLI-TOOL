'''

This file automates to process of taking all the built binaries and putting them
inside the designated release directory

1. opens every build-* file in th root directory
2. checks if theres a ./br binary
3. creates a ~/releases/build-[platfrom-name] directory
4. moves ./br binary into the directory

'''
import os
import shutil

project_root = "../../.."
project_directories = os.listdir(project_root)

#build_directories = set()
build_directories = {}

# iterate through project top level directories
for dir in project_directories:
    # locate a build directory for a platform
    full_path = os.path.join(project_root, dir)
    if os.path.isdir(full_path) and dir.startswith("build-"):
        build_directories[dir] = full_path

top_releases_dir = "../../../releases"

# add each build dir to releases dir
for shortPath, longPath in build_directories.items():
    platform_release_dir = os.path.join(top_releases_dir, shortPath)
    # create directory for release if it doesnt exist already
    if not os.path.isdir(platform_release_dir):
        os.mkdir(platform_release_dir)
    # add ./br if there is one
    exe = "br"
    
    # add .exe if windows build
    if "windows" in shortPath:
        exe = "br.exe"

    br_executable_path = os.path.join(longPath, exe)

    if os.path.exists(br_executable_path):
        try:
            shutil.copy(br_executable_path, platform_release_dir)
        except FileNotFoundError as e:
            print("File System error when coping", e)
            
    


    




