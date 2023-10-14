#include <Espert.hh>

class CrazyLayer : public esp::Layer
{
  virtual ~CrazyLayer() override {}

  virtual void attach() override {}

  virtual void detach() override {}

  virtual void update() override {}

    virtual void handle_event(esp::Event &event) override {
        esp::Event::
            try_hanlder<esp::KeyPressedEvent>(event, ESP_BIND_EVENT_FOR_FUN(CrazyLayer::my_crazy_event_handler));
    }

    bool my_crazy_event_handler(esp::KeyPressedEvent &event)
    {
        // implementation ...
        ESP_INFO("You pressed the button {0}", event.get_code());

    /*
        true - propagate to lower layers
        false - this is the last layer of this event
    */
    return true;
  }
};

class SandBoxApp : public esp::Application
{
 public:
  SandBoxApp() { push_layer(new CrazyLayer()); }
};

esp::Application *esp::createAppInstance()
{
    return new SandBoxApp();
}
