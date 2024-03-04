#pragma once

namespace ImpReflection
{
	class ImpColorType :public Type
	{
	public:
		ImpColorType() : Type("ImpColor", sizeof(ImpStructure::ImpColor), TypeCategory::Primitive) {}

		void Write(const void* object, std::string& data, int indentLevel /* = 0 */)const override
		{
			data += "ImpColor{";
			const ImpStructure::ImpColor* val = reinterpret_cast<const ImpStructure::ImpColor*>(object);

			data += std::to_string(val->r) + ' ';
			data += std::to_string(val->g) + ' ';
			data += std::to_string(val->b) + ' ';
			data += std::to_string(val->a) + ' ';

			data += "}";
		}

		void Read(void* object, const std::string& data, size_t begin, size_t end)const override
		{
			// in : data = "{r g b a}"

			std::string sValue = data.substr(begin + 1, end - begin - 1);

			ImpStructure::ImpColor* color = reinterpret_cast<ImpStructure::ImpColor*>(object);

			size_t start = 0;
			size_t close = sValue.find(' ');

			color->r = std::stof(sValue.substr(start, close - start));
			start = close + 1;
			close = sValue.find(' ', start);

			color->g = std::stof(sValue.substr(start, close - start));
			start = close + 1;
			close = sValue.find(' ', start);

			color->b = std::stof(sValue.substr(start, close - start));
			start = close + 1;
			close = sValue.find(' ', start);

			color->a = std::stof(sValue.substr(start, close - start));
		}

	};
}


