#pragma once
/*#ifndef _HAS_CXX17*/
namespace std
{
	class scoped_lock
	{
	public:
		using mutex_type = std::mutex;
		using _Mutex = std::mutex;

		explicit scoped_lock(_Mutex& _Mtx) : _MyMutex(_Mtx) { // construct and lock
			_MyMutex.lock();
		}

		explicit scoped_lock(adopt_lock_t, _Mutex& _Mtx) : _MyMutex(_Mtx) {} // construct but don't lock

		~scoped_lock()  {
			_MyMutex.unlock();
		}

		scoped_lock(const scoped_lock&) = delete;
		scoped_lock& operator=(const scoped_lock&) = delete;

	private:
		_Mutex& _MyMutex;
	};
}
//#endif