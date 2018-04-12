#ifndef ELEMENT_HPP
#define ELEMENT_HPP

class Element
{
  public:
    // Constructor. Builds an element out of two 
    //  nodes, L and R at locations xL and xR.
    Element( int L, double xL, int R, double xR);

    // Destructor
    ~Element();

    // Gets the length of the element
    double get_length();

  private:
    // Index of the left most node
    int left;

    // Index of the right most node
    int right;

    // Location of the left most node
    double left_pos;

    // Location of the right most node
    double right_pos;

    // The length of the element
    double length;

    // Calculates the length of the element
    void calc_length();

};

#endif
