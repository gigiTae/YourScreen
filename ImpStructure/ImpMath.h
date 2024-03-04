#pragma once

/// <summary>
/// DirectXMath 없이 Position, Rotation 등을 전달하기 위해
/// 필요한 클래스 -> Math 라이브러리를 따로 만들기로 함
///
/// 일단 만들었는데 생각해보니까 이렇게 할 필요가 있는지는 모르겠다.
/// 그냥 Vector3만 만들어주고 xmfloat3로 사용하면 됐던거 아닐까
/// 모든 연산을 지원할 필요가 있나???
/// </summary>
namespace ImpMath
{
	static constexpr float PI = { 3.14159265358979323846f };
	static constexpr float TwoPI = { 2.f * PI };
	static constexpr float HalfPI = { 1.57079632679f };
    static constexpr float QuaterPI = { PI * 0.25f };
	static constexpr float InvPI = { 0.31830988618f };

	// 라디안을 각도
	constexpr float RadianToDegree(float radian)
	{
		return radian * 180.f * InvPI;
	}
	// 각도를 라이안 
	constexpr float DegreeToRadian(float degree)
	{
		return degree * PI / 180.f;
	}
	

	

	struct Quaternion;
	struct Matrix;

	struct Vector3
	{
		Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f); // 생성자

		Vector3(const Vector3& rhs) = default; // 복사 생성자
		Vector3& operator=(const Vector3& rhs) = default; // 복사 대입 연산자

		Vector3 operator-()const;

		// 덧셈 연산자 오버로딩 + +=
		Vector3 operator+(const Vector3& rhs) const;
		void operator+=(const Vector3& rhs);

		// 뺄셈 연산자 오버로딩 - -=
		Vector3 operator-(const Vector3& rhs) const;
		void operator-=(const Vector3& rhs);

		// 스칼라 곱 *, *= 
		Vector3 operator*(const float scalar) const;
		void operator*=(const float scalar);
		  
		// 스칼라 나눗셈 /, /=
		Vector3 operator/(const float scalar) const;
		void operator/=(const float scalar);

		// Dot 연산 == * 지원
		float Dot(const Vector3& other) const;
		float operator*(const Vector3& rhs) const;

		// Cross 연산
		Vector3 Cross(const Vector3& other) const;

		// 논리 연산자 오버로딩 ==, !=
		bool operator==(const Vector3& rhs) const;
		bool operator!=(const Vector3& rhs) const;

		// KJY 추가 : 사전 순서대로 단순 비교
		// 내부적으로 이진 트리로 동작하는 std 컨테이너를 사용하기 위해 임의로 정의
		// 이항 연산자 
		bool operator<(const Vector3& rhs) const;
		bool operator>(const Vector3& rhs) const;

		// 벡터의 크기 계산
		float Length() const;
		// 비교 등을 할 때 루트 연산을 줄이기 위해 제곱 버전도 제공
		float LengthSquared() const;
		// Normalize 연산
		Vector3 Normalize() const;

		// 벡터 사잇각 계산
		float CosTheta(const Vector3& other) const;

		// Matix로 변환한 점의 좌표를 반환한다.
		Vector3 Transform(const Matrix& m) const;

		// 정적 멤버 변수
		static const Vector3 Zero;
		static const Vector3 One;
		static const Vector3 UnitX;
		static const Vector3 UnitY;
		static const Vector3 UnitZ;

