#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create MVC
    Model *model = new Model;
    View *view = new View(ui->graphicsView);
    Controller *controller = new Controller(model, view, this);

    this->controller = controller;

    // Connect UI signals to Controller
    connect(this, SIGNAL(load_from_file_button_clicked(QString)), controller, SLOT(on_load_from_file_button_clicked(QString)));
    // connect(this, SIGNAL(back_button_clicked()), controller, SLOT(on_back_button_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete this->controller;
}

void MainWindow::on_exitButton_clicked()
{
    this->close();
}


#include <QFileDialog>
void MainWindow::on_loadFromFileButton_clicked()
{
    // Should have been entirely in Controller
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), "/home/human/University/cg/lab2", "Text File (*.txt)");

    if (!filename.isNull()) {
        emit this->load_from_file_button_clicked(filename);
    }
}


// void MainWindow::on_backButton_clicked()
// {
    // emit this->back_button_clicked();
// }
