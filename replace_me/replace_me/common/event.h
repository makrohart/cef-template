#pragma once

#include <string>
#include <memory>
#include <functional>

/// <summary>
/// Usage:
//template<typename... Args>
//struct ExtendCallback : Callback
//{
//    ~ExtendCallback() = default;
//
//    ExtendCallback(std::function<void(Args...)> cb) : callback_(std::move(cb)) {}
//    void operator()(Args&&... args)
//    {
//        return callback_(std::forward<Args>(args)...);
//    }
//
//private:
//    std::function<void(Args...)> callback_;
//};
//int main()
//{
//    event::Events events;
//    auto id = events.on("test", [](const int a) {
//        int b = a + 1;
//    });
//    events.emit("test", 10);
//    events.off("test", std::move(id));
// 
//    event::Events<int, event::ExtendCallback> events1;
//    events1.on("test", [](const int a) {
//        int b = a + 1;
//    });
//    events1.emit(kTest, 10);
//}
/// </summary>
namespace event
{
    // ID value reserved for internal use.
    const int kReservedId = 0;

    // Helper class for generating ID values.
    template <typename T>
    class IdGenerator {
    public:
        IdGenerator() : next_id_(kReservedId) {}

        IdGenerator(const IdGenerator&) = delete;
        IdGenerator& operator=(const IdGenerator&) = delete;

        T GetNextId() {
            if (next_id_ == std::numeric_limits<T>::max()) {
                next_id_ = kReservedId;
            }
            return ++next_id_;
        }

    private:
        T next_id_;
    };

    template <typename T>
    struct function_traits : function_traits<decltype(&T::operator())> {};

    template <typename ClassType, typename ReturnType, typename... Args>
    struct function_traits<ReturnType(ClassType::*)(Args...) const> {
        using args_type = std::tuple<Args...>;
        using function_type = std::function<ReturnType(Args...)>;
    };

    struct Callback {
        virtual ~Callback() = default;
    };

    template<typename... Args>
    struct CallbackImpl : Callback {
        CallbackImpl(std::function<void(Args...)> callback) : callback_(std::move(callback)) {}
        void operator()(Args&&... args) { callback_(std::forward<Args>(args)...); }
    private:
        std::function<void(Args...)> callback_;
    };

    template<typename T = int, template<typename...> class ImplType = CallbackImpl>
    struct Events {

        template<typename F>
        T on(const std::string& eventName, F&& callback) {
            using Traits = function_traits<std::decay_t<F>>;
            return onImpl(eventName, typename Traits::function_type(std::forward<F>(callback)));
        }

        void off(const std::string& eventName, const T& id) {
            auto it = callbacks_.find(eventName);
            if (it == callbacks_.cend()) return;
            if (it->second.count(id))
                it->second.erase(id);
        }

        void off(const T& id) {
            for (auto& [eventName, callbackMap] : callbacks_)
            {
                if (callbackMap.count(id))
                    callbackMap.erase(id);
            }
        }

        template<typename... Args>
        void emit(const std::string& eventName, Args... args) {
            auto it = callbacks_.find(eventName);
            if (it == callbacks_.cend()) return;

            // std::decay for T, T&, T&&
            using TargetImpl = ImplType<std::decay_t<Args>...>;

            if (it->second.empty())
                return;

            for (auto& [id, callback] : it->second) {
                auto callbackImpl = std::dynamic_pointer_cast<TargetImpl>(callback);
                if (callbackImpl) {
                    (*callbackImpl)(std::forward<Args>(args)...);
                }
            }
        }

    private:
        template<typename... Args>
        T onImpl(const std::string& eventName, std::function<void(Args...)> callback) {
            T id = idGenerator_.GetNextId();
            auto impl = std::make_shared<ImplType<Args...>>(std::move(callback));
            callbacks_[eventName].emplace(id, impl);
            return id;
        }

        std::unordered_map<std::string, std::unordered_map<T, std::shared_ptr<Callback>>> callbacks_;
        IdGenerator<T> idGenerator_;
    };
}