		/// 멤버 변수
	public:
		float x, y, z;
	};

	// vector3 가 rhs로 들어오는 경우도 생각할 것

	struct Vector4
	{
		Vector4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f); // 생성자
		Vector4(Vector3 vec3);
		Vector4(Vector3 vec3, float w);

		Vector4(const Vector4& rhs) = default; // 복사 생성자
		Vector4& operator=(const Vector4& rhs) = default; // 복사 대입 연산자

		// 덧셈 연산자 오버로딩 + +=		
		Vector4 operator+(const Vector4& rhs) const;
		void operator+=(const Vector4& rhs);

		// 뺄셈 연산자 오버로딩 - -=
		Vector4 operator-(const Vector4& rhs) const;
		void operator-=(const Vector4& rhs);

		// 스칼라 곱 *, *=
		Vector4 operator*(const float scalar) const;
		void operator*=(const float scalar);

		// 스칼라 나눗셈 /, /=
		Vector4 operator/(const float scalar) const;
		void operator/=(const float scalar);

		// Dot 연산 == * 지원
		float Dot(const Vector4& other) const;
		float operator*(const Vector4& rhs) const;

		// 논리 연산자 오버로딩 ==, !=
		bool operator==(const Vector4& rhs) const;
		bool operator!=(const Vector4& rhs) const;

		// 벡터의 크기 계산
		float Length() const;
		// 비교 등을 할 때 루트 연산을 줄이기 위해 제곱 버전도 제공
		float LengthSquared() const;
		// Normalize 연산
		Vector4 Normalize() const;

		// 벡터 사잇각 계산
		float CosTheta(const Vector4& other) const;

		/// 멤버 변수
	public:
		float x, y, z, w;
	};


	struct Matrix
	{
		Matrix();
		Matrix(const float* array);
		Matrix(const Matrix& rhs);

		static Matrix Identity();

		Matrix& operator=(const Matrix& rhs); // 복사 대입 연산자

		Matrix operator*(const Matrix& rhs) const;
		void operator*=(const Matrix& rhs);

		// 오일러 각 3개로 회전 행렬을 만드는 함수
		static Matrix MakeRotationMatrixRollPitchYaw(Vector3 rotation);
		
		// 쿼터니온으로 회전 행렬을 만드는 함수
		static Matrix MakeFromQuaternion(const Quaternion& q);

		// 이동 행렬을 만드는 함수
		static Matrix MakeTranslationMatrix(Vector3 postion);

		// 스케일 행렬을 만드는 함수
		static Matrix MakeScaleMatrix(Vector3 scale);

		// 역행렬 반환 
		Matrix Inverse()const;

		// 전치 행렬
        Matrix Transpose() const;

		bool Decompose(Vector3& scale,
            Quaternion& rotation,
            Vector3& translation) noexcept;

		// 정적 멤버 변수
		static const Vector4 UnitX;
		static const Vector4 UnitY;
		static const Vector4 UnitZ;
		static const Vector4 UnitW;
		static const Vector4 One;
		static const Vector4 Zero;

		/// 멤버 변수
	public:
		union
		{
			float m[4][4];
			Vector4 v[4];
			struct
			{
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
		};
	};

	struct Quaternion
	{
		Quaternion(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f);
		
		// 오일러 각으로 변환한다.
		Vector3 ToEuler()const;

		
		/// opeartor
		Quaternion& operator*= (const Quaternion& q);

		Quaternion operator* (const Quaternion& rfm)const;

		// 역방향 쿼터니온 반환
		Quaternion Inverse() const;

		/// 정적멤버함수

		// 축과 각도로 쿼터니온을 만드는 함수
		static Quaternion MakeFromAxis(const Vector3& axis, float radian);
		// 오일러각으로 쿼터니온을 만드는 함수
		static Quaternion MakeFromYawPitchRoll(float yaw, float pitch, float roll);

		// fromDir에서 toDir가는 회전 쿼터니온을 반환한다.
		static Quaternion MakeFromToRoation(const Vector3& fromDir,
                                            const Vector3& toDir);

		// 바라보는 방향으로 쿼터니온을 만드는 함수
		static Quaternion MakeLookRotation(const Vector3& zAxis,
                                               const Vector3& up);

		static Quaternion QuaternionSlerp(const Quaternion& a,
            const Quaternion& b,
            float ratio);
        static const Quaternion Identity;
		/// 멤버 변수
	public:
		float x=0.f;
		float y=0.f;
		float z=0.f;
		float w=1.f;
	};
}