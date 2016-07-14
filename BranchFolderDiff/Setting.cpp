
#include "stdafx.h"
#include "Setting.h"
#include "Setting.h"
#include "CLSID.h"

#include <winreg.h>

#pragma warning( disable: 4996 ) 

static void
Replace( wstring &s, LPCWSTR sOld, LPCWSTR sNew )
{
	size_t oldlen = wcslen( sOld );
	size_t newlen = wcslen( sNew );
	size_t i = 0;
	while( 1 )
	{
		i = s.find( sOld, i );
		if( i < 0 )
			break;
		s = s.substr( 0, i ) + sNew + s.substr( i + oldlen );
		i += newlen;
	}
}

static void
csv2vec( wchar_t *buf, vector< wstring > &diffs )
{
	if( !*buf )
		return;

	wchar_t *p = buf, *q;
	while( true )
	{
		q = wcschr( p, L',' );
		if( !q )
			break;
		*q = 0;
		diffs.push_back( p );
		p = q + 1;
	}

	diffs.push_back( p );
}

CSetting::CSetting()
{
	wstring guid;
	GUID2String( g_clsid, guid );

	HKEY hkey;
	RegOpenKeyEx( HKEY_CLASSES_ROOT, (L"CLSID\\{" + guid + L"}\\InprocServer32").c_str(), 0, KEY_READ, &hkey );

	DWORD type = REG_SZ;
	DWORD sz = MAX_PATH;
	RegQueryValueEx( hkey, L"", NULL, &type, (LPBYTE)inipath, &sz );
	RegCloseKey( hkey );

	wchar_t *p = wcsrchr( inipath, L'\\' );
	wcscpy( p, L"\\setting.ini" );
	iniLastMod = 0;

	loadIni();
}

CSetting::~CSetting()
{
}

bool
CSetting::loadIni()
{
	_stat st;
	_wstat( inipath, &st );
	if( iniLastMod == st.st_mtime )
		return true;

	proj.clear();

	int bufsz = 512;
	wchar_t *buf = new wchar_t[bufsz];
	GetPrivateProfileString( L"setting", L"diff_exe", L"WinMergeU.exe", buf, bufsz, inipath );
	diff_exe = L"\"";
	diff_exe += buf;
	diff_exe += L"\" ";
	GetPrivateProfileString( L"setting", L"args", L"\"{SRC}\" \"{DST}\"", buf, bufsz, inipath );
	args_base = buf;

	int secsz = 64 * 1024;
	wchar_t *sec = new wchar_t[secsz];
	DWORD dw = GetPrivateProfileSectionNames( sec, secsz, inipath );
	wchar_t *p = sec;
	while( *p )
	{
		// p = section name
		wstring name, root;
		vector< wstring > diffs;

		GetPrivateProfileString( p, L"name", L"", buf, bufsz, inipath );
		name = buf;
		GetPrivateProfileString( p, L"root", L"", buf, bufsz, inipath );
		root = buf;
		if( name.size()  &&  root.size() )
		{
			if( root[ root.size() - 1 ] != L'\\' )
				root += L'\\';
			GetPrivateProfileString( p, L"diff", L"", buf, bufsz, inipath ); // csv
			csv2vec( buf, diffs );
			proj[p] = tuple< wstring, wstring, vector< wstring > >( name, root, diffs );
		}

		p += wcslen( p ) + 1;
	}

	delete []sec;
	delete []buf;

	iniLastMod = st.st_mtime;

	return true;
}

bool
CSetting::GetDiffInfo( wstring path, vector<CSettingData> &diffArgs )
{
	diffArgs.clear();

	if( !loadIni() )
		return false;

	for( auto iter = proj.begin(); iter != proj.end(); iter++ )
	{
		wstring &root = get< 1 >( iter->second );
		if( root.size() > path.size() )
			continue;

		if( wcsicmp( root.c_str(), path.substr( 0, root.size() ).c_str() ) == 0 )
		{
			// found
			vector< wstring > &diffs = get< 2 >( iter->second );
			for( auto iter = diffs.begin(); iter < diffs.end(); iter++ )
			{
				auto f = proj.find( *iter );
				if( f == proj.end() )
					continue;
				wstring other = get< 1 >( f->second );
				other += path.substr( root.size() );
				if( PathFileExists( other.c_str() ) )
				{
					wstring args = args_base;
					Replace( args, L"{SRC}", path.c_str() );
					Replace( args, L"{DST}", other.c_str() );
					diffArgs.push_back( CSettingData( get< 0 >( f->second ), diff_exe + args ) );
				}
			}
		}
	}

	return diffArgs.size() != 0;
}
