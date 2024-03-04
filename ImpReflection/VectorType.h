#pragma once
#include "Type.h"
#include "TypeConverter.h"
#include "TypeManager.h"

namespace ImpReflection
{
	/// <summary>
	/// std::vector
	/// </summary>
	
	template <typename T>
	class VectorType : public Type
	{
	public:
		VectorType() :Type("std::vector<>", sizeof(std::vector<T>), TypeCategory::Vector),
			itemType{TypeConverter<T>::Get()}
		{
			std::string fullName = "std::vecotr<";
			fullName += TypeConverter<T>::Get()->GetFullName();
			fullName += ">";
			TypeManager::GetInstance()->AddTypeInfomation(fullName, { std::type_index(typeid(std::vector<T>)) ,this });
		}

		std::string GetFullName() const override
		{
			// std::vector<itemType> 
			std::string tmp = std::string("std::vector<");
			tmp += itemType->GetFullName();
			tmp += ">";
			
			return tmp;
		}

		void Write(const void* object, std::string& data, int indentLevel /* = 0 */)const override
		{
			const std::vector<T>* vec = reinterpret_cast<const std::vector<T>*>(object);

			size_t size = vec->size();

			data += GetFullName();
			if (size == 0)
			{
				data += "{}";
			}
			else
			{
				data += "{\n";
				for (size_t i = 0; i < size; ++i)
				{
					data += std::string(4 * (static_cast<size_t>(indentLevel) + 1), ' ');
					data += "[" + std::to_string(i) + "]";
					itemType->Write(&(*vec)[i], data, indentLevel + 1);
					data += '\n';
				}
				data += std::string(4 * indentLevel, ' ') + '}';
			}

		}

		void Read(void* object, const std::string& data, size_t begin, size_t end)const override
		{
			std::vector<T>* vec = reinterpret_cast<std::vector<T>*>(object);
			vec->clear();

			// {}로 들어오는경우
			if (begin+1 >= end-1)
			{
				return;
			}
			else
			{
				size_t start = begin+1;	

				while (true)
				{
					size_t index = ImpStringHelper::FindVectorIndex(data, start);
					if (index == std::string::npos)
					{
						break;
					}

					size_t open = ImpStringHelper::FindOpeningBrace(data, start);
					size_t close = ImpStringHelper::FindClosingBrace(data, start);

					T member;

					itemType->Read((char*)&member, data, open, close);
					vec->push_back(member);

					start = close + 1;
				}

			}

		}

	private:
		Type* itemType;
	};

}
