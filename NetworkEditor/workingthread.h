#ifndef WORKINGTHREAD_H

#define WORKINGTHREAD_H
#include <qthread.h>
#include <string.h>
#include "kernel\Constant.h"
class WorkingThread : public QThread{
    Q_OBJECT
private:
    QString message;
protected:
    
public:
    WorkingThread() :QThread(){
        moveToThread(this);
    }
    virtual void run() = 0;
    virtual void abort() = 0;
    virtual void emitMessage(const std::string & msg);
    QString getMessage(){ return message; }
signals:
    void alert();
};

#endif