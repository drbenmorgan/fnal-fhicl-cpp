// ======================================================================
//
// fhicl-expand: read/write specified text, replacing each #include
//               directive with the corresponding file's contents
//
//  If called with no arguments, fhicl-expand reads from standard in.
//  If called with one or more arguments, each argument is
//  interepreted as the name of a file to be processed.
//
//  By default, the expanded inputs are all written to standard
//  output, and any errors during processing are written to standard
//  error. The -o (or --output) and -e (or --error) options can be
//  used to send output to the named files.
//  ======================================================================

#include "boost/program_options.hpp"
#include "cetlib/exception.h"
#include "cetlib/include.h"
#include <fstream>
#include <iostream>
#include <istream>
#include <string>
#include <vector>

namespace bpo = boost::program_options;
using std::cerr;
using std::cin;
using std::cout;
using std::ifstream;
using std::istream;
using std::ofstream;
using std::ostream;
using std::string;
using std::vector;

typedef vector<string> strings;

// ----------------------------------------------------------------------

static  string const &
fhicl_env_var( )
{
  static  string const  fhicl_env_var("FHICL_FILE_PATH");
  return fhicl_env_var;
}


// ----------------------------------------------------------------------

int
do_including( istream & from, ostream & to, ostream& err, 
	      const char* this_filename )
  try
    {
      string result;
      cet::include(from, fhicl_env_var(), result);
      to << result;
      return 0;
    }
  catch( cet::exception const & e )
    {
      err << "Error for file: " << this_filename
	  << "\n" << e.what() << '\n';
      return 1;
    }  // do_including()

// ----------------------------------------------------------------------
int
main( int argc, char* argv[] )
{
  //
  // Use the Boost program options library to deal with parsing the
  // command line
  //
  bpo::options_description desc("fhicl-expand <options> [files]\nOptions");
  string  error_filename;
  string  output_filename;
  strings input_filenames;

  desc.add_options()
    ("help,h",                          "produce help message")
    ("error,e",  bpo::value<string>(&error_filename),  "error file")
    ("output,o", bpo::value<string>(&output_filename),  "output file")
    ("inputs,i", bpo::value<strings>(&input_filenames), "input files");

  bpo::positional_options_description pd;
  pd.add("inputs", -1);
 
  bpo::variables_map varmap;
  try
    {
      bpo::store(bpo::command_line_parser(argc, argv)
		 .options(desc)
		 .positional(pd)
		 .run(),
		 varmap);
      bpo::notify(varmap);
    }
  catch (bpo::error& err)
    {
      cerr << "Error processing command line in " 
		<< argv[0]
		<< ": " << err.what() << '\n';
      return 1;
    };

  if (varmap.count("help"))
    {
      cout << desc << "\n";
      return 0;
    }

  //
  // Now we have the arguments dealt with, do the real work
  //

  int nfailures = 0;

  ofstream errfile(error_filename.c_str());
  ostream& err = error_filename.empty() ? cerr : errfile;
  ofstream outfile(output_filename.c_str());
  ostream& out = output_filename.empty() ? cout : outfile;

  if (input_filenames.empty())
    nfailures += do_including(cin, out, err, "<cin>");
  else
    {
      for (size_t k = 0; k != input_filenames.size(); ++k)
	{
	  const char* this_filename = input_filenames[k].c_str();
	  ifstream  input(this_filename);
	  nfailures += do_including(input, out, err, this_filename);
	}
    };

  return nfailures;

}  // main()
