```cpp
#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>

using namespace std;

class DoubleGraphBufferMgr
    : public enable_shared_from_this<DoubleGraphBufferMgr> {
 public:
  DoubleGraphBufferMgr(int width, int height, GpuBufferFormat format);
  ~DoubleGraphBufferMgr();
  GlTextureBufferSharedPtr GetDrawBuffer();
  GlTextureBufferSharedPtr GetCacheBuffer();
  int Width() { return width_; }
  int Height() { return height_; }

 private:
  void CreateBuffer();
  void EmplaceCacheBuffer(GlTextureBuffer* buf);
  void EmplaceDrawBuffer(GlTextureBuffer* buf);

 private:
  const int width_;
  const int height_;
  const GpuBufferFormat format_;
  const int buffer_count_ = 2;

  mutex mutex_;
  condition_variable draw_cv_;
  condition_variable cache_cv_;
  deque<unique_ptr<GlTextureBuffer>> draw_available_;
  deque<unique_ptr<GlTextureBuffer>> cache_available_;
};
}

DoubleGraphBufferMgr::DoubleGraphBufferMgr(int width, int height,
                                           GpuBufferFormat format)
    : width_(width), height_(height), format_(format) {
  CreateBuffer();
}

DoubleGraphBufferMgr::~DoubleGraphBufferMgr() {}

GlTextureBufferSharedPtr DoubleGraphBufferMgr::GetDrawBuffer() {
  unique_lock<mutex> lock(mutex_);

  unique_ptr<GlTextureBuffer> buffer;
  while (draw_available_.empty()) {
    draw_cv_.wait(lock);
  }

  buffer = move(draw_available_.front());
  draw_available_.pop_front();
  buffer->Reuse();

  weak_ptr<DoubleGraphBufferMgr> weak_mgr(shared_from_this());
  return shared_ptr<GlTextureBuffer>(buffer.release(),
                                     [weak_mgr](GlTextureBuffer* buf) {
                                       auto mgr = weak_mgr.lock();
                                       if (mgr) {
                                         mgr->EmplaceCacheBuffer(buf);
                                       } else {
                                         delete buf;
                                       }
                                     });
}

GlTextureBufferSharedPtr DoubleGraphBufferMgr::GetCacheBuffer() {
  unique_lock<mutex> lock(mutex_);

  unique_ptr<GlTextureBuffer> buffer;
  while (cache_available_.empty()) {
    cache_cv_.wait(lock);
  }

  buffer = move(cache_available_.front());
  cache_available_.pop_front();
  buffer->Reuse();

  weak_ptr<DoubleGraphBufferMgr> weak_mgr(shared_from_this());
  return shared_ptr<GlTextureBuffer>(buffer.release(),
                                     [weak_mgr](GlTextureBuffer* buf) {
                                       auto mgr = weak_mgr.lock();
                                       if (mgr) {
                                         mgr->EmplaceDrawBuffer(buf);
                                       } else {
                                         delete buf;
                                       }
                                     });
}

void DoubleGraphBufferMgr::CreateBuffer() {
  for (int i = 0; i < buffer_count_; i++) {
    unique_ptr<GlTextureBuffer> buffer =
        GlTextureBuffer::Create(width_, height_, format_);
    draw_available_.emplace_back(buffer.release());
  }
}

void DoubleGraphBufferMgr::EmplaceCacheBuffer(GlTextureBuffer* buf) {
  lock_guard<mutex> lock(mutex_);
  cache_available_.emplace_back(buf);
  cache_cv_.notify_all();
}

void DoubleGraphBufferMgr::EmplaceDrawBuffer(GlTextureBuffer* buf) {
  lock_guard<mutex> lock(mutex_);
  draw_available_.emplace_back(buf);
  draw_cv_.notify_all();
}
```

这段代码实现了一个双缓冲区管理器(DoubleGraphBufferMgr)类，用于管理两个纹理缓冲区对象，提供获取绘制缓冲区(GetDrawBuffer())和缓存缓冲区(GetCacheBuffer())的接口。

当需要进行绘制时，调用GetDrawBuffer()方法可以获取一个绘制缓冲区对象，进行渲染操作。当绘制操作完成后，调用EmplaceCacheBuffer()方法将该缓冲区对象置于缓存队列中，等待下一次使用。

当需要进行离线处理时，调用GetCacheBuffer()方法可以获取一个缓存缓冲区对象，进行离线处理操作。当处理操作完成后，调用EmplaceDrawBuffer()方法将该缓冲区对象置于绘制队列中，等待下一次绘制使用。

使用双缓冲区可以避免绘制和离线处理操作之间的竞争条件，从而提高了程序的效率。在这个双缓冲区管理器中，利用互斥锁(mutex)和条件变量(condition_variable)实现了线程间同步。同时，还使用了智能指针(shared_ptr和weak_ptr)来管理缓冲区对象的生命周期，确保在对象不再被使用时能够正确地释放资源。

关键点：draw_available_在一开始开辟2个空间的buffer，保存其指针，cache_available_在一开始为0。当draw_available其中一块buffer被pop_front使用后，draw_available内的buffer指针析构，在删除器中将其buffer指针传递到cache_available_中，此时draw_available和cache_available_的buffer指针数量都是1，循环往复该过程。