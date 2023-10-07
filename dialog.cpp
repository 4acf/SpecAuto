#include "dialog.h"
#include "ui_dialog.h"

#include "imgwav.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setAcceptDrops(true);
    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(false);
    ui->console->setReadOnly(true);
}

QString Dialog::enterFolder(QDir location, QString folder){

    folder.insert(0, "/");
    return location.absolutePath() + folder;

}

void Dialog::resetConsoleandProgressBar(){

    ui->console->clear();
    if(ui->progressBar->isVisible()){
        ui->progressBar->setValue(0);
        ui->progressBar->setVisible(false);
    }

}

void Dialog::sox_Command(QString filename, QDir wavOutputDirectory, QDir pngOutputDirectory){

    /*
     *  example of working SoX command:
     *  sox sound.wav -n spectrogram -o image.png
     */

    filename.chop(4);
    filename += QString(".wav");
    QString inputFilePath = wavOutputDirectory.path() + QString("/") + filename;
    filename.chop(4);
    filename += QString(".png");
    QString outputFilePath = pngOutputDirectory.path() + QString("/") + filename;
    QString command = "sox " + inputFilePath + " -n spectrogram -o " + outputFilePath;

    QProcess process;
    process.startCommand(command);
    process.waitForFinished(-1);

}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_addButton_clicked()
{

    resetConsoleandProgressBar();

    QDir directory = QFileDialog::getExistingDirectory(this, tr("Add Folder"), QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    QStringList images = directory.entryList(QStringList() << "*.jpg" << "*.png", QDir::Files);

    if(!images.isEmpty()){

        foreach(QString filename, images){

            QList<QListWidgetItem *> repeatList = ui->filenameList->findItems(filename, Qt::MatchExactly);

            //no repeats found
            if(repeatList.isEmpty()){

                ui->filenameList->addItem(filename);
                QFile::copy(directory.path() + QString("/") + filename, tempdir.path() + QString("/") + filename);

            }

            //repeats found, prompt file replacement
            else{
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, tr("Overwrite?"), "\"" + filename + "\" already exists. Would you like to replace the file in this destination?", QMessageBox::Yes|QMessageBox::No);

                if(reply == QMessageBox::Yes){
                    QDir temp = tempdir.path();
                    temp.remove(filename);
                    QFile::copy(directory.path() + QString("/") + filename, tempdir.path() + QString("/") + filename);
                }

            }

        }

    }

}


void Dialog::on_replaceButton_clicked()
{

    resetConsoleandProgressBar();

    QDir directory = QFileDialog::getExistingDirectory(this, tr("Replace Files"), QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    QStringList images = directory.entryList(QStringList() << "*.jpg" << "*.png", QDir::Files);

    if(!images.isEmpty()){

        ui->filenameList->clear();

        QDir temp = tempdir.path();
        foreach(QString filename, temp.entryList()){
            temp.remove(filename);
        }

        foreach(QString filename, images){

            ui->filenameList->addItem(filename);
            QFile::copy(directory.path() + QString("/") + filename, tempdir.path() + QString("/") + filename);

        }

    }

}


void Dialog::on_deleteButton_clicked()
{

    resetConsoleandProgressBar();

    QList<QListWidgetItem *> deleteList = ui->filenameList->selectedItems();

    foreach(QListWidgetItem *item, deleteList){

        QDir temp = tempdir.path();
        QString filename = item->text();
        temp.remove(filename);

    }

    qDeleteAll(deleteList);

}

void Dialog::on_clearButton_clicked()
{

    resetConsoleandProgressBar();

    ui->filenameList->clear();
    QDir temp = tempdir.path();
    foreach(QString filename, temp.entryList()){
        temp.remove(filename);
    }

}

void Dialog::on_addItemButton_clicked()
{

    resetConsoleandProgressBar();

    QStringList images = QFileDialog::getOpenFileNames(this, tr("Select one or more files to open"), QDir::homePath(), "Images (*.png *.jpg)");
    if(!images.isEmpty()){

        foreach(QString filepath, images){

            QFileInfo f(filepath);
            QString filename = f.fileName();
            QList<QListWidgetItem *> repeatList = ui->filenameList->findItems(filename, Qt::MatchExactly);

            //no repeats found
            if(repeatList.isEmpty()){

                ui->filenameList->addItem(filename);
                QFile::copy(filepath, tempdir.path() + QString("/") + filename);

            }

            //repeats found, prompt file replacement
            else{
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, tr("Overwrite?"), "\"" + filename + "\" already exists. Would you like to replace the file in this destination?", QMessageBox::Yes|QMessageBox::No);

                if(reply == QMessageBox::Yes){
                    QDir temp = tempdir.path();
                    temp.remove(filename);
                    QFile::copy(filepath, tempdir.path() + QString("/") + filename);
                }

            }

        }

    }
}

