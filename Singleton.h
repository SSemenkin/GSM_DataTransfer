#ifndef SINGLETON_H
#define SINGLETON_H

template<typename T>
class Singleton
{
  public:
    static T* instance()
    {
        static tInstance object;
        return &object;
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

protected:
    Singleton() {}
private:
    struct tInstance : public T
    {
        tInstance() : T() {}
    };
};

#endif // SINGLETON_H
