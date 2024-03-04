#pragma once
#include "Type.h"

namespace ImpReflection
{
/// <summary>
/// std::string
/// </summary>
class StringType: public Type
{
public:
    StringType()
        : Type("std::string", sizeof(std::string), TypeCategory::Primitive)
    {
    }

    void Write(const void* object, std::string& data, int indentLevel /* = 0 */) const override
    {
        data += "std::string{";
        const std::string* val = reinterpret_cast<const std::string*>(object);
        data += *val;
        data += "}";
    }

    void Read(void* object, const std::string& data, size_t begin, size_t end) const override
    {
        // in : "{문자열 형태}"
        std::string str = data.substr(begin + 1, end - begin - 1);

        std::string* strObject = reinterpret_cast<std::string*>(object);
        *strObject             = str;
    }
};


/// <summary>
/// std::wstirng
/// wstring은 조금 복잡하다.
/// </summary>
class WstringType: public Type
{
public:
    WstringType()
        : Type("std::wstring", sizeof(std::wstring), TypeCategory::Primitive)
    {
    }

    void Write(const void* object, std::string& data, int indentLevel /* = 0 */) const override
    {
        const std::wstring* tmp = reinterpret_cast<const std::wstring*>(object);

        std::filesystem::path resouresPath = std::filesystem::current_path();
        resouresPath += L"\\Resources";
        std::filesystem::path currentPath(*tmp);

        if (currentPath.empty())
        {
            data += "std::wstring{}";
            return;
        }

        if (currentPath.is_relative())
        {
            return;
        }

        std::wstring relativePath = currentPath.wstring().substr(
            resouresPath.wstring().size());

        data += "std::wstring{";
        for (wchar_t ch : relativePath)
        {
            data.push_back(static_cast<char>(
                ch & 0xFF)); // 이진 데이터로 저장, 인코딩에 따라 적절히 수정
            data.push_back(static_cast<char>((ch >> 8) & 0xFF));
        }

        data += "}";
    }

    void Read(void* object, const std::string& data, size_t begin, size_t end) const override
    {
        std::wstring* strObj = reinterpret_cast<std::wstring*>(object);
        strObj->clear();

        for (size_t i = begin + 1; i < end - 1; i += 2)
        {
            wchar_t ch = static_cast<unsigned char>(data[i]) |
                         (static_cast<unsigned char>(data[i + 1]) << 8);
            strObj->push_back(ch);
        }

        if (strObj->empty())
        {
            return;
        }

        std::filesystem::path resouresPath = std::filesystem::current_path();
        resouresPath += L"\\Resources";

        *strObj = resouresPath.wstring() + *strObj;
    }
};

} // namespace ImpReflection