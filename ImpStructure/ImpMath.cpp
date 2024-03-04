#include "pch.h"
#include "ImpMath.h"

#define WIN32_LEAN_AND_MEAN

using namespace ImpMath;
#pragma region Util


#pragma endregion Util


#pragma region Vector3
Vector3::Vector3(float x /*= 0.0f*/, float y /*= 0.0f*/, float z /*= 0.0f*/)
    : x(x)
    , y(y)
    , z(z)
{
}

ImpMath::Vector3 Vector3::operator+(const Vector3& rhs) const
{
    return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
}

void Vector3::operator+=(const Vector3& rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
}

ImpMath::Vector3 Vector3::operator-(const Vector3& rhs) const
{
    return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
}

void Vector3::operator-=(const Vector3& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
}

ImpMath::Vector3 Vector3::operator*(const float scalar) const
{
    return Vector3(x * scalar, y * scalar, z * scalar);
}

void Vector3::operator*=(const float scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
}

ImpMath::Vector3 Vector3::operator/(const float scalar) const
{
    return Vector3(x / scalar, y / scalar, z / scalar);
}

void Vector3::operator/=(const float scalar)
{
    x /= scalar;
    y /= scalar;
    z /= scalar;
}

float Vector3::Dot(const Vector3& other) const
{
    return x * other.x + y * other.y + z * other.z;
}

float Vector3::operator*(const Vector3& rhs) const
{
    return x * rhs.x + y * rhs.y + z * rhs.z;
}

ImpMath::Vector3 Vector3::Cross(const Vector3& other) const
{
    return Vector3(y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x);
}

bool Vector3::operator==(const Vector3& rhs) const
{
    return (x == rhs.x && y == rhs.y) && z == rhs.z;
}

bool Vector3::operator!=(const Vector3& rhs) const
{
    return !operator==(rhs);
}

ImpMath::Vector3 Vector3::operator-() const
{
    return Vector3(-x, -y, -z);
}

bool ImpMath::Vector3::operator<(const Vector3& rhs) const
{
    return std::tie(x, y, z) < std::tie(rhs.x, rhs.y, rhs.z);
}

bool ImpMath::Vector3::operator>(const Vector3& rhs) const
{
    return std::tie(x, y, z) > std::tie(rhs.x, rhs.y, rhs.z);
}

float Vector3::Length() const
{
    return sqrt(x * x + y * y + z * z);
}

float Vector3::LengthSquared() const
{
    return x * x + y * y + z * z;
}

ImpMath::Vector3 Vector3::Normalize() const
{
    // 노말라이즈 불가능한경우
    if (*this == Vector3::Zero)
    {
        return Vector3::Zero;
    }

    return Vector3(x / Length(), y / Length(), z / Length());
}

float Vector3::CosTheta(const Vector3& other) const
{
    float cos = Dot(other);
    cos /= Length() * other.Length();

    return cos;
}

ImpMath::Vector3 Vector3::Transform(const Matrix& m) const
{
    using namespace DirectX;

    XMFLOAT3 v(x, y, z);

    const XMVECTOR v1 = XMLoadFloat3(&v);
    DirectX::XMMATRIX xmMatrix(&m._11);
    const XMVECTOR X = XMVector3TransformCoord(v1, xmMatrix);

    XMFLOAT3 result;
    XMStoreFloat3(&result, X);
    return Vector3(result.x, result.y, result.z);
}

const ImpMath::Vector3 ImpMath::Vector3::Zero(0.f, 0.f, 0.f);
const ImpMath::Vector3 ImpMath::Vector3::One(1.f, 1.f, 1.f);
const ImpMath::Vector3 ImpMath::Vector3::UnitX(1.f, 0.f, 0.f);
const ImpMath::Vector3 ImpMath::Vector3::UnitY(0.f, 1.f, 0.f);
const ImpMath::Vector3 ImpMath::Vector3::UnitZ(0.f, 0.f, 1.f);


#pragma endregion Vector3

#pragma region Vector4
Vector4::Vector4(float x /*= 0.0f*/,
    float y /*= 0.0f*/,
    float z /*= 0.0f*/,
    float w /*= 0.0f*/)
    : x(x)
    , y(y)
    , z(z)
    , w(w)
{
}

