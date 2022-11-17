#include <atomic>
#include <barrier>
#include <thread>

#include <cstdint>

std::atomic_int32_t x = 0, y = 0;
std::atomic_int32_t r1, r2;
uint64_t iterationsSinceLastStoreLoad = 0;

void check()
{
	if(r1 == 0 && r2 == 0)
	{
		printf("Storeload delta %llu\n", iterationsSinceLastStoreLoad);
		iterationsSinceLastStoreLoad = 0;
	}

	++iterationsSinceLastStoreLoad;
	r1.store(25, std::memory_order_relaxed);
	r2.store(25, std::memory_order_relaxed);
	x.store(0, std::memory_order_relaxed);
	y.store(0, std::memory_order_relaxed);
}

std::barrier syncPoint{ 2, check};

void thread1()
{
	while(true)
	{
#ifdef TRIGGER_STORELOAD
		x.store(1, std::memory_order_relaxed);
#else
		x.store(1, std::memory_order_seq_cst);
#endif
		r1.store(y, std::memory_order_relaxed);
		syncPoint.arrive_and_wait();
	}
}

void thread2()
{
	while(true)
	{
#ifdef TRIGGER_STORELOAD
		y.store(1, std::memory_order_relaxed);
#else
		y.store(1, std::memory_order_seq_cst);
#endif
		r2.store(x, std::memory_order_relaxed);
		syncPoint.arrive_and_wait();
	}
}

int main()
{
	std::thread t1(thread1);
	std::thread t2(thread2);

	// Need the volatile because otherwise the compiler optimises away
	// the infinite loop and the whole program gets totally broken.
	volatile bool loop = true;
	while(loop);

	return 0;
}
