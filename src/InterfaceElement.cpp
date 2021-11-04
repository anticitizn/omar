#include <src/InterfaceElement.h>

InterfaceElement::InterfaceElement(int Width, int Height, char FillSymbol, InterfaceElement* Parent = NULL) : TileContainer(Width, Height, FillSymbol)
{
	
}

void InterfaceElement::blit(TileContainer& otherContainer, int xOffset, int yOffset)
{
	// blit has to be reserved - paste itself into other containers
	// blit all children onto itself
	for (int i = 0; i < children.size(); i++)
	{
		
	}
	
	TileContainer::blit(otherContainer, xOffset, yOffset);
}

void InterfaceElement::setPosition(Point newPosition)
{
	
}

Point InterfaceElement::getPosition()
{
	
}
