#include "pch.h"
#include "ImpLoader.h"
#include "../ImpStructure/ImpMesh.h"

using namespace std;

using namespace ImpGraphics;

ImpStructure::ImpMesh* ImpLoader::GetMesh(const std::wstring& path)
{
	char* next_token1 = NULL;

	string line{};
	ifstream file(path); // example.txt 파일을 연다. 없으면 생성. 

	ImpStructure::ImpMesh* mesh;
	ImpStructure::ImpVertex* impVertex = nullptr;
	ImpStructure::ImpFace* impFace = nullptr;

	bool isPosColorTex = false;
	bool isQuadVertex = false;
	bool isPosColor = false;
	bool isPosNormal = false;
	bool isMeshVertex = false;

	int vertexSize = 0;
	int faceSize = 0;

#pragma warning(disable : 4244) // double -> float 변환 데이터 손상 에러 안나게...

	if (file.is_open())
	{
		mesh = new ImpStructure::ImpMesh;

		/// 첫 번째 줄 읽어오기
		getline(file, line);
		const char* str = line.c_str(); //구분할 문자열
		char* tempchar = const_cast<char*>(str);
		char* ptr = strtok_s(tempchar, " ", &next_token1); //첫번째 strtok 사용.
		int cnt = 0;
		while (ptr != NULL) //ptr이 NULL일때까지 (= strtok 함수가 NULL을 반환할때까지)
		{
			if (cnt == 1)
			{
				if (!strcmp(ptr, "PosColorTex"))
				{
					isPosColorTex = true;
				}
				else if (!strcmp(ptr, "QuadVertex"))
				{
					isQuadVertex = true;
				}
				else if (!strcmp(ptr, "PosColor"))
				{
					isPosColor = true;
				}
				else if (!strcmp(ptr, "PosNormal"))
				{
					isPosNormal = true;
				}
				else if (!strcmp(ptr, "MeshVertex"))
				{
					isMeshVertex = true;
				}
			}
			else if (cnt == 2) // vertexSize
			{
				vertexSize = atoi(ptr);
			}
			else if (cnt == 3) // indexSize
			{
				faceSize = atoi(ptr);
			}
			ptr = strtok_s(NULL, " ", &next_token1); //자른 문자 다음부터 구분자 또 찾기
			cnt++;
		}

		/// 버텍스 읽기
		for (size_t i = 0; i < vertexSize; i++)
		{
			getline(file, line);
			const char* str = line.c_str(); //구분할 문자열
			char* tempchar = const_cast<char*>(str);
			char* ptr = strtok_s(tempchar, " ", &next_token1); //첫번째 strtok 사용.
			impVertex = new ImpStructure::ImpVertex;

			if (isPosColorTex)
			{
				int tokenNumber = 0;
				while (ptr != NULL) //ptr이 NULL일때까지 (= strtok 함수가 NULL을 반환할때까지)
				{
					if (tokenNumber == 0)
					{
						impVertex->_localPos.x = atof(ptr);
					}
					else if (tokenNumber == 1)
					{
						impVertex->_localPos.y = atof(ptr);
					}
					else if (tokenNumber == 2)
					{
						impVertex->_localPos.z = atof(ptr);
					}
					else if (tokenNumber == 3)
					{
						impVertex->_color.x = atof(ptr);
					}
					else if (tokenNumber == 4)
					{
						impVertex->_color.y = atof(ptr);
					}
					else if (tokenNumber == 5)
					{
						impVertex->_color.z = atof(ptr);
					}
					else if (tokenNumber == 6)
					{
						impVertex->_color.w = atof(ptr);
					}
					else if (tokenNumber == 7)
					{
						impVertex->_u = atof(ptr);
					}
					else if (tokenNumber == 8)
					{
						impVertex->_v = atof(ptr);
					}

					ptr = strtok_s(NULL, " ", &next_token1); //자른 문자 다음부터 구분자 또 찾기
					tokenNumber++;
				}
				mesh->_hasTexture = true;
				mesh->_hasColor = true;
				mesh->_meshVertex.push_back(impVertex);
			}

			else if (isMeshVertex)
			{
				int tokenNumber = 0;
				while (ptr != NULL) //ptr이 NULL일때까지 (= strtok 함수가 NULL을 반환할때까지)
				{
					if (tokenNumber == 0)
					{
						impVertex->_localPos.x = atof(ptr);
					}
					else if (tokenNumber == 1)
					{
						impVertex->_localPos.y = atof(ptr);
					}
					else if (tokenNumber == 2)
					{
						impVertex->_localPos.z = atof(ptr);
					}
					else if (tokenNumber == 3)
					{
                        impVertex->_u = atof(ptr);
					}
					else if (tokenNumber == 4)
					{
                        impVertex->_v = atof(ptr);
					}
					else if (tokenNumber == 5)
					{
						impVertex->_normal.x = atof(ptr);
					}
					else if (tokenNumber == 6)
					{
                        impVertex->_normal.y = atof(ptr);
					}
					else if (tokenNumber == 7)
					{
                        impVertex->_normal.z = atof(ptr);
					}
                    else if (tokenNumber == 8)
                    {
                        impVertex->_tangent.x = atof(ptr);
                    }
                    else if (tokenNumber == 9)
                    {
                        impVertex->_tangent.y = atof(ptr);
                    }
                    else if (tokenNumber == 10)
                    {
                        impVertex->_tangent.z = atof(ptr);
                    }

					ptr = strtok_s(NULL, " ", &next_token1); //자른 문자 다음부터 구분자 또 찾기
					tokenNumber++;
				}
				mesh->_hasTexture = true;
				mesh->_hasNormal = true;
				mesh->_hasTangent = true;
				mesh->_meshVertex.push_back(impVertex);
			}

			else if (isQuadVertex)
			{
				int tokenNumber = 0;
				while (ptr != NULL) //ptr이 NULL일때까지 (= strtok 함수가 NULL을 반환할때까지)
				{
					if (tokenNumber == 0)
					{
						impVertex->_localPos.x = atof(ptr);
					}
					else if (tokenNumber == 1)
					{
						impVertex->_localPos.y = atof(ptr);
					}
					else if (tokenNumber == 2)
					{
						impVertex->_localPos.z = atof(ptr);
					}
					else if (tokenNumber == 3)
					{
						impVertex->_u = atof(ptr);
					}
					else if (tokenNumber == 4)
					{
						impVertex->_v = atof(ptr);
					}

					ptr = strtok_s(NULL, " ", &next_token1); //자른 문자 다음부터 구분자 또 찾기
					tokenNumber++;
				}
				mesh->_hasTexture = true;
				mesh->_meshVertex.push_back(impVertex);
			}

			else if (isPosColor)
			{
				int tokenNumber = 0;
				while (ptr != NULL) //ptr이 NULL일때까지 (= strtok 함수가 NULL을 반환할때까지)
				{
					if (tokenNumber == 0)
					{
						impVertex->_localPos.x = atof(ptr);
					}
					else if (tokenNumber == 1)
					{
						impVertex->_localPos.y = atof(ptr);
					}
					else if (tokenNumber == 2)
					{
						impVertex->_localPos.z = atof(ptr);
					}
					else if (tokenNumber == 3)
					{
						impVertex->_color.x = atof(ptr);
					}
					else if (tokenNumber == 4)
					{
						impVertex->_color.y = atof(ptr);
					}
					else if (tokenNumber == 5)
					{
						impVertex->_color.z = atof(ptr);
					}
					else if (tokenNumber == 6)
					{
						impVertex->_color.w = atof(ptr);
					}

					ptr = strtok_s(NULL, " ", &next_token1); //자른 문자 다음부터 구분자 또 찾기
					tokenNumber++;
				}
				mesh->_hasColor = true;
				mesh->_meshVertex.push_back(impVertex);
			}

			else if (isPosNormal)
			{
				int tokenNumber = 0;
				while (ptr != NULL) //ptr이 NULL일때까지 (= strtok 함수가 NULL을 반환할때까지)
				{
					if (tokenNumber == 0)
					{
						impVertex->_localPos.x = atof(ptr);
					}
					else if (tokenNumber == 1)
					{
						impVertex->_localPos.y = atof(ptr);
					}
					else if (tokenNumber == 2)
					{
						impVertex->_localPos.z = atof(ptr);
					}
					else if (tokenNumber == 3)
					{
						impVertex->_normal.x = atof(ptr);
					}
					else if (tokenNumber == 4)
					{
						impVertex->_normal.y = atof(ptr);
					}
					else if (tokenNumber == 5)
					{
						impVertex->_normal.z = atof(ptr);
					}

					ptr = strtok_s(NULL, " ", &next_token1); //자른 문자 다음부터 구분자 또 찾기
					tokenNumber++;
				}
				mesh->_hasNormal = true;
				mesh->_meshVertex.push_back(impVertex);
				}
		}

		/// 페이스 읽기(인덱스)
		for (size_t i = 0; i < faceSize; i++)
		{
			getline(file, line);
			const char* str = line.c_str(); //구분할 문자열
			char* tempchar = const_cast<char*>(str);
			char* ptr = strtok_s(tempchar, " ", &next_token1); //첫번째 strtok 사용.
			impFace = new ImpStructure::ImpFace;

			int tokenNumber = 0;
			while (ptr != NULL) //ptr이 NULL일때까지 (= strtok 함수가 NULL을 반환할때까지)
			{
				if (tokenNumber == 0)
				{
					impFace->vertexIndex[0] = atoi(ptr);
				}
				else if (tokenNumber == 1)
				{
					impFace->vertexIndex[1] = atoi(ptr);
				}
				else if (tokenNumber == 2)
				{
					impFace->vertexIndex[2] = atoi(ptr);
				}

				ptr = strtok_s(NULL, " ", &next_token1); //자른 문자 다음부터 구분자 또 찾기
				tokenNumber++;
			}
			mesh->_meshFace.push_back(impFace);
		}

		while (getline(file, line))
		{
			// 혹시 남은 줄이 있을까봐?
		}
		file.close(); // 열었던 파일을 닫는다. 
	}
	else
	{
		return nullptr; // 파일을 열지 못함. (파일 존재X)
	}
	
#pragma warning(default : 4244) // double -> float 변환 데이터 손상 에러 안나게...

	return mesh;
}

