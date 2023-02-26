#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create MVC
    Model *model = new Model;
    View *view = new View(ui->graphicsView, ui->descriptionLabel);

    view->set_model(model);

    Controller *controller = new Controller(model, view);
    this->controller = controller;

    // Connect UI signals to Controller
    connect(this, SIGNAL(solve_button_clicked()), controller, SLOT(on_solve_button_clicked()));

    // Let the View know if user presses any keys
    connect(this, SIGNAL(key_pressed(QKeyEvent*)), view, SLOT(key_press_event(QKeyEvent*)));

    // Tmp
    double sqrt3 = std::sqrt(3);

    std::vector<Point> pa{
        // { .x = 0.0, .y = 1.0 }, // default
        // { .x = -sqrt3/2, .y = -1.0/2 },
        // { .x = sqrt3/2, .y = -1.0/2 },
        { .x = 0.0, .y = 1.0 * 10 }, // big
        { .x = -sqrt3/2 * 10, .y = -1.0/2 * 10 },
        { .x = sqrt3/2 * 10, .y = -1.0/2 * 10 },
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
        { .x = 0.0, .y = 1.0 + 1.999 }, // very small intersection area; ok
        { .x = -sqrt3/2, .y = -1.0/2 + 1.999 },
        { .x = sqrt3/2, .y = -1.0/2 + 1.999 },
        // { .x = 0.0 + 0.2, .y = 1.0 + 1.000 }, // FIXED: error: raycast horizontal ray goes right through the vertex -> counts twice
        // { .x = -sqrt3/2 + 0.2, .y = -1.0/2 + 1.000 },
        // { .x = sqrt3/2 + 0.2, .y = -1.0/2 + 1.000 },
        // { .x = 0.0 + 0.2, .y = 1.0 + 0.800 }, // FIXED: error sorting intersection points
        // { .x = -sqrt3/2 + 0.2, .y = -1.0/2 + 0.800 },
        // { .x = sqrt3/2 + 0.2, .y = -1.0/2 + 0.800 },
        // { .x = 0.0 + 0.2, .y = 1.0 + 2.800 }, // no intersection
        // { .x = -sqrt3/2 + 0.2, .y = -1.0/2 + 2.800 },
        // { .x = sqrt3/2 + 0.2, .y = -1.0/2 + 2.800 },
    };

    model->set_first_set_points(pa);
    model->set_second_set_points(pb);
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

