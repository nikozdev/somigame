/* headers */

#include "head.hxx"

/* defines */

#define _NAMESPACE_ENTER_FSYS namespace fsys {
#define _NAMESPACE_LEAVE_FSYS }

/* content */

_NAMESPACE_ENTER
_NAMESPACE_ENTER_FSYS

namespace fstd = std::experimental::filesystem;

using fpaf_t = fstd::path;
using fdir_info_t = fstd::directory_entry;
using fdir_iter_t = fstd::directory_iterator;
using size_info_t = fstd::space_info;

/** typedef **/

/** datadef **/

/** actions **/

extern void fsys_init();

_NAMESPACE_LEAVE_FSYS
_NAMESPACE_LEAVE
