#include <iostream>
#include <coroutine>
#include <thread>

using namespace std;

//
//  Task class for C++ 20 coroutine
//
template <typename T>
struct Task
{
    struct promise_type;
    using co_handle = std::coroutine_handle<promise_type>;
    co_handle _handle;

    struct promise_type
    {
        T value;
        std::coroutine_handle<> _prev = nullptr; // previous promise

        Task get_return_object() { return Task{co_handle::from_promise(*this)}; }

        std::suspend_never initial_suspend() { return {}; }

        std::suspend_never final_suspend() noexcept
        {
            if (_prev)
                _prev.resume();

            return {};
        }

        void unhandled_exception() { std::rethrow_exception(std::current_exception()); }

        std::suspend_never return_value(T v)
        {
            value = v;
            return {};
        }
    };

    T get() const { return _handle.promise().value; }

    //
    // Chain calling
    //

    auto operator co_await()
    {
        struct Awaitor
        {
            Task &task;

            bool await_ready() const noexcept { return false; }
            T await_resume() const noexcept { return task.get(); }

            // template <typename PROMISE>
            void await_suspend(std::coroutine_handle<> h)
            {
                task._handle.promise()._prev = h;
            }
        };

        return Awaitor{*this};
    }
};

template <>
struct Task<void>
{
    struct promise_type;
    using co_handle = std::coroutine_handle<promise_type>;
    co_handle _handle;

    struct promise_type
    {
        std::coroutine_handle<> _prev = nullptr; // previous promise

        Task<void> get_return_object()
        {
            return Task{co_handle::from_promise(*this)};
            ;
        }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept
        {
            if (_prev)
                _prev.resume();

            return {};
        }
        void return_void() {}
        void unhandled_exception() { std::rethrow_exception(std::current_exception()); }
    };

    //
    // Chain calling
    //

    auto operator co_await()
    {
        struct Awaitor
        {
            Task<void> &task;

            bool await_ready() const noexcept { return false; }
            void await_resume() const noexcept {}

            // template <typename PROMISE>
            void await_suspend(std::coroutine_handle<> h)
            {
                task._handle.promise()._prev = h;
            }
        };

        return Awaitor{*this};
    }
};

auto async_running(std::jthread &t)
{
    struct Awaitor
    {
        std::jthread &t;

        bool await_ready() const noexcept { return false; }
        int await_resume() const noexcept { return 1; }
        void await_suspend(std::coroutine_handle<> h)
        {
            t = std::jthread([h]()
                             { h.resume(); });
        }
    };

    return Awaitor{t};
}

Task<int> foo3(std::jthread &t)
{
    auto r = co_await async_running(t);

    cout << this_thread::get_id() << " async thread id" << endl;

    co_return r;
}

Task<int> foo2(std::jthread &t)
{
    auto r = co_await foo3(t);

    cout << "r = " << r << ", " << this_thread::get_id() << " async thread id" << endl;

    co_return r + 1;
}

Task<void> foo1(std::jthread &t)
{
    int a = co_await foo2(t);

    cout << "a = " << a << ", " << this_thread::get_id() << " async thread id" << endl;
}

Task<void> foo(std::jthread &t)
{
    co_await foo1(t);

    cout << this_thread::get_id() << " async thread id" << endl;
}

int run_coroutine_test(int argc, char *argv[])
{
    using namespace std;
    jthread worker;

    cout << this_thread::get_id() << " main thread id" << endl;

    foo(worker);

    return 0;
}