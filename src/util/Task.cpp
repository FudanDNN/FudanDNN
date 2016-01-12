#include "../header/util/Task.h"

Task::Task(string file, int id){
	this->file = file;
	this->id = id;
}

size_t Task::getId(){
	return this->id;
}

void Task::setGradient(shared_ptr<Matrix> gradient){
	this->gradient = gradient;
}
