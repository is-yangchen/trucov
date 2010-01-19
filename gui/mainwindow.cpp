#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "proj_dia.h"

vector<Source_file> g_file_list;

bool read_secret_gui(std::vector<Source_file> & sources)
{
   sources.clear();
   std::string data;

   std::ifstream in(".__secret_gui__");
   if (!in.is_open())
   {
      assert(false);
      return false;
   }

   getline(in, data);
   while (data != "@@@__END_OF_EVERYTHING__@@@")
   {
      Source_file source;
      source.path = data;
      source.name = get_filename(source.path); 
      getline(in, source.percentage);
      getline(in, data);
      while (data != "@@@__END_OF_SOURCE__@@@")
      {
         Function fun;
         fun.name = data;
         fun.source_path = source.path;
   
         getline(in, fun.report_path);
         getline(in, fun.graph_path);
         getline(in, data);
         fun.lineno = atoi( data.c_str() );
         getline(in, fun.percentage);
         getline(in, data);
         source.functions.push_back(fun);
      }
      getline(in, data);
      sources.push_back(source);
   }

   return true;
}

// find the data structure
Function * find_function(
   const std::string & src_fun,
   std::vector<Source_file> & sources )
{
   unsigned int found = src_fun.find_first_of(" ");
   assert( found != std::string::npos );

   std::string src = src_fun.substr(0, found);
   std::string fun = src_fun.substr(found + 1, src_fun.size() - found);

   // find src
   for (unsigned int i = 0; i < sources.size(); i++)
   {
      if (sources[i].name == src)
      {
         for (unsigned int j = 0; j < sources[i].functions.size(); j++)
         {
            if (sources[i].functions[j].name == fun)
            {
               return &(sources[i].functions[j]);
            }
         }
      }
   }

   return NULL;
}

const std::string get_filename( const std::string & pathname )
{
   std::string temp;
   size_t found;

   temp = pathname;
   found = temp.find_last_of("/\\");

   if ( found == 0 )
   {
      return temp;
   }
   else
   {
      return temp.substr( found + 1 );
   }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    show_brief = false;
    show_only_missing = false;
    show_fake_block = false;
    show_demangled_name = false;
    fit_to_window = false;

    project_dir.clear();

    ui->setupUi(this);
    create_action();
    display_splitter();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::create_action()
{
    actionOpen = new QAction(tr("&Open"), this);
    actionOpen = ui->actionOpen;
    actionOpen->setShortcut(tr("Ctrl+O"));
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(display_open_dialog()));

    actionRun = new QAction(this);
    actionRun = ui->actionRun;
    actionRun->setShortcut(tr("Ctrl+F5"));
    connect(actionRun, SIGNAL(triggered()), this, SLOT(run_trucov()));

    actionAbout_Trucov = new QAction(tr("&About Trucov"), this);
    actionAbout_Trucov = ui->actionAbout_Trucov;
    actionAbout_Trucov->setShortcut(tr("Ctrl+T"));
    connect(actionAbout_Trucov, SIGNAL(triggered()), this, SLOT(about()));

    actionHow_To = new QAction(tr("&How TO"), this);
    actionHow_To = ui->actionHow_To;
    actionHow_To->setShortcut(tr("Ctrl+F1"));
    connect(actionHow_To, SIGNAL(triggered()), this, SLOT(show_how_to()));

    actionProjectDir = new QAction(tr("&Project Directory"), this);
    actionProjectDir = ui->actionProject_Directory;
    actionProjectDir->setShortcut(tr("Ctrl+P"));
    connect(actionProjectDir, SIGNAL(triggered()), this, SLOT(display_open_dialog()));

    actionSourceDir = new QAction(tr("&Source Directory"), this);
    actionSourceDir = ui->actionSource_Directories;
    actionSourceDir->setShortcut(tr("Ctrl+S"));
    connect(actionSourceDir, SIGNAL(triggered()), this, SLOT(display_source_dialog()));

    actionBuildDir = new QAction(tr("&Build Directory"), this);
    actionBuildDir = ui->actionBuild_Directories;
    actionBuildDir->setShortcut(tr("Ctrl+B"));
    connect(actionBuildDir, SIGNAL(triggered()), this, SLOT(display_build_dialog()));

    actionBrief = new QAction(this);
    actionBrief = ui->actionShow_Brief_Only;
    connect(actionBrief, SIGNAL(triggered()), this, SLOT(update_show_brief()));

    actionOnly_missing = new QAction(this);
    actionOnly_missing = ui->actionShow_Only_Missing;
    connect(actionOnly_missing, SIGNAL(triggered()), this, SLOT(update_only_missing()));

    actionFake_block = new QAction(this);
    actionFake_block = ui->actionShow_Fake_Blocks_and_Arcs;
    connect(actionFake_block, SIGNAL(triggered()), this, SLOT(update_fake_block()));

    actionDemangled_name = new QAction(this);
    actionDemangled_name = ui->actionShow_Demangle_Names;
    connect(actionDemangled_name, SIGNAL(triggered()), this, SLOT(update_demangled_name()));

    actionFit_To_Window = new QAction(this);
    actionFit_To_Window = ui->actionFit_To_Window;
    connect(actionFit_To_Window, SIGNAL(triggered()), this, SLOT(update_fit_to_window()));
}

