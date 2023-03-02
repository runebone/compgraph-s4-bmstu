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
    Model *m_model;
    View *m_view;
    History *m_history;

public slots:

signals:

};

#endif // CONTROLLER_H
