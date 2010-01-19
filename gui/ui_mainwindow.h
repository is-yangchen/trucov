/********************************************************************************
** Form generated from reading ui file 'mainwindow.ui'
**
** Created: Wed Dec 9 18:48:22 2009
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QAction *actionSave_Project;
    QAction *actionSave_Project_As;
    QAction *actionSave_Report;
    QAction *actionSave_Report_As;
    QAction *actionSave_All_Report;
    QAction *actionSave_Graph;
    QAction *actionSave_Graph_As;
    QAction *actionExit;
    QAction *actionFind;
    QAction *actionFind_Again;
    QAction *actionControl_Flow;
    QAction *actionSource_Code;
    QAction *actionProject_Explorer;
    QAction *actionHow_To;
    QAction *actionAbout_Trucov;
    QAction *actionShow_Brief_Only;
    QAction *actionShow_Only_Missing;
    QAction *actionShow_Fake_Blocks_and_Arcs;
    QAction *actionShow_Demangle_Names;
    QAction *actionSource_Directories;
    QAction *actionBuild_Directories;
    QAction *actionNew;
    QAction *actionRun;
    QAction *actionFit_To_Window;
    QAction *actionProject_Directory;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuView;
    QMenu *menuOptions;
    QMenu *menuHelp;
    QMenu *menuRun;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1014, 696);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionSave_Project = new QAction(MainWindow);
        actionSave_Project->setObjectName(QString::fromUtf8("actionSave_Project"));
        actionSave_Project_As = new QAction(MainWindow);
        actionSave_Project_As->setObjectName(QString::fromUtf8("actionSave_Project_As"));
        actionSave_Report = new QAction(MainWindow);
        actionSave_Report->setObjectName(QString::fromUtf8("actionSave_Report"));
        actionSave_Report_As = new QAction(MainWindow);
        actionSave_Report_As->setObjectName(QString::fromUtf8("actionSave_Report_As"));
        actionSave_All_Report = new QAction(MainWindow);
        actionSave_All_Report->setObjectName(QString::fromUtf8("actionSave_All_Report"));
        actionSave_Graph = new QAction(MainWindow);
        actionSave_Graph->setObjectName(QString::fromUtf8("actionSave_Graph"));
        actionSave_Graph_As = new QAction(MainWindow);
        actionSave_Graph_As->setObjectName(QString::fromUtf8("actionSave_Graph_As"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionFind = new QAction(MainWindow);
        actionFind->setObjectName(QString::fromUtf8("actionFind"));
        actionFind_Again = new QAction(MainWindow);
        actionFind_Again->setObjectName(QString::fromUtf8("actionFind_Again"));
        actionControl_Flow = new QAction(MainWindow);
        actionControl_Flow->setObjectName(QString::fromUtf8("actionControl_Flow"));
        actionControl_Flow->setCheckable(true);
        actionSource_Code = new QAction(MainWindow);
        actionSource_Code->setObjectName(QString::fromUtf8("actionSource_Code"));
        actionSource_Code->setCheckable(true);
        actionProject_Explorer = new QAction(MainWindow);
        actionProject_Explorer->setObjectName(QString::fromUtf8("actionProject_Explorer"));
        actionProject_Explorer->setCheckable(true);
        actionHow_To = new QAction(MainWindow);
        actionHow_To->setObjectName(QString::fromUtf8("actionHow_To"));
        actionAbout_Trucov = new QAction(MainWindow);
        actionAbout_Trucov->setObjectName(QString::fromUtf8("actionAbout_Trucov"));
        actionShow_Brief_Only = new QAction(MainWindow);
        actionShow_Brief_Only->setObjectName(QString::fromUtf8("actionShow_Brief_Only"));
        actionShow_Brief_Only->setCheckable(true);
        actionShow_Only_Missing = new QAction(MainWindow);
        actionShow_Only_Missing->setObjectName(QString::fromUtf8("actionShow_Only_Missing"));
        actionShow_Only_Missing->setCheckable(true);
        actionShow_Fake_Blocks_and_Arcs = new QAction(MainWindow);
        actionShow_Fake_Blocks_and_Arcs->setObjectName(QString::fromUtf8("actionShow_Fake_Blocks_and_Arcs"));
        actionShow_Fake_Blocks_and_Arcs->setCheckable(true);
        actionShow_Demangle_Names = new QAction(MainWindow);
        actionShow_Demangle_Names->setObjectName(QString::fromUtf8("actionShow_Demangle_Names"));
        actionShow_Demangle_Names->setCheckable(true);
        actionSource_Directories = new QAction(MainWindow);
        actionSource_Directories->setObjectName(QString::fromUtf8("actionSource_Directories"));
        actionBuild_Directories = new QAction(MainWindow);
        actionBuild_Directories->setObjectName(QString::fromUtf8("actionBuild_Directories"));
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName(QString::fromUtf8("actionNew"));
        actionRun = new QAction(MainWindow);
        actionRun->setObjectName(QString::fromUtf8("actionRun"));
        actionFit_To_Window = new QAction(MainWindow);
        actionFit_To_Window->setObjectName(QString::fromUtf8("actionFit_To_Window"));
        actionFit_To_Window->setCheckable(true);
        actionProject_Directory = new QAction(MainWindow);
        actionProject_Directory->setObjectName(QString::fromUtf8("actionProject_Directory"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1014, 26));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        menuOptions = new QMenu(menuBar);
        menuOptions->setObjectName(QString::fromUtf8("menuOptions"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        menuRun = new QMenu(menuBar);
        menuRun->setObjectName(QString::fromUtf8("menuRun"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuOptions->menuAction());
        menuBar->addAction(menuRun->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuView->addSeparator();
        menuView->addAction(actionFit_To_Window);
        menuView->addSeparator();
        menuOptions->addAction(actionShow_Brief_Only);
        menuOptions->addAction(actionShow_Only_Missing);
        menuOptions->addAction(actionShow_Fake_Blocks_and_Arcs);
        menuOptions->addAction(actionShow_Demangle_Names);
        menuOptions->addSeparator();
        menuOptions->addAction(actionProject_Directory);
        menuOptions->addAction(actionSource_Directories);
        menuOptions->addAction(actionBuild_Directories);
        menuHelp->addAction(actionHow_To);
        menuHelp->addAction(actionAbout_Trucov);
        menuRun->addAction(actionRun);

        retranslateUi(MainWindow);
        QObject::connect(actionExit, SIGNAL(activated()), MainWindow, SLOT(close()));
        QObject::connect(actionAbout_Trucov, SIGNAL(activated()), MainWindow, SLOT(update()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", 0, QApplication::UnicodeUTF8));
        actionSave_Project->setText(QApplication::translate("MainWindow", "Save Project", 0, QApplication::UnicodeUTF8));
        actionSave_Project_As->setText(QApplication::translate("MainWindow", "Save Project As", 0, QApplication::UnicodeUTF8));
        actionSave_Report->setText(QApplication::translate("MainWindow", "Save Report", 0, QApplication::UnicodeUTF8));
        actionSave_Report_As->setText(QApplication::translate("MainWindow", "Save Report As", 0, QApplication::UnicodeUTF8));
        actionSave_All_Report->setText(QApplication::translate("MainWindow", "Save All Reports", 0, QApplication::UnicodeUTF8));
        actionSave_Graph->setText(QApplication::translate("MainWindow", "Save Graph", 0, QApplication::UnicodeUTF8));
        actionSave_Graph_As->setText(QApplication::translate("MainWindow", "Save Graph As", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0, QApplication::UnicodeUTF8));
        actionFind->setText(QApplication::translate("MainWindow", "Find", 0, QApplication::UnicodeUTF8));
        actionFind_Again->setText(QApplication::translate("MainWindow", "Find Again", 0, QApplication::UnicodeUTF8));
        actionControl_Flow->setText(QApplication::translate("MainWindow", "Control Flow", 0, QApplication::UnicodeUTF8));
        actionSource_Code->setText(QApplication::translate("MainWindow", "Source Code", 0, QApplication::UnicodeUTF8));
        actionProject_Explorer->setText(QApplication::translate("MainWindow", "Project Explorer", 0, QApplication::UnicodeUTF8));
        actionHow_To->setText(QApplication::translate("MainWindow", "How To", 0, QApplication::UnicodeUTF8));
        actionAbout_Trucov->setText(QApplication::translate("MainWindow", "About Trucov", 0, QApplication::UnicodeUTF8));
        actionShow_Brief_Only->setText(QApplication::translate("MainWindow", "Show Brief Only", 0, QApplication::UnicodeUTF8));
        actionShow_Only_Missing->setText(QApplication::translate("MainWindow", "Show Only Missing", 0, QApplication::UnicodeUTF8));
        actionShow_Fake_Blocks_and_Arcs->setText(QApplication::translate("MainWindow", "Show Fake Blocks and Arcs", 0, QApplication::UnicodeUTF8));
        actionShow_Demangle_Names->setText(QApplication::translate("MainWindow", "Show Demangled Names", 0, QApplication::UnicodeUTF8));
        actionSource_Directories->setText(QApplication::translate("MainWindow", "Source Directories", 0, QApplication::UnicodeUTF8));
        actionBuild_Directories->setText(QApplication::translate("MainWindow", "Build Directories", 0, QApplication::UnicodeUTF8));
        actionNew->setText(QApplication::translate("MainWindow", "New", 0, QApplication::UnicodeUTF8));
        actionRun->setText(QApplication::translate("MainWindow", "Run", 0, QApplication::UnicodeUTF8));
        actionFit_To_Window->setText(QApplication::translate("MainWindow", "Fit To Frame", 0, QApplication::UnicodeUTF8));
        actionProject_Directory->setText(QApplication::translate("MainWindow", "Project Directory", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        menuView->setTitle(QApplication::translate("MainWindow", "View", 0, QApplication::UnicodeUTF8));
        menuOptions->setTitle(QApplication::translate("MainWindow", "Options", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0, QApplication::UnicodeUTF8));
        menuRun->setTitle(QApplication::translate("MainWindow", "Project", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
