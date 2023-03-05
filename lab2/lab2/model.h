#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include "pair.h"

class ModelData {
public:
    std::vector<Pair<double>> vertices;
    std::vector<Pair<size_t>> lines;
};

// class ModelMemento {
// public:
    // ModelMemento(ModelData modelData);

    // ModelData getData() const;

// private:
    // ModelData m_modelData;
// };

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);

    void setData(ModelData &modelData);
    ModelData getData() const;

    // ModelMemento createMemento() const;
    // void restoreMemento(const ModelMemento& memento);

private:
    ModelData m_modelData;

signals:
    void updated(ModelData);
    // void memento_restored(ModelData);
};

// class History {
// public:
    // void push(const ModelMemento& memento);
    // ModelMemento pop();
    // ModelMemento back() const;
    // size_t size() const;

// private:
    // std::vector<ModelMemento> m_mementos;
// };

#endif // MODEL_H
