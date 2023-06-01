#ifndef OS_UI_HPP
#define OS_UI_HPP

// ============================================================================

#include "ostree.hpp"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Button.H>

// ============================================================================

class MainWindow : public Fl_Window {
    public:
        MainWindow(OsTree *root);
        ~MainWindow();
        // ====================================================================
    private:
        int width;
        int height;
        int wWidth;
        int wHeight;
        int wIndent;
        Fl_Choice *ldrPartList;
        Fl_Choice *sysPartList;
        Fl_Button *runButton;
        class OsSelectItem : public Fl_Choice {
            public:
                OsTree *osTree;
                OsSelectItem *next;
                OsSelectItem(int X, int Y, int W, int H, 
                        const char *L, OsTree *ost);
                ~OsSelectItem();
                MainWindow *window();
                void onChange();
        };
        OsSelectItem *osSelector;
        // ====================================================================
        OsSelectItem *addOsSelectItem(OsTree *parent);
        void movingWidgets();
        // ====================================================================
        static void osSelect_callback(Fl_Widget *widget, void *);
        static void runButton_callback(Fl_Widget *widget, void *data);
};

// ============================================================================

#endif
