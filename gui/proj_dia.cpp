#include "proj_dia.h"
#include "ui_proj_dia.h"

Proj_dia::Proj_dia(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::Proj_dia)
{
    m_ui->setupUi(this);
    //QAbstractButton*
    create_action();
    scrollBox = m_ui->dir_list;
    dirBox = new QListWidget();
    scrollBox->setWidget(dirBox);
}

Proj_dia::~Proj_dia()
{
    delete m_ui;
}

void Proj_dia::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
void Proj_dia::create_action()
{
    connect(m_ui->but_add, SIGNAL( clicked() ), this, SLOT(add_dir()));
    connect(m_ui->but_remove, SIGNAL( clicked() ), this, SLOT(remove_all_dir()));
}

void Proj_dia::add_dir()
{
    QString filename = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    if ( !filename.isEmpty() )
    {
        dirBox->addItem(filename);
        dirList.append(filename);
    }

}

void Proj_dia::remove_all_dir()
{
    dirBox->clear();
    dirList.clear();
}

QVector< QString > Proj_dia::get_dir_list(QWidget *parent)
{
    Proj_dia *temp = new Proj_dia(parent);
    temp->exec();
        return temp->dirList;
    //}
    //else
    //{
    //    temp->dirList.empty();
    //    return temp->dirList;
    //}
}
