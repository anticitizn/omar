#include <src/Button.h>

Button::Button(int Width, int Height, string Name, Point Position, char FillSymbol)
: Panel(Width, Height, Name, Position, FillSymbol)
{

}

void Button::subscribe(Clickable& subscriber)
{
    subscribed.push_back(&subscriber);
}

void Button::unsubscribe(Clickable& subscriber)
{

}

void Button::onClick(Point clickPos)
{
    for (int i = 0; i < subscribed.size(); i++)
    {
        subscribed[i]->onClick(clickPos);
    }
}

void Button::onHover(Point hoverPos)
{
    for (int i = 0; i < subscribed.size(); i++)
    {
        subscribed[i]->onHover(hoverPos);
    }
}