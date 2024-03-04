#pragma once

/// Reflection Macro Header

#define REFLECTION_FRIEND(type)\
friend class ImpReflection::Meta<type>;

/// ���ø� Ư��ȭ ����, static ���� �ʱ�ȭ�� ����ؼ� type������ �����Ѵ�.

/// class 
#define REFLECTION_CHECK(type)\
		template<>\
		class ImpReflection::Meta<type>\
		{\
		public:\
			constexpr static bool IsReflect = true;\
			static class ImpReflection::ClassType _type;\
			static void Initialize(ImpReflection::ClassType*);\
		};


/// ������Ʈ�� �ƴ� ���÷����� ��ũ�θ� ���
#define REFLECTION_DATA_BEGIN(type)\
		REFLECTION_CHECK(type)\
		ImpReflection::ClassType ImpReflection::Meta<type>::_type\
 			{ImpReflection::Meta<type>::Initialize};\
		void ImpReflection::Meta<type>::Initialize(ImpReflection::ClassType* classType)\
		{\
			using T = type;\
			classType->Set(TypeCategory::Class,#type,sizeof(T),[]()->void*{return new T();},Tag::Default);\
			TypeManager::GetInstance()->AddTypeInfomation(#type,{std::type_index{typeid(T)},classType});\

/// ������Ʈ ������ ���÷����� �Ʒ� ��ũ�θ� ���
#define REFLECTION_BEGIN(type)\
		REFLECTION_CHECK(type)\
		ImpReflection::ClassType ImpReflection::Meta<type>::_type\
 			{ImpReflection::Meta<type>::Initialize};\
		void ImpReflection::Meta<type>::Initialize(ImpReflection::ClassType* classType)\
		{\
			using T = type;\
			classType->Set(TypeCategory::Class,#type,sizeof(T),[]()->void*{return new T();});\
			TypeManager::GetInstance()->AddTypeInfomation(#type,{std::type_index{typeid(T)},classType});\

#define REFLECTION_MEMBER(name)\
		 classType->_members.push_back(ImpReflection::ClassType::Member\
		{#name,offsetof(T,name),ImpReflection::TypeConverter<decltype(T::name)>::Get()});

#define REFLECTION_MEMBER_MAP(name,key,value)\
		classType->_members.push_back(ImpReflection::ClassType::Member\
		{#name,offsetof(T,name),ImpReflection::MapTypeConverter<key,value>::Get()});

#define REFLECTION_MEMBER_UNORDERED_MAP(name,key,value)\
		classType->_members.push_back(ImpReflection::ClassType::Member\
		{#name,offsetof(T,name),ImpReflection::UnorderedMapTypeConverter<key,value>::Get()});

#define REFLECTION_END() }

/// enum class

#define REFLECTION_ENUM_CEHCK(type)\
		template<>\
		class ImpReflection::Meta<type>\
		{\
		public:\
			constexpr static bool IsReflect = true;\
			static class ImpReflection::EnumClassType<type> _type;\
			static void Initialize(ImpReflection::EnumClassType<type>*);\
		};

#define REFLECTION_ENUM_BEGIN(type)\
		REFLECTION_ENUM_CEHCK(type)\
		ImpReflection::EnumClassType<type> ImpReflection::Meta<type>::_type\
		{ImpReflection::Meta<type>::Initialize};\
		void ImpReflection::Meta<type>::Initialize(class ImpReflection::EnumClassType<type>* enumClassType)\
		{\
			using T = type;\
			enumClassType->Set(TypeCategory::EnumClass, #type, sizeof(T),[]()->void*{return nullptr;});\
			TypeManager::GetInstance()->AddTypeInfomation(#type,{std::type_index{typeid(T)},enumClassType});

#define REFLECTION_ENUM_MEMBER(name)\
		enumClassType->AddEnumMember({T::name,#name,static_cast<int>(T::name)});
#define REFLECTION_ENUM_END()\
		};