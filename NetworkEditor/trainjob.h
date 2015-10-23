#ifndef TRAIN_JOB_H
#define TRAIN_JOB_H
#include "networkunitbase.h"
#include "workingthread.h"
#include "networkunitpool.h"
class TrainJob : public WorkingThread {
private:
    size_t batchSize;
    size_t trainingTimes;
    size_t criteria;
    shared_ptr<MasterControl> master;
    WorkingThreadJobType jobType = WorkingThreadJobType::TRAIN_JOB;
    bool pauseFlag=false;
    string fileName;
public:
    void setJobType(WorkingThreadJobType type){
        jobType = type;
    }
    void togglePauseFlag(){ pauseFlag = !pauseFlag; }
    bool getPauseFlag(){ return pauseFlag; }
    void setBatchSize(size_t size){ batchSize = size; }
    void setTrainingTimes(size_t times){ trainingTimes = times; }
    void setCriteria(size_t type){ criteria = type; }
    shared_ptr<MasterControl> getMasterControl(){ return master; }
    void resetMasterControl(){
        master = shared_ptr<MasterControl>(new MasterControl());
    }
    void abort() override {
        master->setTrainingTimes(0);
    }
    void setPredictFileName(string fileName){ this->fileName = fileName; }
    TrainJob() : WorkingThread(){
        master = shared_ptr<MasterControl>(new MasterControl());
    }
    void TrainJob::run() override;
    void TrainJob::runPredict(string fileName);
};
#endif