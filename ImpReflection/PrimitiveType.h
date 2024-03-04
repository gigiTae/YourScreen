#pragma once

#include "Type.h"
#include "PrimitiveFunciton.h"

/// <summary>
/// Primitive Type에 대한 리플렉션 정보를 저장하는 클래스 
/// bool, int, usigned int, float, double, long, long long, usigned long long
/// </summary>

namespace ImpReflection
{
	/// <summary>
	/// int 
	/// </summary>
	class IntType :public Type
	{
	public:
		IntType() : Type("int", sizeof(int), TypeCategory::Primitive) {}

		void Write(const void* object, std::string& data, int indentLevel /* = 0 */)const override
		{
			data += "int{";
			const int* val = reinterpret_cast<const int*>(object);
			data += std::to_string(*val);
			data += "}";
		}

		void Read(void* object, const std::string& data, size_t begin, size_t end)const override
		{
			// in : data = "{intValue}"

			std::string sValue = data.substr(begin + 1, end - begin-1);

			int iValue = std::stoi(sValue);
			int* iObject = reinterpret_cast<int*>(object);
			*iObject = iValue;
		}
	};

	/// <summary>
	/// unsigned int 
	/// </summary>
	class UnsignedIntType :public Type
	{
	public:
		UnsignedIntType() : Type("unsigned int", sizeof(int), TypeCategory::Primitive) {}
		
		void Write(const void* object, std::string& data, int indentLevel /* = 0 */)const override
		{
			data += "unsigned int{";
			const unsigned int* val = reinterpret_cast<const unsigned int*>(object);
			data += std::to_string(*val);
			data += "}";
		}

		void Read(void* object, const std::string& data, size_t begin, size_t end)const override
		{
			// in : data = "{intValue}"

			std::string sValue = data.substr(begin + 1, end - begin - 1);

			unsigned int uiValue = static_cast<unsigned int>(std::stoul(sValue));

			unsigned int* uiObject = reinterpret_cast<unsigned int*>(object);
			*uiObject = uiValue;
		}
	};

	/// <summary>
	/// float
	/// </summary>
	class FloatType : public Type
	{
	public:
		FloatType() : Type("float", sizeof(float), TypeCategory::Primitive) {}

		void Write(const void* object, std::string& data, int indentLevel /* = 0 */)const override
		{
			data += "float{";
			const float* val = reinterpret_cast<const float*>(object);
			data += std::to_string(*val);
			data += "}";
		}

		void Read(void* object, const std::string& data, size_t begin, size_t end)const override
		{
			// in : data = "{floatValue}"

			std::string sValue = data.substr(begin + 1, end - begin - 1);

			float fValue = std::stof(sValue);
			float* fObject = reinterpret_cast<float*>(object);
			*fObject = fValue;
		}
	};

	/// <summary>
	/// long
	/// </summary>
	class LongType : public Type
	{
	public:
		LongType() : Type("long", sizeof(long), TypeCategory::Primitive) {}

		void Write(const void* object, std::string& data, int indentLevel /* = 0 */)const override
		{
			data += "long{";
			const long* val = reinterpret_cast<const long*>(object);
			data += std::to_string(*val);
			data += "}";
		}

		void Read(void* object, const std::string& data, size_t begin, size_t end)const override
		{
			// in : data = "{longValue}"

			std::string sValue = data.substr(begin + 1, end - begin - 1);

			long lValue = std::stol(sValue);
			long* lObject = reinterpret_cast<long*>(object);
			*lObject = lValue;
		}
	};

	/// <summary>
	/// long long
	/// </summary>
	class LongLongType : public Type
	{
	public:
		LongLongType() : Type("long long", sizeof(long long), TypeCategory::Primitive) {}

		void Write(const void* object, std::string& data, int indentLevel /* = 0 */)const override
		{
			data += "long long{";
			const long long* val = reinterpret_cast<const long long*>(object);
			data += std::to_string(*val);
			data += "}";
		}

		void Read(void* object, const std::string& data, size_t begin, size_t end)const override
		{
			// in : data = "{longValue}"

			std::string sValue = data.substr(begin + 1, end - begin - 1);

			long long llValue = std::stoll(sValue);
			long long* llObject = reinterpret_cast<long long*>(object);
			*llObject = llValue;
		}
	};
	
	/// <summary>
	/// unsigned long long
	/// </summary>
	class UnsignedLongLongType : public Type
	{
	public:
		UnsignedLongLongType() : Type("unsigned long long", sizeof(unsigned long long), TypeCategory::Primitive) {}

		void Write(const void* object, std::string& data, int indentLevel /* = 0 */)const override
		{
			data += "unsigned long long{";
			const unsigned long long* val = reinterpret_cast<const unsigned long long*>(object);
			data += std::to_string(*val);
			data += "}";
		}

		void Read(void* object, const std::string& data, size_t begin, size_t end)const override
		{
			// in : data = "{unsigned lobg long Value}"

			std::string sValue = data.substr(begin + 1, end - begin - 1);

			unsigned long long ullValue = std::stoull(sValue);
			unsigned long long* ullObject = reinterpret_cast<unsigned long long*>(object);
			*ullObject = ullValue;
		}
	};

	/// <summary>
	/// double
	/// </summary>
	class DoubleType : public Type
	{
	public:
		DoubleType() : Type("double", sizeof(double), TypeCategory::Primitive) {}

		void Write(const void* object, std::string& data, int indentLevel /* = 0 */)const override
		{
			data += "double{";
			const double* val = reinterpret_cast<const double*>(object);
			data += std::to_string(*val);
			data += "}";
		}

		void Read(void* object, const std::string& data, size_t begin, size_t end)const override
		{
			// in : data = "{doubleValue}"

			std::string sValue = data.substr(begin + 1, end - begin - 1);

			double dValue = std::stod(sValue);
			double* dObject = reinterpret_cast<double*>(object);
			*dObject = dValue;
		}
	};

	/// <summary>
	/// bool
	/// </summary>
	class BoolType : public Type
	{
	public:
		BoolType() :Type("bool", sizeof(bool), TypeCategory::Primitive) {}

		void Write(const void* obj, std::string& data, int indentLevel /* = 0 */) const override
		{
			data += "bool{";
			const bool* val = (const bool*)obj;
			if (*val)
			{
				data += '1';
			}
			else
			{
				data += '0';
			}
			data += "}";
		}

		void Read(void* object, const std::string& data, size_t begin, size_t end) const override
		{
			// in : data = "{0}" or "{1}"

			std::string sValue = data.substr(begin + 1, end - begin-1);
			bool* dObj = reinterpret_cast<bool*>(object);

			if (sValue[0] == '0')
			{
				*dObj = false;
			}
			else
			{
				*dObj = true;
			}
		}
	};
}


