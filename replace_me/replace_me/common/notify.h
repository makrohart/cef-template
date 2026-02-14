#pragma once
#include <memory>

namespace notify
{
    struct IObserver;
    struct INotifier
    {
        struct IArgs
        {
            virtual ~IArgs() = default;
        };

        virtual ~INotifier() = default;
        virtual void add(std::shared_ptr<IObserver> observer) = 0;
        virtual void remove(std::shared_ptr<IObserver> observer) = 0;
        virtual void notify(std::shared_ptr<IArgs> args) = 0;
    };

    struct IObserver
    {
        virtual ~IObserver() = 0;
        virtual void onNotify(std::shared_ptr<INotifier::IArgs> args) = 0;
    };
}