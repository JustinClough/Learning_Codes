#include "printer.hpp"

#include <iostream>
#include <cmath>

Printer::Printer()
{
  entries = 0;
  tab = "  ";
}

Printer::~Printer()
{
  mesh_sizes.clear();
  L2.clear();
  Linf.clear();
  H1.clear();

  L2_order.clear();
  Linf_order.clear();
  H1_order.clear();
}

void Printer::add( int N, double L2_, double Linf_, double H1_)
{
  mesh_sizes.push_back( N);
  L2.push_back( L2_);
  Linf.push_back( Linf_);
  H1.push_back( H1_);

  entries++;

  return;
}

void Printer::calc_order(
                std::vector< double>& errors,
                std::vector< double>& orders)
{
  double divlog2 = 1.0 / std::log( 2.0);

  double now = 0.0;
  double old = 0.0;
  double ans = 0.0;
  for( size_t i = 0; i < errors.size(); i++)
  {
    if( i != 0)
    {
      old = errors[i-1];
      now = errors[i];

      ans = std::log( old / now) * divlog2;
    }

    orders.push_back( ans);
  }

  return;
}

void Printer::calc_orders()
{
  calc_order( L2, L2_order);
  calc_order( Linf, Linf_order);
  calc_order( H1, H1_order);

  return;
}

void Printer::pad( int p)
{
  for( int i = 0; i < p; i++)
  {
    std::cout << " ";
  }
  return;
}

void Printer::print_table_header()
{
  std::cout << "L2"    ; 
  pad( 12);
  std::cout << "L2_o"  ; 
  pad( 10);
  std::cout << "Linf"  ;
  pad( 10);
  std::cout << "Linf_o";
  pad( 8);
  std::cout << "H1"    ;
  pad( 12);
  std::cout << "H1_o"  ;
  pad( 10);

  std::cout << std::endl;
  return;
}

void Printer::print_row( int i)
{
  std::cout <<

    L2[i] << tab << L2_order[i]
    << tab <<
    Linf[i] << tab << Linf_order[i]
    << tab <<
    H1[i] << tab << H1_order[i]

    << std::endl;


  return;
}

void Printer::print()
{
  calc_orders();

  std::cout << std::scientific;

  print_table_header();
  for( int i = 0; i < entries; i++)
  {
    print_row( i);
  }
  std::cout << std::endl;

  return;
}
