#ifndef PARSE_HPP
#define PARSE_HPP

// ============================================================================

#include "ostree.hpp"

#include <string>

// ============================================================================

void read_os_list(OsTree &root, const std::string &file_name);
int read_os_list_line(FILE *in, OsTree *cur, int deep);
bool read_os_list_elem(FILE *in, std::string &out);

// ============================================================================

#endif
