#ifndef ELEMENT1D_HPP
#define ELEMENT1D_HPP

class elem
{

  public:
    // Gets the length of an elment.
    double getLength();

    // Gets the left Node Position.
    double getLeftPos();

    // Gets the right Node Position.
    double getRightPos();

    elem( double leftPos_, double rightPos_);

  private:
    // The length of the element.
    double length;

    // The coordinate of the left node.
    double leftPos; 

    // The coordinate of the right node.
    double rightPos;
};

#endif
