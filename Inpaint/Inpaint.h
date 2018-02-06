#pragma once
#include <stdint.h>
#include <map>
#include <vector>
#include <utility>
using namespace std;

#include "MaskDrawer.h"
class CInpaint : private CMaskDrawer
{
public:
	CInpaint();
	~CInpaint();
	//��ʼ��Ҫ�޲���ͼ�����ģ��
	//width		: ͼ��Ŀ�
	//height	: ͼ��ĸ�
	bool regionReset( int32_t width, int32_t height ) { return resetSize( width, height ); }

	//�����ͿĨ�ķ�ʽ����Ҫ�޲�������
	//��ʼ���ƣ����û��ʵİ뾶�����ʵ���״��Բ�Ρ�
	//radius	: �뾶��Բ��ʵ��ֱ���� radius * 2 + 1����˵��뾶����Ϊ 0��
	//ͨ������갴�¼�ʱ������ regionPencilBegin��
	bool regionPencilBegin( int32_t radius ) { return pencilBegin( radius ); }
	//��������ƶ������ꡣ
	//x,y		: ���ꡣ
	//�������һ������ʱ����������λ�û���ָ���뾶��ʵ��Բ��
	//�����������ʱ����ָ���뾶��ʵ��Բ������ƶ��Ĺ켣���ɳ��ߡ�
	//ͨ��������ƶ�ʱ������ regionPencilPos��
	bool regionPencilPos( int32_t x, int32_t y ) { return pencilPos( x, y ); }
	//������ǰ�Ļ��ơ�
	//ͨ��������ɿ�����ʱ������ regionPencilEnd��
	bool regionPencilEnd() { return pencilEnd(); }

	//����Ҫ�޲��ľ������򣬿��Զ�ε������ö������
	//x,y		: ��������Ͻ����꣨ͼ������Ͻ�����Ϊ(0,0)��
	//width, height	: ����Ŀ�Ⱥ͸߶ȡ�
	bool regionRect( int32_t x, int32_t y, int32_t width, int32_t height ) { return fillRect( x, y, width, height ); }

	//���߶����ӳɵıպ϶��������ΪҪ�޲�������
	//pots		: �����ÿ���ǵ����꣬������ÿ���� int32_t �ֱ�Ϊ x, y��
	//				ÿ�� x,y ����ǰһ���������ӳ��ߣ�������ɱպϵĶ���Ρ�
	//potCount	: �������������Ϊÿ������(x,y)��Ҫ���� int32_t������ potCount �� pots ���鳤�ȵ�һ�롣
	bool regionPath( const int32_t pots[], int32_t potCount ) { return pathClosed( pots, potCount ); }

	//ˮӡ��⡣�����������ͬλ������ˮӡ����ͬ�ֱ��ʵ�ͼ���Զ����ˮӡ������
	//imgBuf	: ͼ������ݣ�ֻ����32λ���ظ�ʽ��
	//pitch		: ͼ��ÿ�е��ֽ�����ͼ��Ŀ���� regionReset ���õ�һ�¡�
	//�������� false ��ʾ��û�гɹ���⵽ˮӡ����Ҫ������������ͼ��
	//�������� true ��ʾ�Ѿ��ɹ���⵽ˮӡ������ʹ�� getRegionImage ��ȡ��ģͼ��鿴��⵽��ˮӡ����
	//				��⵽ˮӡ�󣬻��Զ�ʹ�� regionPath ������ǳ�ˮӡ����
	//�����ͼ��Խ�࣬��⵽��ˮӡ����Խ׼ȷ����ʹ�����Ѿ������� true����Ȼ���Լ�������ͼ��
	//ͨ����������ÿ��ͼƬ�����нϴ��������ô 5 �� 10 �žͿ���׼ȷ�ؼ�⵽ˮӡ���������Ҫ�����ͼƬ��
	//��������Ƶ�е�ˮӡ����Ϊ�ٽ���֡����С�������ÿ�������Ļ�������һ֡���Լ��ټ�������
	//�����Ƶ����̣ܶ����� 10 �룬��ôҲ���Լ����֡����һ�Ρ�
	bool regionDetection( uint8_t* imgBuf, int32_t pitch ) {
		return CMaskDrawer::watermarkDetection( imgBuf, pitch ); }

