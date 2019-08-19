#ifndef EXAMPLESRUNNER_H
#define EXAMPLESRUNNER_H


class ExamplesRunner
{
public:
    static ExamplesRunner* getInstance();

    void runMe();

    void runWargame();
    void runWeapon();


private:
    static ExamplesRunner* self;


    ExamplesRunner();
};

#endif // EXAMPLESRUNNER_H
