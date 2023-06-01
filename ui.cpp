#include "ui.hpp"

// ============================================================================

const int WIN_WIDTH_BASE = 360;
const int WIDGET_WIDTH_BASE = 90;
const int WIDGET_HEIGHT_BASE = 30;
const int WIDGET_INDENT_BASE = 5;
const char *WIN_TITLE = "EllyMinor";
const char *LABEL_RUN_BUTTON = "Install";
const char *LABEL_LDR_DRV_MENU = "Loader: ";
const char *LABEL_SYS_DRV_MENU = "System: ";

// ============================================================================

MainWindow::MainWindow(OsTree *root) 
    : Fl_Window(WIN_WIDTH_BASE, WIDGET_INDENT_BASE, WIN_TITLE) 
{
    width = WIN_WIDTH_BASE;
    height = WIDGET_INDENT_BASE;
    wWidth = WIDGET_WIDTH_BASE;
    wHeight = WIDGET_HEIGHT_BASE;
    wIndent = WIDGET_INDENT_BASE;
    begin();
    ldrPartList = new Fl_Choice(0, 0, 0, 0, LABEL_LDR_DRV_MENU);
    sysPartList = new Fl_Choice(0, 0, 0, 0, LABEL_SYS_DRV_MENU);
    runButton = new Fl_Button(0, 0, 0, 0, LABEL_RUN_BUTTON);
    runButton->callback(runButton_callback);
    osSelector = addOsSelectItem(root);
    movingWidgets();
    end();
    show();
}

MainWindow::~MainWindow()
{
    Fl::delete_widget(runButton);
    Fl::delete_widget(ldrPartList);
    Fl::delete_widget(sysPartList);
    delete osSelector;
}

MainWindow::OsSelectItem::OsSelectItem(int X, int Y, int W, int H, 
        const char *L, OsTree *parent) 
    : Fl_Choice(X, Y, W, H, L)
{
    osTree = parent;
    callback(osSelect_callback);
    for(OsTree *cur = osTree->firstChild(); cur; cur = osTree->nextChild()) {
        add(cur->getName().c_str());
    }
    value(0);
}

MainWindow::OsSelectItem::~OsSelectItem()
{
    if(next) {
        delete next;
    }
}

MainWindow *MainWindow::OsSelectItem::window()
{
    return static_cast<MainWindow*>(Fl_Choice::window());
}

MainWindow::OsSelectItem *MainWindow::addOsSelectItem(OsTree *parent)
{
    OsTree *child = parent->firstChild();
    if(child) {
        OsSelectItem *item = new OsSelectItem(0, 0, 0, 0, 0, parent);
        item->next = addOsSelectItem(child);
        return item;
    } else {
        runButton->user_data(parent);
        return NULL;
    }
}

void MainWindow::movingWidgets()
{
    int bottom = wIndent;
    for(OsSelectItem *cur = osSelector; cur; cur = cur->next) {
        cur->resize(wIndent, bottom, width - 2*wIndent, wHeight);
        bottom += wHeight + wIndent;
    }
    runButton->resize(width - wIndent - wWidth, bottom, 
            wWidth, 2*wHeight + wIndent);
    ldrPartList->resize(wWidth, bottom, wWidth*1.5, wHeight);
    bottom += wHeight + wIndent;
    sysPartList->resize(wWidth, bottom, wWidth*1.5, wHeight);
    bottom += wHeight + wIndent;
    height = bottom;
    size(width, height);
    redraw();
}

void MainWindow::OsSelectItem::onChange()
{
    if(next) {
        delete next;
        next = NULL;
    }
    MainWindow *mw = window();
    mw->begin();
    OsTree *child = osTree->findChild(text());
    next = mw->addOsSelectItem(child);
    mw->end();
    mw->movingWidgets();
}

void MainWindow::osSelect_callback(Fl_Widget *widget, void *)
{
    static_cast<OsSelectItem*>(widget)->onChange();
}

void MainWindow::runButton_callback(Fl_Widget *widget, void *data)
{
    OsTree *tree = static_cast<OsTree*>(data);
    std::string ps;
    if(tree->supportRAW()) {
        ps += "R";
    }
    if(tree->supportMBR()) {
        ps += "M";
    }
    if(tree->supportGPT()) {
        ps += "G";
    }
    printf("%s : %i : %s\n", tree->getPath().c_str(), 
            tree->getIndex(), ps.c_str());
}
