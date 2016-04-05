#include <qthread.h>
#include <string.h>
#include "include/Model.h"
class WorkingThread : public QThread{
    Q_OBJECT
private:
    
public:
    WorkingThread() :QThread(){}
    void emitMessage(std::string msg);
    virtual void run() = 0;
signals:
    void alert(QString s);
};

class TrainThread : public WorkingThread{
private:
    fudandnn::Model model;
    shared_ptr<Solver> solver;
public:
    TrainThread(){}
    virtual void run() override;
    fudandnn::Model& getModel(){
        return model;
    }
    void setSolver(shared_ptr<Solver> solver){
        this->solver = solver;
    }
};