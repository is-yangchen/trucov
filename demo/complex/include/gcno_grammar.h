///////////////////////////////////////////////////////////////////////////////
//  COPYRIGHT (c) 2009 Schweitzer Engineering Laboratories, Pullman, WA
///////////////////////////////////////////////////////////////////////////////
//  Permission is hereby granted, free of charge, to any person
//  obtaining a copy of this software and associated documentation
//  files (the "Software"), to deal in the Software without
//  restriction, including without limitation the rights to use,
//  copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following
//  conditions:
// 
//  The above copyright notice and this permission notice shall be
//  included in all copies or substantial portions of the Software.
// 
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//  OTHER DEALINGS IN THE SOFTWARE.
///////////////////////////////////////////////////////////////////////////////
///  @file gcno_grammar.h
///
///  @brief
///  Defines The GCNO Grammar for the Spirit Parsing Framework
///
///  @remarks
///  The Gcno_grammar struct shall be handled by Spirit Parsing only.
///
///  Requirements Specification:
///     < http://code.google.com/p/trucov/wiki/SRS >
///
///  Design Description: 
///     < http://code.google.com/p/trucov/wiki/TrucovDesignDocumentation >
///////////////////////////////////////////////////////////////////////////////
#ifndef GCNO_GRAMMAR_H
#define GCNO_GRAMMAR_H

//  SYSTEM INCLUDES

#include <fstream>
#include <iostream>
#include <exception>
#include <map>
#include <inttypes.h>

#include <boost/spirit/core.hpp>
#include <boost/spirit/tree/ast.hpp>
#include <boost/spirit/attribute.hpp>
#include <boost/spirit/phoenix.hpp>
#include <boost/variant.hpp>
#include <boost/spirit/utility/loops.hpp>
#include <boost/spirit/dynamic/while.hpp>
#include <boost/ref.hpp>

//  LOCAL INCLUDES

#include "gcno_actors.h"
#include "prims.h"
#include "record.h"

//  USING STATEMENTS

/// Defines the Gcno grammar for the Spirit Parsing Framework.
/// This grammar may be invoked with spirit by using the parse function.
struct Gcno_grammar : public boost::spirit::grammar<Gcno_grammar>
{
    //  PUBLIC DATA FIELDS
public:

    /// @brief
    /// Defines the grammar for the gcno File
    template<typename ScannerT>
    struct definition
    {
        /// @brief
        /// Spirit calls this constructor with a reference to the grammar
        /// object this definition is contained in.
        ///
        /// @param s A reference to definition object; needed for data access.
        definition(Gcno_grammar const & s)
        {
            //  TOKEN DEFINITIONS

            ZERO_32      = TOKEN32(0x00000000);
            MAGIC        = TOKEN32(0x67636E6F);
            TAG_FUNCTION = TOKEN32(0x01000000);
            TAG_BLOCKS   = TOKEN32(0x01410000);
            TAG_ARCS     = TOKEN32(0x01430000);
            TAG_LINES    = TOKEN32(0x01450000);

            //  ALIAS DEFINITIONS

            version   = INT32  [boost::spirit::assign_a(s.pd_ref.version, global_parsed_int32)];
            stamp     = INT32  [boost::spirit::assign_a(s.pd_ref.stamp, global_parsed_int32)];
            rLength   = INT32  [boost::spirit::assign_a(s.pd_ref.rLength, global_parsed_int32)];
            rIdent    = INT32  [boost::spirit::assign_a(s.pd_ref.rIdent, global_parsed_int32)];
            rChecksum = INT32  [boost::spirit::assign_a(s.pd_ref.rChecksum, global_parsed_int32)];
            rName     = STRING [boost::spirit::assign_a(s.pd_ref.rName, global_parsed_string)];
            rSource   = STRING [boost::spirit::assign_a(s.pd_ref.rSource, global_parsed_string)];
            rLineno   = INT32  [boost::spirit::assign_a(s.pd_ref.rLineno, global_parsed_int32)];
            bLength   = INT32  [boost::spirit::assign_a(s.pd_ref.bLength, global_parsed_int32)]
                                   [boost::spirit::assign_a(s.pd_ref.blocks, global_parsed_int32)]
                                   [phoenix::var(s.pd_ref.blocks) -= 1];
            bFlags    = INT32  [boost::spirit::assign_a(s.pd_ref.bFlags, global_parsed_int32)];
            aLength   = INT32  [boost::spirit::assign_a(s.pd_ref.aLength, global_parsed_int32)]
                                   [phoenix::var(s.pd_ref.aLength) /= 2];
            aBlockno  = INT32  [boost::spirit::assign_a(s.pd_ref.aBlockno, global_parsed_int32)];
            aDestBlock = INT32 [boost::spirit::assign_a(s.pd_ref.aDestBlock, global_parsed_int32)];
            aFlags    = INT32  [boost::spirit::assign_a(s.pd_ref.aFlags, global_parsed_int32)];
            lLength   = INT32  [boost::spirit::assign_a(s.pd_ref.lLength, global_parsed_int32)];
            lBlockno  = INT32  [boost::spirit::assign_a(s.pd_ref.lBlockno, global_parsed_int32)];
            lLineno   = NONZERO[boost::spirit::assign_a(s.pd_ref.lLineno, global_parsed_int32)];
            lName     = STRING [boost::spirit::assign_a(s.pd_ref.lName, global_parsed_string)];

            //  GRAMMAR RULE DEFINITIONS

            GcnoFile =
                    MAGIC >> version >> stamp [ do_gcnofile(s.pd_ref) ] >>
                    *( Record )
                ;
            Record   =
                AnnounceFunction [ do_AnnounceFunction(s.pd_ref) ] >>
                BasicBlocks >> 
                boost::spirit::repeat_p(boost::ref(s.pd_ref.blocks)) 
                    [ Arcs ] >>
                *Lines
                ;
            AnnounceFunction =
                TAG_FUNCTION >> rLength >> rIdent >> rChecksum  >> rName >>
                rSource >> rLineno
                ;
            BasicBlocks =
                TAG_BLOCKS >>  bLength [ do_BasicBlocks(s.pd_ref)     ]
                    [ phoenix::var(s.pd_ref.bIteration) = 0 ] >>
                boost::spirit::repeat_p(boost::ref(s.pd_ref.bLength)) [
                    bFlags [ do_bFlags(s.pd_ref)           ]
                           [ phoenix::var(s.pd_ref.bIteration) += 1 ]
                ]
                ;
            Arcs =
                TAG_ARCS >> aLength >>  aBlockno [ do_Arcs(s.pd_ref) ] >>
                boost::spirit::repeat_p(boost::ref(s.pd_ref.aLength)) [
                    Arc [ do_Arc(s.pd_ref) ]
                ]
                ;
            Arc =
                aDestBlock >> aFlags
                ;
            Lines =
                TAG_LINES >> lLength >> lBlockno [ do_Lines(s.pd_ref) ] >>
                *Line >> ZERO_32 >> ZERO_32
                ;
            Line  =
                lLineno [ do_lLineno(boost::ref(s.pd_ref)) ]
                | ZERO_32 >> lName [ do_lName(boost::ref(s.pd_ref)) ] >>
                  lLineno[ do_lLineno(boost::ref(s.pd_ref)) ]
                | ZERO_32 >> lName [ do_lName(boost::ref(s.pd_ref)) ] 
                ;

        } // end of constructor of defintion

