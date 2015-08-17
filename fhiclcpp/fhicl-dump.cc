// ======================================================================
//
// Executable for dumping processed configuration files
//
// ======================================================================

#include "boost/program_options.hpp"

#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/intermediate_table.h"
#include "fhiclcpp/make_ParameterSet.h"
#include "fhiclcpp/parse.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace fhicl;

namespace {
  std::string const fhicl_env_var{"FHICL_FILE_PATH"};
}

int main(int argc, char* argv[])
{

  namespace bpo = boost::program_options;
  bool annotate;
  std::string input_file;
  std::string output_file;
  int         lookup_policy;
  std::string lookup_path;
  bpo::options_description desc("fhicl-dump [-c] <file>\nOptions");
  desc.add_options()
    ( "help,h", "produce this help message")
    ( "config,c", bpo::value<std::string>(&input_file), "input file" )
    ( "output,o", bpo::value<std::string>(&output_file), "output file (default is STDOUT)")
    ( "annotate,a", bpo::bool_switch(&annotate)->default_value(false,"false"), "include source location annotations")
    ( "lookup-policy,l" ,
      bpo::value<int>(&lookup_policy)->default_value(1),
      "lookup policy code:"
      "\n  0 => cet::filepath_maker"
      "\n  1 => cet::filepath_lookup"
      "\n  2 => cet::filepath_lookup_nonabsolute"
      "\n  3 => cet::filepath_lookup_after1")
    ( "path,p"  ,
      bpo::value<std::string>(&lookup_path)->default_value(fhicl_env_var),
      "path or environment variable to be used by lookup-policy")
    ;

  bpo::positional_options_description p;
  p.add("config",-1);

  bpo::variables_map vm;
  try {
    bpo::store(bpo::command_line_parser(argc, argv).
               options(desc).positional(p).run(), vm);
    bpo::notify(vm);
  }
  catch (bpo::error& err) {
    std::cerr << "Error processing command line in " << argv[0]
              << ": " << err.what() << '\n';
    return 1;
  };

  if ( vm.count("help") ) {
    std::cout << desc << std::endl;
    return 2;
  }

  if ( !vm.count("config") ) {
    std::cout << "\nMissing input configuration file.\n\n"
              << desc
              << std::endl;
    return 3;
  }

  std::unique_ptr<cet::filepath_maker> policy;

  std::cerr << "Policy is " << lookup_policy
            << "; path is \"" << lookup_path << "\"\n";
  switch( lookup_policy ) {
  case 0:   policy = std::make_unique<cet::filepath_maker>();
    break;
  case 1:   policy = std::make_unique<cet::filepath_lookup>(lookup_path);
    break;
  case 2:   policy = std::make_unique<cet::filepath_lookup_nonabsolute>(lookup_path);
    break;
  case 3:   policy = std::make_unique<cet::filepath_lookup_after1>(lookup_path);
    break;
  default:  std::cerr << "Error: command line lookup-policy "
                      << lookup_policy << " is unknown; choose 0, 1, 2, or 3\n";
    return 1;
  }

  // Form PSet
  fhicl::intermediate_table tbl;
  fhicl::parse_document(input_file, *policy, tbl);
  fhicl::ParameterSet pset;
  fhicl::make_ParameterSet(tbl, pset);

  // to_indented_string includes '\n' at the end.
  std::string const dump = pset.to_indented_string(0,annotate);

  std::ofstream ofs{ output_file };
  std::ostream & os = output_file.empty() ? std::cout : ofs;
  os << dump;

}
