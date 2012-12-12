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
#line 39 "fo_parser.cpp"


#include "fo_parser.hpp"

/* User implementation prologue.  */


/* Line 299 of lalr1.cc  */
#line 48 "fo_parser.cpp"
/* Unqualified %code blocks.  */

/* Line 300 of lalr1.cc  */
#line 27 "fo_parser.yy"

    static int yylex(FOParser::semantic_type * yylval, OpenGraphtheory::Logic::FOLexer &scanner);



/* Line 300 of lalr1.cc  */
#line 59 "fo_parser.cpp"

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
#line 15 "fo_parser.yy"
namespace OpenGraphtheory { namespace Logic {

/* Line 382 of lalr1.cc  */
#line 147 "fo_parser.cpp"

  /// Build a parser object.
  FOParser::FOParser (OpenGraphtheory::Logic::FOLexer &scanner_yyarg, OpenGraphtheory::Logic::FOFormula** result_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      scanner (scanner_yyarg),
      result (result_yyarg)
  {
  }

  FOParser::~FOParser ()
  {
  }

#if YYDEBUG
  /*--------------------------------.
  | Print this symbol on YYOUTPUT.  |
  `--------------------------------*/

  inline void
  FOParser::yy_symbol_value_print_ (int yytype,
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
  FOParser::yy_symbol_print_ (int yytype,
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
  FOParser::yydestruct_ (const char* yymsg,
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
  FOParser::yypop_ (unsigned int n)
  {
    yystate_stack_.pop (n);
    yysemantic_stack_.pop (n);
    yylocation_stack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  FOParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  FOParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  FOParser::debug_level_type
  FOParser::debug_level () const
  {
    return yydebug_;
  }

  void
  FOParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif

  inline bool
  FOParser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  FOParser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  FOParser::parse ()
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
#line 51 "fo_parser.yy"
    { *result = (yysemantic_stack_[(2) - (1)].fval); }
    break;

  case 3:

/* Line 690 of lalr1.cc  */
#line 52 "fo_parser.yy"
    { *result = (yysemantic_stack_[(2) - (1)].fval); }
    break;

  case 5:

/* Line 690 of lalr1.cc  */
#line 55 "fo_parser.yy"
    { (yyval.fval) = (yysemantic_stack_[(1) - (1)].fval); }
    break;

  case 6:

/* Line 690 of lalr1.cc  */
#line 56 "fo_parser.yy"
    { (yyval.fval) = (yysemantic_stack_[(1) - (1)].fval); }
    break;

  case 7:

/* Line 690 of lalr1.cc  */
#line 58 "fo_parser.yy"
    { (yyval.fval) = new FO_Forall((yysemantic_stack_[(4) - (2)].sval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 8:

/* Line 690 of lalr1.cc  */
#line 59 "fo_parser.yy"
    { (yyval.fval) = new FO_Exists((yysemantic_stack_[(4) - (2)].sval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 9:

/* Line 690 of lalr1.cc  */
#line 60 "fo_parser.yy"
    { (yyval.fval) = new FO_Forall((yysemantic_stack_[(4) - (2)].sval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 10:

/* Line 690 of lalr1.cc  */
#line 61 "fo_parser.yy"
    { (yyval.fval) = new FO_Exists((yysemantic_stack_[(4) - (2)].sval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 11:

/* Line 690 of lalr1.cc  */
#line 65 "fo_parser.yy"
    { (yyval.fval) = new FO_And((yysemantic_stack_[(3) - (1)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 12:

/* Line 690 of lalr1.cc  */
#line 66 "fo_parser.yy"
    { (yyval.fval) = new FO_And((yysemantic_stack_[(3) - (1)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 13:

/* Line 690 of lalr1.cc  */
#line 69 "fo_parser.yy"
    { (yyval.fval) = new FO_Or((yysemantic_stack_[(3) - (1)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 14:

/* Line 690 of lalr1.cc  */
#line 70 "fo_parser.yy"
    { (yyval.fval) = new FO_Or((yysemantic_stack_[(3) - (1)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 15:

/* Line 690 of lalr1.cc  */
#line 73 "fo_parser.yy"
    { (yyval.fval) = new FO_True(); }
    break;

  case 16:

/* Line 690 of lalr1.cc  */
#line 74 "fo_parser.yy"
    { (yyval.fval) = new FO_False(); }
    break;

  case 17:

/* Line 690 of lalr1.cc  */
#line 75 "fo_parser.yy"
    { (yyval.fval) = (yysemantic_stack_[(1) - (1)].fval); }
    break;

  case 18:

/* Line 690 of lalr1.cc  */
#line 76 "fo_parser.yy"
    { (yyval.fval) = new FO_Not( (yysemantic_stack_[(2) - (2)].fval) ); }
    break;

  case 19:

/* Line 690 of lalr1.cc  */
#line 77 "fo_parser.yy"
    { (yyval.fval) = new FO_Eq((yysemantic_stack_[(3) - (1)].sval), (yysemantic_stack_[(3) - (3)].sval)); }
    break;

  case 20:

/* Line 690 of lalr1.cc  */
#line 78 "fo_parser.yy"
    { (yyval.fval) = new FO_Neq((yysemantic_stack_[(3) - (1)].sval), (yysemantic_stack_[(3) - (3)].sval)); }
    break;

  case 21:

/* Line 690 of lalr1.cc  */
#line 79 "fo_parser.yy"
    { (yyval.fval) = (yysemantic_stack_[(3) - (2)].fval); }
    break;

  case 22:

/* Line 690 of lalr1.cc  */
#line 80 "fo_parser.yy"
    { (yyval.fval) = (yysemantic_stack_[(3) - (2)].fval); }
    break;

  case 23:

/* Line 690 of lalr1.cc  */
#line 81 "fo_parser.yy"
    { (yyval.fval) = new FO_Not( (yysemantic_stack_[(4) - (3)].fval) ); }
    break;

  case 24:

/* Line 690 of lalr1.cc  */
#line 82 "fo_parser.yy"
    { (yyval.fval) = new FO_Not( (yysemantic_stack_[(4) - (3)].fval) ); }
    break;

  case 25:

/* Line 690 of lalr1.cc  */
#line 85 "fo_parser.yy"
    { (yyval.fval) = new FO_Atomic((yysemantic_stack_[(3) - (1)].sval), (yysemantic_stack_[(3) - (3)].pval)); }
    break;

  case 26:

/* Line 690 of lalr1.cc  */
#line 87 "fo_parser.yy"
    { (yyval.pval) = new FO_Parameter((yysemantic_stack_[(2) - (1)].sval), NULL); }
    break;

  case 27:

/* Line 690 of lalr1.cc  */
#line 88 "fo_parser.yy"
    { (yyval.pval) = new FO_Parameter((yysemantic_stack_[(3) - (1)].sval), (yysemantic_stack_[(3) - (3)].pval)); }
    break;



/* Line 690 of lalr1.cc  */
#line 586 "fo_parser.cpp"
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
  FOParser::yysyntax_error_ (int, int)
  {
    return YY_("syntax error");
  }


  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
  const signed char FOParser::yypact_ninf_ = -14;
  const signed char
  FOParser::yypact_[] =
  {
         2,   -13,    -9,     3,   -14,   -14,     2,    36,    35,    40,
      15,    28,     1,   -14,    39,    43,     2,    37,   -14,    34,
      22,    38,    41,    42,   -14,   -14,    29,    29,   -14,    29,
      29,     2,     2,    44,    24,   -14,   -14,   -14,   -14,    -2,
     -14,   -14,   -14,   -14,   -14,   -14,    26,   -14,    26,   -14,
     -14,    42,   -14,   -14
  };

  /* YYDEFACT[S] -- default reduction number in state S.  Performed when
     YYTABLE doesn't specify something else to do.  Zero means the
     default is an error.  */
  const unsigned char
  FOParser::yydefact_[] =
  {
         4,     0,     0,     0,    15,    16,     4,     0,     0,     0,
       5,     6,     0,    17,     0,     0,     4,     0,    18,     0,
       0,     0,     0,     0,     1,     2,     0,     0,     3,     0,
       0,     4,     4,     0,     0,    21,    22,    19,    20,     0,
      25,    11,    13,    12,    14,     7,     9,     8,    10,    23,
      24,     0,    26,    27
  };

  /* YYPGOTO[NTERM-NUM].  */
  const signed char
  FOParser::yypgoto_[] =
  {
       -14,   -14,    -4,   -14,   -14,    -6,    49,     4
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const signed char
  FOParser::yydefgoto_[] =
  {
        -1,     8,     9,    10,    11,    12,    13,    40
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If YYTABLE_NINF_, syntax error.  */
  const signed char FOParser::yytable_ninf_ = -1;
  const unsigned char
  FOParser::yytable_[] =
  {
        20,    28,    19,    14,    51,     1,     2,    15,    29,    30,
      34,     3,    33,    52,     4,     5,     6,    16,     7,    17,
      41,    42,    26,    43,    44,    46,    48,    45,    47,    29,
      30,    29,    30,    29,    30,    24,    27,    36,     3,    50,
      25,     4,     5,     6,    31,     7,    21,    22,    32,    35,
      23,    23,    18,     0,    37,    53,     0,    38,    39,    49
  };

  /* YYCHECK.  */
  const signed char
  FOParser::yycheck_[] =
  {
         6,     0,     6,    16,     6,     3,     4,    16,     7,     8,
      16,     9,    16,    15,    12,    13,    14,    14,    16,    16,
      26,    27,     7,    29,    30,    31,    32,    31,    32,     7,
       8,     7,     8,     7,     8,     0,     8,    15,     9,    15,
       0,    12,    13,    14,     5,    16,    10,    11,     5,    15,
      14,    14,     3,    -1,    16,    51,    -1,    16,    16,    15
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  FOParser::yystos_[] =
  {
         0,     3,     4,     9,    12,    13,    14,    16,    18,    19,
      20,    21,    22,    23,    16,    16,    14,    16,    23,    19,
      22,    10,    11,    14,     0,     0,     7,     8,     0,     7,
       8,     5,     5,    19,    22,    15,    15,    16,    16,    16,
      24,    22,    22,    22,    22,    19,    22,    19,    22,    15,
      15,     6,    15,    24
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  FOParser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  FOParser::yyr1_[] =
  {
         0,    17,    18,    18,    19,    19,    19,    19,    19,    19,
      19,    20,    20,    21,    21,    22,    22,    22,    22,    22,
      22,    22,    22,    22,    22,    23,    24,    24
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  FOParser::yyr2_[] =
  {
         0,     2,     2,     2,     0,     1,     1,     4,     4,     4,
       4,     3,     3,     3,     3,     1,     1,     1,     2,     3,
       3,     3,     3,     4,     4,     3,     2,     3
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const FOParser::yytname_[] =
  {
    "\"end of file\"", "error", "$undefined", "FORALL", "EXISTS", "DOT",
  "COMMA", "AND", "OR", "NOT", "EQ", "NEQ", "_TRUE", "_FALSE", "OPEN",
  "CLOSE", "STRING", "$accept", "S", "FOFORMULA", "ANDS", "ORS",
  "AFOFORMULA", "RELATION", "PARAMS", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const FOParser::rhs_number_type
  FOParser::yyrhs_[] =
  {
        18,     0,    -1,    19,     0,    -1,    22,     0,    -1,    -1,
      20,    -1,    21,    -1,     3,    16,     5,    19,    -1,     4,
      16,     5,    19,    -1,     3,    16,     5,    22,    -1,     4,
      16,     5,    22,    -1,    20,     7,    22,    -1,    22,     7,
      22,    -1,    21,     8,    22,    -1,    22,     8,    22,    -1,
      12,    -1,    13,    -1,    23,    -1,     9,    23,    -1,    16,
      10,    16,    -1,    16,    11,    16,    -1,    14,    19,    15,
      -1,    14,    22,    15,    -1,     9,    14,    19,    15,    -1,
       9,    14,    22,    15,    -1,    16,    14,    24,    -1,    16,
      15,    -1,    16,     6,    24,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned char
  FOParser::yyprhs_[] =
  {
         0,     0,     3,     6,     9,    10,    12,    14,    19,    24,
      29,    34,    38,    42,    46,    50,    52,    54,    56,    59,
      63,    67,    71,    75,    80,    85,    89,    92
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned char
  FOParser::yyrline_[] =
  {
         0,    51,    51,    52,    54,    55,    56,    58,    59,    60,
      61,    65,    66,    69,    70,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    85,    87,    88
  };

  // Print the state stack on the debug stream.
  void
  FOParser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (state_stack_type::const_iterator i = yystate_stack_.begin ();
	 i != yystate_stack_.end (); ++i)
      *yycdebug_ << ' ' << *i;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  FOParser::yy_reduce_print_ (int yyrule)
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
  FOParser::token_number_type
  FOParser::yytranslate_ (int t)
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

  const int FOParser::yyeof_ = 0;
  const int FOParser::yylast_ = 59;
  const int FOParser::yynnts_ = 8;
  const int FOParser::yyempty_ = -2;
  const int FOParser::yyfinal_ = 24;
  const int FOParser::yyterror_ = 1;
  const int FOParser::yyerrcode_ = 256;
  const int FOParser::yyntokens_ = 17;

  const unsigned int FOParser::yyuser_token_number_max_ = 271;
  const FOParser::token_number_type FOParser::yyundef_token_ = 2;


/* Line 1136 of lalr1.cc  */
#line 15 "fo_parser.yy"
} } // OpenGraphtheory::Logic

/* Line 1136 of lalr1.cc  */
#line 1020 "fo_parser.cpp"


/* Line 1138 of lalr1.cc  */
#line 90 "fo_parser.yy"


#include "../../../Headers/logic/FirstOrderLogic/fo_lexer.h"

void FOParser::error(const FOParser::location_type &loc, const std::string &msg)
{
    std::cerr << "Error: " << msg << std::endl;
}

static int yylex(FOParser::semantic_type * yylval, OpenGraphtheory::Logic::FOLexer &scanner)
{
    return scanner.yylex(yylval);
}

