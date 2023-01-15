#include <iostream>
#include <future>
#include <thread>
#include <vector>

using namespace std;

void show_list() {}//递归终止函数
template <typename T,typename ...Args>
void show_list(T&& value,Args&& ...args){
    cout << value << endl;
    show_list(args...);
}
namespace newFeature{
    //The class template std::future provides a mechanism to access the
    // result of asynchronous operations
    // future对象提供访问异步操作结果的机制，很轻松解决从异步任务中返回结果。
    void testFuture(){
        auto f1 = std::async(std::launch::async, [](){
            std::this_thread::sleep_for(std::chrono::seconds(1));
            return 43;
        });
        cout << f1.get() << endl;
        cout << f1.valid() << endl;

        std::vector<std::future<size_t>> futures;
        for (size_t i = 0; i < 10; ++i) {
            futures.emplace_back(std::async(std::launch::async, [](size_t param){
                std::this_thread::sleep_for(std::chrono::seconds(param));
                return param;
            }, i));
        }

        std::cout << "Start querying" << std::endl;

        for (auto &future : futures) {
            std::cout << future.get() << std::endl;
        }
    }
}

int main(int argc,char *argv[])
{
//    show_list(1,123,"qqq");
    newFeature::testFuture();
    return 0;
}