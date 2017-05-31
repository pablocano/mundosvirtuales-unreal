#include "Queue.h"

template<typename T>
Queue<T>::Queue() : m_mutex(), m_condv()
{
}

template<typename T>
void Queue<T>::add(T item)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_queue.push_back(item);
	m_condv.notify_one();
}

template<typename T>
T Queue<T>::remove()
{
	std::unique_lock<std::mutex> lock(m_mutex);
	while (m_queue.size() == 0)
	{
		m_condv.wait(lock);
	}
	T item = m_queue.front();
	m_queue.pop_front();
	return item;
}

template<typename T>
int Queue<T>::size()
{
	std::unique_lock<std::mutex> lock(m_mutex);
	int size = queue.size();
	return size;
}