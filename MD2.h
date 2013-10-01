#ifndef _MD2_H_
#define _MD2_H_

#include <d3d9.h>
#include <d3dx9.h>

#define MD2_FVF ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2 )

//���ؽ� ����
typedef struct _tagVertexList
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 nor;
	D3DXVECTOR2 tex1;
	D3DXVECTOR2 tex2;
} VERTEXLIST, *LPVERTEXLIST;

//������ ����
typedef struct _tagFrame
{
	CHAR fra_name[16];
	LPVERTEXLIST ver;
} FRAME, *LPFRAME;

//�ִϸ��̼� ����
typedef struct _tagAnimation
{
	CHAR ani_name[16];
	SHORT startFrame;
	SHORT endFrame;
	SHORT frameNum;
} ANIMATION, *LPANIMATION;

//�ε��� ����
typedef struct _tagIndex
{
	union//�޸𸮰��� ����
	{
		WORD ptr[3];
		struct{ WORD _0, _1, _2; };
	};
} INDEXLIST, *LPINDEXLIST;



//�ؽ��� ��ǥ
typedef struct _tagTexCoordIdx
{
	short u;				//U��ǥ
	short v;				//V��ǥ
} TEXCOORDIDX, *LPTEXCOORDIDX;

//�ϳ��� ������ ������ ���� ����
typedef struct _tagVertexPerFrame
{
	unsigned char v[3];			//���� ��ǥ
	unsigned char normalIndex;	//����
} VERTEXPERFRAME, *LPVERTEXPERFRAME;

//�ϳ��� �����ӿ� ���� ����
typedef struct _tagFrameInfo
{
	float scale[3];			//������ �������� Ȯ��.���
	float translate[3];		//������ �������� �̵�
	char name[16];			//�� �̸�
	VERTEXPERFRAME listVertex[1];
} FRAMEINFO, *LPFRAMEINFO;

//�ϳ��� �ﰢ�� ������
typedef struct _tagMesh
{
	unsigned short meshIndex[3];	//���� ���ε�
	unsigned short stIndex[3];		//�ؽ��� ��ǥ ���ε�
} MESHLIST, *LPMESHLIST;


//MD2 ���
typedef struct _tagModelHeader
{
	int ID;					//�ĺ����̵� 844121161 // IDP2
	int iVersion;			//���� 8
	int iTexWidth;			//�ؽ��� ����
	int iTexHeight;			//�ؽ��� ����
	int iFrameSize;			//Ű�����Ӵ� ����Ʈ��
	int iNumTextures;		//�ؽ���(��Ų) ����
	int iNumVertices;		//���ؽ�(����) �� ����
	int iNumTexcoords;		//�ؽ��� ��ǥ�� ����
	int iNumTriangles;		//�ε����� ����
	int iNumGLCommands;		//triangle strips || fans�� ����ȭ�Ǿ��ִ��� �˷���
	int iNumFrames;			//�ִϸ��̼� KeyFrame�� ����

	int iOffsetTexture;		//�ؽ��� ������ġ
	int iOffsetTexCoord;	//�ؽ��� UV ���� ������ġ
	int iOffsetTriangleIdx;	//�ε������� ������ġ
	int iOffsetFrames;		//�����ӹ��� ������ġ
	int iOffsetGLCommends;	//��ɾ� ���� ������ġ
	int iOffsetEnd;			//md2 ���� ��
} MODELHEADER, *LPMODELHEADER;



typedef class CMD2Model
{
public://���� �ִϸ��̼ǵ� ����
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
	//�𵨵����͸� Ŭ����ȭ �ؾ���.

public:
	//���� ���� �Լ���
	INT GetNumVertices();
	INT GetNumTriangles();
	LPFRAME GetListFrame(int _iValue);
	LPINDEXLIST GetListTriangle();
	INT GetNowFrame();
	VOID SetNowFrame(MODELSTATE _iModelState, int _iValue);
	INT GetStartFrame(int _iValue);
	INT GetEndFrame(int _iValue);

private:
	INT m_iNumFrames;	//������ ����
	INT m_iNumVertices;	//���ؽ� ����
	INT m_iNumTriangles;//�ε��� ����
	INT m_iNumTexture;	//�ؽ��� ��ȣ

	LPMODELHEADER	m_pHeader;
	LPVERTEXLIST	m_pListVertex;
	LPFRAME			m_pListFrame;
	LPANIMATION		m_pListAnimation;
	LPINDEXLIST		m_pListTriangle;	//Index List

	LPANIMATION		m_pMotion;		//�̰� ���ִ��� �𸣰ھ ������
	INT				m_iMotSel;
	FLOAT			m_fNowFrame;
	//LPTEXCOORD	  m_pListTexCoord;
	//BOOL m_bDrawState;


} *LPMD2MODEL;

#endif	// _MD2_H_