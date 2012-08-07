#include <nall/platform.hpp>
#include <nall/directory.hpp>
#include <nall/file.hpp>
#include <nall/string.hpp>
#include <nall/vector.hpp>
#include <nall/windows/registry.hpp>
using namespace nall;

#define INITGUID
#include <initguid.h>
#include <shlguid.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <strsafe.h>
#include <list>
#include <string>
#define IDM_CFOPEN 0

HINSTANCE module = NULL;
unsigned referenceCount = 0;

#include "guid.hpp"
#include "settings.hpp"
#include "extension.hpp"
#include "factory.hpp"