	//ȡ�������޲�����Ĳ��������������Ѿ������Ĳ�������
	//current	: ���ص�ǰ������������
	//��Ϊ���Գ������������������� current ����С�ں����ķ���ֵ����ʾ������һЩ������
	int32_t	regionOperNum( int32_t& current ) { return operNum( current ); }

	//����һ��������
	//ÿ�γ���������ʹ�� regionOperNum ����ȡ�� current ֵ���֮ǰ��1��ֱ��Ϊ0��
	bool regionUndo() { return undo(); }
	//�����������Ĳ�����
	//ÿ������������ʹ�� regionOperNum ����ȡ�� current ֵ���֮ǰ��1��ֱ���� regionOperNum �ķ���ֵ��ȡ�
	bool regionRedo() { return redo(); }
	
	//ÿ�ν����������޲�����Ĳ���(��������������)�󣬿���ȡ����ģ�ܵ�Ӱ��ľ�������
	//������ж�β�����ŵ��ñ�����ȡ����ģ�ܵ�Ӱ��ľ���������õ������ۻ��ϲ��ľ�������
	bool getRegionChangedRect( int32_t& x, int32_t& y, int32_t& width, int32_t& height ) { return getChangedRect( x, y, width, height ); }
	//ȡ����ģ��ͼ��
	//��õ���һ�� 8bit ��λͼ����ÿ���ֽڱ�ʾһ�����ء�
	//pitch		: ��ģͼ��ÿ�е��ֽ�����ͼ��Ŀ���� regionReset ���õ���ͬ��
	//���ص�ͼ�������У�����ֽ�ֵ��Ϊ0����ʾ��ǰλ�õ������Ѿ�������Ϊ���޲���
	const uint8_t* getRegionImage( int32_t& pitch ) { return getMaskImage( pitch ); }

	//��ʼ�޲�ͼ��ֻ����32λ���ظ�ʽ��
	//imgBuf	: ͼ�������
	//pitch		: ͼ��ÿ�е��ֽ�����ͼ��Ŀ���� regionReset ���õ�һ�¡�
	//inpType	: �޲��ķ�ʽ�� 0 �� 1 �� 2
	//���Ҫ�Զ���ͼƬ�����޲�����ÿ��ͼƬ��ˮӡλ�ú�ͼ��ֱ�����ͬʱ������Ҫ�ظ����ô��޲�����
	//����Ҫ�޲���ͼƬ��ˮӡλ�û�ֱ������ϴβ�ͬʱ��������� regionReset��Ȼ���������ô��޲�����
	//�޲���ͼ��֮�󣬶��޲�������ģ(mask)�Ĳ����Ͳ��ܳ����ˣ�����ٴ������޲�������ʹ�������õ��޲�����
	bool inpaint( uint8_t* imgBuf, int32_t pitch, int32_t inpType );
private:

#define	PIXEL_DIFF_MIN		16
#define	PIXEL_DIFF_MAX		128


#define	BLOCK_RADIUS	7
#define	MAX_BLOCK_FIND_RADIUS	50//( MAX_EDGE_BLOCK_RADIUS * 10 )
#define	MAX_BLOCK_DIAMETER		( MAX_EDGE_BLOCK_RADIUS * 2 + 1 )


#pragma pack(push)
#pragma pack(1)
	struct SOrgPixel
	{
		uint8_t	b;
		uint8_t	g;
		uint8_t	r;
		uint8_t	a;
		SOrgPixel( uint8_t _b = 0, uint8_t _g = 0, uint8_t _r = 0, uint8_t _a = 0 )
		{
			b = _b; g = _g; r = _r; a = _a;
		}
	};

#define	PIXEL_INVALID	0x80
#define	PIXEL_MISSING	0x40
#define	PIXEL_BORDER	0x20
#define	PIXEL_PATCHED	0x10
#define	IS_STATIC_PIXEL( pix )	( ( (pix) & ( PIXEL_INVALID | PIXEL_MISSING ) ) == 0 )	//ͼ����һֱ���ڵ�
#define	IS_PATCHED_PIXEL( pix )	( (pix) & ( PIXEL_PATCHED ) )	//�Ѿ��޸�
#define	IS_UNREPAIR_PIXEL( pix )	( ( (pix) & ( PIXEL_MISSING | PIXEL_PATCHED ) ) == PIXEL_MISSING )	//δ�޸�
#define	IS_MISSING_PIXEL( pix )	( (pix) & ( PIXEL_MISSING ) )	//���޸�


#define	PIXEL_BOREDR_RADIUS_MASK		0x0F
#define	IS_SEARCH_PIXEL( pix )	( ( (pix) & ( PIXEL_INVALID | PIXEL_MISSING | PIXEL_BOREDR_RADIUS_MASK ) ) == 0 )	//ͼ����һֱ���ڵ�
#define	IS_VALID_PIXEL( pix )	( IS_STATIC_PIXEL( pix ) || ( (pix) & ( PIXEL_BOREDR_RADIUS_MASK ) ) )
#define	IS_BORDER_PIXEL( pix )	( (pix) & ( PIXEL_BORDER ) )	//�޸�����߽�

#pragma pack(pop)
	CImage<SOrgPixel>	m_imgOrg;
	CImage<uint8_t>		m_imgProc;

