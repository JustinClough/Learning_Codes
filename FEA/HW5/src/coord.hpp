#ifndef COORD_HPP
#define COORD_HPP

class coord
{
  public:
    // Constuctor
    coord( double x_, double y_);

    double get_x();

    double get_y();

  private:
    double x;
    double y;

};

#endif
