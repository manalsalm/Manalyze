#ifndef  _NT_VALUES_H_
# define _NT_VALUES_H_

#include <string>
#include <map>
#include <vector>
#include <boost/assign.hpp>

// Directory Entries - copied from WinNT.h
// There is no need for a map for this one: we won't have to translate the values back to their names.
#define IMAGE_DIRECTORY_ENTRY_EXPORT          0   // Export Directory
#define IMAGE_DIRECTORY_ENTRY_IMPORT          1   // Import Directory
#define IMAGE_DIRECTORY_ENTRY_RESOURCE        2   // Resource Directory
#define IMAGE_DIRECTORY_ENTRY_EXCEPTION       3   // Exception Directory
#define IMAGE_DIRECTORY_ENTRY_SECURITY        4   // Security Directory
#define IMAGE_DIRECTORY_ENTRY_BASERELOC       5   // Base Relocation Table
#define IMAGE_DIRECTORY_ENTRY_DEBUG           6   // Debug Directory
#define IMAGE_DIRECTORY_ENTRY_COPYRIGHT       7   // (X86 usage)
#define IMAGE_DIRECTORY_ENTRY_ARCHITECTURE    7   // Architecture Specific Data
#define IMAGE_DIRECTORY_ENTRY_GLOBALPTR       8   // RVA of GP
#define IMAGE_DIRECTORY_ENTRY_TLS             9   // TLS Directory
#define IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG    10   // Load Configuration Directory
#define IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT   11   // Bound Import Directory in headers
#define IMAGE_DIRECTORY_ENTRY_IAT            12   // Import Address Table
#define IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT   13   // Delay Load Import Descriptors
#define IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR 14   // COM Runtime descriptor

