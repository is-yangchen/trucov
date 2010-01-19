#ifndef PROJ_DIA_H
#define PROJ_DIA_H

#include <QtGui>
#include <QtGui/QDialog>
#include <QFileDialog>
#include <QScrollArea>
#include <QListWidget>

namespace Ui {
    class Proj_dia;
}

class Proj_dia : public QDialog {
    Q_OBJECT
public:
    static QVector < QString > get_dir_list(QWidget *parent = 0);
    ~Proj_dia();

public slots:
    void add_dir();
    void remove_all_dir();


protected:
    void changeEvent(QEvent *e);

private:
    Proj_dia(QWidget *parent = 0);
    void create_action();

    Ui::Proj_dia *m_ui;
    QScrollArea *scrollBox;
    QListWidget* dirBox;
    QVector< QString > dirList;
};

#endif // PROJ_DIA_H
