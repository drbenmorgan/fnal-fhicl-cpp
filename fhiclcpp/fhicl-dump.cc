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

  // Error categories
  std::string const help{"Help"};
  std::string const processing{"Processing"};
  std::string const config{"Configuration"};

  using std::string;

  struct Options {
    bool    annotate{false};
    string  output_filename;
    string  input_filename;
    int     lookup_policy{};
    string  lookup_path;
  };

  Options process_arguments( int argc, char* argv[] );

  fhicl::ParameterSet
  form_pset(string const & filename, cet::filepath_maker & lookup_policy);

  std::unique_ptr<cet::filepath_maker>
  get_policy(int const lookup_policy, string const& lookup_path);

}

//======================================================================

int main(int argc, char* argv[])
{

  Options opts;
  try {
    opts = process_arguments( argc, argv );
  }
  catch(cet::exception const& e) {
    if ( e.category() == help       ) return 0;
    if ( e.category() == processing ) return 1;
    if ( e.category() == config     ) return 2;
  }

  auto const policy = get_policy(opts.lookup_policy, opts.lookup_path);
  auto const pset   = form_pset(opts.input_filename, *policy);

  std::ofstream ofs { opts.output_filename };
  std::ostream &os = opts.output_filename.empty() ? std::cout : ofs;
  os << pset.to_indented_string(0,opts.annotate);

}

//======================================================================

namespace {

  Options process_arguments( int argc, char* argv[] )
  {
    namespace bpo = boost::program_options;

    Options opts;

    bpo::options_description desc("fhicl-dump [-c] <file>\nOptions");
    desc.add_options()
      ( "help,h", "produce this help message")
      ( "config,c", bpo::value<std::string>(&opts.input_filename), "input file" )
      ( "output,o", bpo::value<std::string>(&opts.output_filename), "output file (default is STDOUT)")
      ( "annotate,a", bpo::bool_switch(&opts.annotate)->default_value(false,"false"), "include source location annotations")
      ( "lookup-policy,l" ,
        bpo::value<int>(&opts.lookup_policy)->default_value(1),
        "lookup policy code:"
        "\n  0 => cet::filepath_maker"
        "\n  1 => cet::filepath_lookup"
        "\n  2 => cet::filepath_lookup_nonabsolute"
        "\n  3 => cet::filepath_lookup_after1")
      ( "path,p"  ,
        bpo::value<std::string>(&opts.lookup_path)->default_value(fhicl_env_var),
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
      std::ostringstream err_stream;
      err_stream << "Error processing command line in " << argv[0]
                 << ": " << err.what() << '\n';
      throw cet::exception(processing) << err_stream.str();
    };

    if ( vm.count("help") ) {
      std::cout << desc << '\n';
      throw cet::exception(help);
    }

    if ( !vm.count("config") ) {
      std::ostringstream err_stream;
      err_stream << "\nMissing input configuration file.\n\n"
                 << desc << '\n';
      throw cet::exception(config) << err_stream.str();
    }
    return opts;
  }

  std::unique_ptr<cet::filepath_maker>
  get_policy(int const lookup_policy, std::string const& lookup_path)
  {
    std::cerr << "Policy is " << lookup_policy
              << "; path is \"" << lookup_path << "\"\n";
    switch( lookup_policy ) {
    case 0: return std::make_unique<cet::filepath_maker>();
    case 1: return std::make_unique<cet::filepath_lookup>(lookup_path);
    case 2: return std::make_unique<cet::filepath_lookup_nonabsolute>(lookup_path);
    case 3: return std::make_unique<cet::filepath_lookup_after1>(lookup_path);
    default:
      std::ostringstream err_stream;
      err_stream << "Error: command line lookup-policy "
                 << lookup_policy
                 << " is unknown; choose 0, 1, 2, or 3\n";
      throw std::runtime_error(err_stream.str());
    }
  }

  fhicl::ParameterSet form_pset(std::string const   & filename,
                                cet::filepath_maker & lookup_policy)
  {
    fhicl::intermediate_table tbl;
    fhicl::parse_document(filename, lookup_policy, tbl);
    fhicl::ParameterSet pset;
    fhicl::make_ParameterSet(tbl, pset);
    return pset;
  }

}
