//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef FILE_H
#define FILE_H

#ifdef FILE_USE_DLL
#ifdef FILE_LIBRARY_EXPORTS
#define FILE_LIBRARY_API __declspec(dllexport)
#else
#define FILE_LIBRARY_API __declspec(dllimport)
#endif
#else
#define FILE_LIBRARY_API
#endif

// Make the assumption of c-char strings, not UNICODE
namespace Azul
{
	class File
	{
	public:

		// Constants for the library
		static const uint32_t MAJOR_VERSION = 4;
		static const uint32_t MINOR_VERSION = 1;
		// maximum path in Windows 10/11
		static const uint32_t BASE_PATH_SIZE = MAX_PATH;

		typedef void* Handle;

		enum class Mode : uint32_t
		{
			READ,
			WRITE,
			READ_WRITE
		};

		enum class Position : uint32_t
		{
			BEGIN,
			CURRENT,
			END
		};

		enum class Error : uint32_t
		{
			SUCCESS,
			BASE_PATH_FAIL,
			BASE_PATH_NULLPTR_FAIL,
			BASE_PATH_INSUFFICIENT_SPACE_FAIL,
			LOCAL_WORKING_DIR_FAIL,
			LOCAL_WORKING_DIR_NULLPTR_FAIL,
			LOCAL_WORKING_DIR_INSUFFICIENT_SPACE_FAIL,
			OPEN_FAIL,
			OPEN_FILENAME_FAIL,
			OPEN_BASE_PATH_FAIL,
			CLOSE_FAIL,
			WRITE_FAIL,
			READ_FAIL,
			SEEK_FAIL,
			TELL_FAIL,
			FLUSH_FAIL,
			UNDEFINED
		};

	public:
		// GetLocalWorkingDir()
		//   returns Visual Studio's working directory
		//   Open() uses this as the default working directory
		FILE_LIBRARY_API static File::Error GetLocalWorkingDir(char *const pDirOut, size_t DirOutSize) noexcept;
		FILE_LIBRARY_API static File::Error GetBasePath(char * const pDirOut, size_t DirOutSize) noexcept;
		
		// SetBasePath()
		//     BasePath - path that is appended to file path for File Open
		//     ClearBaseDir() - uses the LocalWorkingDirectory (default)
		//                    - same as the one set in Visual Studio Settings
		//                            Properties-->Debugging-->Working Directory
		//     SetBasePath() - absolute path example:   "C:\\Code\\Engine\Temp\\"
		//                   - relative path example:   ".\\Temp\\"
		FILE_LIBRARY_API static File::Error SetBasePath(const char *const pDir) noexcept;
		FILE_LIBRARY_API static File::Error ClearBasePath() noexcept;

		// Open()
		//     - Specify the filename string with or without path
		//           - absolute path example:   "C:\\Code\\Engine\Temp\\Dog.data"
        //           - relative path example:   ".\\Temp\\Dog.data"
		//           - uses LocalWorkingDir :   "Dog.data"
		//     - UseBasePath set true --> appends path to filename string
		FILE_LIBRARY_API static File::Error Open(File::Handle &fh, const char* const fileName, File::Mode mode, bool UseBasePath = false) noexcept;
		FILE_LIBRARY_API static File::Error Close(File::Handle &fh) noexcept;
		FILE_LIBRARY_API static File::Error Write(File::Handle fh, const void* const buffer, const DWORD inSize) noexcept;
		FILE_LIBRARY_API static File::Error Read(File::Handle fh, void* const _buffer, const DWORD _size) noexcept;
		FILE_LIBRARY_API static File::Error Seek(File::Handle fh, File::Position location, int offset) noexcept;
		FILE_LIBRARY_API static File::Error Tell(File::Handle fh, DWORD& offset) noexcept;
		FILE_LIBRARY_API static File::Error Flush(File::Handle fh) noexcept;
		FILE_LIBRARY_API static bool IsHandleValid(File::Handle fh) noexcept;
		FILE_LIBRARY_API static File::Error Magic(unsigned int &number) noexcept;

	private:
		FILE_LIBRARY_API static DWORD privGetFileDesiredAccess(File::Mode mode) noexcept;
		FILE_LIBRARY_API static DWORD privGetSeek(File::Position location) noexcept;
		FILE_LIBRARY_API static File::Error privCopyToBaseDir(const char * const pDir);
	};

}

#endif

// --- End of File ---
