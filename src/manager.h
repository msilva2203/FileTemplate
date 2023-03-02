//
// Created by Marco Silva
//

#include <string.h>
#include <fstream>
#include <vector>

#include "mstring.h"

enum class EResult
{
    SUCCESS = 0,
    FAIL = 1,
    FAIL_FETCH = 2,
    FAIL_CREATION = 3,
    FAIL_FORMAT = 4,
    FAIL_NODATA = 5
};

enum class ESetting
{
    NONE = 0,
    NAME = 1,
    AUTHOR = 2,
    INCLUDE = 3,
    USER_INCLUDE = 4,
    TEXT = 5,
    CLASS = 6
};

struct SSettings
{
    SSettings() : _fileName("file.file"), _fileAuthor("unknown author"), _result(EResult::FAIL) {}

    inline void SetName(const std::string& mName) { _fileName = mName; } 
    inline void SetAuthor(const std::string& mAuthor) { _fileAuthor = mAuthor; }
    inline void SetClass(const std::string& mClass) { _class = mClass; }
    inline void AddInclude(const std::string& mStr) { _includes.push_back(mStr); }
    inline void AddUserInclude(const std::string& mStr) { _userIncludes.push_back(mStr); }
    inline void AddText(const std::string& mStr) { _text.push_back(mStr); }
    inline void SetResult(const EResult mResult) { _result = mResult; }

    const std::string& GetName() const
    {
        if (HasName()) return _fileName;
        return GetClass();
    }
    const std::string& GetAuthor() const { return _fileAuthor; }
    const std::string& GetClass() const { return _class; }
    const std::vector<std::string>& GetIncludes() const { return _includes; }
    const std::vector<std::string>& GetUserIncludes() const { return _userIncludes; }
    const std::vector<std::string>& GetText() const { return _text; }
    const bool& IsClass() const { return _isClass; }
    const bool& HasName() const { return _hasName; }
    const EResult GetResult() const { return _result; }

    void UpdateSetting(const ESetting& mSetting, std::string& mStr)
    {
        mstring::trim(mStr);
        switch (mSetting)
        {
            case ESetting::AUTHOR:
            SetAuthor(mStr);
            return;

            case ESetting::NAME:
            SetName(mStr);
            _hasName = true;
            return;

            case ESetting::INCLUDE:
            AddInclude(mStr);
            return;

            case ESetting::USER_INCLUDE:
            AddUserInclude(mStr);
            return;

            case ESetting::TEXT:
            AddText(mStr);
            return;

            case ESetting::CLASS:
            SetClass(mStr);
            _isClass = true;
            return;

            case ESetting::NONE:
            return;
        }
    }

    private:
    std::string _fileName;
    std::string _fileAuthor;
    std::string _class;
    std::vector<std::string> _includes;
    std::vector<std::string> _userIncludes;
    std::vector<std::string> _text;
    bool _hasName = false;
    bool _isClass = false;
    EResult _result;
};

class Manager
{
    public:
    Manager(const std::string& mFileName = "template_settings.txt");

    private:
    static const SSettings FetchSettings(const std::string& mFileName);
    static const EResult CreateTemplateFiles(const SSettings& mSettings);
    static void WriteFileHeader(const SSettings& mSettings, std::fstream& mFile);
    static void WriteClassDefinition(const SSettings& mSettings, std::fstream& mFile);
    static void WriteClassDeclaration(const SSettings& mSettings, std::fstream& mFile);

    static ESetting GetSetting(const std::string& mStr);
    inline void PrintResult(const EResult& mResult);

    std::string _fileName;

};