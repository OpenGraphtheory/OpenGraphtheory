/* A Bison parser, made by GNU Bison 2.5.  */

/* Skeleton implementation for Bison LALR(1) parsers in C++
   
      Copyright (C) 2002-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* First part of user declarations.  */


/* Line 293 of lalr1.cc  */
#line 39 "ctl_parser.cpp"


#include "ctl_parser.hpp"

/* User implementation prologue.  */


/* Line 299 of lalr1.cc  */
#line 48 "ctl_parser.cpp"
/* Unqualified %code blocks.  */

/* Line 300 of lalr1.cc  */
#line 27 "ctl_parser.yy"

  static int yylex(CTLParser::semantic_type * yylval, OpenGraphtheory::Logic::CTLLexer &scanner);



/* Line 300 of lalr1.cc  */
#line 59 "ctl_parser.cpp"

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* FIXME: INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                               \
 do                                                                    \
   if (N)                                                              \
     {                                                                 \
       (Current).begin = YYRHSLOC (Rhs, 1).begin;                      \
       (Current).end   = YYRHSLOC (Rhs, N).end;                        \
     }                                                                 \
   else                                                                \
     {                                                                 \
       (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;        \
     }                                                                 \
 while (false)
#endif

/* Suppress unused-variable warnings by "using" E.  */
#define YYUSE(e) ((void) (e))

/* Enable debugging if requested.  */
#if YYDEBUG

/* A pseudo ostream that takes yydebug_ into account.  */
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)	\
do {							\
  if (yydebug_)						\
    {							\
      *yycdebug_ << Title << ' ';			\
      yy_symbol_print_ ((Type), (Value), (Location));	\
      *yycdebug_ << std::endl;				\
    }							\
} while (false)

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug_)				\
    yy_reduce_print_ (Rule);		\
} while (false)

# define YY_STACK_PRINT()		\
do {					\
  if (yydebug_)				\
    yystack_print_ ();			\
} while (false)

#else /* !YYDEBUG */

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_REDUCE_PRINT(Rule)
# define YY_STACK_PRINT()

#endif /* !YYDEBUG */

#define yyerrok		(yyerrstatus_ = 0)
#define yyclearin	(yychar = yyempty_)

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


/* Line 382 of lalr1.cc  */
#line 16 "ctl_parser.yy"
namespace OpenGraphtheory { namespace Logic {

/* Line 382 of lalr1.cc  */
#line 147 "ctl_parser.cpp"

  /// Build a parser object.
  CTLParser::CTLParser (OpenGraphtheory::Logic::CTLLexer &scanner_yyarg, OpenGraphtheory::Logic::CTLFormula** result_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      scanner (scanner_yyarg),
      result (result_yyarg)
  {
  }

  CTLParser::~CTLParser ()
  {
  }

#if YYDEBUG
  /*--------------------------------.
  | Print this symbol on YYOUTPUT.  |
  `--------------------------------*/

  inline void
  CTLParser::yy_symbol_value_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yyvaluep);
    switch (yytype)
      {
         default:
	  break;
      }
  }


  void
  CTLParser::yy_symbol_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    *yycdebug_ << (yytype < yyntokens_ ? "token" : "nterm")
	       << ' ' << yytname_[yytype] << " ("
	       << *yylocationp << ": ";
    yy_symbol_value_print_ (yytype, yyvaluep, yylocationp);
    *yycdebug_ << ')';
  }
