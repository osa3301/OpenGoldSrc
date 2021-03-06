/*
 *	This file is part of OGS Engine
 *	Copyright (C) 2016-2017 OGS Dev Team
 *
 *	OGS Engine is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	OGS Engine is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with OGS Engine.  If not, see <http://www.gnu.org/licenses/>.
 *
 *	In addition, as a special exception, the author gives permission to
 *	link the code of OGS Engine with the Half-Life Game Engine ("GoldSrc/GS
 *	Engine") and Modified Game Libraries ("MODs") developed by Valve,
 *	L.L.C ("Valve").  You must obey the GNU General Public License in all
 *	respects for all of the code used other than the GoldSrc Engine and MODs
 *	from Valve.  If you modify this file, you may extend this exception
 *	to your version of the file, but you are not obligated to do so.  If
 *	you do not wish to do so, delete this exception statement from your
 *	version.
 */

/// @file
/// @brief high-level file system

// filesystem_engine.h
// this was originally the C wrapper for the filesystem interface
// that's still present in original GoldSrc sources
// but it was upgraded to class here

#pragma once

#include "common/maintypes.h"
#include "public/interface.h"
#include "public/tier1/utlvector.h"
#include "public/FileSystem.h"
#include "public/archtypes.h"

// clang-format off
#ifdef _WIN32
	const char FILESYSTEM_DLL_NAME[] = "filesystem_stdio.dll";
#else
	const char FILESYSTEM_DLL_NAME[] = "filesystem_stdio.so";
#endif

#ifdef HOOK_ENGINE
	#define g_fallbackLocalizationFiles (*pg_fallbackLocalizationFiles)
	#define s_pBaseDir (*ps_pBaseDir)
	#define bLowViolenceBuild (*pbLowViolenceBuild)

	#define g_pFileSystemModule (*pg_pFileSystemModule)
	#define g_FileSystemFactory (*pg_FileSystemFactory)
	
	#define g_pFileSystem (*pg_pFileSystem)
#endif // HOOK_ENGINE
// clang-format on

extern CUtlVector<char *> g_fallbackLocalizationFiles;
extern bool bLowViolenceBuild;

extern IFileSystem *gpFileSystem;

NOXREF void *GetFileSystemFactory();

bool BEnabledHDAddon();
bool BEnableAddonsFolder();

// Why here???

void Host_SetHDModels_f();
void Host_SetAddonsFolder_f();
void Host_SetVideoLevel_f();

int Host_GetVideoLevel();

void *FS_LoadLibrary(const char *dllName);

namespace opengoldsrc
{
	struct IFile;
};

class CFileSystem // : public opengoldsrc::IFileSystem
{
public:
	CFileSystem(IFileSystem *apFileSystem) : mpFileSystem(apFileSystem){}
	~CFileSystem() = default;
	
	int Init(char *basedir);
	void Shutdown();
	
	void CreatePath(char *path);
	
	NOXREF void WriteFile(char *filename, void *data, int len);
	
	NOXREF void CopyFile(char *netpath, char *cachepath);
	
	const char *GetBaseDirectory();
	const char *GetGameDirectory(); // GetGameDir
	
	int SetGameDirectory(const char *pDefaultDir, const char *pGameDir);
	
	void AddDefaultDir(char *pszDir);
	int AddFallbackGameDir(const char *pGameDir);
	
	void AddSearchPath(const char *pPath, const char *pathID);
	NOXREF int RemoveSearchPath(const char *pPath);
	void RemoveAllSearchPaths();

	void RemoveFile(const char *pRelativePath, const char *pathID);
	void CreateDirHierarchy(const char *path, const char *pathID);

	int FileExists(const char *pFileName);
	NOXREF int IsDirectory(const char *pFileName);

	/*FileHandle_t*/ CFile *Open(const char *pFileName, const char *pOptions);
	/*FileHandle_t*/ CFile *OpenPathID(const char *pFileName, const char *pOptions, const char *pathID);

	void Close(/*FileHandle_t*/ opengoldsrc::IFile *apFile);

	unsigned int FileSize(const char *pFileName);
	
	NOXREF int ExpandFileName(char *filename);

	int32 GetFileTime(const char *pFileName);
	NOXREF void FileTimeToString(char *pStrip, int maxCharsIncludingTerminator, int32 fileTime);

	const char *FindFirst(const char *pWildCard, FileFindHandle_t *pHandle, const char *pathID);
	const char *FindNext(FileFindHandle_t handle);
	NOXREF int FindIsDirectory(FileFindHandle_t handle);
	void FindClose(FileFindHandle_t handle);

	void GetLocalCopy(const char *pFileName);

	const char *GetLocalPath(const char *pFileName, char *pLocalPath, int localPathBufferSize);

	NOXREF char *ParseFile(char *pFileBytes, char *pToken, int *pWasQuoted);

	NOXREF int FullPathToRelativePath(const char *pFullpath, char *pRelative);

	NOXREF int GetCurrentDirectory(char *pDirectory, int maxlen);

	NOXREF void PrintOpenedFiles();

	NOXREF void SetWarningFunc(void (*pfnWarning)(const char *, ...));
	NOXREF void SetWarningLevel(FileWarningLevel_t level);

	void LogLevelLoadStarted(const char *name);
	void LogLevelLoadFinished(const char *name);

	int SetVBuf(FileHandle_t stream, char *buffer, int mode, size_t size);

	void GetInterfaceVersion(char *p, int maxlen);

	void *GetReadBuffer(FileHandle_t file, int *outBufSize);
	void ReleaseReadBuffer(FileHandle_t file, void *buffer);

	void Unlink(const char *filename);

	void Rename(const char *originalName, const char *newName);
private:
	void CheckLiblistForFallbackDir(const char *pGameDir, bool bLanguage, const char *pLanguage, bool bLowViolenceBuild_);
	bool SetupDirectories(); // SetupGameDirectories
	void ParseDirectoryFromCmd(const char *pCmdName, char *pDirName, const char *pDefault);
	
	char msBaseDir[512];
	char com_gamedir[MAX_PATH]; // MAX_OSPATH
	
	IFileSystem *mpFileSystem{nullptr}; // pimpl
};