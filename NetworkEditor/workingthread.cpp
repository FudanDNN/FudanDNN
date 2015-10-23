#include "workingthread.h"
#include <qstring.h>
void WorkingThread::emitMessage(const std::string &msg){
    this->message =msg.c_str();
    emit alert();
}