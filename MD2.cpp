#include "MD2.h"
#include <stdio.h>
#include <iostream>
using namespace std;

CMD2Model::CMD2Model()
{
	m_pHeader			= NULL;
	m_pListVertex		= NULL;
	m_pListFrame		= NULL;
	m_pListAnimation	= NULL;
	m_pListTriangle		= NULL;
	m_pMotion			= NULL;

	m_iNumFrames		= 0;
	m_iNumVertices		= 0;
	m_iNumTriangles		= 0;
	m_iNumTexture		= 0;

	m_iMotSel			= 0;
	m_fNowFrame			= 0;

	//cout<<"CMD2Model()"<<endl;
}

CMD2Model::~CMD2Model()
{
	if(m_pListFrame)
	{
		for(int i=0; i<m_iNumFrames; i++)
		{
			if(m_pListFrame[i].ver)
				free(m_pListFrame[i].ver);
		}
		
		free(m_pListFrame);
	}
	if(m_pListVertex)		free(m_pListVertex);
	if(m_pListTriangle)		free(m_pListTriangle);
	//cout<<"~CMD2Model()"<<endl;
}

INT CMD2Model::LoadMD2(LPCTSTR _md2ModelFile)
{
	//파일 포인터
	FILE *fp = NULL;
	//cout<<"fp 주소 : "<<fp<<endl;
	//파일 버퍼
	char *pBuffer;

	//TODO: 0819(22시) fopen 함수 제대로 고쳐야함
	//모델 파일을 연다
	//fp = fopen(_md2ModelFile, "rb");
	fp = _wfopen(_md2ModelFile, L"rb");
	//cout<<"fp 주소 : "<<fp<<endl;

	// MD2 File Size Search
	fseek(fp, 0, SEEK_END);		// fseek 함수가 뭔지 찾아보자
	int iSize = ftell(fp);
	fseek(fp, 0, SEEK_SET); 

	// MD2 File Read
	pBuffer = (char*) malloc( sizeof(char) * iSize);
	fread(pBuffer, sizeof(char), iSize, fp);

	// Header Copy
	m_pHeader = (LPMODELHEADER)pBuffer;

	// Code Check
	if( (m_pHeader->ID != '2PDI') || (m_pHeader->iVersion != 8) )
	{
		fclose(fp);
		delete m_pHeader;
		return 0;
	}

	//정점 갯수, 프레임 갯수, 각프레임 크기
	m_iNumVertices = m_pHeader->iNumVertices;
	m_iNumFrames = m_pHeader->iNumFrames;
	m_iNumTriangles = m_pHeader->iNumTriangles;

	IndexSort(pBuffer);
	CreateFrame(pBuffer);
	CreateAnimateArray(pBuffer);

	m_pListVertex = (LPVERTEXLIST)malloc(sizeof(VERTEXLIST) * m_iNumVertices);
	memset(m_pListVertex, NULL, sizeof(VERTEXLIST) * m_iNumVertices);

	free(pBuffer);
	fclose(fp);

	//cout<<"LoadMD2()"<<endl;

	return 0;
}

VOID CMD2Model::CreateFrame(PCHAR _pBuffer)
{
	int cnt = 0;
	LPFRAMEINFO pFrameInfo = NULL;

	// Normalize
	D3DXVECTOR3 vCrossA, vCrossB;
	D3DXVECTOR3 vNormalize;

	// Frame Initialize
	m_pListFrame = (LPFRAME) malloc( sizeof(FRAME) * m_iNumFrames );

	// Texture Initialize
	LPTEXCOORDIDX pTexCoord = (LPTEXCOORDIDX)(_pBuffer + m_pHeader->iOffsetTexCoord);
	FLOAT fTexWidth = 1.0f / (FLOAT)m_pHeader->iTexWidth;
	FLOAT fTexHeight = 1.0f / (FLOAT)m_pHeader->iTexHeight;

	LPMESHLIST pTriangle = (LPMESHLIST)(_pBuffer + m_pHeader->iOffsetTriangleIdx);

	// Vertex Initialize
	for(int i=0; i<m_iNumFrames; i++)
	{
		// Vertex Create
		m_pListFrame[i].ver = (LPVERTEXLIST)malloc( sizeof(VERTEXLIST) * m_iNumVertices );
		memset(m_pListFrame[i].ver, NULL, sizeof(VERTEXLIST) * m_iNumVertices);

		// Frame Offset
		pFrameInfo = (LPFRAMEINFO)(_pBuffer + m_pHeader->iOffsetFrames + m_pHeader->iFrameSize * i);
		for(int j=0; j<m_iNumTriangles; j++)
		{
			for(int k=0; k<3; k++)
			{
				// Triangle
				VERTEXLIST &newVer = m_pListFrame[i].ver[ m_pListTriangle[j].ptr[k] ];
				WORD &verIdx = pTriangle[j].meshIndex[k];
				WORD &texIdx = pTriangle[j].stIndex[k];

				// Position
				newVer.pos.x = (FLOAT)pFrameInfo->listVertex[verIdx].v[0] * pFrameInfo->scale[0] + pFrameInfo->translate[0];
				newVer.pos.z = (FLOAT)pFrameInfo->listVertex[verIdx].v[1] * pFrameInfo->scale[1] + pFrameInfo->translate[1];
				newVer.pos.y = (FLOAT)pFrameInfo->listVertex[verIdx].v[2] * pFrameInfo->scale[2] + pFrameInfo->translate[2];

				// TexCoord
				newVer.tex1.x = (FLOAT)pTexCoord[texIdx].u * fTexWidth;
				newVer.tex1.y = (FLOAT)pTexCoord[texIdx].v * fTexHeight;
			}// for(int k)

			// Normalize
			vCrossA = m_pListFrame[i].ver[ m_pListTriangle[j]._2 ].pos - m_pListFrame[i].ver[ m_pListTriangle[j]._1 ].pos;
			vCrossB = m_pListFrame[i].ver[ m_pListTriangle[j]._0 ].pos - m_pListFrame[i].ver[ m_pListTriangle[j]._1 ].pos;
			D3DXVec3Cross(&vNormalize, &vCrossA, &vCrossB);
			D3DXVec3Normalize(&vNormalize, &vNormalize);

			m_pListFrame[i].ver[ m_pListTriangle[j]._0 ].nor += vNormalize;
			m_pListFrame[i].ver[ m_pListTriangle[j]._1 ].nor += vNormalize;
			m_pListFrame[i].ver[ m_pListTriangle[j]._2 ].nor += vNormalize;
		}// for(int j)

		for(int j=0; j<m_iNumVertices; j++)
			D3DXVec3Normalize( &m_pListFrame[i].ver[j].nor, &m_pListFrame[i].ver[j].nor );
	}// for(int i)
	//cout<<"CreateFrame()"<<endl;
}

