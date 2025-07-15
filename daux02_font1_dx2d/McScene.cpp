// Implementation of the CMcScene class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


struct TT_OFFSET_TABLE
{
	USHORT	uMajorVersion;
	USHORT	uMinorVersion;
	USHORT	uNumOfTables;
	USHORT	uSearchRange;
	USHORT	uEntrySelector;
	USHORT	uRangeShift;
};

struct TT_TABLE_DIRECTORY
{
	char	szTag[4];			//table name
	ULONG	uCheckSum;			//Check sum
	ULONG	uOffset;			//Offset from beginning of file
	ULONG	uLength;			//length of the table in bytes
};

struct TT_NAME_TABLE_HEADER
{
	USHORT	uFSelector;			//format selector. Always 0
	USHORT	uNRCount;			//Name Records count
	USHORT	uStorageOffset;		//Offset for strings storage, from start of the table
};

struct TT_NAME_RECORD
{
	USHORT	uPlatformID;
	USHORT	uEncodingID;
	USHORT	uLanguageID;
	USHORT	uNameID;
	USHORT	uStringLength;
	USHORT	uStringOffset;	//from start of storage area
};

#define SWAPWORD(x)		MAKEWORD(HIBYTE(x), LOBYTE(x))
#define SWAPLONG(x)		MAKELONG(SWAPWORD(HIWORD(x)), SWAPWORD(LOWORD(x)))

int font_name(std::string* str_out, const char* file_name)
{
	FILE* fp = fopen(file_name, "rb");

	std::string csCopyright;
	std::string csName;
	std::string csTrademark;
	std::string csFamily;

	if(!fp)
		return -1;


	TT_OFFSET_TABLE ttOffsetTable;
	fread(&ttOffsetTable, sizeof(TT_OFFSET_TABLE), 1, fp);
	ttOffsetTable.uNumOfTables = SWAPWORD(ttOffsetTable.uNumOfTables);
	ttOffsetTable.uMajorVersion = SWAPWORD(ttOffsetTable.uMajorVersion);
	ttOffsetTable.uMinorVersion = SWAPWORD(ttOffsetTable.uMinorVersion);

	//check is this is a true type font and the version is 1.0
	//if(ttOffsetTable.uMajorVersion != 1 || ttOffsetTable.uMinorVersion != 0)
	//{
	//	fclose(fp);
	//	return -1;
	//}

	TT_TABLE_DIRECTORY tblDir;
	BOOL bFound = FALSE;
	std::string csTemp;

	for(int i=0; i< ttOffsetTable.uNumOfTables; i++)
	{
		fread(&tblDir, sizeof(TT_TABLE_DIRECTORY), 1, fp);

		char buf[128]{};
		strncpy(buf, tblDir.szTag, 4);
		if(0 == _stricmp(buf, "name"))
		{
			bFound = TRUE;
			tblDir.uLength = SWAPLONG(tblDir.uLength);
			tblDir.uOffset = SWAPLONG(tblDir.uOffset);
			break;
		}
		else if (buf[0] == 0)
		{
			break;
		}
	}

	if(!bFound)
	{
		fclose(fp);
		return -1;
	}


	fseek(fp, tblDir.uOffset, SEEK_SET);

	TT_NAME_TABLE_HEADER ttNTHeader;

	fread(&ttNTHeader, sizeof(TT_NAME_TABLE_HEADER), 1, fp);
	ttNTHeader.uNRCount       = SWAPWORD(ttNTHeader.uNRCount);
	ttNTHeader.uStorageOffset = SWAPWORD(ttNTHeader.uStorageOffset);
	TT_NAME_RECORD ttRecord;
	bFound = FALSE;

	for(int i=0; i<ttNTHeader.uNRCount; ++i)
	{
		fread(&ttRecord, sizeof(TT_NAME_RECORD), 1, fp);

		ttRecord.uNameID       = SWAPWORD(ttRecord.uNameID);
		ttRecord.uStringLength = SWAPWORD(ttRecord.uStringLength);
		ttRecord.uStringOffset = SWAPWORD(ttRecord.uStringOffset);

		if(ttRecord.uNameID == 0 || ttRecord.uNameID == 1|| ttRecord.uNameID == 4 || ttRecord.uNameID == 7)
		{
			int nPos = ftell(fp);
			fseek(fp, tblDir.uOffset + ttRecord.uStringOffset + ttNTHeader.uStorageOffset, SEEK_SET);

			char  c_buf[1040]{};
			short w_buf[ 520]{};
			fread(c_buf, ttRecord.uStringLength, 1, fp);

			std::wstring strUni;
			std::string strMulti;

			if(!c_buf[0])
			{
				for(int i=0; i<ttRecord.uStringLength/2; ++i)
				{
					w_buf[i] = MAKEWORD(c_buf[i*2+1], c_buf[i*2+0]);			// font is big endian.
				}

				strUni =std::wstring((wchar_t*)w_buf);
				int len = WideCharToMultiByte( CP_ACP, 0, &strUni[0], -1, NULL, 0, NULL, NULL );
				strMulti = std::string(len,0);
				WideCharToMultiByte( CP_ACP, 0,  &strUni[0], -1, &strMulti[0], len, NULL, NULL );
			}
			else
			{
				//strMulti = c_buf;

				//char*	tmp_utf         = c_buf;
				//wchar_t	tmp_uni  [ 520] = {0};
				//char    tmp_multi[1040] = {0};

				//int len = 0;

				//len = MultiByteToWideChar(CP_UTF8, 0, tmp_utf, strlen(tmp_utf), NULL, NULL);
				//MultiByteToWideChar(CP_UTF8, 0, tmp_utf, strlen(tmp_utf), tmp_uni, len);

				//len = WideCharToMultiByte( CP_ACP, 0, tmp_uni, -1, NULL, 0, NULL, NULL );
				//WideCharToMultiByte( CP_ACP, 0, tmp_uni, -1, tmp_multi, len, NULL, NULL );

				//strMulti = tmp_multi;
			}


			if(strMulti.length())
			{
				switch(ttRecord.uNameID)
				{
				case 0:
					csCopyright = strMulti;
					break;

				case 1:
					csFamily = strMulti;
					break;

				case 4:
					csName = strMulti;
					break;

				case 7:
					csTrademark = strMulti;
					break;

				default:
					break;
				}
			}
			fseek(fp, nPos, SEEK_SET);
		}
	}
	fclose(fp);

	if(csName.empty())
		csName = csFamily;

	*str_out = csFamily;
	return 0;
}



