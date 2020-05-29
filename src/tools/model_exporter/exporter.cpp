/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*******************************************************************************/


#include "precompiled.h"
#include "exporter.h"

using namespace Framework;
using namespace Tool;


void ListFiles(const string &aSourcePath, const string &aTargetPath, std::function<void(const std::string&, const std::string&, const std::string&)> aCallback)
{
    if (auto lDir = opendir(aSourcePath.c_str()))
    {
        while (auto f = readdir(lDir))
        {
            //Empty directory
            if (!f->d_name || f->d_name[0] == '.')
                continue;
            //Directory
            if (f->d_type == DT_DIR)
                ListFiles(aSourcePath + f->d_name + Utils::GetPathSeparator(), aTargetPath + f->d_name + Utils::GetPathSeparator(), aCallback);
            //Regular file
            if (f->d_type == DT_REG)
                aCallback(aSourcePath, aTargetPath, f->d_name);
        }
        closedir(lDir);
    }
}

void ExportObject(const std::string &aSourcePath, const std::string &aTargerPath, const std::string &aFileName)
{
    string lFileName = Utils::GetFileName(aFileName);
    string lFileExtension = Utils::GetFileExtension(aFileName);

    if (lFileExtension.compare("obj") != 0)
    {
        CRASH("Can not export the '%s' file to the engine", aFileName.c_str());
        return;
    }

    //Create the directory
    if (!fs::exists(aTargerPath))
        fs::create_directories(aTargerPath);

    //Export the object
    string lSourcePath = string(aSourcePath + aFileName);
    string lTargetModelPath = string(aTargerPath + lFileName + string(".model"));

    vector<char*> lArgv;
    lArgv.push_back((char*)"Program");
    lArgv.push_back((char*)"-o");
    lArgv.push_back((char*)lSourcePath.c_str());
    lArgv.push_back((char*)lTargetModelPath.c_str());
    Tool::Obj2Engine(static_cast<int>(lArgv.size()), lArgv.data());
}

void ExportObjectImages(const std::string &aSourcePath, const std::string &aTargerPath, const std::string &aFileName)
{
    string lFileName = Utils::GetFileName(aFileName);
    string lFileExtension = Utils::GetFileExtension(aFileName);

    //Create the directory
    if (!fs::exists(aTargerPath))
        fs::create_directories(aTargerPath);

    //Export the prev image
    {
        string lImgFileName = string("img_") + lFileName + string(".jpg");
        string lSourcePath = string(aSourcePath + lImgFileName);
        string lTargetPath = string(aTargerPath + lImgFileName);
        if(fs::exists(lSourcePath))
            fs::copy(lSourcePath, lTargetPath); // copy file        
    }

    //Export the 2d image
    {
        string lImgFileName = string("img_wired_") + lFileName + string(".png");
        string lSourcePath = string(aSourcePath + lImgFileName);
        string lTargetPath = string(aTargerPath + lImgFileName);
        if (fs::exists(lSourcePath))
            fs::copy(lSourcePath, lTargetPath); // copy file
    }
}


void Export(const std::string &aSourcePath, const std::string &aTargerPath, const std::string &aFileName)
{
    string lFileName = Utils::GetFileName(aFileName);
    string lFileExtension = Utils::GetFileExtension(aFileName);

    if (lFileExtension.compare("obj") == 0)
    {
        ExportObject(aSourcePath, aTargerPath, aFileName);
        ExportObjectImages(aSourcePath, aTargerPath, aFileName);
    }
}

void IterateRecursively(fs::path path)
{
    if (fs::is_directory(path))
    {
        for (auto & child : fs::directory_iterator(path))
            IterateRecursively(child.path());
    }

    fs::remove_all(path);
    //std::cout << path << std::endl;
}

int Exporter(int argc, char **argv)
{
    if (argc < 4)
    {
        INFO(LogLevel::eLEVEL2, "Insomnium Engine Tools\n\n");
        INFO(LogLevel::eLEVEL2, "Usage: [Exporter] ... PARAMERTERS\n");
        INFO(LogLevel::eLEVEL2, "\n");

        INFO(LogLevel::eLEVEL2, "Options:\n");
        INFO(LogLevel::eLEVEL2, "  -e, --export   Directory\n");
        INFO(LogLevel::eLEVEL2, "                                                    <input_obj>: directory containing the geometry.obj, material.mtl and all textures files\n");
        INFO(LogLevel::eLEVEL2, "                                                    <output_engine>: filename for the engine binary representation file\n\n");
        exit(1);
    }

    if (fs::exists("C:\\Users\\giron\\WorkSpace\\test\\"))
        IterateRecursively("C:\\Users\\giron\\WorkSpace\\test\\");
    ListFiles("C:\\Users\\giron\\WorkSpace\\interviewtest-art\\", "C:\\Users\\giron\\WorkSpace\\test\\", Export);

    INFO(LogLevel::eLEVEL2, "Created %s succesfully\n\n", argv[3]);
    return 0;
}


int main(int argc, char **argv)
{
    Exporter(argc, argv);
    getchar();
    return 0;
}
