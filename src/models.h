#pragma once

#include <ctime>

class Lesson
{
private:
    /* data */
public:
    Lesson(const char* title, const char* brief, time_t startTime, time_t endTime);
    // ~Lesson();
    const char* title;
    const char* brief;
    time_t startTime;
    time_t endTime;
};

Lesson::Lesson(const char* title, const char* brief, time_t startTime, time_t endTime)
{
    this->title = title;
    this->brief = brief;
    this->startTime = startTime;
    this->endTime = endTime;
}

// Lesson::~Lesson()
// {
// }
