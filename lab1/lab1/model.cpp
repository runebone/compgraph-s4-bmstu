#include "model.h"

Model::Model(QObject *parent) : QObject(parent) {}

void Model::setData(ModelData &modelData)
{
    m_modelData = modelData;
    emit updated();
}

ModelData Model::getData() const {
    return m_modelData;
}

ModelMemento Model::createMemento() const {
    return Memento(m_modelData);
}

void Model::restoreMemento(const ModelMemento &memento) {
    m_modelData = memento.getData();
    emit updated();
}

#include "algorithm.h"
err_t Model::solve()
{
    err_t status = Algorithm::solve(m_modelData.firstSetPoints, m_modelData.secondSetPoints, m_modelData.solutionData);

    // XXX: ugly
    if (status == OK
            || status == ERR::POLYGONS_DO_NOT_INTERSECT
            || status == ERR::LESS_THAN_THREE_POINTS_OF_POLYGONS_INTERSECTION)
    {
        emit updated();
    }

    return status;
}

ModelMemento::ModelMemento(ModelData &modelData) {
    m_modelData = modelData;
}

ModelData ModelMemento::getData() const {
    return m_modelData;
}

void History::push(const ModelMemento &memento) {
    m_mementos.push_back(memento);
}

ModelMemento History::pop() {
    ModelMemento memento = m_mementos.back();
    m_mementos.pop_back();

    return memento;
}
