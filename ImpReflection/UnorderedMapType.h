#pragma once
#include "Type.h"
#include "TypeConverter.h"
#include "TypeManager.h"

namespace ImpReflection
{
	/// <summary>
	/// std::unordered_map
	/// T1 : key , T2 : value
	/// </summary>
	template <typename T1, typename T2>
	class UnorderedMapType : public Type
	{
	public:
		UnorderedMapType() : Type("std::unordered_map<>", sizeof(std::unordered_map<T1, T2>), TypeCategory::UnorderedMap),
			keyType{ TypeConverter<T1>::Get() }, valueType{ TypeConverter<T2>::Get() }
		{
			/// 객체생성시에는 virtual 함수를 호출하지 않는다.
			std::string fullName = "std::unordered_map<";
			fullName += TypeConverter<T1>::Get()->GetFullName();
			fullName += ",";
			fullName += TypeConverter<T2>::Get()->GetFullName();
			fullName += ">";
			TypeManager::GetInstance()->AddTypeInfomation(
				fullName, { std::type_index(typeid(std::unordered_map<T1,T2>)), this });
		}

		std::string GetFullName()const override
		{
			std::string fullName = "std::unordered_map<";
			fullName += keyType->GetFullName();
			fullName += ",";
			fullName += valueType->GetFullName();
			fullName += ">";
			return fullName;
		}

		void Write(const void* object, std::string& data, int indentLevel /* = 0 */)const override
		{
			const std::unordered_map<T1, T2>* map = 
				reinterpret_cast<const std::unordered_map<T1, T2>*>(object);

			data += GetFullName();
			if (map->size() == 0)
			{
				data += "{}";
			}
			else
			{
				// { [key]value }
				data += "{\n";
				for (const auto& iter : *map)
				{
					data += std::string(4 * (indentLevel + 1), ' ');
					data += '[';
					// key
					keyType->Write(&(iter.first), data, indentLevel + 1);
					data += ']';
					valueType->Write(&(iter.second), data, indentLevel + 1);
					data += '\n';
				}
				data += std::string(4 * indentLevel, ' ') + '}';
			}
		}
		
		void Read(void* object, const std::string& data, size_t begin, size_t end)const override
		{
			std::unordered_map<T1, T2>* map = reinterpret_cast<std::unordered_map<T1, T2>*>(object);
			map->clear();

			if (begin + 1 >= end - 1)
			{
				return;
			}
			else
			{
				size_t start = begin + 1;

				while (true)
				{
					std::string sub = ImpStringHelper::GetKeySubstr(data, start);

					if (sub == ImpStringHelper::empty)
					{
						return;
					}

					T1 key;
					keyType->Read(&key, sub, 0, sub.size());

					start = ImpStringHelper::FindClosingBrace(data, start) + 1;

					size_t open = ImpStringHelper::FindOpeningBrace(data, start);
					size_t close = ImpStringHelper::FindClosingBrace(data, start);

					T2 value;
					valueType->Read(&value, data, open, close);

					map->insert({ key,value });

					start = close + 1;
				}
			}

		}

	private:
		Type* keyType;
		Type* valueType;
	};

}
