#ifndef SINGLETON_H
#define SINGLETON_H

template<typename T>
class Singleton
{
  public:
    static T* instance()
    {
        static T object;
        return &object;
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

protected:
    Singleton() {}
private:
    struct derived_from_T : public T
    {
        derived_from_T() : T() {}
    };
};

#endif // SINGLETON_H
