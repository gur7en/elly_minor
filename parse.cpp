#include "parse.hpp"

// ============================================================================

void read_os_list(OsTree &root, const std::string &file_name) {
    FILE *file = fopen(file_name.c_str(), "r");
    while(!feof(file)) {
        read_os_list_line(file, &root, 0);
    }
}

int read_os_list_line(FILE *in, OsTree *cur, int deep)
{
    std::string name;
    std::string part_style;
    std::string path;
    int index = 1;
    if(read_os_list_elem(in, name)) {
        if(read_os_list_elem(in, part_style)) {
            if(read_os_list_elem(in, path)) {
                std::string tmp;
                if(read_os_list_elem(in, tmp)) {
                    while(fgetc(in) != '\n')
                    {}
                }
                index = stoi(tmp);
            }
        }
    }
    if(feof(in)) {
        return 0;
    }
    OsTree *child = cur->addChild(name, path, part_style, index);
    int next_deep = 0;
    int c;
    while(isspace(c = fgetc(in))) {
        if(c != '\n') {
            ++next_deep;
        }
    }
    ungetc(c, in);
    while(next_deep > deep) {
        next_deep = read_os_list_line(in, child, next_deep);
    }
    return next_deep;
}

bool read_os_list_elem(FILE *in, std::string &out)
{
    if(feof(in)) {
        return false;
    }
    int c;
    while(isspace(c = fgetc(in)))
    {}
    while(c != ':' && c != '\n' && c != EOF) {
        out += c;
        c = fgetc(in);
    }
    if(!out.empty()) {
        while(isspace(out[out.size() - 1])) {
            out.pop_back();
        }
    }
    if(c == '\n') {
        return false;
    }
    return true;
}

