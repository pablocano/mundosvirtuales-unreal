#include "../Utils/network/TcpComm.h"
#include <list>
#include <thread>

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

//template <typename T> class queueClients
//{
//protected:
//	std::list<T> queue;
//	pthread_mutex_t mutex;
//	pthread_cond_t condv;
//public:
//	queueClients() {
//		pthread_mutex_init(&mutex, NULL);
//		pthread_cond_init(&condv, NULL);
//	}
//	~queueClients() {
//		pthread_mutex_destroy(&mutex);
//		pthread_cond_destroy(&condv);
//	}
//	void add(T item) {
//		pthread_mutex_lock(&mutex);
//		queue.push_back(item);
//		pthread_cond_signal(&condv);
//		pthread_mutex_unlock(&mutex);
//	}
//	T remove() {
//		pthread_mutex_lock(&mutex);
//		while (queue.size() == 0) {
//			pthread_cond_wait(&condv, &mutex);
//		}
//		T item = queue.front();
//		queue.pop_front();
//		pthread_mutex_unlock(&mutex);
//		return item;
//	}
//	int size() {
//		pthread_mutex_lock(&mutex);
//		int size = queue.size();
//		pthread_mutex_unlock(&mutex);
//		return size;
//	}
//};

//class WorkItem
//{
//	TCPStream* m_stream;
//
//public:
//	WorkItem(TCPStream* stream) : m_stream(stream) {}
//	~WorkItem() { delete m_stream; }
//
//	TCPStream* getStream() { return m_stream; }
//};
//
//class ConnectionHandler : public Thread
//{
//	queueClients<WorkItem*>& queue;
//
//public:
//	ConnectionHandler(queueClients<WorkItem*>& queue) : queue(queue) {}
//
//	void* run() {
//		// Remove 1 item at a time and process it. Blocks until an item 
//		// is placed on the queue.
//		for (int i = 0;; i++) {
//			printf("thread %lu, loop %d - waiting for item...\n",
//				(long unsigned int)self(), i);
//			WorkItem* item = m_queue.remove();
//			printf("thread %lu, loop %d - got one item\n",
//				(long unsigned int)self(), i);
//			TCPStream* stream = item->getStream();
//
//			// Echo messages back the client until the connection is 
//			// closed
//			char input[256];
//			int len;
//			while ((len = stream->receive(input, sizeof(input) - 1)) > 0) {
//				input[len] = NULL;
//				stream->send(input, len);
//				printf("thread %lu, echoed '%s' back to the client\n",
//					(long unsigned int)self(), input);
//			}
//			delete item;
//		}
//
//		// Should never get here
//		return NULL;
//	}
//};

class ServerData
{
protected:
	TcpComm tcpComm;
};
