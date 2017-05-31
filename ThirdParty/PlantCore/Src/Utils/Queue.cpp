#include "Queue.h"

//template<typename T>
//Queue::Queue() {
//	pthread_mutex_init(&mutex, NULL);
//	pthread_cond_init(&condv, NULL);
//}
//
//template<typename T>
//Queue::~Queue() {
//	pthread_mutex_destroy(&mutex);
//	pthread_cond_destroy(&condv);
//}
//
//template<typename T>
//void Queue::add(T item) {
//	pthread_mutex_lock(&mutex);
//	queue.push_back(item);
//	pthread_cond_signal(&condv);
//	pthread_mutex_unlock(&mutex);
//}
//
//template<typename T>
//T Queue::remove() {
//	pthread_mutex_lock(&mutex);
//	while (queue.size() == 0) {
//		pthread_cond_wait(&condv, &mutex);
//	}
//	T item = queue.front();
//	queue.pop_front();
//	pthread_mutex_unlock(&mutex);
//	return item;
//}
//
//template<typename T>
//int Queue::size() {
//	pthread_mutex_lock(&mutex);
//	int size = queue.size();
//	pthread_mutex_unlock(&mutex);
//	return size;
//}