//
// Created by adria on 04/07/2020.
//

#ifndef GAMEENGINE_DELAYEDSETTER_HPP
#define GAMEENGINE_DELAYEDSETTER_HPP
namespace GameEngine {
namespace Internal {

// This class is used internally to help update a value when we want,
// but always return the last value that it has been set
template<typename T>
class DelayedSetter
{
    T currentValue_;
    T newValue_;
    bool needUpdate_;
public:
    DelayedSetter() :
        currentValue_(T()),
        newValue_(T()),
        needUpdate_(false)
    {};

    void setNewValue(T newValue)
    {
        newValue_ = newValue;
        needUpdate_ = true;
    }

    bool update()
    {
        if (needUpdate_)
        {
            currentValue_ = newValue_;
            newValue_ = T();
            needUpdate_ = false;

            return true;
        }
        return false;
    }

    T get() const
    {
        return needUpdate_ ? newValue_ : currentValue_;
    }

    bool hasChanges() const
    {
        return needUpdate_;
    }

    void operator= (const T &value)
    {
        setNewValue(value);
    }
};
}
}

#endif //DANGEROUSROBOTSINTHEDESERT_DELAYEDSETTER_HPP
