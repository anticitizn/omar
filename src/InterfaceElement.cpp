#include <src/InterfaceElement.h>

InterfaceElement::InterfaceElement(int Width, int Height, string Name, char FillSymbol = ' ', InterfaceElement* Parent = NULL, Point Position=Point(0,0)) : TileContainer(Width, Height, FillSymbol)
{
	position = Position;
}

void InterfaceElement::blitInto(InterfaceElement& otherContainer)
{
	for (int i = 0; i < children.size(); i++)
	{
		children[i].blitInto(*this);
	}
	otherContainer.blit(*this, this->getPosition().x, this->getPosition().y);
}

void InterfaceElement::setPosition(Point newPosition)
{
	position = newPosition;
}

Point InterfaceElement::getPosition()
{
	return position;
}
