#pragma once

class CSource
{
private:
	wstring		m_strKey;
	wstring		m_strRelativePath;

public:
	void		SetKey(const wstring& _key)			{ m_strKey = _key; }
	void		SetRelativePath(const wstring& _path) { m_strRelativePath = _path; }

	wstring&	GetKey() { return m_strKey; }
	wstring&	GetRelativePath() { return m_strRelativePath; }

public:
	CSource();
	virtual ~CSource();
};

