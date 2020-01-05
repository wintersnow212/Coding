#include <atomic>
#include <chrono>
#include <cstddef>
#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <set>
#include <string>
#include <thread>
#include <utility>
#include <vector>
#include <condition_variable>
 
// 支持多个Producer/Consumder的简单阻塞队列
template <typename T>
class BlockingQueue {
 public:
  explicit BlockingQueue(size_t capacity) : capacity_(capacity) {}
 
  std::size_t Size() const {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    return internal_queue_.size();
  }
 
    // 向队列中添加一个元素，如果当前队列大小等于’capacity_‘的话则阻塞等待
    // 成功添加则返回true，持续等待'timeout_ms'毫秒后仍无法添加则返回false
    template <typename U>
     bool PushWithTimeout(U &&element, std::int64_t timeout_ms) 
    {
    std::unique_lock<std::mutex> lock(queue_mutex_);
    if (internal_queue_.size() >= capacity_) 
    {
      queue_nonfull_condition_.wait_for(
          lock, std::chrono::milliseconds(timeout_ms),
          [&]{ return internal_queue_.size() < capacity_; });
        
      if (internal_queue_.size() >= capacity_) 
      {
            return false;
      }
    }
    internal_queue_.emplace(std::forward<U>(element));
    lock.unlock();
    queue_nonempty_condition_.notify_one();
    return true;
  }
 
  // 从队列中取出一个元素，如果当前队列为空的话则阻塞等待
  // 持续等待'timeout_ms'毫秒后则返回std::nullopt
  std::optional<T> PopWithTimeOut(std::int64_t timeout_ms) {
      
    std::unique_lock<std::mutex> lock(queue_mutex_);
    if (internal_queue_.empty()) {
      queue_nonempty_condition_.wait_for(
          lock, std::chrono::milliseconds(timeout_ms),
          [&]{ return !internal_queue_.empty(); });
      if (internal_queue_.empty()) {
        return std::nullopt;
      }
    }
    T value(std::move(internal_queue_.front()));
    internal_queue_.pop();
    lock.unlock();
    queue_nonfull_condition_.notify_one();
    return value;
  }
 
 private:
  const size_t capacity_;
  std::queue<T> internal_queue_;
  mutable std::mutex queue_mutex_;
  std::condition_variable queue_nonempty_condition_;
  std::condition_variable queue_nonfull_condition_;
};
 
using ValueType = std::string;
 
// 默认等待100ms为超时状态
constexpr std::int64_t kDefaultTimeout = 100;
 
// 如果每生成一个数据就发送给阻塞队列的话，会有严重的lock contention，性能还不如单线程
// 所以实际中一般是一个batch一个batch的发，下面这个参数可以控制batch大小，比如可以设置成1000
// 这里为了方便阅读程序的输出只设置成1
constexpr size_t kMaxBatchSize = 1;
 
using DataBatch = std::unique_ptr<std::vector<ValueType>>;
 
// 生成数据的线程
class StreamingWorker {
 public:
  // 每秒生成'messages_per_second'条数据
  StreamingWorker(size_t worker_id, std::int64_t messages_per_second,
                  BlockingQueue<DataBatch>* blocking_queue)
      : worker_id_(worker_id),
        interval_ms_(1000 / messages_per_second),
        blocking_queue_(blocking_queue),
        running_(false) {}
 
  // 创建线程并运行
  void Start() {
    running_.store(true);
    thread_ = std::make_unique<std::thread>([this] {
      this->Run();
    });
  }
 
  // 结束线程
  void Stop() {
    running_.store(false);
    thread_->join();
    thread_ = nullptr;
  }
 
 private:
  // 不断生成数据并批量添加到共享数据队列中
  void Run() {
    DataBatch batch = NewBatch();
    while (running_) {
      if (batch->size() >= kMaxBatchSize) {
        // 这里使用了timeout机制避免线程block后无法正常终止
        if (!blocking_queue_->PushWithTimeout(std::move(batch),
                                              kDefaultTimeout)) {
          continue;
        }
        batch = NewBatch();
      }
      ++message_id_;
      const std::string message =
          static_cast<char>('A' + worker_id_) + std::to_string(message_id_);
      // 注：这里用字符串拼接而不是输出流是为了避免多线程情况下的输出串流
      std::cout << ("Streaming worker " + std::to_string(worker_id_) + " is "
                    "generating message " + message + "\n");
      batch->push_back("message " + message + " from worker " +
                       std::to_string(worker_id_));
      std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms_));
    }
    std::cout << ("Streaming worker " + std::to_string(worker_id_) +
                  " is stopping\n");
  }
 
  static DataBatch NewBatch() {
    DataBatch batch = std::make_unique<std::vector<ValueType>>();
    batch->reserve(kMaxBatchSize);
    return batch;
  }
 
 private:
  const size_t worker_id_;
  const std::int64_t interval_ms_;
  BlockingQueue<DataBatch>* const blocking_queue_;
  std::unique_ptr<std::thread> thread_;
  std::atomic<bool> running_;
  size_t message_id_ = 0;
};
 
