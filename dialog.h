#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QMainWindow>
#include <QDir>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>
#include <vector>
#include <QTemporaryDir>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QProcess>
#include <QSettings>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();


private slots:
    void on_addButton_clicked();

    void on_replaceButton_clicked();

    void on_deleteButton_clicked();

    void on_runButton_clicked();

    void on_clearButton_clicked();

    void on_addItemButton_clicked();

    void dragEnterEvent(QDragEnterEvent*);

    void dropEvent(QDropEvent*);



private:
    Ui::Dialog *ui;
    QStringList images;
    QTemporaryDir tempdir;

    QString enterFolder(QDir, QString);
    void resetConsoleandProgressBar();
    void sox_Command(QString, QDir, QDir);

};

#endif // DIALOG_H
