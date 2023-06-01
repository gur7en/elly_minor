#include "ostree.hpp"

#include <stdio.h>

// ============================================================================

const int BASE_INDENT = 4;

// ============================================================================

OsTree::OsTree()
{
    iName = "";
    iPath = "";
    iIndex = 0;
    iPStyle = Undefined;
    iParent = NULL;
    iChildHead = NULL;
    iChildLast = NULL;
    iChildCurrent = NULL;
}

OsTree::OsTree(OsTree *parent, const std::string &name, 
        const std::string &part_style, const std::string &path, int index)
{
    iName = name;
    iPath = path;
    iIndex = index;
    iParent = parent;
    iChildHead = NULL;
    iChildLast = NULL;
    iChildCurrent = NULL;
    iPStyle = Undefined;
    convPartStyleString(part_style);
    if(iPStyle == Undefined) {
        iPStyle = iParent->iPStyle;
    }
}

OsTree::ChildItem::ChildItem(OsTree *osTree)
{
    iOsTree = osTree;
}

OsTree::ChildItem::~ChildItem()
{
    delete iOsTree;
    if(next) {
        delete next;
    }
}

void OsTree::convPartStyleString(const std::string &pstyle)
{
    const char *c = pstyle.c_str();
    for(; *c; ++c) {
        if(*c == 'r' ||  *c == 'R') {
            iPStyle |= RAW;
        } else if(*c == 'm' || *c =='M') {
            iPStyle |= MBR;
        } else if(*c == 'g' || *c == 'G') {
            iPStyle |= GPT;
        }
    }
}

OsTree *OsTree::findChild(const std::string &name)
{
    for(ChildItem *cur = iChildHead; cur; cur = cur->next) {
        if(cur->iOsTree->getName() == name) {
            return cur->iOsTree;
        }
    }
    return NULL;
}

OsTree *OsTree::firstChild()
{
    if(iChildHead) {
        iChildCurrent = iChildHead;
        return iChildHead->iOsTree;
    }
    return NULL;
}

OsTree *OsTree::nextChild()
{
    if(iChildCurrent) {
        if(iChildCurrent->next) {
            iChildCurrent = iChildCurrent->next;
            return iChildCurrent->iOsTree;
        }
        return NULL;
    }
    return firstChild();
}

OsTree *OsTree::curChild()
{
    if(iChildCurrent) {
        return iChildCurrent->iOsTree;
    }
    return NULL;
}

OsTree *OsTree::addChild(const std::string &name, const std::string &path,
        const std::string &part_style, int index)
{
    OsTree *child = findChild(name);
    if(child) {
        child->iPath = path;
        child->iIndex = index;
        child->convPartStyleString(part_style);
    } else {
        child = new OsTree(this, name, part_style, path, index);
        ChildItem *item = new ChildItem(child);
        if(iChildLast) {
            iChildLast->next = item;
            iChildLast = iChildLast->next;
        } else {
            iChildLast = item;
            iChildHead = iChildLast;
        }
    }
    return child;
}

std::string OsTree::getName()
{
    return iName;
}

int OsTree::getIndex()
{
    return iIndex;
}

bool OsTree::supportRAW()
{
   return iPStyle & RAW;
}

bool OsTree::supportMBR()
{
   return iPStyle & MBR;
}

bool OsTree::supportGPT()
{
   return iPStyle & GPT;
}

std::string OsTree::getPath()
{
    std::string path;
    if(iParent) {
        path += iParent->getPath();
    }
    path += iPath;
    return path;
}

void OsTree::print()
{
    print(0);
}

void OsTree::print(int indent)
{
    printf("%*c%s : %i : %s : %i\n", indent, 0,
            iName.c_str(), iPStyle, iPath.c_str(), iIndex);
    for(ChildItem *cur = iChildHead; cur; cur = cur->next) {
        cur->iOsTree->print(indent + BASE_INDENT);
    }
}