void MainWindow::about()
{
    QMessageBox about_msgbox;
    about_msgbox.setWindowTitle("About Trucov");
    about_msgbox.setText("<h2>Trucov</h2> <p>Trucov &copy; Schweitzer Engineering Laboratories.  MIT License...Bla bla</p>");
    about_msgbox.setStandardButtons(QMessageBox::Close);
    about_msgbox.exec();
}

void MainWindow::show_how_to()
{
    QMessageBox how_to_msgbox;
    how_to_msgbox.setWindowTitle("How To");
    how_to_msgbox.setText("<h2>Configure Program</h2> <p>Go to Menu >> Options and configure.</p>");
    how_to_msgbox.setStandardButtons(QMessageBox::Close);
    how_to_msgbox.exec();
}

void MainWindow::display_splitter()
{
    // project explorer initializing model and items
    this->initialize_explorer();

    editor2 = new SvgWindow;
    editor2->resize(300, 300);

    editor3 = new CodeEditor;
    editor3->resize(300,300);
    //editor3->setReadOnly(true);

    // setting WordWrap off
    editor3->setWordWrapMode(QTextOption::NoWrap);

    // splitter
    splitter = new QSplitter(Qt::Horizontal);

    // adding components to the splitter
    splitter->addWidget(proj_explorer);
    splitter->addWidget(editor2);
    splitter->addWidget(editor3);
    mainStatusBar = new QStatusBar(this);
    this->setStatusBar(mainStatusBar);
    mainStatusBar->showMessage(tr("Ready"));
    //splitter->addWidget(myStatusBar);



    setCentralWidget(splitter);
}

void MainWindow::display_open_dialog()
{
    // display dialog and get the file name
    QString filename = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);

    // file name is not empty
    if ( !filename.isEmpty() )
    {
        project_dir = filename;
    }
}

void MainWindow::display_source_dialog()
{
    source_dir_list.clear();
    source_dir_list = Proj_dia::get_dir_list(this);
}

void MainWindow::display_build_dialog()
{
    build_dir_list.clear();
    build_dir_list = Proj_dia::get_dir_list(this);
}

void MainWindow::display_save_dialog()
{
    QString filename = QFileDialog::getSaveFileName(this);
}

void MainWindow::initialize_explorer()
{
    // project explorer initializing model and items
    model = new QStandardItemModel;

    // initializing splitter components
    src_tree = new QTreeView;
    proj_explorer = new QTabWidget;
    proj_explorer->addTab(src_tree, "Sources");

    src_tree->header()->hide();
    src_tree->setModel(model);

    connect(src_tree, SIGNAL(clicked(QModelIndex)), this, SLOT(tree_item_clicked(QModelIndex)));
}

void MainWindow::update_svgwindow(const QString &filename)
{
    editor2->openFile(filename);
}

void MainWindow::update_source_window(const QString &filename, int line_no = 0)
{
    QFile infile( filename );
    if ( infile.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream in(&infile);

        QFont font;
        font.setFamily("Monospace");
        font.setFixedPitch(true);
        font.setPointSize(10);
        editor3->setFont(font);
        highlighter = new Highlighter(editor3->document());
        QString tmp;
        tmp = in.readAll();
        editor3->setPlainText( tmp );
        editor3->wordWrapMode();

        // highlight function header
        cursor = &(editor3->textCursor());
        cursor->movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, line_no + 1);
        cursor->movePosition(QTextCursor::Up, QTextCursor::KeepAnchor, 1);
        editor3->ensureCursorVisible();
        editor3->setFocus();
    }
    else
    {
        QMessageBox *file_fail = new QMessageBox(this);
        file_fail->setWindowTitle("ERROR");
        file_fail->setText("Cannot open source file");
        file_fail->exec();
    }
}

void MainWindow::update_show_brief()
{
    show_brief = ui->actionShow_Brief_Only->isChecked();
}

void MainWindow::update_only_missing()
{
    show_only_missing = ui->actionShow_Only_Missing->isChecked();
}

void MainWindow::update_fake_block()
{
    show_fake_block = ui->actionShow_Fake_Blocks_and_Arcs->isChecked();
}

void MainWindow::update_demangled_name()
{
    show_demangled_name = ui->actionShow_Demangle_Names->isChecked();
}

void MainWindow::update_fit_to_window()
{
    fit_to_window = ui->actionFit_To_Window->isChecked();
}