#endif

  void
  CTLParser::yydestruct_ (const char* yymsg,
			   int yytype, semantic_type* yyvaluep, location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yymsg);
    YYUSE (yyvaluep);

    YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

    switch (yytype)
      {
  
	default:
	  break;
      }
  }

  void
  CTLParser::yypop_ (unsigned int n)
  {
    yystate_stack_.pop (n);
    yysemantic_stack_.pop (n);
    yylocation_stack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  CTLParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  CTLParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  CTLParser::debug_level_type
  CTLParser::debug_level () const
  {
    return yydebug_;
  }

  void
  CTLParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif

  inline bool
  CTLParser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  CTLParser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  CTLParser::parse ()
  {
    /// Lookahead and lookahead in internal form.
    int yychar = yyempty_;
    int yytoken = 0;

    /* State.  */
    int yyn;
    int yylen = 0;
    int yystate = 0;

    /* Error handling.  */
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// Semantic value of the lookahead.
    semantic_type yylval;
    /// Location of the lookahead.
    location_type yylloc;
    /// The locations where the error started and ended.
    location_type yyerror_range[3];

    /// $$.
    semantic_type yyval;
    /// @$.
    location_type yyloc;

    int yyresult;

    YYCDEBUG << "Starting parse" << std::endl;


    /* Initialize the stacks.  The initial state will be pushed in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystate_stack_ = state_stack_type (0);
    yysemantic_stack_ = semantic_stack_type (0);
    yylocation_stack_ = location_stack_type (0);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* New state.  */
  yynewstate:
    yystate_stack_.push (yystate);
    YYCDEBUG << "Entering state " << yystate << std::endl;

    /* Accept?  */
    if (yystate == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    /* Backup.  */
  yybackup:

    /* Try to take a decision without lookahead.  */
    yyn = yypact_[yystate];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    /* Read a lookahead token.  */
    if (yychar == yyempty_)
      {
	YYCDEBUG << "Reading a token: ";
	yychar = yylex (&yylval, scanner);
      }


    /* Convert token to internal form.  */
    if (yychar <= yyeof_)
      {
	yychar = yytoken = yyeof_;
	YYCDEBUG << "Now at end of input." << std::endl;
      }
    else
      {
	yytoken = yytranslate_ (yychar);
	YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
      }

    /* If the proper action on seeing token YYTOKEN is to reduce or to
       detect an error, take that action.  */
    yyn += yytoken;
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yytoken)
      goto yydefault;

    /* Reduce or error.  */
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
	if (yy_table_value_is_error_ (yyn))
	  goto yyerrlab;
	yyn = -yyn;
	goto yyreduce;
      }

    /* Shift the lookahead token.  */
    YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

    /* Discard the token being shifted.  */
    yychar = yyempty_;

    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* Count tokens shifted since error; after three, turn off error
       status.  */
    if (yyerrstatus_)
      --yyerrstatus_;

    yystate = yyn;
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystate];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    /* If YYLEN is nonzero, implement the default value of the action:
       `$$ = $1'.  Otherwise, use the top of the stack.

       Otherwise, the following line sets YYVAL to garbage.
       This behavior is undocumented and Bison
       users should not rely upon it.  */
    if (yylen)
      yyval = yysemantic_stack_[yylen - 1];
    else
      yyval = yysemantic_stack_[0];

    {
      slice<location_type, location_stack_type> slice (yylocation_stack_, yylen);
      YYLLOC_DEFAULT (yyloc, slice, yylen);
    }
    YY_REDUCE_PRINT (yyn);
    switch (yyn)
      {
	  case 2:

/* Line 690 of lalr1.cc  */
#line 50 "ctl_parser.yy"
    { *result = (yysemantic_stack_[(2) - (1)].fval); }
    break;

  case 3:

/* Line 690 of lalr1.cc  */
#line 51 "ctl_parser.yy"
    { *result = (yysemantic_stack_[(2) - (1)].fval); }
    break;

  case 5:

/* Line 690 of lalr1.cc  */
#line 54 "ctl_parser.yy"
    { (yyval.fval) = new CTL_Not((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 6:

/* Line 690 of lalr1.cc  */
#line 55 "ctl_parser.yy"
    { (yyval.fval) = new CTL_And((yysemantic_stack_[(3) - (1)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 7:

/* Line 690 of lalr1.cc  */
#line 56 "ctl_parser.yy"
    { (yyval.fval) = new CTL_Or((yysemantic_stack_[(3) - (1)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 8:

/* Line 690 of lalr1.cc  */
#line 58 "ctl_parser.yy"
    { (yyval.fval) = new CTL_ExistsNext((yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 9:

/* Line 690 of lalr1.cc  */
#line 59 "ctl_parser.yy"
    { (yyval.fval) = new CTL_ExistsGlobally((yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 10:

/* Line 690 of lalr1.cc  */
#line 60 "ctl_parser.yy"
    { (yyval.fval) = new CTL_ExistsFinally((yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 11:

/* Line 690 of lalr1.cc  */
#line 61 "ctl_parser.yy"
    { (yyval.fval) = new CTL_ExistsUntil((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 12:

/* Line 690 of lalr1.cc  */
#line 63 "ctl_parser.yy"
    { (yyval.fval) = new CTL_AlwaysNext((yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 13:

/* Line 690 of lalr1.cc  */
#line 64 "ctl_parser.yy"
    { (yyval.fval) = new CTL_AlwaysGlobally((yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 14:

/* Line 690 of lalr1.cc  */
#line 65 "ctl_parser.yy"
    { (yyval.fval) = new CTL_ExistsFinally((yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 15:

/* Line 690 of lalr1.cc  */
#line 66 "ctl_parser.yy"
    { /* $$ = new CTL_AlwaysUntil($2, $4);*/ (yyval.fval) = NULL; }
    break;

  case 16:

/* Line 690 of lalr1.cc  */
#line 69 "ctl_parser.yy"
    { (yyval.fval) = new CTL_True(); }
    break;

  case 17:

/* Line 690 of lalr1.cc  */
#line 70 "ctl_parser.yy"
    { (yyval.fval) = new CTL_False(); }
    break;

  case 18:

/* Line 690 of lalr1.cc  */
#line 71 "ctl_parser.yy"
    { (yyval.fval) = new CTL_Atomic((yysemantic_stack_[(1) - (1)].sval)); }
    break;

  case 19:

/* Line 690 of lalr1.cc  */
#line 72 "ctl_parser.yy"
    { (yyval.fval) = (yysemantic_stack_[(3) - (2)].fval); }
    break;



/* Line 690 of lalr1.cc  */
#line 530 "ctl_parser.cpp"
	default:
          break;
      }
    /* User semantic actions sometimes alter yychar, and that requires
       that yytoken be updated with the new translation.  We take the
       approach of translating immediately before every use of yytoken.
       One alternative is translating here after every semantic action,
       but that translation would be missed if the semantic action
       invokes YYABORT, YYACCEPT, or YYERROR immediately after altering
       yychar.  In the case of YYABORT or YYACCEPT, an incorrect
       destructor might then be invoked immediately.  In the case of
       YYERROR, subsequent parser actions might lead to an incorrect
       destructor call or verbose syntax error message before the
       lookahead is translated.  */
    YY_SYMBOL_PRINT ("-> $$ =", yyr1_[yyn], &yyval, &yyloc);

    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();

    yysemantic_stack_.push (yyval);
    yylocation_stack_.push (yyloc);

    /* Shift the result of the reduction.  */
    yyn = yyr1_[yyn];
    yystate = yypgoto_[yyn - yyntokens_] + yystate_stack_[0];
    if (0 <= yystate && yystate <= yylast_
	&& yycheck_[yystate] == yystate_stack_[0])
      yystate = yytable_[yystate];
    else
      yystate = yydefgoto_[yyn - yyntokens_];
    goto yynewstate;

  /*------------------------------------.
  | yyerrlab -- here on detecting error |
  `------------------------------------*/
  yyerrlab:
    /* Make sure we have latest lookahead translation.  See comments at
       user semantic actions for why this is necessary.  */
    yytoken = yytranslate_ (yychar);

    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus_)
      {
	++yynerrs_;
	if (yychar == yyempty_)
	  yytoken = yyempty_;
	error (yylloc, yysyntax_error_ (yystate, yytoken));
      }

    yyerror_range[1] = yylloc;
    if (yyerrstatus_ == 3)
      {
	/* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

	if (yychar <= yyeof_)
	  {
	  /* Return failure if at end of input.  */
	  if (yychar == yyeof_)
	    YYABORT;
	  }
	else
	  {
	    yydestruct_ ("Error: discarding", yytoken, &yylval, &yylloc);
	    yychar = yyempty_;
	  }
      }

    /* Else will try to reuse lookahead token after shifting the error
       token.  */
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;

    yyerror_range[1] = yylocation_stack_[yylen - 1];
    /* Do not reclaim the symbols of the rule which action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    yystate = yystate_stack_[0];
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;	/* Each real token shifted decrements this.  */

    for (;;)
      {
	yyn = yypact_[yystate];
	if (!yy_pact_value_is_default_ (yyn))
	{
	  yyn += yyterror_;
	  if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
	    {
	      yyn = yytable_[yyn];
	      if (0 < yyn)
		break;
	    }
	}

	/* Pop the current state because it cannot handle the error token.  */
	if (yystate_stack_.height () == 1)
	YYABORT;

	yyerror_range[1] = yylocation_stack_[0];
	yydestruct_ ("Error: popping",
		     yystos_[yystate],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);
	yypop_ ();
	yystate = yystate_stack_[0];
	YY_STACK_PRINT ();
      }

    yyerror_range[2] = yylloc;
    // Using YYLLOC is tempting, but would change the location of
    // the lookahead.  YYLOC is available though.
    YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yyloc);

    /* Shift the error token.  */
    YY_SYMBOL_PRINT ("Shifting", yystos_[yyn],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);

    yystate = yyn;
    goto yynewstate;

    /* Accept.  */
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    /* Abort.  */
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (yychar != yyempty_)
      {
        /* Make sure we have latest lookahead translation.  See comments
           at user semantic actions for why this is necessary.  */
        yytoken = yytranslate_ (yychar);
        yydestruct_ ("Cleanup: discarding lookahead", yytoken, &yylval,
                     &yylloc);
      }

    /* Do not reclaim the symbols of the rule which action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (yystate_stack_.height () != 1)
      {
	yydestruct_ ("Cleanup: popping",
		   yystos_[yystate_stack_[0]],
		   &yysemantic_stack_[0],
		   &yylocation_stack_[0]);
	yypop_ ();
      }

    return yyresult;
  }

  // Generate an error message.
  std::string
  CTLParser::yysyntax_error_ (int, int)
  {
    return YY_("syntax error");
  }


  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
  const signed char CTLParser::yypact_ninf_ = -7;
  const signed char
  CTLParser::yypact_[] =
  {
        23,    34,    45,     7,    -7,    -7,    23,    -7,     6,     9,
       8,     7,     7,     7,     2,     7,     7,     7,    17,    -7,
      -2,    -6,    -7,    -7,    -7,     7,     7,    -7,    -7,    -7,
       7,    -7,    -7,    -7,     7,    -7,    -7,    -7,    -7,    -7
  };

  /* YYDEFACT[S] -- default reduction number in state S.  Performed when
     YYTABLE doesn't specify something else to do.  Zero means the
     default is an error.  */
  const unsigned char
  CTLParser::yydefact_[] =
  {
         4,     0,     0,     0,    16,    17,     4,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     5,
       0,     0,     1,     2,     3,     0,     0,    14,    12,    13,
       0,    10,     8,     9,     0,    19,     6,     7,    15,    11
  };

  /* YYPGOTO[NTERM-NUM].  */
  const signed char
  CTLParser::yypgoto_[] =
  {
        -7,    -7,    22,    -1
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const signed char
  CTLParser::yydefgoto_[] =
  {
        -1,     8,     9,    10
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If YYTABLE_NINF_, syntax error.  */
  const signed char CTLParser::yytable_ninf_ = -1;
  const unsigned char
  CTLParser::yytable_[] =
  {
        14,    18,    19,    25,    26,    21,    22,    30,    24,    23,
      27,    28,    29,    35,    31,    32,    33,    25,    26,     4,
       5,     6,    34,     7,    36,    37,     1,     2,    20,    38,
       0,     0,     0,    39,     3,     4,     5,     6,     0,     7,
      11,    12,    13,     0,     0,     0,     4,     5,     6,     0,
       7,    15,    16,    17,     0,     0,     0,     4,     5,     6,
       0,     7
  };

  /* YYCHECK.  */
  const signed char
  CTLParser::yycheck_[] =
  {
         1,     2,     3,     9,    10,     6,     0,     5,     0,     0,
      11,    12,    13,    15,    15,    16,    17,     9,    10,    12,
      13,    14,     5,    16,    25,    26,     3,     4,     6,    30,
      -1,    -1,    -1,    34,    11,    12,    13,    14,    -1,    16,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,
      16,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    16
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  CTLParser::yystos_[] =
  {
         0,     3,     4,    11,    12,    13,    14,    16,    18,    19,
      20,     6,     7,     8,    20,     6,     7,     8,    20,    20,
      19,    20,     0,     0,     0,     9,    10,    20,    20,    20,
       5,    20,    20,    20,     5,    15,    20,    20,    20,    20
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  CTLParser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  CTLParser::yyr1_[] =
  {
         0,    17,    18,    18,    19,    19,    19,    19,    19,    19,
      19,    19,    19,    19,    19,    19,    20,    20,    20,    20
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  CTLParser::yyr2_[] =
  {
         0,     2,     2,     2,     0,     2,     3,     3,     3,     3,
       3,     4,     3,     3,     3,     4,     1,     1,     1,     3
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const CTLParser::yytname_[] =
  {
    "\"end of file\"", "error", "$undefined", "ALWAYS", "EXISTS", "UNTIL",
  "FINALLY", "NEXT", "GLOBALLY", "AND", "OR", "NOT", "_TRUE", "_FALSE",
  "OPEN", "CLOSE", "STRING", "$accept", "S", "CTLFORMULA", "ACTLFORMULA", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const CTLParser::rhs_number_type
  CTLParser::yyrhs_[] =
  {
        18,     0,    -1,    19,     0,    -1,    20,     0,    -1,    -1,
      11,    20,    -1,    20,     9,    20,    -1,    20,    10,    20,
      -1,     4,     7,    20,    -1,     4,     8,    20,    -1,     4,
       6,    20,    -1,     4,    20,     5,    20,    -1,     3,     7,
      20,    -1,     3,     8,    20,    -1,     3,     6,    20,    -1,
       3,    20,     5,    20,    -1,    12,    -1,    13,    -1,    16,
      -1,    14,    19,    15,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned char
  CTLParser::yyprhs_[] =
  {
         0,     0,     3,     6,     9,    10,    13,    17,    21,    25,
      29,    33,    38,    42,    46,    50,    55,    57,    59,    61
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned char
  CTLParser::yyrline_[] =
  {
         0,    50,    50,    51,    53,    54,    55,    56,    58,    59,
      60,    61,    63,    64,    65,    66,    69,    70,    71,    72
  };

  // Print the state stack on the debug stream.
  void
  CTLParser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (state_stack_type::const_iterator i = yystate_stack_.begin ();
	 i != yystate_stack_.end (); ++i)
      *yycdebug_ << ' ' << *i;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  CTLParser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    /* Print the symbols being reduced, and their result.  */
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
	       << " (line " << yylno << "):" << std::endl;
    /* The symbols being reduced.  */
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
		       yyrhs_[yyprhs_[yyrule] + yyi],
		       &(yysemantic_stack_[(yynrhs) - (yyi + 1)]),
		       &(yylocation_stack_[(yynrhs) - (yyi + 1)]));
  }
#endif // YYDEBUG

  /* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
  CTLParser::token_number_type
  CTLParser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
           0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int CTLParser::yyeof_ = 0;
  const int CTLParser::yylast_ = 61;
  const int CTLParser::yynnts_ = 4;
  const int CTLParser::yyempty_ = -2;
  const int CTLParser::yyfinal_ = 22;
  const int CTLParser::yyterror_ = 1;
  const int CTLParser::yyerrcode_ = 256;
  const int CTLParser::yyntokens_ = 17;

  const unsigned int CTLParser::yyuser_token_number_max_ = 271;
  const CTLParser::token_number_type CTLParser::yyundef_token_ = 2;


/* Line 1136 of lalr1.cc  */
#line 16 "ctl_parser.yy"
} } // OpenGraphtheory::Logic

/* Line 1136 of lalr1.cc  */
#line 952 "ctl_parser.cpp"


/* Line 1138 of lalr1.cc  */
#line 74 "ctl_parser.yy"



#include "../../../Headers/logic/ComputationTreeLogic/ctl_lexer.h"

void CTLParser::error(const CTLParser::location_type &loc, const std::string &msg)
{
    std::cerr << "Error: " << msg << std::endl;
}

static int yylex(CTLParser::semantic_type * yylval, OpenGraphtheory::Logic::CTLLexer &scanner)
{
    return scanner.yylex(yylval);
}