VOID CMD2Model::CreateAnimateArray(PCHAR _pBuffer)
{
	m_pMotion = (LPANIMATION) malloc( sizeof(ANIMATION)*16 );

	m_pMotion[STAND].startFrame = 0;
	m_pMotion[STAND].endFrame = 39;
	m_pMotion[RUN].startFrame = 40;
	m_pMotion[RUN].endFrame = 46;
	m_pMotion[ATTACK_NOACTION].startFrame = 47;
	m_pMotion[ATTACK_NOACTION].endFrame = 60;
	m_pMotion[ATTACK_SHOULDER].startFrame = 61;
	m_pMotion[ATTACK_SHOULDER].endFrame = 66;
	m_pMotion[JUMP].startFrame = 67;
	m_pMotion[JUMP].endFrame = 73;
	m_pMotion[STAND_2].startFrame = 74;
	m_pMotion[STAND_2].endFrame = 95;
	m_pMotion[ATTACK_REACTION].startFrame = 96;
	m_pMotion[ATTACK_REACTION].endFrame = 112;
	m_pMotion[STAND_3].startFrame = 113;
	m_pMotion[STAND_3].endFrame = 122;
	m_pMotion[STAND_4].startFrame = 123;
	m_pMotion[STAND_4].endFrame = 135;
	m_pMotion[CROUCH].startFrame = 136;
	m_pMotion[CROUCH].endFrame = 154;
	m_pMotion[CROUCH_WALK].startFrame = 155;
	m_pMotion[CROUCH_WALK].endFrame = 161;
	m_pMotion[CROUCH_STAND].startFrame = 162;
	m_pMotion[CROUCH_STAND].endFrame = 169;
	m_pMotion[CROUCH_DEATH].startFrame = 170;
	m_pMotion[CROUCH_DEATH].endFrame = 177;
	m_pMotion[DEATH_FALLBACK].startFrame = 178;
	m_pMotion[DEATH_FALLBACK].endFrame = 185;
	m_pMotion[DEATH_FORWARD].startFrame = 186;
	m_pMotion[DEATH_FORWARD].endFrame = 190;
	m_pMotion[DEATH_FALLBACKSLOW].startFrame = 191;
	m_pMotion[DEATH_FALLBACKSLOW].endFrame = 197;

	for(int i=0; i<16; i++)
		m_pMotion[i].frameNum = m_pMotion[i].endFrame - m_pMotion[i].startFrame -1;

	//cout<<"CreateAnimateArray()"<<endl;
}

