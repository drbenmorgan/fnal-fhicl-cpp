// ======================================================================
//
// fhicl-expand: read/write specified text, replacing each #include
//               directive with the corresponding file's contents
//
//  If called with no arguments, fhicl-expand reads from standard in.
//  If called with one or more arguments, each argument is
//  interepreted as the name of a file to be processed. If '-' (a
//  single hyphen) is passed as a filename, this is interpreted as
//  instruction to read from standard input.
//
//  By default, the expanded inputs are all written to standard
//  output, and any errors during processing are written to standard
//  error. The -o (or --output) and -e (or --error) options can be
//  used to send output to the named files.
//  ======================================================================

#include "boost/program_options.hpp"
#include "cetlib/exception.h"
#include "cetlib/filepath_maker.h"
#include "cetlib/includer.h"
#include "cpp0x/memory"
#include "cpp0x/string"
#include <fstream>
#include <iostream>
#include <istream>
#include <vector>

// ----------------------------------------------------------------------

static  std::string const &
  fhicl_env_var( )
{
  static  std::string const  fhicl_env_var("FHICL_FILE_PATH");
  return fhicl_env_var;
}

// ----------------------------------------------------------------------

int
  do_including( std::string const   & starting_filename
              , cet::filepath_maker & lookup_policy
              , std::ostream        & to
              , std::ostream        & err
              )
try
{
  cet::includer inc(starting_filename, lookup_policy);
  cet::includer::const_iterator it  = inc.begin()
                              , end = inc.end();
  std::ostream_iterator<char> out(to);

  std::copy(it, end, out);
  return 0;
}
catch( cet::exception const & e )
{
  err << "Error for file: " << starting_filename
      << '\n' << e.what() << '\n';
  return 1;
}  // do_including()

// ----------------------------------------------------------------------

int
  main( int argc, char* argv[] )
{
  using namespace std;
  namespace bpo = boost::program_options;

  typedef vector<string> strings;

  // Parse the command line:

  bpo::options_description desc("fhicl-expand <options> [files]\nOptions");
  string  error_filename;
  string  output_filename;
  strings input_filenames;
  int     lookup_policy;
  string  lookup_path;

  desc.add_options()
    ( "help,h"  , "produce help message"
                )
    ( "inputs,i", bpo::value<strings>(&input_filenames)
                , "input files"
                )
    ( "output,o", bpo::value<string >(&output_filename)
                , "output file"
                )
    ( "error,e" , bpo::value<string >(&error_filename)
                , "error file"
                )
    ( "lookup-policy,l"
                , bpo::value<int>(&lookup_policy)
                  ->default_value(1)
                , "lookup policy code:"
                  "\n  0 => cet::filepath_maker"
                  "\n  1 => cet::filepath_lookup"
                  "\n  2 => cet::filepath_lookup_nonabsolute"
                  "\n  3 => cet::filepath_lookup_after1"
                )
    ( "path,p"  , bpo::value<string>(&lookup_path)
                  ->default_value(fhicl_env_var())
                , "path or environment variable to be used by lookup-policy"
                )
    ;

  bpo::positional_options_description pd;
  pd.add("inputs", -1);

  bpo::variables_map varmap;
  try {
    bpo::store( bpo::command_line_parser(argc, argv).options(desc)
                                                    .positional(pd)
                                                    .run()
              , varmap
              );
    bpo::notify(varmap);
  }
  catch (bpo::error& err) {
    cerr << "Error processing command line in " << argv[0]
         << ": " << err.what() << '\n';
    return 1;
  };

  // Interpret options:

  if( varmap.count("help") ) {
    cout << desc << "\n";
    return 0;
  }

  if( input_filenames.empty() )
    input_filenames.push_back("-");

  ofstream  outfile(output_filename.c_str());
  ostream & out = output_filename.empty() ? cout : outfile;

  ofstream  errfile(error_filename.c_str());
  ostream & err = error_filename.empty() ? cerr : errfile;

  std::unique_ptr<cet::filepath_maker> policy;

cerr << "Policy is " << lookup_policy
     << "; path is \"" << lookup_path << "\"\n";
  switch( lookup_policy ) {
    case 0:   policy.reset( new cet::filepath_maker );
              break;
    case 1:   policy.reset( new cet::filepath_lookup(lookup_path) );
              break;
    case 2:   policy.reset( new cet::filepath_lookup_nonabsolute(lookup_path) );
              break;
    case 3:   policy.reset( new cet::filepath_lookup_after1(lookup_path) );
              break;
    default:  cerr << "Error: command line lookup-policy "
              << lookup_policy << " is unknown; choose 0, 1, 2, or 3\n";
              return 1;
  }

  // Do the real work:

  int nfailures = 0;
  for( size_t k = 0; k != input_filenames.size(); ++k )
    nfailures += do_including(input_filenames[k], *policy, out, err);
  return nfailures;

}  // main()
