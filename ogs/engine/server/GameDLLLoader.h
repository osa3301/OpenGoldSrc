#pragma once

class CGameDLLLoader
{
public:
	CGameDLLLoader();
	~CGameDLLLoader();
	
	void LoadEntityDLLs(const char *szBaseDir);
	void ReleaseEntityDlls();
};