void ImpLoader::SaveMesh(ImpStructure::ImpMesh* mesh, std::wstring fileName)
{
	string line;
	ofstream file(fileName);

	if (file.is_open()) {
		// 첫 번째 줄: 데이터 정보
		file << "VertexType ";
        if (mesh->_hasTangent)
        {
            file << "MeshVertex ";
        }
		else if (mesh->_hasColor && mesh->_hasTexture)
		{
			file << "PosColorTex ";
		}
		else if (mesh->_hasColor)
		{
			file << "PosColor ";
		}
		else if (mesh->_hasNormal)
		{
			file << "PosNormal ";
		}
		else if (mesh->_hasTexture)
		{
			file << "QuadVertex ";
		}

		file << mesh->_meshVertex.size() << " ";
		file << mesh->_meshFace.size() << "\n"; // FaceSize


		if (mesh->_hasTangent)
		{
            for (size_t i = 0; i < mesh->_meshVertex.size(); i++)
            {
                file << mesh->_meshVertex[i]->_localPos.x << " "
                     << mesh->_meshVertex[i]->_localPos.y << " "
                     << mesh->_meshVertex[i]->_localPos.z << " ";
                file << mesh->_meshVertex[i]->_u << " "
                     << mesh->_meshVertex[i]->_v << " ";
                file << mesh->_meshVertex[i]->_normal.x << " "
                     << mesh->_meshVertex[i]->_normal.y << " "
                     << mesh->_meshVertex[i]->_normal.z << " ";
                file << mesh->_meshVertex[i]->_tangent.x << " "
                     << mesh->_meshVertex[i]->_tangent.y << " "
                     << mesh->_meshVertex[i]->_tangent.z << "\n";
            }
		}

		// 실제 버텍스
		else if (mesh->_hasColor && mesh->_hasTexture)
		{
			for (size_t i = 0; i < mesh->_meshVertex.size(); i++)
			{
				file << mesh->_meshVertex[i]->_localPos.x << " " << mesh->_meshVertex[i]->_localPos.y << " " << mesh->_meshVertex[i]->_localPos.z << " ";
				file << mesh->_meshVertex[i]->_color.x << " " << mesh->_meshVertex[i]->_color.y << " " << mesh->_meshVertex[i]->_color.z << " " << mesh->_meshVertex[i]->_color.w << " ";
				file << mesh->_meshVertex[i]->_u << " " << mesh->_meshVertex[i]->_v << "\n";
			}
		}
		else if (mesh->_hasNormal && mesh->_hasTexture)
		{
			for (size_t i = 0; i < mesh->_meshVertex.size(); i++)
			{
				file << mesh->_meshVertex[i]->_localPos.x << " " << mesh->_meshVertex[i]->_localPos.y << " " << mesh->_meshVertex[i]->_localPos.z << " ";
				file << mesh->_meshVertex[i]->_normal.x << " " << mesh->_meshVertex[i]->_normal.y << " " << mesh->_meshVertex[i]->_normal.z << " ";
				file << mesh->_meshVertex[i]->_u << " " << mesh->_meshVertex[i]->_v << "\n";
			}
		}
		else if (mesh->_hasColor)
		{
			for (size_t i = 0; i < mesh->_meshVertex.size(); i++)
			{
				file << mesh->_meshVertex[i]->_localPos.x << " " << mesh->_meshVertex[i]->_localPos.y << " " << mesh->_meshVertex[i]->_localPos.z << " ";
				file << mesh->_meshVertex[i]->_color.x << " " << mesh->_meshVertex[i]->_color.y << " " << mesh->_meshVertex[i]->_color.z << " " << mesh->_meshVertex[i]->_color.w << "\n";
			}
		}
		else if (mesh->_hasNormal)
		{
			for (size_t i = 0; i < mesh->_meshVertex.size(); i++)
			{
				file << mesh->_meshVertex[i]->_localPos.x << " " << mesh->_meshVertex[i]->_localPos.y << " " << mesh->_meshVertex[i]->_localPos.z << " ";
				file << mesh->_meshVertex[i]->_normal.x << " " << mesh->_meshVertex[i]->_normal.y << " " << mesh->_meshVertex[i]->_normal.z << "\n";
			}
		}
		else if (mesh->_hasTexture)
		{
			for (size_t i = 0; i < mesh->_meshVertex.size(); i++)
			{
				file << mesh->_meshVertex[i]->_localPos.x << " " << mesh->_meshVertex[i]->_localPos.y << " " << mesh->_meshVertex[i]->_localPos.z << " ";
				file << mesh->_meshVertex[i]->_u << " " << mesh->_meshVertex[i]->_v << "\n";
			}
		}

		for (size_t i = 0; i < mesh->_meshFace.size(); i++)
		{
			file << mesh->_meshFace[i]->vertexIndex[0] << " " << mesh->_meshFace[i]->vertexIndex[1] << " " << mesh->_meshFace[i]->vertexIndex[2] << "\n";
		}

		file.close();
	}
}
