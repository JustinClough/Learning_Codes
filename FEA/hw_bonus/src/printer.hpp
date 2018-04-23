#ifndef PRINTER_HPP
#define PRINTER_HPP

#include <vector>
#include <string>

class Printer
{
  public:
    // Constructor
    Printer();

    // Destructor
    ~Printer();
  
    // Print collected data to screen
    void print();

    // Adds solution data
    void add( int N, double L2_, double Linf_, double H1_);

  private:
    // Amount of data collected
    int entries; 

    // Two spaces 
    std::string tab;

    // Vector of mesh sizes
    std::vector< int> mesh_sizes;

    // Vector of L2 errors
    std::vector< double> L2;

    // Vector of Linf errors
    std::vector< double> Linf;

    // Vector of H1 errors
    std::vector< double> H1;

    // Vector of L2 orders
    std::vector< double> L2_order;

    // Vector of Linf orders
    std::vector< double> Linf_order;

    // Vector of H1 orders
    std::vector< double> H1_order;

    // Calculates the orders of convergence
    void calc_orders();

    // Calcuates the order for one vector
    void calc_order(
          std::vector< double>& errors,
          std::vector< double>& orders);

    // Printers row i of the solution table
    void print_row( int i);

    // Prints the table header
    void print_table_header();

    // Pads with p spaces
    void pad( int p);
};

#endif 