	GPoint			m_boundTopLeft;
	GRect			m_findRect;

	struct SEdgePoint : public GPoint
	{
		int32_t		pixCount;
		int32_t		xLength;
		int32_t		yLength;
		SOrgPixel	avgColor;
		SOrgPixel	xEndColor;
		SOrgPixel	yEndColor;
		SEdgePoint() { pixCount = 0; avgColor = SOrgPixel(); xLength = yLength = 0; }
		SEdgePoint( const GPoint& pt, int32_t _pixCount = 0, SOrgPixel _color = SOrgPixel() )
			:GPoint( pt )
		{
			pixCount	= _pixCount;
			xEndColor = yEndColor = avgColor = _color;
			xLength = yLength = 1;
		}
		SEdgePoint( int32_t _x, int32_t _y, int32_t _pixCount = 0, SOrgPixel _color = SOrgPixel() )
			:GPoint( _x, _y )
		{
			pixCount	= _pixCount;
			xEndColor = yEndColor = avgColor = _color;
			xLength = yLength = 1;
		}
	};
	struct SBorderPoint : public GPoint
	{
		int32_t		sum;
		GPoint		offset;
		SBorderPoint() { sum = 0; }
		SBorderPoint( const GPoint& pt, int32_t _sum = 0 )
			:GPoint( pt )	{ sum	= _sum;	}
		SBorderPoint( int32_t _x, int32_t _y, int32_t _sum = 0 )
			:GPoint( _x, _y ) {	sum	= _sum;	}
	};
	SEdgePoint*		m_edgePots;
	int32_t			m_edgeAlloc;
	int32_t			m_edgeCount;
	vector<GPoint>	m_borderPtFirsts;
	vector<SBorderPoint>	m_borderPots;

	int32_t findEdgePots();
	int32_t findBorderPots();
	void	fastInpaint();
	void	fastInpaint2();
	void	expendBorder();
	void	drawbackBorder();
	//SOrgPixel getAvgColor( int32_t x, int32_t y, int32_t radius );
	inline int32_t checkFirstEdge( int32_t x, int32_t y );
	inline int32_t checkFirstEdge2( int32_t x, int32_t y );
	inline bool checkFirstBorder( int32_t x, int32_t y, int32_t radius = PIXEL_BOREDR_RADIUS_MASK );
		
	void structuralRepair( int32_t x, int32_t y );

	const	GPoint	poOctree8[8]	= { { 0, -1 }, { 1, -1 }, { 1, 0 }, { 1, 1 }, { 0, 1 }, { -1, 1 }, { -1, 0 }, { -1, -1 } };
};