VOID CMD2Model::IndexSort(PCHAR _pBuffer)
{
	// Index Pointer
	WORD texIdx[3];

	LPMESHLIST pTriangle = (LPMESHLIST)(_pBuffer + m_pHeader->iOffsetTriangleIdx);

	// Index Initialize				//일단 이건 malloc으로 해둠.. 나중에 new로 바꿔보자
	m_pListTriangle = (LPINDEXLIST)malloc(sizeof(INDEXLIST) * m_iNumTriangles);
	memset(m_pListTriangle, NULL, sizeof(INT) * m_iNumVertices);

	// vertex sort pointer
	LPINT pCheck = (LPINT)malloc(sizeof(INT) * m_iNumVertices);
	memset(pCheck, -1, sizeof(INT) * m_iNumVertices);

	int cnt = 0;
	for(int i=0; i<m_pHeader->iNumTriangles; i++)
	{
		// Index 복사
		m_pListTriangle[i]._0 = pTriangle[i].meshIndex[0];
		m_pListTriangle[i]._1 = pTriangle[i].meshIndex[1];
		m_pListTriangle[i]._2 = pTriangle[i].meshIndex[2];

		// 텍스쳐 좌표 복사
		texIdx[0] = pTriangle[i].stIndex[0];
		texIdx[1] = pTriangle[i].stIndex[1];
		texIdx[2] = pTriangle[i].stIndex[2];

		for(int j=0; j<3; j++)
		{
			//텍스쳐 좌표 복사
			if( -1 == pCheck[ m_pListTriangle[i].ptr[j] ] )
				pCheck[ m_pListTriangle[i].ptr[j] ] = texIdx[j];
			else if( pCheck[ m_pListTriangle[i].ptr[j] ] != texIdx[j] )
				m_pListTriangle[i].ptr[j] = m_iNumVertices++;
		}// for(int j)
	}// for(int i)

	free(pCheck);
	//cout<<"IndexSort()"<<endl;
}

LPVERTEXLIST CMD2Model::Animate(MODELSTATE _motion, FLOAT _fTime)
{
	if(_motion != m_iMotSel)
	{
		m_iMotSel = _motion;
		m_fNowFrame = m_pMotion[m_iMotSel].startFrame;
	}

	m_fNowFrame += _fTime * 10;

	if(m_fNowFrame > m_pMotion[m_iMotSel].endFrame+1)
	   m_fNowFrame = m_pMotion[m_iMotSel].startFrame;

	INT CurnFrame = (INT)m_fNowFrame;
	INT NextFrame = CurnFrame + 1;
	INT tDelta = (INT)(m_fNowFrame - CurnFrame);

	if(NextFrame > m_pMotion[m_iMotSel].endFrame)
	   NextFrame = m_pMotion[m_iMotSel].startFrame;

	LPVERTEXLIST pListCurnVertex = m_pListFrame[CurnFrame].ver;
	LPVERTEXLIST pListNextVertex = m_pListFrame[NextFrame].ver;

	FLOAT x1, y1, z1, x2, y2, z2;

	for(int i=0; i<m_iNumVertices; i++)
	{
		x1 = pListCurnVertex[i].pos.x;
		y1 = pListCurnVertex[i].pos.y;
		z1 = pListCurnVertex[i].pos.z;
		x2 = pListNextVertex[i].pos.x;
		y2 = pListNextVertex[i].pos.y;
		z2 = pListNextVertex[i].pos.z;

		m_pListVertex[i].pos.x = x1*(1-tDelta) + x2 * tDelta;
		m_pListVertex[i].pos.y = y1*(1-tDelta) + y2 * tDelta;
		m_pListVertex[i].pos.z = z1*(1-tDelta) + z2 * tDelta;

		x1 = pListCurnVertex[i].nor.x;
		y1 = pListCurnVertex[i].nor.y;
		z1 = pListCurnVertex[i].nor.z;
		x2 = pListCurnVertex[i].nor.x;
		y2 = pListCurnVertex[i].nor.y;
		z2 = pListCurnVertex[i].nor.z;

		m_pListVertex[i].nor.x = x1*(1-tDelta) + x2 * tDelta;
		m_pListVertex[i].nor.y = y1*(1-tDelta) + y2 * tDelta;
		m_pListVertex[i].nor.z = z1*(1-tDelta) + z2 * tDelta;

		m_pListVertex[i].tex1.x = pListCurnVertex[i].tex1.x;
		m_pListVertex[i].tex1.y = pListCurnVertex[i].tex1.y;
	}

	return m_pListVertex;
}


// 리턴전용 함수들 ------------------------
INT CMD2Model::GetNumVertices()
{
	return m_iNumVertices;
}

INT CMD2Model::GetNumTriangles()
{
	return m_iNumTriangles;
}

LPFRAME CMD2Model::GetListFrame(int _iValue)
{
	//예외처리 넣자
	return &m_pListFrame[_iValue];
}

LPINDEXLIST CMD2Model::GetListTriangle()
{
	return m_pListTriangle;
}

INT CMD2Model::GetNowFrame()
{
	return (INT)m_fNowFrame;
}

void CMD2Model::SetNowFrame(MODELSTATE _iModelState, int _iValue)
{
	m_fNowFrame = (float)m_pMotion[_iModelState].startFrame +(float)_iValue;
}

INT CMD2Model::GetStartFrame(int _iValue)
{
	return (int)m_pMotion[_iValue].startFrame;
}

INT CMD2Model::GetEndFrame(int _iValue)
{
	return (int)m_pMotion[_iValue].endFrame;
}