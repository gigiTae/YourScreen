#pragma once

#include "Type.h"

namespace ImpReflection
{
	/// <summary>
	/// EnumClass Reflection
	/// </summary>
	template <typename ENUM_TYPE>
	class EnumClassType : public Type
	{
	public:
		struct EnumMember
		{
			ENUM_TYPE type;
			std::string name;
			int castValue; // enum class -> int cast 값
		};

	public:
		EnumClassType(void(*Initialize)(EnumClassType<ENUM_TYPE>*))
			:Type("", 0, TypeCategory::EnumClass)
		{
			Initialize(this);
		}
		~EnumClassType() {}

		/// Eunm Class Data to String
		void Write(const void* object, std::string& data, int indentLevel /* = 0 */)const override;

		/// String to Enum class
		void Read(void* object, const std::string& data, size_t begin, size_t end)const override;
		
		/// EunmMember를 vector에 담아서 반환한다.
		const std::vector<ImpReflection::Type::EnumMember>& GetEnumMembers() const override;

	private:
		/// EunmClass Member 추가
		void AddEnumMember(const EnumMember& member);

	private:
		std::vector<EnumMember> _enumMembers;
		
		template <typename T> friend class ImpReflection::Meta;
	};

	template <typename ENUM_TYPE>
	const std::vector<ImpReflection::Type::EnumMember>& ImpReflection::EnumClassType<ENUM_TYPE>::GetEnumMembers() const
	{
		static std::vector<ImpReflection::Type::EnumMember> vec;

		if (vec.empty())
		{
			for (const EnumMember& member : _enumMembers)
			{
				vec.push_back({ member.name,member.castValue });
			}
		}

		return vec;
	}

	template <typename ENUM_TYPE>
	void ImpReflection::EnumClassType<ENUM_TYPE>::AddEnumMember(const EnumMember& member)
	{
		_enumMembers.push_back(member);
	}

	template <typename ENUM_TYPE>
	void ImpReflection::EnumClassType<ENUM_TYPE>::Write(const void* object, std::string& data, int indentLevel /* = 0 */) const
	{
		data += GetFullName();
		data += "{";
		const int* iObject = reinterpret_cast<const int*>(object);
		data += std::to_string(*iObject);
		data += "}";
	}

	template <typename ENUM_TYPE>
	void ImpReflection::EnumClassType<ENUM_TYPE>::Read(void* object, const std::string& data, size_t begin, size_t end) const
	{
		// in : data = {int}

		int* iObject = reinterpret_cast<int*>(object);

		std::string sValue = data.substr(begin + 1, end - begin);
		int iValue = std::stoi(sValue);

		*iObject = iValue;
	}

}