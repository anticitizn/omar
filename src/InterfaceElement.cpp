#include <src/InterfaceElement.h>

InterfaceElement::InterfaceElement(int Width, int Height, string Name, Point Position=Point(0,0), char FillSymbol = ' ') : TileContainer(Width, Height, FillSymbol)
{
	name = Name;
	position = Position;
	parent = NULL;
}

void InterfaceElement::addChild(InterfaceElement* child)
{
	children.push_back(child);
	child->setParent(this);
}

void InterfaceElement::removeChild(Point pos)
{
	for (int i = 0; i < children.size(); i++)
	{
		Point upperLeft(children[i]->getPosition().x, children[i]->getPosition().y);
		Point lowerRight(upperLeft.x + children[i]->getDimensions().x, upperLeft.y + children[i]->getDimensions().y);
		if (pos.x >= upperLeft.x && pos.x <= lowerRight.x && pos.y >= upperLeft.y && pos.y <= lowerRight.y)
		{
			children.erase(children.begin() + i);
			i--;
		}
	}
}

void InterfaceElement::removeChild(string childName)
{
	for (int i = 0; i < children.size(); i++)
	{
		if (children[i]->getName() == childName)
		{
			children.erase(children.begin() + i);
			i--;
		}
	}
}

void InterfaceElement::blitInto(TileContainer& otherContainer)
{
	for (int i = 0; i < children.size(); i++)
	{
		children[i]->blitInto(*this);
	}
	otherContainer.blit(*this, this->getPosition().x, this->getPosition().y);
}

void InterfaceElement::setParent(InterfaceElement* newParent)
{
	parent = newParent;
}

void InterfaceElement::onClick(Point clickPos)
{
	Point pos(clickPos.x - getPosition().x, clickPos.y - getPosition().y);

	for (int i = 0; i < children.size(); i++)
	{
		Point upperLeft(children[i]->getPosition().x, children[i]->getPosition().y);
		Point lowerRight(upperLeft.x + children[i]->getDimensions().x - 1, upperLeft.y + children[i]->getDimensions().y - 1);

		if (pos.x >= upperLeft.x && pos.x <= lowerRight.x && pos.y >= upperLeft.y && pos.y <= lowerRight.y)
		{
			children[i]->onClick(Point(pos.x + children[i]->getPosition().x, pos.y + children[i]->getPosition().y));
		}
	}
}

void InterfaceElement::onHover(Point hoverPos)
{
	Point pos(hoverPos.x - getPosition().x, hoverPos.y - getPosition().y);

	for (int i = 0; i < children.size(); i++)
		{
			Point upperLeft(children[i]->getPosition().x, children[i]->getPosition().y);
			Point lowerRight(upperLeft.x + children[i]->getDimensions().x, upperLeft.y + children[i]->getDimensions().y);

			if (pos.x >= upperLeft.x && pos.x <= lowerRight.x && pos.y >= upperLeft.y && pos.y <= lowerRight.y)
			{
				children[i]->onClick(Point(pos.x - children[i]->getPosition().x, pos.y - children[i]->getPosition().y));
			}
		}
}

string InterfaceElement::getName()
{
	return name;
}

void InterfaceElement::setPosition(Point newPosition)
{
	position = newPosition;
}

Point InterfaceElement::getPosition()
{
	return position;
}
