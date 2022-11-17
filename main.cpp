#include <atomic>
#include <barrier>
#include <thread>

#include <cstdint>

#ifdef COMPILER_BARRIER
#	define THE_BARRIER asm volatile("" ::: "memory")
#elif defined ASM_FULL_BARRIER
#	define THE_BARRIER asm volatile("mfence" ::: "memory")
#elif defined CPP11_FULL_BARRIER
#	define THE_BARRIER std::atomic_thread_fence(std::memory_order_seq_cst)
#else
#	define THE_BARRIER
#endif

int x = 0, y = 0;
int r1, r2;
uint64_t iterationsSinceLastStoreLoad = 0;

void check()
{
	if(r1 == 0 && r2 == 0)
	{
		printf("Storeload delta %llu\n", iterationsSinceLastStoreLoad);
		iterationsSinceLastStoreLoad = 0;
	}

	++iterationsSinceLastStoreLoad;
	r1 = 25;
	r2 = 25;
	x = 0;
	y = 0;
}

std::barrier syncPoint{ 2, check};

void thread1()
{
	while(true)
	{
		x = 1;
		THE_BARRIER;
		r1 = y;
		syncPoint.arrive_and_wait();
	}
}

void thread2()
{
	while(true)
	{
		y = 1;
		THE_BARRIER;
		r2 = x;
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