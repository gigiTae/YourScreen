#include "pch.h"
#include "ImpMesh.h"
#include "../ImpLib/Inc/SimpleMath.h"

#include <windows.h>

void ImpStructure::ImpAnimationNode::SetNode(Matrix parentMat)
{
	_worldTransform = _localTransform * parentMat;

	if (_bone != nullptr)
	{
		_bone->_boneTM_WorldTM = _worldTransform;
	}

	if (_childNodes.size() == 0) return;

	for (int i = 0; i < _childNodes.size(); i++)
	{
		_childNodes[i]->SetNode(_worldTransform);
	}
}

void ImpStructure::ImpAnimationNode::Translate(Vector3 distance)
{
	ImpAnimationNode* curNode = _parentNode;
	DirectX::SimpleMath::Matrix totalMat = DirectX::XMMatrixIdentity();

	DirectX::SimpleMath::Matrix tempLocal = DirectX::XMMatrixIdentity();

// 	if (curNode != nullptr)
// 	{
// 		tempLocal = DirectX::SimpleMath::Matrix(&curNode->_localTransform._11);
// 	}

	while (curNode != nullptr)
	{
        tempLocal = DirectX::SimpleMath::Matrix(&curNode->_localTransform._11);
		totalMat *= tempLocal;
		curNode = curNode->_parentNode;
	}

	// 부모 행렬 구해서 NodeTM만들기
	tempLocal = DirectX::SimpleMath::Matrix(&_localTransform._11);
	//tempLocal *= totalMat;

	DirectX::SimpleMath::Vector3 scale;
	DirectX::SimpleMath::Quaternion rotation;
	DirectX::SimpleMath::Vector3 translation;

	tempLocal.Decompose(scale, rotation, translation);

	// POS값 바꾸고
	translation.x = distance.x;
	translation.y = distance.y;
	translation.z = distance.z;

	DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScalingFromVector(scale);
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&rotation));
	DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixTranslationFromVector(translation);

	// 분해 한것 다시 합치기
	DirectX::SimpleMath::Matrix transformMatrix = scaleMatrix * rotationMatrix * translationMatrix;

	tempLocal = transformMatrix /** totalMat.Invert()*/;
	_localTransform = ImpMath::Matrix(&tempLocal._11);

	SetNode(ImpMath::Matrix(&totalMat._11));
}

void ImpStructure::ImpAnimationNode::Rotate(Quaternion nextRotation)
{
	//MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);

	if (_name == "Bone002")
	{
		int a = 3;
	}

	ImpAnimationNode* curNode = _parentNode;
	DirectX::SimpleMath::Matrix totalMat = {};

	DirectX::SimpleMath::Matrix tempLocal = DirectX::XMMatrixIdentity();

	while (curNode != nullptr)
	{
		tempLocal = DirectX::SimpleMath::Matrix(&curNode->_localTransform._11);
		totalMat *= tempLocal;
		curNode = curNode->_parentNode;
	}

	tempLocal = DirectX::SimpleMath::Matrix(&_localTransform._11);
	DirectX::SimpleMath::Vector3 scale;
	DirectX::SimpleMath::Quaternion rotation;
	DirectX::SimpleMath::Vector3 translation;

	tempLocal.Decompose(scale, rotation, translation);

	// Rot값 바꾸고
	rotation.x = nextRotation.x;
	rotation.y = nextRotation.y;
	rotation.z = nextRotation.z;
	rotation.w = nextRotation.w;

	DirectX::SimpleMath::Matrix scaleMatrix = DirectX::XMMatrixScalingFromVector(scale);
	DirectX::SimpleMath::Matrix rotationMatrix = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&rotation));
	DirectX::SimpleMath::Matrix translationMatrix = DirectX::XMMatrixTranslationFromVector(translation);

	// 분해 한것 다시 합치기
	DirectX::SimpleMath::Matrix transformMatrix = scaleMatrix * rotationMatrix * translationMatrix;

	tempLocal = transformMatrix;
	_localTransform = ImpMath::Matrix(&tempLocal._11);

	SetNode(ImpMath::Matrix(&totalMat._11));
}