Vector4::Vector4(Vector3 vec3)
    : x(vec3.x)
    , y(vec3.y)
    , z(vec3.z)
    , w(0)
{
}

Vector4::Vector4(Vector3 vec3, float w)
    : x(vec3.x)
    , y(vec3.y)
    , z(vec3.z)
    , w(w)
{
}

ImpMath::Vector4 Vector4::operator+(const Vector4& rhs) const
{
    return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
}

void Vector4::operator+=(const Vector4& rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    w += rhs.w;
}

ImpMath::Vector4 Vector4::operator-(const Vector4& rhs) const
{
    return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
}

void Vector4::operator-=(const Vector4& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    w -= rhs.w;
}

ImpMath::Vector4 Vector4::operator*(const float scalar) const
{
    return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
}

void Vector4::operator*=(const float scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;
}

ImpMath::Vector4 Vector4::operator/(const float scalar) const
{
    return Vector4(x / scalar, y / scalar, z / scalar, w / scalar);
}

void Vector4::operator/=(const float scalar)
{
    x /= scalar;
    y /= scalar;
    z /= scalar;
    w /= scalar;
}

float Vector4::Dot(const Vector4& other) const
{
    return x * other.x + y * other.y + z * other.z + w * other.w;
}

float Vector4::operator*(const Vector4& rhs) const
{
    return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
}

bool Vector4::operator==(const Vector4& rhs) const
{
    return (x == rhs.x && y == rhs.y) && (z == rhs.z && w == rhs.w);
}

bool Vector4::operator!=(const Vector4& rhs) const
{
    return !operator==(rhs);
}

float Vector4::Length() const
{
    return sqrt(x * x + y * y + z * z + w * w);
}

float Vector4::LengthSquared() const
{
    return x * x + y * y + z * z + w * w;
}

ImpMath::Vector4 Vector4::Normalize() const
{
    return Vector4(x / Length(), y / Length(), z / Length(), w / Length());
}

float Vector4::CosTheta(const Vector4& other) const
{
    float cos = Dot(other);
    cos /= Length() * other.Length();

    return cos;
}
#pragma endregion Vector4

#pragma region Quaternion

Quaternion::Quaternion(float x /*= 0.0f*/,
    float y /*= 0.0f*/,
    float z /*= 0.0f*/,
    float w /*= 1.0f*/)
    : x(x)
    , y(y)
    , z(z)
    , w(w)
{
}

ImpMath::Vector3 Quaternion::ToEuler() const
{
    const float xx = x * x;
    const float yy = y * y;
    const float zz = z * z;

    const float m31 = 2.f * x * z + 2.f * y * w;
    const float m32 = 2.f * y * z - 2.f * x * w;
    const float m33 = 1.f - 2.f * xx - 2.f * yy;

    const float cy = sqrtf(m33 * m33 + m31 * m31);
    const float cx = atan2f(-m32, cy);
    if (cy > 16.f * FLT_EPSILON)
    {
        const float m12 = 2.f * x * y + 2.f * z * w;
        const float m22 = 1.f - 2.f * xx - 2.f * zz;

        return Vector3(cx, atan2f(m31, m33), atan2f(m12, m22));
    }
    else
    {
        const float m11 = 1.f - 2.f * yy - 2.f * zz;
        const float m21 = 2.f * x * y - 2.f * z * w;

        return Vector3(cx, 0.f, atan2f(-m21, m11));
    }
}

ImpMath::Quaternion Quaternion::MakeFromAxis(const Vector3& axis, float radian)
{
    using namespace DirectX;

    XMFLOAT3 xmAxis(axis.x, axis.y, axis.z);

    const XMVECTOR a = XMLoadFloat3(&xmAxis);

    XMFLOAT4 result;
    XMStoreFloat4(&result, XMQuaternionRotationAxis(a, radian));

    return Quaternion(result.x, result.y, result.z, result.w);
}

ImpMath::Quaternion Quaternion::MakeFromYawPitchRoll(float yaw, float pitch, float roll)
{
    using namespace DirectX;

    XMFLOAT4 result;

    XMStoreFloat4(&result, XMQuaternionRotationRollPitchYaw(pitch, yaw, roll));
    return Quaternion(result.x, result.y, result.z, result.w);
}

