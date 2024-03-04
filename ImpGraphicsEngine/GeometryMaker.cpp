#include "pch.h"
#include "Resource.h"

#include "../ImpStructure/ImpMesh.h"

using namespace ImpGraphics;
using namespace ImpStructure;

ImpStructure::ImpMesh* ImpGraphics::GeometryMaker::MakeBoxLineListMesh(ImpMath::Vector3 center, ImpMath::Vector3 length)
{
	ImpMesh* box = new ImpMesh;

	box->_hasColor = true;

	ImpVertex* tempVertex = new ImpVertex;
	tempVertex->_localPos = { length.x, length.y, length.z };
	tempVertex->_color = {0.0f, 1.0f, 0.0f, 1.0f};
	box->_meshVertex.push_back(tempVertex);

	tempVertex = new ImpVertex;
	tempVertex->_localPos = { length.x, length.y, -length.z };
	tempVertex->_color = { 0.0f, 1.0f, 0.0f, 1.0f };
	box->_meshVertex.push_back(tempVertex);

	tempVertex = new ImpVertex;
	tempVertex->_localPos = { length.x, -length.y, length.z };
	tempVertex->_color = { 0.0f, 1.0f, 0.0f, 1.0f };
	box->_meshVertex.push_back(tempVertex);

	tempVertex = new ImpVertex;
	tempVertex->_localPos = { length.x, -length.y, -length.z };
	tempVertex->_color = { 0.0f, 1.0f, 0.0f, 1.0f };
	box->_meshVertex.push_back(tempVertex);

	tempVertex = new ImpVertex;
	tempVertex->_localPos = { -length.x, + length.y, + length.z };
	tempVertex->_color = { 0.0f, 1.0f, 0.0f, 1.0f };
	box->_meshVertex.push_back(tempVertex);

	tempVertex = new ImpVertex;
	tempVertex->_localPos = { -length.x, length.y, -length.z };
	tempVertex->_color = { 0.0f, 1.0f, 0.0f, 1.0f };
	box->_meshVertex.push_back(tempVertex);

	tempVertex = new ImpVertex;
	tempVertex->_localPos = { -length.x, -length.y, length.z };
	tempVertex->_color = { 0.0f, 1.0f, 0.0f, 1.0f };
	box->_meshVertex.push_back(tempVertex);

	tempVertex = new ImpVertex;
	tempVertex->_localPos = { -length.x, -length.y, -length.z };
	tempVertex->_color = { 0.0f, 1.0f, 0.0f, 1.0f };
	box->_meshVertex.push_back(tempVertex);

	box->_meshFace.push_back(new ImpFace{ {0, 1, 2} });
	box->_meshFace.push_back(new ImpFace{ {3, 4, 5} });
	box->_meshFace.push_back(new ImpFace{ {6, 7, 0} });
	box->_meshFace.push_back(new ImpFace{ {4, 1, 5} });
	box->_meshFace.push_back(new ImpFace{ {2, 6, 3} });
	box->_meshFace.push_back(new ImpFace{ {7, 0, 2} });
	box->_meshFace.push_back(new ImpFace{ {1, 3, 4} });
	box->_meshFace.push_back(new ImpFace{ {6, 5, 7} });

	return box;
	//index
	//0,1, 2,3 4,5, 6,7 0,4, 1,5, 2,6, 3,7 0,2 1,3, 4,6, 5,7 

	//box->_meshFace.push_back();
}

