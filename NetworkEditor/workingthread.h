#ifndef WORKINGTHREAD_H

#define WORKINGTHREAD_H
#include <qthread.h>
#include <string.h>
#include "kernel\Constant.h"
class WorkingThread : public QThread{
    Q_OBJECT
protected:
    
public:
    WorkingThread() :QThread(){
        moveToThread(this);
    }
    virtual void run() = 0;
    virtual void abort() = 0;
    virtual void emitMessage(const std::string & msg);
signals:
    void alert(QString);
};

#endif