#include "workingthread.h"
#include <qstring.h>
void WorkingThread::emitMessage(const std::string &msg){
    emit alert(QString(msg.c_str()));
}