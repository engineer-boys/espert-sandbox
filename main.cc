#include <Espert.hh>

class CrazyLayer : public Espert::Layer
{
  virtual ~CrazyLayer() override {}

  virtual void attach() override {}

  virtual void detach() override {}

  virtual void update() override {}

  virtual void handle_event(Espert::Event& event) override
  {
    Espert::Event::try_hanlder<Espert::KeyPressedEvent>(
        event, ESP_BIND_EVENT_FOR_FUN(CrazyLayer::my_crazy_event_handler));
  }

  bool my_crazy_event_handler(Espert::KeyPressedEvent& event)
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

class SandBoxApp : public Espert::Application
{
  public:
  SandBoxApp() { push_layer(new CrazyLayer()); }
};

Espert::Application* Espert::createAppInstance() { return new SandBoxApp(); }
