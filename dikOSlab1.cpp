﻿#include <iostream>
#include <pthread.h>
#include <unistd.h>

pthread_cond_t condition = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
bool ready = 0;

void* producer(void*)
{
    while (true)
    {
        pthread_mutex_lock(&lock);
        if (ready)
        {
            pthread_mutex_unlock(&lock);
            continue;
        }
        ready = true;
        std::cout << "Сигнал отправлен поставщиком" << std::endl;
        pthread_cond_signal(&condition);
        pthread_mutex_unlock(&lock);
        sleep(1);
    }
    return nullptr;
}

void* consumer(void*)
{
    while (true)
    {
        pthread_mutex_lock(&lock);
        while (!ready)
        {
            pthread_cond_wait(&condition, &lock);
        }
        ready = false;
        std::cout << "Сигнал получен потребителем" << std::endl;
        pthread_mutex_unlock(&lock);
    }
    return nullptr;
}

int main() {
    pthread_t producerThread, consumerThread;

    // Запускаем потоки
    pthread_create(&producerThread, nullptr, producer, nullptr);
    pthread_create(&consumerThread, nullptr, consumer, nullptr);

    return 0;
}