ImpMath::Quaternion Quaternion::operator*(const Quaternion& rfm) const
{
    using namespace DirectX;

    XMFLOAT4 lfmQ(x, y, z, w);
    XMFLOAT4 rfmQ(rfm.x, rfm.y, rfm.z, rfm.w);

    const XMVECTOR q1 = XMLoadFloat4(&lfmQ);
    const XMVECTOR q2 = XMLoadFloat4(&rfmQ);

    XMFLOAT4 result;
    XMStoreFloat4(&result, XMQuaternionMultiply(q1, q2));
    return Quaternion(result.x, result.y, result.z, result.w);
}

ImpMath::Quaternion& Quaternion::operator*=(const Quaternion& rfm)
{
    using namespace DirectX;

    XMFLOAT4 lfmQ(x, y, z, w);
    XMFLOAT4 rfmQ(rfm.x, rfm.y, rfm.z, rfm.w);

    const XMVECTOR q1 = XMLoadFloat4(&lfmQ);
    const XMVECTOR q2 = XMLoadFloat4(&rfmQ);

    XMFLOAT4 result;
    XMStoreFloat4(&result, XMQuaternionMultiply(q1, q2));
    *this = { result.x, result.y, result.z, result.w };
    return *this;
}

ImpMath::Quaternion Quaternion::MakeLookRotation(const Vector3& forward,
    const Vector3& up)
{
    using namespace DirectX;
    Quaternion q1 = MakeFromToRoation(-Vector3::UnitZ, forward);

    XMFLOAT3 xmUp{ up.x, up.y, up.z };
    XMFLOAT3 xmForward{ forward.x, forward.y, forward.z };

    const XMVECTOR C = XMVector3Cross(
        XMLoadFloat3(&xmForward), XMLoadFloat3(&xmUp));
    if (XMVector3NearEqual(XMVector3LengthSq(C), g_XMZero, g_XMEpsilon))
    {
        // forward and up are co-linear
        return q1;
    }

    XMFLOAT4 xmQ1{ q1.x, q1.y, q1.z, q1.w };
    XMFLOAT4 xmUp4{ 0.f, 1.f, 0.f, 0.f };

    const XMVECTOR U = XMQuaternionMultiply(
        XMLoadFloat4(&xmQ1), XMLoadFloat4(&xmUp4));
    XMFLOAT4 xmU{};
    XMStoreFloat4(&xmU, U);

    Quaternion q2 = MakeFromToRoation(Vector3(xmU.x, xmU.y, xmU.z), up);

    return q2 * q1;
}


ImpMath::Quaternion Quaternion::MakeFromToRoation(const Vector3& fromDir,
    const Vector3& toDir)
{
    using namespace DirectX;

    XMFLOAT3 from{ fromDir.x, fromDir.y, fromDir.z };
    XMFLOAT3 to{ toDir.x, toDir.y, toDir.z };

    constexpr XMFLOAT3 right = { 1.f, 0.f, 0.f };
    constexpr XMFLOAT3 up    = { 0.f, 1.f, 0.f };

    XMFLOAT4 result;
    // Melax, "The Shortest Arc Quaternion", Game Programming Gems, Charles River Media (2000).

    const XMVECTOR F = XMVector3Normalize(XMLoadFloat3(&from));
    const XMVECTOR T = XMVector3Normalize(XMLoadFloat3(&to));

    const float dot = XMVectorGetX(XMVector3Dot(F, T));
    if (dot >= 1.f)
    {
        result = { 0.f, 0.f, 0.f, 1.f };
    }
    else if (dot <= -1.f)
    {
        XMVECTOR axis = XMVector3Cross(F, XMLoadFloat3(&right));
        if (XMVector3NearEqual(XMVector3LengthSq(axis), g_XMZero, g_XMEpsilon))
        {
            axis = XMVector3Cross(F, XMLoadFloat3(&up));
        }

        constexpr float nearPI = PI * 0.999999f;

        // Pi값에 근접할경우 뒤집힌 쿼터니온을 반환해서 nearPI 사용
        const XMVECTOR Q = XMQuaternionRotationAxis(axis, nearPI); // XM_PI);
        XMStoreFloat4(&result, Q);
    }
    else
    {
        const XMVECTOR C = XMVector3Cross(F, T);
        XMStoreFloat4(&result, C);

        const float s = sqrtf((1.f + dot) * 2.f);
        result.x /= s;
        result.y /= s;
        result.z /= s;
        result.w = s * 0.5f;
    }

    return Quaternion(result.x, result.y, result.z, result.w);
}


