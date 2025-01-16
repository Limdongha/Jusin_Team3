#pragma once

class CTextures; //전방선언

class CResMgr
{
	SINGLE(CResMgr)

public:
	void		Initialize();

public:
	wchar_t*		GetContentPath()	{ return m_szFilePath; }

public:
	CTextures*		LoadTexture(const wstring& _strKey, const TCHAR* _strRelativePath);
	CTextures*		FindTexture(const wstring& _strKey);

private:
	map<wstring, CTextures*>	m_mapTex;
	wchar_t						m_szFilePath[255];

};

