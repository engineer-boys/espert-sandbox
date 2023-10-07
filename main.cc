#include <Esper.hh>

class SandBoxApp : public Esper::Application
{
};

Esper::Application *Esper::createAppInstance()
{
    return new SandBoxApp();
}
