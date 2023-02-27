#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>

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

    // Let the View know if user presses any keys
    // (I should've initially created custom QGraphicsView...)
    connect(this, SIGNAL(key_pressed(QKeyEvent*)), view, SLOT(key_press_event(QKeyEvent*)));

    // Tmp
    double sqrt3 = std::sqrt(3);

    std::vector<Point> pa{
        // { .x = 0.0, .y = 1.0 }, // default
        // { .x = -sqrt3/2, .y = -1.0/2 },
        // { .x = sqrt3/2, .y = -1.0/2 },
        // { .x = 0.0, .y = 1.0 * 10 }, // big
        // { .x = -sqrt3/2 * 10, .y = -1.0/2 * 10 },
        // { .x = sqrt3/2 * 10, .y = -1.0/2 * 10 },
    };

    std::vector<Point> pb{
        // { .x = sqrt3/2, .y = 1.5 }, // FIXED: error sorting intersection points
        // { .x = 0.0, .y = 0.0 },
        // { .x = 2*sqrt3/2, .y = 0.0 },
        // { .x = sqrt3/2 - 0.5, .y = 1.5 + 0.2}, // FIXED: error sorting intersection points
        // { .x = 0.0 - 0.5, .y = 0.0 + 0.2},
        // { .x = 2*sqrt3/2 - 0.5, .y = 0.0 + 0.2},
        // { .x = sqrt3/2 + 0.0, .y = 1.5 + 1.0}, // share common side, no intersection area
        // { .x = 0.0 + 0.0, .y = 0.0 + 1.0},
        // { .x = 2*sqrt3/2 + 0.0, .y = 0.0 + 1.0},
        // { .x = 0.0, .y = 1.0 + 1.999 }, // very small intersection area; ok
        // { .x = -sqrt3/2, .y = -1.0/2 + 1.999 },
        // { .x = sqrt3/2, .y = -1.0/2 + 1.999 },
        // { .x = 0.0 + 0.2, .y = 1.0 + 1.000 }, // FIXED: error: raycast horizontal ray goes right through the vertex -> counts twice
        // { .x = -sqrt3/2 + 0.2, .y = -1.0/2 + 1.000 },
        // { .x = sqrt3/2 + 0.2, .y = -1.0/2 + 1.000 },
        // { .x = 0.0 + 0.0, .y = 1.0 + 2.000 }, // share common point
        // { .x = -sqrt3/2 + 0.0, .y = -1.0/2 + 2.000 },
        // { .x = sqrt3/2 + 0.0, .y = -1.0/2 + 2.000 },
        // { .x = 0.0 + 0.2, .y = 1.0 + 2.800 }, // no intersection
        // { .x = -sqrt3/2 + 0.2, .y = -1.0/2 + 2.800 },
        // { .x = sqrt3/2 + 0.2, .y = -1.0/2 + 2.800 },
    };

    // for (Point p: pa) {
        // std::printf("1 %lf %lf\n", p.x, p.y);
    // }

    // for (Point p: pb) {
        // std::printf("2 %lf %lf\n", p.x, p.y);
    // }

    // model->set_first_set_points(pa);
    // model->set_second_set_points(pb);
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

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    emit this->key_pressed(event);
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

void MainWindow::on_loadFromFileButton_clicked()
{
    // Should have been entirely in Controller
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), "/home/human/University/cg/lab1", "Text File (*.txt)");

    emit this->load_from_file_button_clicked(filename);
}


void MainWindow::on_backButton_clicked()
{
    emit this->back_button_clicked();
}

