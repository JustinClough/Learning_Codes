#include "cypher_class.hpp"

Cypher::Cypher()
{
  wheel = new Wheel();

}

Cypher::~Cypher()
{
  delete wheel;

}

void Cypher::set_base( char* base_)
{
  base = base_;
  sanatize_base();
  return;
}

void Cypher::set_target( char* target_)
{
  target = target_;
  return;
}

void Cypher::set_result( char* result_)
{
  result = result_;
  return;
}

std::string Cypher::get_file_contents(const char* filename)
{
  std::ifstream in(filename, std::ios::in | std::ios::binary);
  if (in)
  {
    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return(contents);
  }
  throw(errno);
}

void Cypher::print_status()
{
  std::cout 
  << "Using Base: " << base
  << std::endl
  << "Using target: " << target
  << std::endl
  << "Using result: " << result
  << std::endl;

  wheel->print_wheel();

  return;
}

void Cypher::operate()
{
  str pt = get_file_contents( target.c_str());
  str enc = encrypt( pt);
  write_to_file( enc);

  return;
}

str Cypher::get_safe_letters()
{
  str safe = " ";
  safe += "!";
  safe += "?";
  safe += ",";
  safe += ".";
  safe += "\n";
  safe += "\"";
  safe += "\'";
  safe += "*";
  safe += "-";
  safe += "_";
  safe += ":";
  safe += "+";
  safe += ";";
  safe += "(";
  safe += ")";
  return safe;  
}

bool Cypher::encryptable( char letter)
{
  str sf = get_safe_letters();
  for( unsigned int i=0 ; i < sf.size(); i++)
  {
    if( letter == sf[i])
    {
      return false;
    }
  }

  return true;
}

str Cypher::encrypt( str pt)
{
  int b = pt.size();
  str enc;

  for( int k = 0; k < b; k++)
  {
    char letter_in = pt[k];
    char letter_out;
    if( encryptable( letter_in))
    {
      letter_out = wheel->get_letter( letter_in, working_base, k);
    }
    else
    {
      letter_out = letter_in;
    }
    enc += letter_out;
  }

  return enc;
}

void Cypher::sanatize_base()
{
  for( unsigned int i=0; i < base.size(); i++)
  {
    if( !encryptable( base[i]))
    {
      working_base += 'a';
    }
    else if( std::isupper( base[i]))
    {
      working_base += std::tolower( base[i]);
    }
    else
    {
      working_base += base[i];
    }
  }   

  return;
}

void Cypher::write_to_file( str enc)
{
  std::ofstream file( result);
  file << enc;
  file.close();

  return;
}