void MainWindow::run_trucov()
{
    QString command(" all_report");
    if (!project_dir.isEmpty())
    {
        command += " -c ";
        command += project_dir;
    }

    if (show_brief)
    {
        command += " --show-brief";
    }
    if (show_only_missing)
    {
        command += " --show-only-missing";
    }
    if (show_fake_block)
    {
        command += " --show-fake-block";
    }
    if (!source_dir_list.empty())
    {
        QString temp;
        for(int i = 0; i < source_dir_list.size(); i++)
        {
            temp.append(" -s ");
            temp.append(source_dir_list[i]);
        }
        command += temp;
    }
    if (!build_dir_list.empty())
    {
        QString temp;
        for(int i = 0; i < build_dir_list.size(); i++)
        {
            temp.append(" -b ");
            temp.append(build_dir_list[i]);
        }
        command += temp;
    }

    command += " -o .secret_gui_dir --secret-gui --per-function --mangle";
    mainStatusBar->showMessage(command);
    
    // Steal pipe function from tru_utility
    string message = execute_pipe("trucov", command.toStdString());

    // Read Secret Gui file
    g_file_list.clear();
    read_secret_gui( g_file_list );

    // Populate info
     populate_tree();
}

void MainWindow::tree_item_clicked(QModelIndex index)
{
    QStandardItem *item = model->itemFromIndex(index);
    QString function_name;
    if (item->parent() != NULL)
    {
        function_name = item->parent()->data(2).toString();
        QString chop_char = " ";
        int chop_point = function_name.lastIndexOf(" ");
        function_name.truncate(chop_point - 1);
        function_name += " " + item->data(2).toString();
        chop_point = function_name.lastIndexOf(" ");
        function_name.truncate(chop_point - 1);
        /*
        ////////////////////////// Testing find_function ///////////////////////////////
        string test_str = "bst.cpp Bst::post_order(Node*)";

        //Function *fun_ptr = find_function(function_name.toStdString(), g_file_list);

        if (g_file_list.size() != 0)
        {
            QString g_list;
            g_list = g_file_list[0].name.c_str();
            g_list += " ";
            g_list += g_file_list[0].functions[0].name.c_str();
            QMessageBox test_glist(this);
            test_glist.setText(g_list);
            test_glist.exec();
            test_str = g_list.toStdString();
        }
        ///////////////////////////////////////////////////////////////////////////////////////////
        */
        Function *fun_ptr = find_function(function_name.toStdString(), g_file_list);

        if (fun_ptr != NULL)
        {
            QString temp_str = fun_ptr->graph_path.c_str();
            editor2->setWidgetResizable(fit_to_window);
            this->update_svgwindow(temp_str);
            temp_str = fun_ptr->source_path.c_str();
            this->update_source_window(temp_str, fun_ptr->lineno - 1);
            //editor3->moveCursor(QTextCursor::Down,

            // making status string
            temp_str = fun_ptr->name.c_str();
            temp_str += "  ";
            temp_str += fun_ptr->percentage.c_str();
            QString line_no = QString("  Line#: %1").arg(fun_ptr->lineno);
            temp_str += line_no;
            // update status bar
            mainStatusBar->showMessage(temp_str);
        }
        else
        {
            QMessageBox error_message(this);
            error_message.setText("error::" + function_name);
            error_message.exec();
        }

    }
}


void MainWindow::populate_tree()
{
    vector<Source_file>::iterator itr;

    string test;

    int idx = 0;
    for (itr = g_file_list.begin(); itr < g_file_list.end(); itr++)
    {
        test.clear();
        test += itr->name + "  " + itr->percentage;
        QString tmp_str(test.c_str());
        QStandardItem *temp_item = new QStandardItem(tmp_str);
        model->setItem(idx, temp_item);
        temp_item->setEditable(false);
        temp_item->setSelectable(false);
        idx++;

        vector<Function>::iterator f_itr;
        for (f_itr = itr->functions.begin(); f_itr < itr->functions.end(); f_itr++)
        {
            test.clear();
            test += f_itr->name + "  " + f_itr->percentage;
            QString tmp_fun_str(test.c_str());
            QStandardItem *temp_fun_item = new QStandardItem(tmp_fun_str);
            temp_item->appendRow(temp_fun_item);
            temp_fun_item->setEditable(false);
            temp_fun_item->setSelectable(true);
        }
    }
}

string MainWindow::execute_pipe(string command_name, string command_argument)
{
   char input[5000];
   string rec_name = "";
   command_name.push_back(' ');
   FILE *fp = popen( (command_name + command_argument).c_str(), "r" );

   if (fp != NULL)
   {
      char * buf = fgets( input, sizeof( input ), fp );

      rec_name = input;

      unsigned int found = rec_name.find( "\n" );
      if ( found != string::npos )
      {
          rec_name.resize( found );
      }

      pclose(fp);
   }
   return rec_name;
}
