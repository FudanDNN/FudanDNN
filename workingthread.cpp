#include "workingthread.h"
#include "include/Model.h"
#include "networkunitpool.h"
#include <qstring.h>
void WorkingThread::emitMessage(std::string msg){
    QString s(msg.c_str());
    emit alert(s);
}

void TrainThread::run(){
    NetworkUnitPool::parseInformation(model, solver);
    model.run(this);
}