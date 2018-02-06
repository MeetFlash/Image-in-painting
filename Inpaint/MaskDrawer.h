#pragma once
#include <stdint.h>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>
using namespace std;
#include "GRect.h"
#include "GSize.h"
#include "GPoint.h"
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

class CMaskDrawer
{
public:
	CMaskDrawer();
	~CMaskDrawer();

	bool resetSize( int32_t width, int32_t height );
	bool fillRect( int32_t x, int32_t y, int32_t width, int32_t height );
	bool pencilBegin( int32_t radius );
	bool pencilPos( int32_t x, int32_t y );
	bool pencilEnd();

	bool pathClosed( const int32_t pots[], int32_t potCount );

	int32_t	operNum( int32_t& current );
	bool redo();
	bool undo();

	bool getChangedRect( int32_t& x, int32_t& y, int32_t& width, int32_t& height );
	const uint8_t* getMaskImage( int32_t& pitch ) { pitch = m_maskImage.pitch; return (uint8_t*)m_maskImage.pixel( 0, 0 ); }

	bool watermarkDetection( uint8_t* imgBuf, int32_t pitch );
protected:
	template <class pixel_t>
	class CImage
	{
	public:
		int32_t		width;
		int32_t		height;
		int32_t		pitch;
		int32_t		allocBytes;
		int8_t		bytesPerPixel;
		bool		useExternal;
		pixel_t*	imgBuf;
		GRect		rtImage;
		CImage()
		{
			width = height = pitch = bytesPerPixel = allocBytes = 0;
			useExternal = false; imgBuf = NULL;
		}
		~CImage()
		{
			clear();
		}
		bool create( pixel_t* _imgBuf, int32_t _width, int32_t _height, int32_t _pitch = 0, bool _useExternal = false )
		{
			if ( _height <= 0 || _height <= 0 ) return false;
			if ( _imgBuf == NULL && _useExternal ) return false;
			if ( _useExternal )
			{
				if ( _pitch <= 0 ) _pitch = ( sizeof( pixel_t ) * _width + 3 ) / 4 * 4;
				pitch = _pitch;
				imgBuf = _imgBuf;
			}
			else
			{
				int32_t		pitchTmp	= max( sizeof( pixel_t ) * _width, _pitch );
				if ( pitchTmp * _height > allocBytes )
				{
					pixel_t*	imgTmp		= new pixel_t[pitchTmp * _height];
					if ( nullptr == imgTmp ) return false;
					clear();
					allocBytes	= pitchTmp * _height;
					imgBuf	= imgTmp;
				}
				pitch	= pitchTmp;
				if ( _imgBuf )
				{
					for ( int32_t y = 0; y < _height; ++y )
					{
						memcpy( imgBuf + y * pitch, _imgBuf + y * _pitch, pitch );
					}
				}
				else
				{
					memset( imgBuf, 0, pitch * _height );
				}
			}
			width = _width;
			height = _height;
			bytesPerPixel = sizeof( pixel_t );
			useExternal = _useExternal;
			rtImage	= GRect( 0, 0, width, height );
			return true;
		}
		void clear()
		{
			if ( !useExternal && imgBuf )
				delete[]imgBuf;
			width = height = pitch = bytesPerPixel = allocBytes = 0;
			useExternal = false; imgBuf = NULL;
		}
		inline pixel_t* pixel( int32_t x, int32_t y )
		{
			return (pixel_t*)( ( (uint8_t*)( imgBuf + x )) + y * pitch );
		}
		inline pixel_t* pixelEx( int32_t x, int32_t y )
		{
			int32_t	xd	= abs( x ) % ( width * 2 - 2 );
			x	= xd - ( ( ( xd + 1 ) % width ) * ( xd / width * 2 ) );
			int32_t	yd	= abs( y ) % ( height * 2 - 2 );
			y	= yd - ( ( ( yd + 1 ) % height ) * ( yd / height * 2 ) );
			return (pixel_t*)( ( (uint8_t*)( imgBuf + x ) ) + y * pitch );
		}
		int32_t byteCount() { return pitch * height; }
		bool isVaild() { return width && height; }

	};

	enum EChipType
	{
		eRect,
		ePencil,
		ePath,
	};

	struct SChip
	{
		EChipType	eType;
		int32_t		penRadius;		//���ʵİ뾶�����أ�
		GRect		rect;
		vector<GPoint>	pots;
		SChip()
		{
			penRadius	= 0;
			eType		= eRect;
		}
	};

	#pragma pack(push)
	#pragma pack(1)
	struct SMaskPixel
	{
		uint8_t	drawCount	: 7;
		uint8_t	curDraw		: 1;
	};
	#pragma pack(pop)

	CImage<SMaskPixel>	m_maskImage_exp;
	CImage<SMaskPixel>	m_maskImage;
	vector<SChip*>	m_picChips;		//��ģ�������������򡢻��ʡ��պ�·�����б�
	int32_t			m_currOper;		//��ǰ��ɵ���ģ��������������������1������������1��
	vector<GRect>	m_changedRect;
	SChip*			m_curPencil;
	bool			m_maskReset;	//mask �Ѿ�ʧЧ����Ҫ���³�ʼ����

	bool			m_unitedChanged;
	GRect			m_inpRect;
	vector<GRect>	m_rtUniteds;
	vector<GRect>	m_rtAbsolutes;

	struct SDetectPixel
	{
		uint8_t		sobel1 : 1;		//����ǰͼ�������Ƿ��� sobel ���Ӽ�������Ե��1���ǣ�0�����ǡ�
		uint8_t		sobel2 : 1;		//������ͼ�������Ƿ��� sobel ���Ӽ�������Ե��1���ǣ�0�����ǡ�
		uint8_t		mark : 1;		//������ˮӡ�����ء�
		uint8_t		mark2 : 1;		//ȷ����ˮӡ�����ء�
		uint8_t		weight;			//��Ե���ر��������Ĵ���������ڶ���ͼƬ��ͬλ�����ض��Ǳ�Ե����ô�����ؿ��ܾ���ˮӡͼ��ı�Ե��
		uint16_t	integral;		//����ͼ�����ڿ���ͳ��һ�������ڵ���Ч�ı�Ե����������

	};
	CImage<SDetectPixel>	m_watermark;	//���ˮӡ����¼ÿ�����ص�״̬���Ƿ���ˮӡ��
	uint32_t				m_sobelSum;		//���һ֡ͼ����ʹ�� sobel ���Ӽ����Ե����Ч�ı�Ե����������
	uint32_t				m_maxWeight;	//���ˮӡ��������������Ȩ�ء�
	vector<int32_t>			m_wmarkChips;	//���ˮӡ��õ�ˮӡ�����Ապ�·����ʽ��ӵ���ģ�����б�ʱ��������
	GRect drawLine( const GPoint& poStart, const GPoint& poEnd, float radius );
	void fillPath( const vector<GPoint>& pots );
	bool getUnited();
	int32_t	otsuHistogram( int32_t hisCount, uint32_t* hisTab );
	inline bool	otsuOnlyNumber( int32_t hisCount, uint32_t* numTab, vector<int32_t>& lines );
	template <class mapIterator>
	mapIterator otsuHistogram( mapIterator begin, mapIterator end );
	void resetMask();
	bool convexClosure( vector<GPoint>& pots );
	bool redoOper( SChip* chip );
	bool undoOper( SChip* chip );
};
