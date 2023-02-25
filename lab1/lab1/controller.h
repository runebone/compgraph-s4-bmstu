#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>

#include "model.h"
#include "view.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(Model *model, View *view, QObject *parent = nullptr);
    ~Controller();

private:
    Model *model;
    View *view;

signals:
    void model_solve();

public slots:
    void on_solve_button_clicked();

};

#endif // CONTROLLER_H
