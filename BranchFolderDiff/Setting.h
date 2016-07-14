
#pragma once

#include <string>
#include <vector>
#include <map>

using namespace std;

class CSettingData
{
public:
	CSettingData( wstring &n, wstring &p ) { name = n; exeAndArgs = p; }
	wstring name;	// menu title
	wstring exeAndArgs;
};

class CSetting
{
public:
			CSetting();
	virtual	~CSetting();

//	wstring	GetDiffExe() { return diff_exe; }
	bool	GetDiffInfo( wstring path, vector<CSettingData> &diffArgs );

protected:
	bool	loadIni();

	wchar_t inipath[MAX_PATH];
	__time64_t iniLastMod;

	wstring	diff_exe;
	wstring	args_base;

	map< wstring, tuple< wstring, wstring, vector< wstring > > > proj; // name, root, diff[]
};