ImpStructure::ImpMesh* ImpGraphics::GeometryMaker::MakeSphereLineListMesh(ImpMath::Vector3 center, float radius)
{
	ImpMesh* sphere = new ImpMesh;
	sphere->_hasColor = true;

	ImpVertex* tempVertex = nullptr;

	for (UINT degree = 0; degree < 360; degree++)
	{
		float radian = ImpMath::DegreeToRadian(degree);

		tempVertex = new ImpVertex;
        tempVertex->_localPos = { cosf(radian) * radius, sinf(radian) * radius, 0 };
		tempVertex->_color = { 1.0f, 0.0f, 0.0f, 1.0f };
		sphere->_meshVertex.push_back(tempVertex);
	}

	for (UINT degree = 0; degree < 360; degree++)
	{
		float radian = ImpMath::DegreeToRadian(degree);

		tempVertex = new ImpVertex;
        tempVertex->_localPos = { cosf(radian) * radius, 0, sinf(radian) * radius };
		tempVertex->_color = { 0.0f, 1.0f, 0.0f, 1.0f };
		sphere->_meshVertex.push_back(tempVertex);
	}

	for (UINT degree = 0; degree < 360; degree++)
	{
		float radian = ImpMath::DegreeToRadian(degree);

		tempVertex = new ImpVertex;
        tempVertex->_localPos = { 0, cosf(radian) * radius, sinf(radian) * radius };
		tempVertex->_color = { 0.0f, 0.0f, 1.0f, 1.0f };
		sphere->_meshVertex.push_back(tempVertex);
	}

	UINT count = 0;
	for (UINT i = 0; i <= 720; i+=2)
	{
		if (count % 4 == 0)
		{
			sphere->_meshFace.push_back(new ImpFace{ {i, i+1, i+1} });
		}
		else if (count % 4 == 1)
		{
			sphere->_meshFace.push_back(new ImpFace{ {i, i, i + 1} });
		}
		else if (count % 4 == 2)
		{
			sphere->_meshFace.push_back(new ImpFace{ {i -1, i, i} });
		}
		else if (count % 4 == 3)
		{
			if (i == 720)
			{
				sphere->_meshFace.push_back(new ImpFace{ {i - 1, i - 1, 0} });
			}
			else
			{
				sphere->_meshFace.push_back(new ImpFace{ {i - 1, i - 1, i} });
				i -= 2;
			}
		}
		count++;
	}

	for (UINT i = 720; i <= 720 + 720; i += 2)
	{
		if (count % 4 == 0)
		{
			sphere->_meshFace.push_back(new ImpFace{ {i, i + 1, i + 1} });
		}
		else if (count % 4 == 1)
		{
			sphere->_meshFace.push_back(new ImpFace{ {i, i, i + 1} });
		}
		else if (count % 4 == 2)
		{
			sphere->_meshFace.push_back(new ImpFace{ {i - 1, i, i} });
		}
		else if (count % 4 == 3)
		{
			if (i == 720 + 720)
			{
				sphere->_meshFace.push_back(new ImpFace{ {i - 1, i - 1, 0} });
			}
			else
			{
				sphere->_meshFace.push_back(new ImpFace{ {i - 1, i - 1, i} });
				i -= 2;
			}
		}
		count++;
	}

	for (UINT i = 1440; i <= 1440 + 720; i += 2)
	{
		if (count % 4 == 0)
		{
			sphere->_meshFace.push_back(new ImpFace{ {i, i + 1, i + 1} });
		}
		else if (count % 4 == 1)
		{
			sphere->_meshFace.push_back(new ImpFace{ {i, i, i + 1} });
		}
		else if (count % 4 == 2)
		{
			sphere->_meshFace.push_back(new ImpFace{ {i - 1, i, i} });
		}
		else if (count % 4 == 3)
		{
			if (i == 1440 + 720)
			{
				sphere->_meshFace.push_back(new ImpFace{ {i - 1, i - 1, 0} });
			}
			else
			{
				sphere->_meshFace.push_back(new ImpFace{ {i - 1, i - 1, i} });
				i -= 2;
			}
		}
		count++;
	}

	return sphere;
}

