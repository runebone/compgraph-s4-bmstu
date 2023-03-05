#include "model.h"

#include <QDebug>

Model::Model(QObject *parent) : QObject(parent) {}

void Model::setData(ModelData &modelData)
{
    m_modelData = modelData;
    emit updated(m_modelData);
}

ModelData Model::getData() const {
    return m_modelData;
}

// ModelMemento Model::createMemento() const {
    // return ModelMemento(m_modelData);
// }

// void Model::restoreMemento(const ModelMemento &memento) {
    // m_modelData = memento.getData();
    // emit memento_restored(m_modelData);
// }

// ModelMemento::ModelMemento(ModelData modelData) {
    // m_modelData = modelData;
// }

// ModelData ModelMemento::getData() const {
    // return m_modelData;
// }

// #define HISTORY_SIZE 100
// void History::push(const ModelMemento &memento) {
    // if (m_mementos.size() == HISTORY_SIZE) {
        // m_mementos.erase(m_mementos.begin() + 1);
    // }
    // m_mementos.push_back(memento);
// }

// ModelMemento History::pop() {
    // ModelMemento memento = m_mementos.back();
    // m_mementos.pop_back();

    // return memento;
// }

// ModelMemento History::back() const
// {
    // return m_mementos.back();
// }

// size_t History::size() const
// {
    // return m_mementos.size();
// }
