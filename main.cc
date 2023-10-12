#include <Espert.hh>

class SandBoxApp : public Espert::Application
{
};

Espert::Application *Espert::createAppInstance()
{
    return new SandBoxApp();
}
