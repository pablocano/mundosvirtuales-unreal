#include <list>
#include <thread>
#include <mutex>
#include <condition_variable> 

template <typename T> class Queue
{
protected:
	std::list<T> m_queue;
	std::mutex m_mutex;
	std::condition_variable m_condv;

public:
	Queue();

	void add(T item);

	T remove();

	int size();
};