namespace nt {

typedef std::map<std::string, int> flag_dict;

static flag_dict PE_CHARACTERISTICS = 
	boost::assign::map_list_of ("IMAGE_FILE_RELOCS_STRIPPED",			0x0001) 
							   ("IMAGE_FILE_EXECUTABLE_IMAGE",			0x0002)
							   ("IMAGE_FILE_LINE_NUMS_STRIPPED",		0x0004)
							   ("IMAGE_FILE_LOCAL_SYMS_STRIPPED",		0x0008)
							   ("IMAGE_FILE_AGGRESIVE_WS_TRIM",			0x0010)
							   ("IMAGE_FILE_LARGE_ADDRESS_AWARE",		0x0020)
							   ("IMAGE_FILE_BYTES_REVERSED_LO",			0x0080)
							   ("IMAGE_FILE_32BIT_MACHINE",				0x0100)
							   ("IMAGE_FILE_DEBUG_STRIPPED",			0x0200)
							   ("IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP",	0x0400)
							   ("IMAGE_FILE_NET_RUN_FROM_SWAP",			0x0800)
							   ("IMAGE_FILE_SYSTEM",					0x1000)
							   ("IMAGE_FILE_DLL",						0x2000)
							   ("IMAGE_FILE_UP_SYSTEM_ONLY",			0x4000)
							   ("IMAGE_FILE_BYTES_REVERSED_HI",			0x8000);

static flag_dict MACHINE_TYPES =
	boost::assign::map_list_of ("IMAGE_FILE_MACHINE_UNKNOWN",			0)
							   ("IMAGE_FILE_MACHINE_I386",				0x014c)
							   ("IMAGE_FILE_MACHINE_R3000",				0x0162)
							   ("IMAGE_FILE_MACHINE_R4000",				0x0166)
							   ("IMAGE_FILE_MACHINE_R10000",			0x0168)
							   ("IMAGE_FILE_MACHINE_WCEMIPSV2",			0x0169)
							   ("IMAGE_FILE_MACHINE_ALPHA",				0x0184)
							   ("IMAGE_FILE_MACHINE_SH3",				0x01a2)
							   ("IMAGE_FILE_MACHINE_SH3DSP",			0x01a3)
							   ("IMAGE_FILE_MACHINE_SH3E",				0x01a4)
							   ("IMAGE_FILE_MACHINE_SH4",				0x01a6)
							   ("IMAGE_FILE_MACHINE_SH5",				0x01a8)
							   ("IMAGE_FILE_MACHINE_ARM",				0x01c0)
							   ("IMAGE_FILE_MACHINE_THUMB",				0x01c2)
							   ("IMAGE_FILE_MACHINE_AM33",				0x01d3)
							   ("IMAGE_FILE_MACHINE_POWERPC",			0x01F0)
							   ("IMAGE_FILE_MACHINE_POWERPCFP",			0x01f1)
							   ("IMAGE_FILE_MACHINE_IA64",				0x0200)
							   ("IMAGE_FILE_MACHINE_MIPS16",			0x0266)
							   ("IMAGE_FILE_MACHINE_ALPHA64",			0x0284)
							   ("IMAGE_FILE_MACHINE_MIPSFPU",			0x0366)
							   ("IMAGE_FILE_MACHINE_MIPSFPU16",			0x0466)
							   ("IMAGE_FILE_MACHINE_TRICORE",			0x0520)
							   ("IMAGE_FILE_MACHINE_CEF",				0x0CEF)
							   ("IMAGE_FILE_MACHINE_EBC",				0x0EBC)
							   ("IMAGE_FILE_MACHINE_AMD64",				0x8664)
							   ("IMAGE_FILE_MACHINE_M32R",				0x9041)
							   ("IMAGE_FILE_MACHINE_CEE",				0xC0EE);

static flag_dict IMAGE_OPTIONAL_HEADER_MAGIC =
	boost::assign::map_list_of ("PE32",		0x10b)
							   ("PE32+",	0x20b);

static flag_dict SUBSYSTEMS =
	boost::assign::map_list_of ("IMAGE_SUBSYSTEM_UNKNOWN", 0)
							   ("IMAGE_SUBSYSTEM_NATIVE", 1)
							   ("IMAGE_SUBSYSTEM_WINDOWS_GUI", 2)
							   ("IMAGE_SUBSYSTEM_WINDOWS_CUI", 3)
							   ("IMAGE_SUBSYSTEM_POSIX_CUI", 7)
							   ("IMAGE_SUBSYSTEM_NATIVE_WINDOWS", 8)
							   ("IMAGE_SUBSYSTEM_WINDOWS_CE_GUI", 9)
							   ("IMAGE_SUBSYSTEM_EFI_APPLICATION", 10)
							   ("IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER", 11)
							   ("IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER", 12)
							   ("IMAGE_SUBSYSTEM_EFI_ROM", 13)
							   ("IMAGE_SUBSYSTEM_XBOX", 14)
							   ("IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION", 16);

static flag_dict DLL_CHARACTERISTICS =
	boost::assign::map_list_of ("IMAGE_LIBRARY_PROCESS_INIT", 0x0001)
							   ("IMAGE_LIBRARY_PROCESS_TERM", 0x0002)
							   ("IMAGE_LIBRARY_THREAD_INIT", 0x0004)
							   ("IMAGE_LIBRARY_THREAD_TERM", 0x0008)
							   ("IMAGE_DLLCHARACTERISTICS_HIGH_ENTROPY_VA", 0x0020)
							   ("IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE", 0x0040)
							   ("IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY", 0x0080)
							   ("IMAGE_DLLCHARACTERISTICS_NX_COMPAT", 0x0100)
							   ("IMAGE_DLLCHARACTERISTICS_NO_ISOLATION", 0x0200)
							   ("IMAGE_DLLCHARACTERISTICS_NO_SEH", 0x0400)
							   ("IMAGE_DLLCHARACTERISTICS_NO_BIND", 0x0800)
							   ("IMAGE_DLLCHARACTERISTICS_APPCONTAINER", 0x1000)
							   ("IMAGE_DLLCHARACTERISTICS_WDM_DRIVER", 0x2000)
							   ("IMAGE_DLLCHARACTERISTICS_GUARD_CF", 0x4000)
							   ("IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE", 0x8000);

static flag_dict SECTION_CHARACTERISTICS =
	boost::assign::map_list_of ("IMAGE_SCN_TYPE_REG", 0x00000000)
							   ("IMAGE_SCN_TYPE_DSECT", 0x00000001)
							   ("IMAGE_SCN_TYPE_NOLOAD", 0x00000002)
							   ("IMAGE_SCN_TYPE_GROUP", 0x00000004)
							   ("IMAGE_SCN_TYPE_NO_PAD", 0x00000008)
							   ("IMAGE_SCN_TYPE_COPY", 0x00000010)
							   ("IMAGE_SCN_CNT_CODE", 0x00000020)
							   ("IMAGE_SCN_CNT_INITIALIZED_DATA", 0x00000040)
							   ("IMAGE_SCN_CNT_UNINITIALIZED_DATA", 0x00000080)
							   ("IMAGE_SCN_LNK_OTHER", 0x00000100)
							   ("IMAGE_SCN_LNK_INFO", 0x00000200)
							   ("IMAGE_SCN_TYPE_OVER", 0x00000400)
							   ("IMAGE_SCN_LNK_REMOVE", 0x00000800)
							   ("IMAGE_SCN_LNK_COMDAT", 0x00001000)
							   ("IMAGE_SCN_NO_DEFER_SPEC_EXC", 0x00004000)
							   ("IMAGE_SCN_GPREL", 0x00008000)
							   ("IMAGE_SCN_MEM_FARDATA", 0x00008000)
							   ("IMAGE_SCN_MEM_PURGEABLE", 0x00020000)
							   ("IMAGE_SCN_MEM_LOCKED", 0x00040000)
							   ("IMAGE_SCN_MEM_PRELOAD", 0x00080000)
							   ("IMAGE_SCN_ALIGN_MASK", 0x00)
							   ("IMAGE_SCN_LNK_NRELOC_OVFL", 0x01000000)
							   ("IMAGE_SCN_MEM_DISCARDABLE", 0x02000000)
							   ("IMAGE_SCN_MEM_NOT_CACHED", 0x04000000)
							   ("IMAGE_SCN_MEM_NOT_PAGED", 0x08000000)
							   ("IMAGE_SCN_MEM_SHARED", 0x10000000)
							   ("IMAGE_SCN_MEM_EXECUTE", 0x20000000)
							   ("IMAGE_SCN_MEM_READ", 0x40000000)
							   ("IMAGE_SCN_MEM_WRITE", 0x80000000);

/**
 *	@brief	Breaks down an integer given as input as a combination of flags.
 *
 *	@param	int value The integer to translate
 *	@param	flag_dict& dict A map containing the list of available flags and corresponding
 *			integer values.
 *
 *	@return	A list of matching flags.
 */
std::vector<std::string> translate_to_flags(int value, const flag_dict& dict);

/**
 *	@brief	Looks up the flag corresponding to a given value, if any.
 *
 *	@param	int value The integer to translate
 *	@param	flag_dict& dict A map containing the list of available flags and corresponding
 *			integer values.
 *
 *	@return	The corresponding flag, or "UNKNOWN" if no match is found.
 */
std::string translate_to_flag(int value, const flag_dict& dict);

} // !namespace nt

#endif