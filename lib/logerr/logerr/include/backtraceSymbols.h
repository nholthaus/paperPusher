// attribution: https://oroboro.com/printing-stack-traces-file-line/

#ifdef Q_OS_WIN
#error "this file should only be used on linux"
#endif

// include all the headers you're going to need.
#include <bfd.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dlfcn.h>
#include <execinfo.h>
#include <link.h>

#include <string>

class FileMatch
{
public:
	FileMatch(void* addr)
	    : mAddress(addr)
	    , mFile(nullptr)
	    , mBase(nullptr)
	{
	}

	void*       mAddress;
	const char* mFile;
	void*       mBase;
};

class FileLineDesc
{
public:
	FileLineDesc(asymbol** syms, bfd_vma pc)
	    : mPc(pc)
	    , mFound(false)
	    , mSyms(syms)
	{
	}

	void findAddressInSection(bfd* abfd, asection* section);

	bfd_vma      mPc;
	std::string  mFilename;
	std::string  mFunctionname;
	unsigned int mLine;
	int          mFound;
	asymbol**    mSyms;
};

static int       findMatchingFile(struct dl_phdr_info* info, size_t size, void* data);
static asymbol** kstSlurpSymtab(bfd* abfd, const char* fileName);
static char**    translateAddressesBuf(bfd* abfd, bfd_vma* addr, int numAddr, asymbol** syms);
static char**    processFile(const char* fileName, bfd_vma* addr, int naddr);
static void      findAddressInSection(bfd* abfd, asection* section, void* data);
char**           backtraceSymbols(void* const* addrList, int numAddr);