void Dialog::dragEnterEvent(QDragEnterEvent *e){

    if(e->mimeData()->hasUrls()){
        e->acceptProposedAction();
    }

}

void Dialog::dropEvent(QDropEvent *e){

    resetConsoleandProgressBar();

    foreach (const QUrl &url, e->mimeData()->urls()) {
        QString filepath = url.toLocalFile();
        QFileInfo f(filepath);
        QString filename = f.fileName();
        QList<QListWidgetItem *> repeatList = ui->filenameList->findItems(filename, Qt::MatchExactly);

        //no repeats found
        if(repeatList.isEmpty()){

            ui->filenameList->addItem(filename);
            QFile::copy(filepath, tempdir.path() + QString("/") + filename);

        }

        //repeats found, prompt file replacement
        else{
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, tr("Overwrite?"), "\"" + filename + "\" already exists. Would you like to replace the file in this destination?", QMessageBox::Yes|QMessageBox::No);

            if(reply == QMessageBox::Yes){
                QDir temp = tempdir.path();
                temp.remove(filename);
                QFile::copy(filepath, tempdir.path() + QString("/") + filename);
            }
        }

    }

}

void Dialog::on_runButton_clicked()
{

    resetConsoleandProgressBar();

    //check if there's items
    if(ui->filenameList->count() == 0){
        QMessageBox::information(this, tr("Error"), "No files added.");
        return;
    }

    //open save dialog (save name will be identical to source file, use listwidget to determine that)
    QDir saveDirectory = QFileDialog::getExistingDirectory(this, tr("Choose Save Location"), QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(!saveDirectory.exists()){
        QMessageBox::information(this, tr("Error"), "Save location does not exist.");
        return;
    }

    /*actual script*/

    //make folder hierarchy
    if(saveDirectory.exists("Output")){
        QMessageBox::information(this, tr("Error"), "Folder named \"Output\" already exists at this location.");
        return;
    }

    saveDirectory.mkdir("Output");
    QDir outputDirectory = enterFolder(saveDirectory, "Output");

    outputDirectory.mkdir("pngOutput");
    outputDirectory.mkdir("wavOutput");
    QDir pngOutputDirectory = enterFolder(outputDirectory, "pngOutput");
    QDir wavOutputDirectory = enterFolder(outputDirectory, "wavOutput");

    ui->progressBar->setVisible(true);

    //accessing files
    QDir temp = tempdir.path();
    int filecount = temp.entryList(QStringList() << "*.jpg" << "*.png", QDir::Files).count();
    ui->progressBar->setMaximum(filecount);
    int filescompleted = 0;

    QMessageBox::information(this, "Press OK to begin", "Press OK to begin"); //include est time to complete???

    foreach(QString filename, temp.entryList(QStringList() << "*.jpg" << "*.png", QDir::Files)){

        imgwav imgwav;
        imgwav.writeout(tempdir.path(), filename, wavOutputDirectory);

        sox_Command(filename, wavOutputDirectory, pngOutputDirectory);

        filescompleted++;
        ui->progressBar->setValue(filescompleted);

        //make background green to signify it is done
        QList<QListWidgetItem*> match = ui->filenameList->findItems(filename, Qt::MatchExactly);
        QListWidgetItem *current = match.at(0);
        current->setBackground(QBrush(Qt::green));

    }

    ui->console->append("All done.");


}








