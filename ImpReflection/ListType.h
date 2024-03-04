#pragma once
#include "Type.h"
#include "TypeManager.h"
#include "TypeConverter.h"

namespace ImpReflection
{
	/// <summary>
	/// std::list
	/// </summary>
	template <typename T>
	class ListType : public Type
	{
	public:
		ListType() : Type("std::list<>", sizeof(std::list<T>), TypeCategory::List),
			itemType{ TypeConverter<T>::Get() }
		{
			std::string fullName = "std::list<";
			fullName += TypeConverter<T>::Get()->GetFullName();
			fullName += ">";
			TypeManager::GetInstance()->AddTypeInfomation(fullName, { std::type_index(typeid(std::list<T>)),this });
		}

	public:
		std::string GetFullName()const override
		{
			// std::list<T>
			std::string tmp = std::string("std::list<");
			tmp += itemType->GetFullName();
			tmp += ">";

			return tmp;
		}

		void Write(const void* object, std::string& data, int indentLevel /* = 0 */)const override
		{
			const std::list<T>* list = reinterpret_cast<const std::list<T>*>(object);

			size_t size = list->size();

			data += GetFullName();
			
			// 빈 List인 경우
			if (size == 0)
			{
				data += "{}";
			}
			else
			{
				data += "{\n";
				int i = 0;
				for (const T& item : *list)
				{
					data += std::string(4 * (indentLevel + 1), ' ');
					data += "[" + std::to_string(i) + "]";
					itemType->Write(reinterpret_cast<const void*>(&item), data, indentLevel + 1);
					data += '\n';
					++i;
				}
				data += std::string(4 * indentLevel, ' ') + '}';
			}
		}

		void Read(void* object, const std::string& data, size_t begin, size_t end)const override
		{
			std::list<T>* list = reinterpret_cast<std::list<T>*>(object);
			list->clear();

			// {} 인 경우
			if (begin + 1 >= end - 1)
			{
				return ;
			}
			else
			{
				size_t start = begin + 1;

				while (true)
				{
					size_t index = ImpStringHelper::FindVectorIndex(data, start);
					if (index == std::string::npos)
					{
						break;;
					}

					size_t open = ImpStringHelper::FindOpeningBrace(data, start);
					size_t close = ImpStringHelper::FindClosingBrace(data, start);
					
					T member;

					itemType->Read((char*)&member, data, open, close);
					list->push_back(member);

					start = close + 1;
				}
			}

		}

	private:
		Type* itemType;

	};


}