/* A Bison parser, made by GNU Bison 2.7.12-4996.  */

/* Skeleton implementation for Bison LALR(1) parsers in C++
   
      Copyright (C) 2002-2013 Free Software Foundation, Inc.
   
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

/* Line 283 of lalr1.cc  */
#line 38 "tikz_parser.cpp"


#include "tikz_parser.hpp"

/* User implementation prologue.  */

/* Line 289 of lalr1.cc  */
#line 46 "tikz_parser.cpp"
/* Unqualified %code blocks.  */
/* Line 290 of lalr1.cc  */
#line 26 "tikz_parser.yy"

    static int yylex(TIKZParser::semantic_type * yylval, OpenGraphtheory::Import::TIKZLexer &scanner);


/* Line 290 of lalr1.cc  */
#line 55 "tikz_parser.cpp"


# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

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

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (/*CONSTCOND*/ false)
# endif


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
# define YY_SYMBOL_PRINT(Title, Type, Value, Location) YYUSE(Type)
# define YY_REDUCE_PRINT(Rule)        static_cast<void>(0)
# define YY_STACK_PRINT()             static_cast<void>(0)

#endif /* !YYDEBUG */

#define yyerrok		(yyerrstatus_ = 0)
#define yyclearin	(yychar = yyempty_)

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

/* Line 357 of lalr1.cc  */
#line 14 "tikz_parser.yy"
namespace OpenGraphtheory { namespace Import {
/* Line 357 of lalr1.cc  */
#line 151 "tikz_parser.cpp"

  /// Build a parser object.
  TIKZParser::TIKZParser (OpenGraphtheory::Import::TIKZLexer &scanner_yyarg, OpenGraphtheory::Import::TikzCode** result_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      scanner (scanner_yyarg),
      result (result_yyarg)
  {
  }

  TIKZParser::~TIKZParser ()
  {
  }

#if YYDEBUG
  /*--------------------------------.
  | Print this symbol on YYOUTPUT.  |
  `--------------------------------*/

  inline void
  TIKZParser::yy_symbol_value_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yyvaluep);
    std::ostream& yyo = debug_stream ();
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    YYUSE (yytype);
  }


  void
  TIKZParser::yy_symbol_print_ (int yytype,
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
  TIKZParser::yydestruct_ (const char* yymsg,
			   int yytype, semantic_type* yyvaluep, location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yymsg);
    YYUSE (yyvaluep);

    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

    YYUSE (yytype);
  }

