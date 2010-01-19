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
///  @file gcda_grammar.h
///
///  @brief
///  Defines The GCDA Grammar for the Spirit Parsing Framework
///
///  @remarks
///  The Gcda_grammar struct shall be handled by Spirit Parsing only.
///
///  Requirements Specification:
///     < http://code.google.com/p/trucov/wiki/SRS > 
///
///  Design Description: 
///     < http://code.google.com/p/trucov/wiki/TrucovDesignDocumentation >
///////////////////////////////////////////////////////////////////////////////
#ifndef GCDA_GRAMMAR_H
#define GCDA_GRAMMAR_H

//  SYSTEM INCLUDES

#include <fstream>
#include <iostream>
#include <exception>
#include <map>

#include <boost/spirit/core.hpp>
#include <boost/spirit/tree/ast.hpp>
#include <boost/spirit/attribute.hpp>
#include <boost/spirit/phoenix.hpp>
#include <boost/variant.hpp>
#include <boost/spirit/utility/loops.hpp>
#include <boost/spirit/dynamic/while.hpp>
#include <boost/ref.hpp>

//  LOCAL INCLUDES

#include "parser_builder.h"
#include "gcda_actors.h"
#include "prims.h"
#include "record.h"

/// @brief
/// Defines the Gcda grammar for the Spirit Parsing Framework.
/// This grammar may be invoked with spirit by using the parse function.
struct Gcda_grammar : public boost::spirit::grammar<Gcda_grammar>
{
    //  PUBLIC DATA FIELDS
public:

    /// @brief
    /// Defines the grammar for the gcda File.
    template<typename ScannerT>
    struct definition
    {
        /// @brief
        /// Spirit calls this constructor with a reference to the grammar
        /// object this definition is contained in.
        ///
        /// @param s A reference to definition object; needed for data access.
        definition(Gcda_grammar const & s)
        {
            //  TOKEN DEFINITIONS

            ZERO_32             = TOKEN32(0x00000000);
            MAGIC               = TOKEN32(0x67636461);
            TAG_FUNCTION        = TOKEN32(0x01000000);
            TAG_COUNTER_BASE    = TOKEN32(0x01A10000);
            TAG_OBJECT_SUMMARY  = TOKEN32(0xA1000000);
            TAG_PROGRAM_SUMMARY = TOKEN32(0xA3000000);

            //  ALIAS DEFINITIONS

            version   = INT32 [boost::spirit::assign_a(s.pd_ref.version, global_parsed_int32)];
            stamp     = INT32 [boost::spirit::assign_a(s.pd_ref.stamp, global_parsed_int32)];
            rLength   = INT32 [boost::spirit::assign_a(s.pd_ref.rLength, global_parsed_int32)];
            rIdent    = INT32 [boost::spirit::assign_a(s.pd_ref.rIdent, global_parsed_int32)];
            rChecksum = INT32 [boost::spirit::assign_a(s.pd_ref.rChecksum, global_parsed_int32)];
            aCount    = INT64 [boost::spirit::assign_a(s.pd_ref.aCount, global_parsed_int64)];
            aLength   = INT32 [boost::spirit::assign_a(s.pd_ref.aLength, global_parsed_int32)]
                                 [phoenix::var(s.pd_ref.aLength) /= 2];
            oChecksum = INT32 [boost::spirit::assign_a(s.pd_ref.oChecksum, global_parsed_int32)];
            oLength   = INT32 [boost::spirit::assign_a(s.pd_ref.oLength, global_parsed_int32)];
            oCounts   = INT32 [boost::spirit::assign_a(s.pd_ref.oCounts, global_parsed_int32)];
            oRuns     = INT32 [boost::spirit::assign_a(s.pd_ref.oRuns, global_parsed_int32)];
            oSumall   = INT64 [boost::spirit::assign_a(s.pd_ref.oSumall, global_parsed_int64)];
            oRunmax   = INT64 [boost::spirit::assign_a(s.pd_ref.oRunmax, global_parsed_int64)];
            oSummax   = INT64 [boost::spirit::assign_a(s.pd_ref.oSummax, global_parsed_int64)];
            pChecksum = INT32 [boost::spirit::assign_a(s.pd_ref.pChecksum, global_parsed_int32)];
            pLength   = INT32 [boost::spirit::assign_a(s.pd_ref.pLength, global_parsed_int32)];
            pCounts   = INT32 [boost::spirit::assign_a(s.pd_ref.pCounts, global_parsed_int32)];
            pRuns     = INT32 [boost::spirit::assign_a(s.pd_ref.pRuns, global_parsed_int32)];
            pSumall   = INT64 [boost::spirit::assign_a(s.pd_ref.pSumall, global_parsed_int64)];
            pRunmax   = INT64 [boost::spirit::assign_a(s.pd_ref.pRunmax, global_parsed_int64)];
            pSummax   = INT64 [boost::spirit::assign_a(s.pd_ref.pSummax, global_parsed_int64)];

            //  GRAMMAR RULE DEFINITIONS

            GcdaFile =
                MAGIC >> version >> stamp [ do_gcdafile(s.pd_ref) ] >>
                *( Record ) >> ObjectSummary [ do_ObjectSummary(s.pd_ref) ] >>
                *( ProgramSummary [ do_ProgramSummary(s.pd_ref) ] ) >>
                ZERO_32
                ;
            Record =
                AnnounceFunction [ do_AnnounceFunction(s.pd_ref) ] >>
                *( Counters )
                ;
            AnnounceFunction =
                TAG_FUNCTION >> rLength >> rIdent >> rChecksum
                ;
            Counters =
                TAG_COUNTER_BASE >> aLength [ do_aLength(s.pd_ref) ] >>
                boost::spirit::repeat_p(boost::ref(s.pd_ref.aLength)) [
                    aCount [ do_aCount(s.pd_ref) ]
                ]
                ;
            ObjectSummary =
                TAG_OBJECT_SUMMARY >> oLength >> oChecksum >> oCounts >>
                oRuns >> oSumall >> oRunmax >> oSummax
                ;
            ProgramSummary =
                TAG_PROGRAM_SUMMARY >> pLength >> pChecksum >> pCounts >>
                pRuns >> pSumall >> pRunmax >> pSummax
                ;

        } // end of constructor of defintion

