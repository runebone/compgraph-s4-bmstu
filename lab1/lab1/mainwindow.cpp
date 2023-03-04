#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create MVC
    Model *model = new Model;
    View *view = new View(ui->graphicsView, ui->scrollAreaLeft, ui->scrollAreaRight, ui->descriptionLabel);

    view->set_model(model);

    Controller *controller = new Controller(model, view, this);
    this->controller = controller;

    // Connect UI signals to Controller
    connect(this, SIGNAL(solve_button_clicked()), controller, SLOT(on_solve_button_clicked()));
    connect(this, SIGNAL(add1_button_clicked()), controller, SLOT(on_add1_button_clicked()));
    connect(this, SIGNAL(add2_button_clicked()), controller, SLOT(on_add2_button_clicked()));
    connect(this, SIGNAL(load_from_file_button_clicked(QString)), controller, SLOT(on_load_from_file_button_clicked(QString)));
    connect(this, SIGNAL(back_button_clicked()), controller, SLOT(on_back_button_clicked()));
    connect(this, SIGNAL(clear_screen()), controller, SLOT(on_clear_screen_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete this->controller;
}

void MainWindow::on_actionTask_triggered()
{
    QString text = "Заданы два множества точек на плоскости. В каждом множестве найти три равноудалѐнные друг от друга точки. Через эти точки провести окружности. В каждую окружность вписать шестиугольник. Найти площадь пересечения двух получившихся шестиугольников.";
    this->ui->descriptionLabel->setText(text);
}


void MainWindow::on_solveButton_clicked()
{
    emit this->solve_button_clicked();
}

void MainWindow::on_exitButton_clicked()
{
    this->close();
}


void MainWindow::on_addPoint1Button_clicked()
{
    emit this->add1_button_clicked();
}

void MainWindow::on_addPoint2Button_clicked()
{
    emit this->add2_button_clicked();
}

#include <QFileDialog>
void MainWindow::on_loadFromFileButton_clicked()
{
    // Should have been entirely in Controller
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), "/home/human/University/cg/lab1", "Text File (*.txt)");

    if (!filename.isNull()) {
        emit this->load_from_file_button_clicked(filename);
    }
}


void MainWindow::on_backButton_clicked()
{
    emit this->back_button_clicked();
}


void MainWindow::on_actionClear_Screen_triggered()
{
    emit this->clear_screen();
}