ImpMath::Quaternion Quaternion::Inverse() const
{
    using namespace DirectX;

    XMFLOAT4 q1{ x, y, z, w };

    const XMVECTOR q = XMLoadFloat4(&q1);
    XMStoreFloat4(&q1, XMQuaternionInverse(q));

    return Quaternion(q1.x, q1.y, q1.z, q1.w);
}

ImpMath::Quaternion Quaternion::QuaternionSlerp(const Quaternion& a,
    const Quaternion& b,
    float ratio)
{
    using namespace DirectX;

    XMFLOAT4 quaternion1{ a.x, a.y, a.z, a.w };
    XMFLOAT4 quaternion2{ b.x, b.y, b.z, b.w };

    const XMVECTOR q1 = XMLoadFloat4(&quaternion1);
    const XMVECTOR q2 = XMLoadFloat4(&quaternion2);

    const XMVECTOR q = XMQuaternionSlerp(q1, q2, ratio);
    XMStoreFloat4(&quaternion1, q);

    return Quaternion(quaternion1.x, quaternion1.y, quaternion1.z, quaternion1.w);
}

const ImpMath::Quaternion ImpMath::Quaternion::Identity = { 0.f, 0.f, 0.f, 1.f };

#pragma endregion Quaternion

#pragma region Matrix
// Todo:
// _11 분기문 말고 따로 값을 넣어주는 게 속도가 더 빠를 것으로 예상
Matrix::Matrix()
{
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            if (i == j)
            {
                m[i][j] = 1;
                continue;
            }
            m[i][j] = 0;
        }
    }
}

Matrix::Matrix(const Matrix& rhs)
{
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            m[i][j] = rhs.m[i][j];
        }
    }
}

Matrix::Matrix(const float* array)
{
    m[0][0] = array[0];
    m[0][1] = array[1];
    m[0][2] = array[2];
    m[0][3] = array[3];

    m[1][0] = array[4];
    m[1][1] = array[5];
    m[1][2] = array[6];
    m[1][3] = array[7];

    m[2][0] = array[8];
    m[2][1] = array[9];
    m[2][2] = array[10];
    m[2][3] = array[11];

    m[3][0] = array[12];
    m[3][1] = array[13];
    m[3][2] = array[14];
    m[3][3] = array[15];
}

Matrix Matrix::Identity()
{
    return Matrix();
}

Matrix Matrix::operator*(const Matrix& rhs) const
{
    // 	DirectX::XMFLOAT4X4 lhs4x4(&_11);
    // 	DirectX::XMFLOAT4X4 rhs4x4(&rhs._11);

    DirectX::XMMATRIX lhsM(&_11);
    DirectX::XMMATRIX rhsM(&rhs._11);

    DirectX::XMFLOAT4X4 result;
    XMStoreFloat4x4(&result, lhsM * rhsM);

    return Matrix(&result._11);
}

void Matrix::operator*=(const Matrix& rhs)
{
    DirectX::XMMATRIX lhsM(&_11);
    DirectX::XMMATRIX rhsM(&rhs._11);

    DirectX::XMFLOAT4X4 result;
    XMStoreFloat4x4(&result, lhsM * rhsM);

    *this = Matrix(&result._11);
}

ImpMath::Matrix& Matrix::operator=(const Matrix& rhs)
{
    m[0][0] = rhs.m[0][0];
    m[0][1] = rhs.m[0][1];
    m[0][2] = rhs.m[0][2];
    m[0][3] = rhs.m[0][3];

    m[1][0] = rhs.m[1][0];
    m[1][1] = rhs.m[1][1];
    m[1][2] = rhs.m[1][2];
    m[1][3] = rhs.m[1][3];

    m[2][0] = rhs.m[2][0];
    m[2][1] = rhs.m[2][1];
    m[2][2] = rhs.m[2][2];
    m[2][3] = rhs.m[2][3];

    m[3][0] = rhs.m[3][0];
    m[3][1] = rhs.m[3][1];
    m[3][2] = rhs.m[3][2];
    m[3][3] = rhs.m[3][3];

    return *this;
}

