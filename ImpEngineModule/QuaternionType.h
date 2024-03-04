#pragma once

namespace ImpReflection
{
	class QuaternionType : public Type
	{
	public:
		QuaternionType() : Type("quaternion", sizeof(ImpMath::Quaternion), TypeCategory::Primitive) {}

		void Write(const void* object, std::string& data, int indentLevel /* = 0 */)const override
		{
			data += "quaternion{";
			const ImpMath::Quaternion* val = reinterpret_cast<const ImpMath::Quaternion*>(object);

			data += std::to_string(val->x) + ' ';
			data += std::to_string(val->y) + ' ';
			data += std::to_string(val->z) + ' ';
			data += std::to_string(val->w) + ' ';

			data += "}";
		}

		void Read(void* object, const std::string& data, size_t begin, size_t end)const override
		{
			// in : data = "{x y z w}"

			std::string sValue = data.substr(begin + 1, end - begin - 1);

			ImpMath::Quaternion* q = reinterpret_cast<ImpMath::Quaternion*>(object);

			size_t start = 0;
			size_t close = sValue.find(' ');

			q->x = std::stof(sValue.substr(start, close - start));
			start = close + 1;
			close = sValue.find(' ', start);
			
			q->y = std::stof(sValue.substr(start, close - start));
			start = close + 1;
			close = sValue.find(' ', start);

			q->z = std::stof(sValue.substr(start, close - start));
			start = close + 1;
			close = sValue.find(' ', start);

			q->w = std::stof(sValue.substr(start, close - start));
		}
	};

}
