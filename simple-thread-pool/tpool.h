#ifndef __TPOOL__
#define __TPOOL__

#include <vector>
#include <thread>
#include <functional>

class tpool{

private:
	//keep thread nums running in this pool
	int thread_num;

	//keep threads in pool
	std::vector<std::thread> thread_list;
	tpool(){}
	static void dummy_f(){};
public:
	tpool(int num = 1,std::function<void()> f = dummy_f ):thread_num(num){
		for(int i=0;i<thread_num;++i)
		thread_list.push_back(std::thread(f));}
    
    tpool(int num,std::function<void(int)> f):thread_num(num){
        for (int i =0; i<thread_num;++i) {
            thread_list.push_back(std::thread(f,i));
        }
    }

	~tpool(){
		for(auto& each:thread_list){
			if(each.joinable())
			each.join();
		}
	}		

	//swap thread with another pool
	void swap(tpool& rtp){
		thread_list.swap(rtp.thread_list);
		int temp = rtp.thread_num;
		rtp.thread_num = thread_num;
		thread_num = temp;
	}

	std::vector<std::thread::id>  get_ids() const{
		std::vector<std::thread::id> ids;
		for(auto& each:thread_list){
			ids.push_back(each.get_id());
		}
		return ids;
	}

	std::thread::native_handle_type native_handle(){
			return thread_list.begin()->native_handle();
	}

	//Detach all threads.
	void detach_all(){
		for(auto& each: thread_list)
			if(each.joinable())
			each.detach();
	}

	int get_thread_count() const {
		return thread_num;
	}
};



#endif
