#pragma once
#include <stack>
template<typename T,int MAX_SIZE=50>
class ObjectPool
{
public:
	ObjectPool() {}
	~ObjectPool() 
	{
		while (!objects_.empty())
		{
			objects_.pop();
		}
	}

	shared_ptr<T> acquire()
	{
		if (objects_.empty()) //풀이 비었으면 새로 할당
		{
			for (int i = 0; i < MAX_SIZE; ++i)
			{
				shared_ptr<T> newObject = make_shared<T>();
				objects_.push(newObject);
			}
		}

		auto object = objects_.top();
		objects_.pop();
		return object;
	}

	void release(shared_ptr<T>& object)
	{
		objects_.push(object);
	}

	size_t getSize()
	{
		return objects_.size();
	}
	bool isEmpty()
	{
		return objects_.empty();
	}
private:
	stack<shared_ptr<Entity>> objects_;
};

