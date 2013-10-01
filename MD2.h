#ifndef _MD2_H_
#define _MD2_H_

#include <d3d9.h>
#include <d3dx9.h>

#define MD2_FVF ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2 )

//버텍스 정보
typedef struct _tagVertexList
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 nor;
	D3DXVECTOR2 tex1;
	D3DXVECTOR2 tex2;
} VERTEXLIST, *LPVERTEXLIST;

//프레임 정보
typedef struct _tagFrame
{
	CHAR fra_name[16];
	LPVERTEXLIST ver;
} FRAME, *LPFRAME;

//애니메이션 정보
typedef struct _tagAnimation
{
	CHAR ani_name[16];
	SHORT startFrame;
	SHORT endFrame;
	SHORT frameNum;
} ANIMATION, *LPANIMATION;

//인덱스 정보
typedef struct _tagIndex
{
	union//메모리공간 공유
	{
		WORD ptr[3];
		struct{ WORD _0, _1, _2; };
	};
} INDEXLIST, *LPINDEXLIST;



//텍스쳐 좌표
typedef struct _tagTexCoordIdx
{
	short u;				//U좌표
	short v;				//V좌표
} TEXCOORDIDX, *LPTEXCOORDIDX;

//하나의 프레임 정점에 대한 정보
typedef struct _tagVertexPerFrame
{
	unsigned char v[3];			//정점 좌표
	unsigned char normalIndex;	//법선
} VERTEXPERFRAME, *LPVERTEXPERFRAME;

//하나의 프레임에 대한 정보
typedef struct _tagFrameInfo
{
	float scale[3];			//프레임 정점들의 확대.축소
	float translate[3];		//프레임 정점들의 이동
	char name[16];			//모델 이름
	VERTEXPERFRAME listVertex[1];
} FRAMEINFO, *LPFRAMEINFO;

//하나의 삼각형 데이터
typedef struct _tagMesh
{
	unsigned short meshIndex[3];	//정점 색인들
	unsigned short stIndex[3];		//텍스쳐 좌표 색인들
} MESHLIST, *LPMESHLIST;


//MD2 헤더
typedef struct _tagModelHeader
{
	int ID;					//식별아이디 844121161 // IDP2
	int iVersion;			//버전 8
	int iTexWidth;			//텍스쳐 넓이
	int iTexHeight;			//텍스쳐 높이
	int iFrameSize;			//키프레임당 바이트수
	int iNumTextures;		//텍스쳐(스킨) 갯수
	int iNumVertices;		//버텍스(정점) 총 갯수
	int iNumTexcoords;		//텍스쳐 좌표의 개수
	int iNumTriangles;		//인덱스의 개수
	int iNumGLCommands;		//triangle strips || fans에 최적화되어있는지 알려줌
	int iNumFrames;			//애니메이션 KeyFrame의 개수

	int iOffsetTexture;		//텍스쳐 시작위치
	int iOffsetTexCoord;	//텍스쳐 UV 버퍼 시작위치
	int iOffsetTriangleIdx;	//인덱스버퍼 시작위치
	int iOffsetFrames;		//프레임버퍼 시작위치
	int iOffsetGLCommends;	//명령어 버퍼 시작위치
	int iOffsetEnd;			//md2 버퍼 끝
} MODELHEADER, *LPMODELHEADER;



typedef class CMD2Model
{
public://모델이 애니메이션될 상태
	enum MODELSTATE
	{
		STAND = 0,
		RUN,
		ATTACK_NOACTION,
		ATTACK_SHOULDER,
		JUMP,
		STAND_2,
		ATTACK_REACTION,
		STAND_3,
		STAND_4,
		CROUCH,
		CROUCH_WALK,
		CROUCH_STAND,
		CROUCH_DEATH,
		DEATH_FALLBACK,
		DEATH_FORWARD,
		DEATH_FALLBACKSLOW
	};

public:
	CMD2Model();
	~CMD2Model();
public:
	INT	 LoadMD2(LPCTSTR _md2ModelFile);
	VOID CreateFrame(PCHAR _pBuffer);
	VOID CreateAnimateArray(PCHAR _pBuffer);
	VOID IndexSort(PCHAR _pBuffer);
	LPVERTEXLIST Animate(MODELSTATE _motion, FLOAT _fTime);
public:
	//모델데이터를 클래스화 해야함.

public:
	//리턴 전용 함수들
	INT GetNumVertices();
	INT GetNumTriangles();
	LPFRAME GetListFrame(int _iValue);
	LPINDEXLIST GetListTriangle();
	INT GetNowFrame();
	VOID SetNowFrame(MODELSTATE _iModelState, int _iValue);
	INT GetStartFrame(int _iValue);
	INT GetEndFrame(int _iValue);

private:
	INT m_iNumFrames;	//프레임 갯수
	INT m_iNumVertices;	//버텍스 갯수
	INT m_iNumTriangles;//인덱스 갯수
	INT m_iNumTexture;	//텍스쳐 번호

	LPMODELHEADER	m_pHeader;
	LPVERTEXLIST	m_pListVertex;
	LPFRAME			m_pListFrame;
	LPANIMATION		m_pListAnimation;
	LPINDEXLIST		m_pListTriangle;	//Index List

	LPANIMATION		m_pMotion;		//이건 왜있는지 모르겠어서 빼놨음
	INT				m_iMotSel;
	FLOAT			m_fNowFrame;
	//LPTEXCOORD	  m_pListTexCoord;
	//BOOL m_bDrawState;


} *LPMD2MODEL;

#endif	// _MD2_H_