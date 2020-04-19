#ifndef CONTAINER_H
#define CONTAINER_H

#include <vector>

#include <GUI/widget.hpp>

namespace GUI
{

class Container : public Widget
{
public:
    typedef std::shared_ptr<Container> Ptr;

public:
    Container();

    void            pack(Widget::Ptr widget);

    virtual bool    isSelectable() const;
    virtual void    handleEvent(const sf::Event &event);
    void update(sf::Time dt) override;

private:
    virtual void    draw(sf::RenderTarget &target, sf::RenderStates states) const;

    bool            hasSelection() const;
    void            select(std::size_t index);
    void            selectNext();
    void            selectPrevious();

private:
    std::vector<Widget::Ptr>    mChildren;
    int                         mSelectedChild;
    sf::Time                    mJoystickCooldown = sf::Time::Zero;
};

}

#endif // CONTAINER_H
