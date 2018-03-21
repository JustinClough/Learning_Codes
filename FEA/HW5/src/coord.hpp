#ifndef COORD_HPP
#define COORD_HPP

class coord
{
  public:
    // Constuctor
    coord( double x_, double y_);

    // Get the x component
    double get_x();

    // Get the y component
    double get_y();

    // Print coordinate information to std::cout
    void   print();

  private:
    double x;
    double y;

};

#endif