        //  TOKENS

        boost::spirit::rule<ScannerT> ZERO_32;
        boost::spirit::rule<ScannerT> MAGIC;
        boost::spirit::rule<ScannerT> TAG_FUNCTION;
        boost::spirit::rule<ScannerT> TAG_BLOCKS;
        boost::spirit::rule<ScannerT> TAG_ARCS;
        boost::spirit::rule<ScannerT> TAG_LINES;

        //  ALIASES

        boost::spirit::rule<ScannerT> version;
        boost::spirit::rule<ScannerT> stamp;
        boost::spirit::rule<ScannerT> rLength;
        boost::spirit::rule<ScannerT> rIdent;
        boost::spirit::rule<ScannerT> rChecksum;
        boost::spirit::rule<ScannerT> rName;
        boost::spirit::rule<ScannerT> rSource;
        boost::spirit::rule<ScannerT> rLineno;
        boost::spirit::rule<ScannerT> bLength;
        boost::spirit::rule<ScannerT> bFlags;
        boost::spirit::rule<ScannerT> aLength;
        boost::spirit::rule<ScannerT> aBlockno;
        boost::spirit::rule<ScannerT> aDestBlock;
        boost::spirit::rule<ScannerT> aFlags;
        boost::spirit::rule<ScannerT> lLength;
        boost::spirit::rule<ScannerT> lBlockno;
        boost::spirit::rule<ScannerT> lLineno;
        boost::spirit::rule<ScannerT> lName;

        //  GRAMMAR RULES

        boost::spirit::rule<ScannerT> GcnoFile;
        boost::spirit::rule<ScannerT> Record;
        boost::spirit::rule<ScannerT> AnnounceFunction;
        boost::spirit::rule<ScannerT> BasicBlocks;
        boost::spirit::rule<ScannerT> Arcs;
        boost::spirit::rule<ScannerT> Arc;
        boost::spirit::rule<ScannerT> Lines;
        boost::spirit::rule<ScannerT> Line;

        /// @brief
        /// Defines the start rule for the GCNO Grammar definition. This 
        /// method is used by spirit.
        ///
        /// @return The top GCNO grammar rule
        boost::spirit::rule<ScannerT> const & start() const
        {
            return GcnoFile;
        }

    }; // end of struct definition

    //  PUBLIC METHODS

    /// @brief
    /// Initializes a reference to the internal parsing data for spirit and
    /// the error messages.
    /// 
    /// @param records Reference to records map that will be populated by
    /// the parse method.
    Gcno_grammar(Parser_builder & parser_builder, 
    const bool is_dump_ref, ofstream & dump_file_ref) 
    : parsing_data(parser_builder, is_dump_ref, dump_file_ref), pd_ref(parsing_data)
    {
        // Initialize error messages
        error_msg_tag_blocks   =
            "ERROR: Failed to match token TAG_BLOCKS Failed.";
        error_msg_tag_arcs     =
            "ERROR: Failed to match token TAG_ARCS Failed.";
    }

    //  PRIVATE DATA FIELDS
private:

    /// The private temporary internal parsing data of the file
    Parsing_data_gcno parsing_data;

    ///  A private error message for the token TAG_BLOCKS
    string error_msg_tag_blocks;

    ///  A private error message for the token TAG_ARCS
    string error_msg_tag_arcs;

    ///  A public reference to the Parsing Data that spirit will use
    Parsing_data_gcno & pd_ref;

}; // end of struct Gcno_grammar

#endif