// 很多情况下，从数据源获得的数据需要根据一定方式分割（比如hash值，key range等等）后
// 再写入到不同的文件，每个文件称作一个shard。
//
// 这里为了简单就不做sharding（也叫做partitioning、shuffling）操作了，
// 也不写入文件而是直接打印。
class MaterializationWorker {
 public:
  // 每秒打印'batches_per_second'个batch的数据
  MaterializationWorker(size_t worker_id, std::int64_t batches_per_second,
                        BlockingQueue<DataBatch>* blocking_queue)
      : worker_id_(worker_id),
        interval_ms_(1000 / batches_per_second),
        blocking_queue_(blocking_queue),
        state_(INIT) {}
 
  // 创建线程并运行
  void Start() {
    state_.store(RUNNING);
    thread_ = std::make_unique<std::thread>([this] {
      this->Run();
    });
  }
 
  // 结束线程
  void Stop() {
    state_.store(TERMINATED);
    thread_->join();
    thread_ = nullptr;
  }
 
  // 处理完数据队列中的所有数据。然后结束线程
  void DrainAndStop() {
    state_.store(DRAINING);
    thread_->join();
    thread_ = nullptr;
  }
 
 private:
  void Run() {
    while (true) {
      if (state_ == TERMINATED) break;
      if (state_ == DRAINING && blocking_queue_->Size() == 0) break;
      const std::optional<DataBatch> batch =
          blocking_queue_->PopWithTimeOut(kDefaultTimeout);
      if (!batch.has_value()) {
        continue;
      }
      for (const ValueType& value : **batch) {
        std::cout << ("Materialization worker " + std::to_string(worker_id_) +
                      " is writing " + value + " to file\n");
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms_));
    }
    std::cout << ("Materialization worker " + std::to_string(worker_id_) +
                  " is stopping\n");
  }
 
  enum State {
    INIT = 0,
    RUNNING = 1,
    DRAINING = 2,
    TERMINATED = 3
  };
 
 
  const size_t worker_id_;
  const std::int64_t interval_ms_;
  BlockingQueue<DataBatch>* const blocking_queue_;
  std::unique_ptr<std::thread> thread_;
  std::atomic<State> state_;
};
 
int main(int argc, char* argv[]) {
  // 生成数据的线程数量
  const size_t kNumStreamingWorkers = 3;
 
  // 每秒每线程数据生成速度
  const std::int64_t kStreamingRate = 2;
 
  // 写入文件的线程数量
  const size_t kNumMaterializationWorkers = 2;
 
  // 每秒每线程数据写入速度
  const std::int64_t kBatchMaterializationRate = 1;
 
  // 共享队列大小
  const std::size_t kBlockingQueueCapacity = 4;
 
 
  BlockingQueue<DataBatch> blocking_queue(kBlockingQueueCapacity);
 
  std::int64_t worker_id = 0;
 
  std::vector<std::unique_ptr<StreamingWorker>> streaming_workers;
  std::vector<std::unique_ptr<MaterializationWorker>> materialization_workers;
 
  // 创建并运行数据生成线程
  for (size_t i = 0; i < kNumStreamingWorkers; ++i) {
    streaming_workers.emplace_back(
        std::make_unique<StreamingWorker>(
            worker_id++, kStreamingRate, &blocking_queue));
    streaming_workers.back()->Start();
  }
 
  // 创建并运行写文件线程
  for (size_t i = 0; i < kNumMaterializationWorkers; ++i) {
    materialization_workers.emplace_back(
        std::make_unique<MaterializationWorker>(
            worker_id++, kBatchMaterializationRate, &blocking_queue));
    materialization_workers.back()->Start();
  }
 
  // 等待10秒
  std::this_thread::sleep_for(std::chrono::seconds(10));
 
  // 终止所有数据生成线程
  for (auto& worker : streaming_workers) {
    worker->Stop();
  }
 
  // 终止所有写文件线程
  for (auto& worker : materialization_workers) {
    worker->DrainAndStop();
  }
 
  return 0;
}
