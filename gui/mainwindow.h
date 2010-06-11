
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define IO_ReadOnly QIODevice::ReadOnly

#include <QtGui>
#include <string>
#include <QtGui/QMainWindow>
#include <QtGui/QMessageBox>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QTreeView>
#include <QtGui/QSplitter>
#include <QtGui/QLayout>
#include <QtGui/QStatusBar>
#include <qstring.h>
#include <string>

#include "highlighter.h"
#include "codeeditor.h"
#include "svgwindow.h"


using namespace std;


namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void about();
    void show_how_to();
    void display_splitter();
    void display_open_dialog();
    void display_save_dialog();

    void run_trucov();
    void display_source_dialog();
    void display_build_dialog();

    void initialize_explorer(); //input should be project dir
    void update_svgwindow(const QString &filename);
    void update_source_window(const QString &filename, int line_no);

    void update_show_brief();
    void update_only_missing();
    void update_fake_block();
    void update_demangled_name();
    void update_fit_to_window();

    void tree_item_clicked(QModelIndex);

private:
    Ui::MainWindow *ui;
    QStandardItemModel *model;

    QDirModel *dir_model; // = new QDirModel;

    QTreeView *src_tree;
    QTabWidget *proj_explorer;
    SvgWindow *editor2;
    QPlainTextEdit *editor3;

    QTextCursor *cursor;
    QSplitter *splitter; //= new QSplitter(Qt::Horizontal);
    //status bar idea
    QStatusBar *mainStatusBar;

    void create_action();
    void populate_tree();
    string execute_pipe(string command_name, string command_argument);

    QAction *actionAbout_Trucov;
    QAction *actionHow_To;
    QAction *actionOpen;
    QAction *actionRun;

    QAction *actionProjectDir;
    QAction *actionSourceDir;
    QAction *actionBuildDir;

    QAction *actionBrief;
    QAction *actionOnly_missing;
    QAction *actionFake_block;
    QAction *actionDemangled_name;
    QAction *actionFit_To_Window;

    Highlighter *highlighter;

    QString project_dir;
    QVector< QString > source_dir_list;
    QVector< QString > build_dir_list;
    bool show_brief;
    bool show_only_missing;
    bool show_fake_block;
    bool show_demangled_name;

    bool fit_to_window;
};

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <assert.h>

struct Function;

struct Source_file
{
   std::string percentage;
   std::string name; // Use this as the display
   std::string path;
   std::vector<Function> functions;
};

struct Function
{
   std::string percentage;
   int lineno;
   std::string name; // Use this as the display
   std::string report_path;
   std::string graph_path;
   std::string source_path;
};

bool read_secret_gui(std::vector<Source_file> & sources);

Function * find_function(
   const std::string & src_fun,
   std::vector<Source_file> & sources );

const std::string get_filename( const std::string & pathname );

#endif // MAINWINDOW_H
