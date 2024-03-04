#pragma once

namespace ImpEngineModule
{
	
	/// <summary>
	/// 리소스관련 정보를 저장하는 클래스
	/// </summary>
	class Resource
	{
	public:
		Resource(ResourceCategory category, const std::wstring& path);
		virtual ~Resource(){}

		/// 리소스를 로드한다.
		virtual void Load() abstract;

		/// 리소스의 경로 반환
		const std::wstring& GetPath() const {  return _path; }
		
		/// 리소스 경로 재설정
		void SetPath(std::wstring path) { _path = path; }
	private:
		std::wstring _path; // 경로 
		ResourceCategory _category; // 리소스 종류
	};



}