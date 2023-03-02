//
// Created by Marco Silva
//

#include "manager.h"
#include "debug.h"

Manager::Manager(const std::string& mFileName)
: _fileName(mFileName)
{
    SSettings settings = FetchSettings(_fileName);
    EResult result = EResult::FAIL;

    if (settings.GetResult() == EResult::SUCCESS)
    {
        result = CreateTemplateFiles(settings);
    }
    else result = settings.GetResult();

    PrintResult(result);
}

const SSettings Manager::FetchSettings(const std::string& mFileName)
{
    Debug::PrintString("Searching for template settings file...");

    SSettings settings{};
    std::fstream file;

    file.open(mFileName, std::ios::in);

    if (file.is_open())
    {
        Debug::PrintString("Reading settings file...");

        std::string temp;
        int count = 0;
        ESetting currentSetting = ESetting::NONE;
        ESetting addSetting = ESetting::NONE;

        while (std::getline(file, temp, '+'))
        {
            mstring::trim(temp);
            currentSetting = GetSetting(temp);

            if (currentSetting == ESetting::NONE)
            {
                if (addSetting == ESetting::NONE)
                {
                    settings.SetResult(EResult::FAIL_FORMAT);
                    return settings;
                }

                settings.UpdateSetting(addSetting, temp);
                addSetting = ESetting::NONE;
            }
            else addSetting = currentSetting;
        }

        settings.SetResult(EResult::SUCCESS);
    }
    else settings.SetResult(EResult::FAIL_FETCH);

    if (!(settings.HasName() || settings.IsClass())) settings.SetResult(EResult::FAIL_NODATA);

    file.close();

    return settings;
}

const EResult Manager::CreateTemplateFiles(const SSettings& mSettings)
{
    Debug::PrintString("Creating file...");

    std::vector<std::string> includes;
    std::fstream file;
    file.open(mSettings.GetName() + ".cpp", std::ios::out);
    if (!file.is_open()) return EResult::FAIL_CREATION;

    Debug::PrintString("Populating C++ file...");

    Manager::WriteFileHeader(mSettings, file);

    if (mSettings.IsClass())
    {
        Manager::WriteClassDeclaration(mSettings, file);

        includes = mSettings.GetText();

        if (!includes.empty())
        {
            for (const auto s : includes)
            {
                file << s << std::endl;
            }
        }

        Debug::PrintString("Populating Header file...");

        file.close();
        file.open(mSettings.GetName() + ".h", std::ios::out);
        if (!file.is_open()) return EResult::FAIL_CREATION;

        Manager::WriteFileHeader(mSettings, file);
    }

    includes = mSettings.GetIncludes();

    if (!includes.empty())
    {
        for (const auto s : includes)
        {
            file << "#include <" << s << ">" << std::endl;
        }

        file << std::endl;
    }

    includes = mSettings.GetUserIncludes();

    if (!includes.empty())
    {
        for (const auto s : includes)
        {
            file << "#include \"" << s << "\"" << std::endl;
        }

        file << std::endl;
    }

    Manager::WriteClassDefinition(mSettings, file);

    file.close();

    return EResult::SUCCESS;
}

void Manager::WriteFileHeader(const SSettings& mSettings, std::fstream& mFile)
{
    mFile << "//" << std::endl;
    mFile << "// Created by " << mSettings.GetAuthor() << std::endl;
    mFile << "//" << std::endl << std::endl;
}

void Manager::WriteClassDefinition(const SSettings& mSettings, std::fstream& mFile)
{
    if (!mSettings.IsClass()) return;

    mFile << "class " << mSettings.GetClass() << std::endl;
    mFile << "{" << std::endl;
    mFile << "\tpublic:" << std::endl;
    mFile << "\t" << mSettings.GetClass() << "();" << std::endl;
    mFile << "\t~" << mSettings.GetClass() << "();" << std::endl << std::endl;
    mFile << "\tprivate:" << std::endl << std::endl;
    mFile << "};" << std::endl;

}

void Manager::WriteClassDeclaration(const SSettings& mSettings, std::fstream& mFile)
{
    mFile << "#include \"" << mSettings.GetClass() << ".h\"" << std::endl << std::endl;
    mFile << mSettings.GetClass() << "::" << mSettings.GetClass() << "()" << std::endl;
    mFile << "{" << std::endl;
    mFile << "}" << std::endl << std::endl;
    mFile << mSettings.GetClass() << "::" << "~" << mSettings.GetClass() << "()" << std::endl;
    mFile << "{" << std::endl;
    mFile << "}" << std::endl << std::endl;
}

inline void Manager::PrintResult(const EResult& mResult)
{
    Debug::PrintString("---------------------------------------------------");

    switch (mResult)
    {
        case EResult::SUCCESS:
        Debug::PrintString("Task Complete. File created successfully...");
        Debug::PrintString("---------------------------------------------------");
        return;
        
        case EResult::FAIL:
        Debug::PrintString("Failed to complete task...");
        Debug::PrintString("---------------------------------------------------");
        return;

        case EResult::FAIL_FETCH:
        Debug::PrintString("Failed to complete task. Could not find settings file...");
        Debug::PrintString("---------------------------------------------------");
        return;

        case EResult::FAIL_CREATION:
        Debug::PrintString("Failed to complete task. Coud not create file...");
        Debug::PrintString("---------------------------------------------------");
        return;

        case EResult::FAIL_FORMAT:
        Debug::PrintString("Failed to complete task. Incorrect formatting...");
        Debug::PrintString("---------------------------------------------------");
        return;

        case EResult::FAIL_NODATA:
        Debug::PrintString("Failed to completed task. No enough data. Please, input a File Name or Class Name...");
        Debug::PrintString("---------------------------------------------------");
        return;

        default:
        Debug::PrintString("Default result!");
        Debug::PrintString("---------------------------------------------------");
        return;
    }
}

ESetting Manager::GetSetting(const std::string& mStr)
{
    const char* str = mStr.c_str();

    if (strcmp(str, "NAME") == 0) return ESetting::NAME;
    if (strcmp(str, "AUTHOR") == 0) return ESetting::AUTHOR;
    if (strcmp(str, "INCLUDE") == 0) return ESetting::INCLUDE; 
    if (strcmp(str, "USER INCLUDE") == 0) return ESetting::USER_INCLUDE;
    if (strcmp(str, "TEXT") == 0) return ESetting::TEXT;
    if (strcmp(str, "CLASS") == 0) return ESetting::CLASS;

    return ESetting::NONE;
}