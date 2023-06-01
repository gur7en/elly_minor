#include <wimlib.h>

#include "ostree.hpp"
#include "parse.hpp"
#include "ui.hpp"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

// ============================================================================

const char *OS_LIST_FILE = "os.lst";
const char *FL_SCHEME = "gleam";
const char *PREC_READ_OS_SCRIPT = "1_start.lua";
const char *SETUP_SCRIPT = "2_setup.lua";

// ============================================================================

int lua_wimapply(lua_State *lua);
wchar_t *readUtf16(lua_State *vml, int index);

// ============================================================================

int main()
{
    Fl::scheme(FL_SCHEME);
    OsTree ostr;
    lua_State *vml = luaL_newstate();
    lua_register(vml, "wimapply", lua_wimapply);
    luaL_openlibs(vml);
    luaL_dofile(vml, PREC_READ_OS_SCRIPT);
    read_os_list(ostr, OS_LIST_FILE);
    MainWindow mw(&ostr);
    return Fl::run();
}

int lua_wimapply(lua_State *vml)
{
    const char *file = lua_tostring(vml, 1);
//  const wchar_t *file = readUtf16(vml, 1);
    int index = lua_tonumber(vml, 2);
    const char *target = lua_tostring(vml, 3);
//  const wchar_t *target = readUtf16(vml, 1);
    lua_pop(vml, 3);
//    WIMStruct *wim;
//    int error = wimlib_open_wim(file, WIMLIB_OPEN_FLAG_CHECK_INTEGRITY, &wim);
//    if(!error) {
//        error = wimlib_extract_image(wim, index, target, 0);
//        wimlib_free(wim);
//    }
//    lua_pushinteger(vml, error);
    printf("%s\n%i\n%s\n", file, index, target);
    lua_pushinteger(vml, 7);
    return 1;
}

wchar_t *readUtf16(lua_State *vml, int number)
{
    unsigned short *result = NULL;
    const char *src = lua_tostring(vml, number);
    int srclen = strlen(src);
    int reslen = fl_utf8toUtf16(src, srclen, result, 0);
    result = (unsigned short*)calloc(reslen + 1, sizeof(unsigned short));
    fl_utf8toUtf16(src, srclen, result, reslen);
    return (wchar_t*)result;
}