CMcScene::CMcScene()
{
	m_pDev	= NULL;

	m_pDXFont1= NULL;
	m_pDXFont2= NULL;
}


CMcScene::~CMcScene()
{
	Destroy();
}

void CMcScene::Destroy()
{
	HRESULT hr=0;
	hr = RemoveFontResourceEx("font/NanumBarunGothicBold.otf", FR_NOT_ENUM, NULL);
	hr = RemoveFontResourceEx("font/08SeoulHangangM.ttf", FR_NOT_ENUM, NULL);

	SAFE_RELEASE(	m_pDXFont1	);
	SAFE_RELEASE(	m_pDXFont2	);
}


INT CMcScene::Create(LPDIRECT3DDEVICE9 pDev)
{
	HRESULT hr=0;
	m_pDev	= pDev;


	// 서울 서체: http://design.seoul.go.kr/policy/data_view.php?id=63

	font_name(&cs_name1, "font/NanumBarunGothicBold.otf" );
	font_name(&cs_name2, "font/08SeoulHangangM.ttf");

	// _WIN32_WINNT >= 0x0500 이상. WinGDI.h
	hr = AddFontResourceEx("font/NanumBarunGothicBold.otf", FR_NOT_ENUM, NULL);
	hr = AddFontResourceEx("font/08SeoulHangangM.ttf", FR_NOT_ENUM, NULL);

	hr = D3DXCreateFont(m_pDev
		, 36, 0, FW_BOLD
		, 1, FALSE
		, HANGUL_CHARSET
		, OUT_DEFAULT_PRECIS
		, ANTIALIASED_QUALITY
		, FF_DONTCARE, cs_name1.c_str()
		, &m_pDXFont1);

	if(FAILED(hr))
		return -1;


	D3DXFONT_DESC hFont =
	{
		72, 0, FW_BOLD
			, 1, 0
			, HANGUL_CHARSET
			, OUT_DEFAULT_PRECIS
			, ANTIALIASED_QUALITY
			, FF_DONTCARE
	};

	if(cs_name2.length())
		strcpy(hFont.FaceName, cs_name2.c_str());
	else
		strcpy(hFont.FaceName, "Arial");

	hr = D3DXCreateFontIndirect(m_pDev, &hFont, &m_pDXFont2);

	if(FAILED(hr))
		return -1;



	return 0;
}


INT CMcScene::Restore()
{
	m_pDXFont1->OnResetDevice();
	m_pDXFont2->OnResetDevice();

	return 0;
}

void CMcScene::Invalidate()
{
	m_pDXFont1->OnLostDevice();
	m_pDXFont2->OnLostDevice();
}


INT CMcScene::FrameMove()
{
	return 0;
}

void CMcScene::Render()
{
	RECT rct={0};

	char	sMsg[1024]={0};

	lstrcpy( sMsg, cs_name1.c_str() );
	lstrcat( sMsg, ": 아름다운 겨례글 Α.Ω. 3141C" );
	rct.left	= 2;
	rct.right	= 800;
	rct.top		= 160;
	rct.bottom	= rct.top + 40;
	m_pDXFont1->DrawText(NULL, sMsg, -1, &rct, 0, 0xFFFF00FF );

	lstrcpy( sMsg, cs_name2.c_str() );
	lstrcat( sMsg, ": 아름다운 한글 ABC" );
	rct.left	= 2;
	rct.right	= 800;
	rct.top		= 250;
	rct.bottom	= rct.top + 70;
	m_pDXFont2->DrawText(NULL, sMsg, -1, &rct, 0, 0xFF00FFFF );
}

