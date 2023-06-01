#ifndef OS_TREE_HPP
#define OS_TREE_HPP

// ============================================================================

#include <string>
#include <list>

// ============================================================================

class OsTree {
    public:
        OsTree();
        OsTree(OsTree *parent, const std::string &name, 
                const std::string &part_style, 
                const std::string &path, int index);
        // ====================================================================
        OsTree *firstChild();
        OsTree *nextChild();
        OsTree *curChild();
        OsTree *findChild(const std::string &name);
        OsTree *addChild(const std::string &name, const std::string &path,
                const std::string &part_style, int index);
        std::string getPath();
        std::string getName();
        int getIndex();
        int getPartStyle();
        bool supportRAW();
        bool supportMBR();
        bool supportGPT();
        void print();
    private:
        std::string iName;
        std::string iPath;
        int iIndex;
        OsTree *iParent;
        // ====================================================================
        struct ChildItem {
            OsTree *iOsTree;
            ChildItem *next;
            ChildItem(OsTree *osTree);
            ~ChildItem();
        };
        ChildItem *iChildHead;
        ChildItem *iChildLast;
        ChildItem *iChildCurrent;
        // ====================================================================
        enum {
            Undefined = 0,
            RAW = 1 << 0,
            MBR = 1 << 1,
            GPT = 1 << 2,
        };
        int iPStyle;
        // ====================================================================
        void print(int indent);
        void convPartStyleString(const std::string &pstyle);

};

// ============================================================================

#endif
