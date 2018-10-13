#pragma once 

template <typename lock_t>
class ref_shared_lock_t
{
public:
	ref_shared_lock_t(lock_t& lock) : m_lock(lock)
	{
		m_lock.acquireLockShared();
	}
	~ref_shared_lock_t()
	{
		m_lock.releaseLockShared();
	}
private:
	lock_t& m_lock;
	ref_shared_lock_t& operator=(const ref_shared_lock_t&); //C4512 warning
};

template <typename lock_t>
class ref_exclusive_lock_t
{
public:
	ref_exclusive_lock_t(lock_t& lock) : m_lock(lock)
	{
		m_lock.acquireLockExclusive();
	}
	~ref_exclusive_lock_t()
	{
		m_lock.releaseLockExclusive();
	}
private:
	lock_t& m_lock;
	ref_exclusive_lock_t& operator=(const ref_exclusive_lock_t&); //C4512 warning
};