        //  TOKENS

        boost::spirit::rule<ScannerT> ZERO_32;
        boost::spirit::rule<ScannerT> MAGIC;
        boost::spirit::rule<ScannerT> TAG_FUNCTION;
        boost::spirit::rule<ScannerT> TAG_COUNTER_BASE;
        boost::spirit::rule<ScannerT> TAG_OBJECT_SUMMARY;
        boost::spirit::rule<ScannerT> TAG_PROGRAM_SUMMARY;
        boost::spirit::rule<ScannerT> TAG_GCDA_EOF;

        //  ALIASES

        boost::spirit::rule<ScannerT> version;
        boost::spirit::rule<ScannerT> stamp;
        boost::spirit::rule<ScannerT> rLength;
        boost::spirit::rule<ScannerT> rIdent;
        boost::spirit::rule<ScannerT> rChecksum;
        boost::spirit::rule<ScannerT> aCount;
        boost::spirit::rule<ScannerT> aLength;
        boost::spirit::rule<ScannerT> oChecksum;
        boost::spirit::rule<ScannerT> oLength;
        boost::spirit::rule<ScannerT> oCounts;
        boost::spirit::rule<ScannerT> oRuns;
        boost::spirit::rule<ScannerT> oSumall;
        boost::spirit::rule<ScannerT> oRunmax;
        boost::spirit::rule<ScannerT> oSummax;
        boost::spirit::rule<ScannerT> pChecksum;
        boost::spirit::rule<ScannerT> pLength;
        boost::spirit::rule<ScannerT> pCounts;
        boost::spirit::rule<ScannerT> pRuns;
        boost::spirit::rule<ScannerT> pSumall;
        boost::spirit::rule<ScannerT> pRunmax;
        boost::spirit::rule<ScannerT> pSummax;

        //  GRAMMAR RULES

        boost::spirit::rule<ScannerT> GcdaFile;
        boost::spirit::rule<ScannerT> Record;
        boost::spirit::rule<ScannerT> AnnounceFunction;
        boost::spirit::rule<ScannerT> Counters;
        boost::spirit::rule<ScannerT> ObjectSummary;
        boost::spirit::rule<ScannerT> ProgramSummary;

        /// @brief
        /// Defines the start rule for the GCDA Grammar definition. This public
        /// method is invoked by the spirit framework.
        ///
        /// @return rule const & 
        boost::spirit::rule<ScannerT> const & start() const
        {
            return GcdaFile;
        }

    }; // end of struct definition

    //  PUBLIC METHODS

    /// @brief
    /// Initializes a reference to the internal parsing data for spirit and
    /// the error messages.
    ///
    /// @param records Reference to the map of records to be populated by the
    /// parse method.
    Gcda_grammar(Parser_builder & parser_builder, const bool is_dump,
                 ofstream & dump_file) 
        : parsing_data(parser_builder, is_dump, dump_file), pd_ref(parsing_data)
    {
         // Do nothing 
    }

    //  PRIVATE DATA FIELDS
private:

    ///  The private temporary internal parsing data of the file
    Parsing_data_gcda parsing_data;

    ///  A private error message for the token TAG_BLOCKS
    string error_msg_tag_blocks;

    ///  A private error message for the token TAG_ARCS
    string error_msg_tag_arcs;

    ///  A public reference to the Parsing Data that spirit will use
    Parsing_data_gcda & pd_ref;

}; // end of struct Gcda_grammar

#endif