ImpStructure::ImpMesh* GeometryMaker::MakeGrid()
{
    ImpMesh* grid = new ImpMesh;
    grid->_hasColor = true;

    for (int i = 0; i < 100; i++)
    {
        ImpVertex* tempVertex = new ImpVertex;
        tempVertex->_localPos = { (i * 10) - 500.f, 0, -500 };
        tempVertex->_color = { 0.662745118f, 0.662745118f, 0.662745118f, 1.f };
        grid->_meshVertex.push_back(tempVertex);

		tempVertex            = new ImpVertex;
        tempVertex->_localPos = { (i * 10) - 500.f, 0, 500 };
        tempVertex->_color = { 0.662745118f, 0.662745118f, 0.662745118f, 1.f };
        grid->_meshVertex.push_back(tempVertex);
    }

	for (int i = 0; i < 100; i++)
    {
        ImpVertex* tempVertex = new ImpVertex;
        tempVertex->_localPos = { -500.f, 0, (i * 10) - 500.f };
        tempVertex->_color = { 0.662745118f, 0.662745118f, 0.662745118f, 1.f };
        grid->_meshVertex.push_back(tempVertex);

        tempVertex            = new ImpVertex;
        tempVertex->_localPos = { 500.f, 0, (i * 10) - 500.f };
        tempVertex->_color = { 0.662745118f, 0.662745118f, 0.662745118f, 1.f };
        grid->_meshVertex.push_back(tempVertex);
    }

	int count = 0;
	for (size_t i = 0; i < 400; i+=3)
	{
        grid->_meshFace.push_back(new ImpFace{ { i, i + 1, i + 2 } });
	}

	return grid;
}

ImpStructure::ImpMesh* GeometryMaker::MakeQuadPolygonMesh(ImpMath::Vector4 uiPos,
                                                          DirectX::XMFLOAT2 screenSize)
{
    ImpMesh* quad = new ImpMesh;

    quad->_hasTexture = true;

	//point.x / screenSize.x + point.z / screenSize.x, point.y / screenSize.y + point.w / screenSize.y
	// Vertex
	ImpVertex* tempVertex = new ImpVertex;
    tempVertex->_localPos = { (uiPos.x / screenSize.x) * 2 - (uiPos.z / screenSize.x), ((uiPos.y / screenSize.y + uiPos.w / screenSize.y) * 2 - (uiPos.w / screenSize.y)) * -1, 0 };
    tempVertex->_u = 0;
    tempVertex->_v        = 1; 
    quad->_meshVertex.push_back(tempVertex);

	tempVertex            = new ImpVertex;
    tempVertex->_localPos = { (uiPos.x / screenSize.x) * 2 - (uiPos.z / screenSize.x), ((uiPos.y / screenSize.y) * 2 - (uiPos.w / screenSize.y)) * -1, 0 };
    tempVertex->_u        = 0;
    tempVertex->_v = 0;
    quad->_meshVertex.push_back(tempVertex);

	tempVertex            = new ImpVertex;
    tempVertex->_localPos = { (uiPos.x / screenSize.x + uiPos.z / screenSize.x) * 2 - (uiPos.z / screenSize.x), ((uiPos.y / screenSize.y) * 2 - (uiPos.w / screenSize.y)) * -1, 0 };
    tempVertex->_u        = 1;
    tempVertex->_v = 0;
    quad->_meshVertex.push_back(tempVertex);

	tempVertex            = new ImpVertex;
    tempVertex->_localPos = { (uiPos.x / screenSize.x + uiPos.z / screenSize.x) * 2 - (uiPos.z / screenSize.x), ((uiPos.y / screenSize.y + uiPos.w / screenSize.y) * 2 - (uiPos.w / screenSize.y)) * -1, 0 };
    tempVertex->_u        = 1;
    tempVertex->_v = 1;
    quad->_meshVertex.push_back(tempVertex);

	// Index
	quad->_meshFace.push_back(new ImpFace{ { 0, 1, 2 } });
    quad->_meshFace.push_back(new ImpFace{ { 0, 2, 3 } });

	return quad;

/*
-1 -1 0 0 1
-1 1 0 0 0
1 1 0 1 0
1 -1 0 1 1

0 1 2
0 2 3*/
}