Matrix Matrix::MakeRotationMatrixRollPitchYaw(Vector3 rotation)
{
    float pitch = DirectX::XMConvertToRadians(rotation.x);
    float yaw   = DirectX::XMConvertToRadians(rotation.y);
    float roll  = DirectX::XMConvertToRadians(rotation.z);

    DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(
        pitch, yaw, roll);

    DirectX::XMFLOAT4X4 result;
    XMStoreFloat4x4(&result, rotationMatrix);

    return Matrix(&result._11);
}

ImpMath::Matrix Matrix::MakeTranslationMatrix(Vector3 postion)
{
    DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(
        postion.x, postion.y, postion.z);

    DirectX::XMFLOAT4X4 result;

    XMStoreFloat4x4(&result, translation);

    return Matrix(&result._11);
}

ImpMath::Matrix Matrix::MakeScaleMatrix(Vector3 scale)
{
    DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(
        scale.x, scale.y, scale.z);

    DirectX::XMFLOAT4X4 result;

    XMStoreFloat4x4(&result, scaleMatrix);

    return Matrix(&result._11);
}

ImpMath::Matrix Matrix::MakeFromQuaternion(const Quaternion& q)
{
    using namespace DirectX;

    XMFLOAT4 xmQ(q.x, q.y, q.z, q.w);

    const XMVECTOR quatv = XMLoadFloat4(&xmQ);

    DirectX::XMFLOAT4X4 result;

    XMStoreFloat4x4(&result, XMMatrixRotationQuaternion(quatv));
    return Matrix(&result._11);
}

ImpMath::Matrix Matrix::Inverse() const
{
    using namespace DirectX;

    DirectX::XMMATRIX xmMatrix(&_11);

    XMFLOAT4X4 result;
    XMVECTOR det;
    XMStoreFloat4x4(&result, XMMatrixInverse(&det, xmMatrix));

    return Matrix(&result._11);
}


ImpMath::Matrix Matrix::Transpose() const
{
    using namespace DirectX;
    XMFLOAT4X4 xmMatrix(&_11);

    const XMMATRIX M = XMLoadFloat4x4(&xmMatrix);

    XMStoreFloat4x4(&xmMatrix, XMMatrixTranspose(M));

    return Matrix(&xmMatrix._11);
}


bool Matrix::Decompose(Vector3& scale, Quaternion& rotation, Vector3& translation) noexcept
{
    using namespace DirectX;

    XMVECTOR s, r, t;

    XMFLOAT4X4 xmFloat4x4(&_11);
    DirectX::XMMATRIX mat = XMLoadFloat4x4(&xmFloat4x4);

    if (!XMMatrixDecompose(&s, &r, &t, mat)) return false;

    XMFLOAT3 xmScale;
    XMFLOAT4 xmRot;
    XMFLOAT3 xmTrans;

    XMStoreFloat3(&xmScale, s);
    XMStoreFloat4(&xmRot, r);
    XMStoreFloat3(&xmTrans, t);

    scale       = Vector3(xmScale.x, xmScale.y, xmScale.z);
    rotation    = Quaternion(xmRot.x, xmRot.y, xmRot.z, xmRot.w);
    translation = Vector3(xmTrans.x, xmTrans.y, xmTrans.z);

    return true;
}

const ImpMath::Vector4 ImpMath::Matrix::UnitX(1.f, 0.f, 0.f, 0.f);
const ImpMath::Vector4 ImpMath::Matrix::UnitY(0.f, 1.f, 0.f, 0.f);
const ImpMath::Vector4 ImpMath::Matrix::UnitZ(0.f, 0.f, 1.f, 0.f);
const ImpMath::Vector4 ImpMath::Matrix::UnitW(0.f, 0.f, 0.f, 1.f);
const ImpMath::Vector4 ImpMath::Matrix::One(1.f, 1.f, 1.f, 1.f);
const ImpMath::Vector4 ImpMath::Matrix::Zero(0.f, 0.f, 0.f, 0.f);

#pragma endregion Matrix