  void
  TIKZParser::yypop_ (unsigned int n)
  {
    yystate_stack_.pop (n);
    yysemantic_stack_.pop (n);
    yylocation_stack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  TIKZParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  TIKZParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  TIKZParser::debug_level_type
  TIKZParser::debug_level () const
  {
    return yydebug_;
  }

  void
  TIKZParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif

  inline bool
  TIKZParser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  TIKZParser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  TIKZParser::parse ()
  {
    /// Lookahead and lookahead in internal form.
    int yychar = yyempty_;
    int yytoken = 0;

    // State.
    int yyn;
    int yylen = 0;
    int yystate = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// Semantic value of the lookahead.
    static semantic_type yyval_default;
    semantic_type yylval = yyval_default;
    /// Location of the lookahead.
    location_type yylloc;
    /// The locations where the error started and ended.
    location_type yyerror_range[3];

    /// $$.
    semantic_type yyval;
    /// @$.
    location_type yyloc;

    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    /* Initialize the stacks.  The initial state will be pushed in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystate_stack_.clear ();
    yysemantic_stack_.clear ();
    yylocation_stack_.clear ();
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

    // Compute the default @$.
    {
      slice<location_type, location_stack_type> slice (yylocation_stack_, yylen);
      YYLLOC_DEFAULT (yyloc, slice, yylen);
    }

    // Perform the reduction.
    YY_REDUCE_PRINT (yyn);
    switch (yyn)
      {
          case 2:
/* Line 664 of lalr1.cc  */
#line 62 "tikz_parser.yy"
    { *result = (yysemantic_stack_[(3) - (2)].tval); }
    break;

  case 3:
/* Line 664 of lalr1.cc  */
#line 64 "tikz_parser.yy"
    { (yyval.tval) = NULL; }
    break;

  case 4:
/* Line 664 of lalr1.cc  */
#line 65 "tikz_parser.yy"
    { (yyval.tval) = (yysemantic_stack_[(2) - (2)].tval); (yyval.tval)->PrevLines = (yysemantic_stack_[(2) - (1)].tval); }
    break;

  case 5:
/* Line 664 of lalr1.cc  */
#line 70 "tikz_parser.yy"
    { (yyval.tval) = new TikzDeclareNode((yysemantic_stack_[(16) - (3)].sval), (yysemantic_stack_[(16) - (7)].fval), (yysemantic_stack_[(16) - (10)].fval), (yysemantic_stack_[(16) - (14)].sval)); }
    break;

  case 6:
/* Line 664 of lalr1.cc  */
#line 72 "tikz_parser.yy"
    { (yyval.tval) = new TikzDeclareNode((yysemantic_stack_[(15) - (3)].sval), (yysemantic_stack_[(15) - (7)].fval), (yysemantic_stack_[(15) - (10)].fval), NULL); }
    break;

  case 7:
/* Line 664 of lalr1.cc  */
#line 74 "tikz_parser.yy"
    { (yyval.tval) = new TikzDrawNode((yysemantic_stack_[(9) - (3)].sval), (yysemantic_stack_[(9) - (7)].fval)); }
    break;

  case 8:
/* Line 664 of lalr1.cc  */
#line 76 "tikz_parser.yy"
    { (yyval.tval) = new TikzDrawNode((yysemantic_stack_[(9) - (3)].sval), (yysemantic_stack_[(9) - (7)].fval)); }
    break;

  case 9:
/* Line 664 of lalr1.cc  */
#line 78 "tikz_parser.yy"
    { (yyval.tval) = new TikzDrawEdge((yysemantic_stack_[(12) - (6)].sval), (yysemantic_stack_[(12) - (10)].sval), (yysemantic_stack_[(12) - (2)].fval)); }
    break;

  case 10:
/* Line 664 of lalr1.cc  */
#line 80 "tikz_parser.yy"
    { (yyval.tval) = new TikzDrawArc((yysemantic_stack_[(12) - (6)].sval), (yysemantic_stack_[(12) - (10)].sval), (yysemantic_stack_[(12) - (2)].fval)); }
    break;

  case 11:
/* Line 664 of lalr1.cc  */
#line 82 "tikz_parser.yy"
    { (yyval.tval) = new TikzDefinePencolor((yysemantic_stack_[(8) - (3)].fval), (yysemantic_stack_[(8) - (5)].fval), (yysemantic_stack_[(8) - (7)].fval)); }
    break;

  case 12:
/* Line 664 of lalr1.cc  */
#line 84 "tikz_parser.yy"
    { (yyval.tval) = new TikzDefineBrushcolor((yysemantic_stack_[(8) - (3)].fval), (yysemantic_stack_[(8) - (5)].fval), (yysemantic_stack_[(8) - (7)].fval)); }
    break;


/* Line 664 of lalr1.cc  */
#line 482 "tikz_parser.cpp"
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
    while (1 < yystate_stack_.height ())
      {
        yydestruct_ ("Cleanup: popping",
                     yystos_[yystate_stack_[0]],
                     &yysemantic_stack_[0],
                     &yylocation_stack_[0]);
        yypop_ ();
      }

    return yyresult;
    }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (yychar != yyempty_)
          {
            /* Make sure we have latest lookahead translation.  See
               comments at user semantic actions for why this is
               necessary.  */
            yytoken = yytranslate_ (yychar);
            yydestruct_ (YY_NULL, yytoken, &yylval, &yylloc);
          }

        while (1 < yystate_stack_.height ())
          {
            yydestruct_ (YY_NULL,
                         yystos_[yystate_stack_[0]],
                         &yysemantic_stack_[0],
                         &yylocation_stack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  // Generate an error message.
  std::string
  TIKZParser::yysyntax_error_ (int, int)
  {
    return YY_("syntax error");
  }


  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
  const signed char TIKZParser::yypact_ninf_ = -17;
  const signed char
  TIKZParser::yypact_[] =
  {
         5,   -17,    12,    -3,   -17,   -17,     2,     3,    -9,    -8,
       4,     6,   -17,    -7,    -5,    -4,     1,    -1,     0,    13,
      14,    15,    16,    10,    11,    17,   -12,    20,    21,     9,
      18,    23,    19,    22,    24,    25,    27,    28,    29,    26,
      30,    36,    40,    31,    32,    33,    45,    44,    39,    41,
      46,    47,    43,    48,    49,    53,    54,   -17,   -17,    50,
     -17,   -17,    51,    52,    57,    56,    58,    59,    55,    61,
      60,   -17,   -17,   -16,    62,    65,   -17,    64,   -17
  };

  /* YYDEFACT[S] -- default reduction number in state S.  Performed when
     YYTABLE doesn't specify something else to do.  Zero means the
     default is an error.  */
  const unsigned char
  TIKZParser::yydefact_[] =
  {
         0,     3,     0,     0,     1,     2,     0,     0,     0,     0,
       0,     0,     4,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    11,    12,     0,
       8,     7,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     9,    10,     0,     0,     0,     6,     0,     5
  };

  /* YYPGOTO[NTERM-NUM].  */
  const signed char
  TIKZParser::yypgoto_[] =
  {
       -17,   -17,   -17,   -17
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const signed char
  TIKZParser::yydefgoto_[] =
  {
        -1,     2,     3,    12
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If YYTABLE_NINF_, syntax error.  */
  const signed char TIKZParser::yytable_ninf_ = -1;
  const unsigned char
  TIKZParser::yytable_[] =
  {
        74,     5,     6,     7,     8,     9,    10,    11,     1,    75,
      32,    33,     4,    13,    14,    15,    16,    21,    19,    17,
      20,    18,    22,    23,    24,    25,    26,    29,    30,    27,
      28,    34,    35,    36,    38,     0,    31,     0,     0,     0,
       0,     0,    37,    39,    43,    44,    40,    46,    48,    41,
      42,    47,    49,    45,    52,    50,    51,    53,    54,    55,
      59,    56,    57,    58,    62,    63,    60,    61,    68,     0,
      69,    70,     0,    71,    64,    73,    65,    66,    67,    72,
      76,    77,    78
  };

  /* YYCHECK.  */
  const signed char
  TIKZParser::yycheck_[] =
  {
        16,     4,     5,     6,     7,     8,     9,    10,     3,    25,
      22,    23,     0,    11,    11,    24,    24,    21,    25,    15,
      25,    15,    21,    24,    24,    12,    12,    17,    17,    14,
      14,    11,    11,    24,    11,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    24,    24,    17,    17,    24,    21,    12,    25,
      25,    21,    12,    24,    21,    24,    24,    12,    14,    20,
      17,    20,    16,    16,    11,    11,    18,    18,    12,    -1,
      12,    12,    -1,    18,    24,    15,    25,    25,    21,    18,
      18,    16,    18
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  TIKZParser::yystos_[] =
  {
         0,     3,    27,    28,     0,     4,     5,     6,     7,     8,
       9,    10,    29,    11,    11,    24,    24,    15,    15,    25,
      25,    21,    21,    24,    24,    12,    12,    14,    14,    17,
      17,    19,    22,    23,    11,    11,    24,    24,    11,    24,
      24,    25,    25,    17,    17,    24,    21,    21,    12,    12,
      24,    24,    21,    12,    14,    20,    20,    16,    16,    17,
      18,    18,    11,    11,    24,    25,    25,    21,    12,    12,
      12,    18,    18,    15,    16,    25,    18,    16,    18
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  TIKZParser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  TIKZParser::yyr1_[] =
  {
         0,    26,    27,    28,    28,    29,    29,    29,    29,    29,
      29,    29,    29
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  TIKZParser::yyr2_[] =
  {
         0,     2,     3,     0,     2,    16,    15,     9,     9,    12,
      12,     8,     8
  };

#if YYDEBUG
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const TIKZParser::yytname_[] =
  {
    "\"end of file\"", "error", "$undefined", "BEGINTIKZPICTURE",
  "ENDTIKZPICTURE", "NODE", "DRAWNODE", "DRAWEDGE", "DRAWARC", "PENCOLOR",
  "BRUSHCOLOR", "OPEN", "CLOSE", "OPENBRACKET", "CLOSEBRACKET",
  "OPENBRACES", "CLOSEBRACES", "COMMA", "SEMICOLON", "AT", "TO", "CM",
  "CIRCLE", "BCIRCLE", "FLOAT", "STRING", "$accept", "S", "TIKZDOCUMENT",
  "TIKZLINE", YY_NULL
  };


  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const TIKZParser::rhs_number_type
  TIKZParser::yyrhs_[] =
  {
        27,     0,    -1,     3,    28,     4,    -1,    -1,    28,    29,
      -1,     5,    11,    25,    12,    19,    11,    24,    21,    17,
      24,    21,    12,    15,    25,    16,    18,    -1,     5,    11,
      25,    12,    19,    11,    24,    21,    17,    24,    21,    12,
      15,    16,    18,    -1,     6,    11,    25,    12,    23,    24,
      21,    14,    18,    -1,     6,    11,    25,    12,    22,    24,
      21,    12,    18,    -1,     7,    24,    21,    14,    11,    25,
      12,    20,    11,    25,    12,    18,    -1,     8,    24,    21,
      14,    11,    25,    12,    20,    11,    25,    12,    18,    -1,
       9,    15,    24,    17,    24,    17,    24,    16,    -1,    10,
      15,    24,    17,    24,    17,    24,    16,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned char
  TIKZParser::yyprhs_[] =
  {
         0,     0,     3,     7,     8,    11,    28,    44,    54,    64,
      77,    90,    99
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned char
  TIKZParser::yyrline_[] =
  {
         0,    62,    62,    64,    65,    69,    71,    73,    75,    77,
      79,    81,    83
  };

  // Print the state stack on the debug stream.
  void
  TIKZParser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (state_stack_type::const_iterator i = yystate_stack_.begin ();
	 i != yystate_stack_.end (); ++i)
      *yycdebug_ << ' ' << *i;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  TIKZParser::yy_reduce_print_ (int yyrule)
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
  TIKZParser::token_number_type
  TIKZParser::yytranslate_ (int t)
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
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int TIKZParser::yyeof_ = 0;
  const int TIKZParser::yylast_ = 82;
  const int TIKZParser::yynnts_ = 4;
  const int TIKZParser::yyempty_ = -2;
  const int TIKZParser::yyfinal_ = 4;
  const int TIKZParser::yyterror_ = 1;
  const int TIKZParser::yyerrcode_ = 256;
  const int TIKZParser::yyntokens_ = 26;

  const unsigned int TIKZParser::yyuser_token_number_max_ = 280;
  const TIKZParser::token_number_type TIKZParser::yyundef_token_ = 2;

/* Line 1135 of lalr1.cc  */
#line 14 "tikz_parser.yy"
} } // OpenGraphtheory::Import
/* Line 1135 of lalr1.cc  */
#line 952 "tikz_parser.cpp"
/* Line 1136 of lalr1.cc  */
#line 86 "tikz_parser.yy"


#include "../../../Headers/import/tikz/tikz_lexer.h"

void TIKZParser::error(const TIKZParser::location_type &loc, const std::string &msg)
{
    std::cerr << "Error: " << msg << std::endl;
}

static int yylex(TIKZParser::semantic_type * yylval, OpenGraphtheory::Import::TIKZLexer &scanner)
{
    return scanner.yylex(yylval);
}
