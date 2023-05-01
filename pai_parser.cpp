// A Bison parser, made by GNU Bison 3.7.5.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.

// "%code top" blocks.
#line 5 "pai.y"

#include <variant>
#include <charconv>
#include "pai.h"

#line 45 "pai_parser.cpp"




#include "pai_parser.h"




#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
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
    while (false)
# endif


// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace yy {
#line 144 "pai_parser.cpp"

  /// Build a parser object.
  parser::parser ()
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr)
#else

#endif
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | symbol kinds.  |
  `---------------*/

  // basic_symbol.
  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
    , location (that.location)
  {
    switch (this->kind ())
    {
      case symbol_kind::S_bin_op: // bin_op
        value.copy< OperatorType > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_expr: // expr
        value.copy< SharedExpr > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NUMBER: // NUMBER
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
        value.copy< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_exprs: // exprs
        value.copy< std::vector<SharedExpr>* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_numbers: // numbers
        value.copy< std::vector<i64>* > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }



  template <typename Base>
  parser::symbol_kind_type
  parser::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }

  template <typename Base>
  bool
  parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->kind ())
    {
      case symbol_kind::S_bin_op: // bin_op
        value.move< OperatorType > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_expr: // expr
        value.move< SharedExpr > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_NUMBER: // NUMBER
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
        value.move< std::string > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_exprs: // exprs
        value.move< std::vector<SharedExpr>* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_numbers: // numbers
        value.move< std::vector<i64>* > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

    location = YY_MOVE (s.location);
  }

  // by_kind.
  parser::by_kind::by_kind ()
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  parser::by_kind::by_kind (by_kind&& that)
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  parser::by_kind::by_kind (const by_kind& that)
    : kind_ (that.kind_)
  {}

  parser::by_kind::by_kind (token_kind_type t)
    : kind_ (yytranslate_ (t))
  {}

  void
  parser::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  void
  parser::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  parser::symbol_kind_type
  parser::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }

  parser::symbol_kind_type
  parser::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_kind_type
  parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_bin_op: // bin_op
        value.YY_MOVE_OR_COPY< OperatorType > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_expr: // expr
        value.YY_MOVE_OR_COPY< SharedExpr > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NUMBER: // NUMBER
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_exprs: // exprs
        value.YY_MOVE_OR_COPY< std::vector<SharedExpr>* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_numbers: // numbers
        value.YY_MOVE_OR_COPY< std::vector<i64>* > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_bin_op: // bin_op
        value.move< OperatorType > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_expr: // expr
        value.move< SharedExpr > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NUMBER: // NUMBER
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
        value.move< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_exprs: // exprs
        value.move< std::vector<SharedExpr>* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_numbers: // numbers
        value.move< std::vector<i64>* > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_bin_op: // bin_op
        value.copy< OperatorType > (that.value);
        break;

      case symbol_kind::S_expr: // expr
        value.copy< SharedExpr > (that.value);
        break;

      case symbol_kind::S_NUMBER: // NUMBER
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
        value.copy< std::string > (that.value);
        break;

      case symbol_kind::S_exprs: // exprs
        value.copy< std::vector<SharedExpr>* > (that.value);
        break;

      case symbol_kind::S_numbers: // numbers
        value.copy< std::vector<i64>* > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_bin_op: // bin_op
        value.move< OperatorType > (that.value);
        break;

      case symbol_kind::S_expr: // expr
        value.move< SharedExpr > (that.value);
        break;

      case symbol_kind::S_NUMBER: // NUMBER
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
        value.move< std::string > (that.value);
        break;

      case symbol_kind::S_exprs: // exprs
        value.move< std::vector<SharedExpr>* > (that.value);
        break;

      case symbol_kind::S_numbers: // numbers
        value.move< std::vector<i64>* > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.kind_ = yytranslate_ (yylex (&yyla.value, &yyla.location));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_bin_op: // bin_op
        yylhs.value.emplace< OperatorType > ();
        break;

      case symbol_kind::S_expr: // expr
        yylhs.value.emplace< SharedExpr > ();
        break;

      case symbol_kind::S_NUMBER: // NUMBER
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
        yylhs.value.emplace< std::string > ();
        break;

      case symbol_kind::S_exprs: // exprs
        yylhs.value.emplace< std::vector<SharedExpr>* > ();
        break;

      case symbol_kind::S_numbers: // numbers
        yylhs.value.emplace< std::vector<i64>* > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // program: exprs
#line 50 "pai.y"
          {
        for(auto e : *yystack_[0].value.as < std::vector<SharedExpr>* > ())
            print(e);
        delete yystack_[0].value.as < std::vector<SharedExpr>* > ();
    }
#line 777 "pai_parser.cpp"
    break;

  case 3: // exprs: %empty
#line 57 "pai.y"
            {
        yylhs.value.as < std::vector<SharedExpr>* > () = new std::vector<SharedExpr>();
        *yylhs.value.as < std::vector<SharedExpr>* > () = {};
     }
#line 786 "pai_parser.cpp"
    break;

  case 4: // exprs: exprs expr
#line 62 "pai.y"
                {
        yystack_[1].value.as < std::vector<SharedExpr>* > ()->push_back(yystack_[0].value.as < SharedExpr > ());
        yylhs.value.as < std::vector<SharedExpr>* > () = yystack_[1].value.as < std::vector<SharedExpr>* > ();
     }
#line 795 "pai_parser.cpp"
    break;

  case 5: // expr: IDENTIFIER
#line 68 "pai.y"
               {
        yylhs.value.as < SharedExpr > () = identifier(yystack_[0].value.as < std::string > ());
    }
#line 803 "pai_parser.cpp"
    break;

  case 6: // expr: NUMBER
#line 72 "pai.y"
           {
        i64 value;
        pexit(std::from_chars(yystack_[0].value.as < std::string > ().data(), yystack_[0].value.as < std::string > ().data() + yystack_[0].value.as < std::string > ().size(), value).ec == std::errc{},
              "std::from_chars failed\n");
        yylhs.value.as < SharedExpr > () = number(value);
    }
#line 814 "pai_parser.cpp"
    break;

  case 7: // expr: TRUE
#line 79 "pai.y"
         {
        yylhs.value.as < SharedExpr > () = boolean(true);
    }
#line 822 "pai_parser.cpp"
    break;

  case 8: // expr: FALSE
#line 83 "pai.y"
          {
        yylhs.value.as < SharedExpr > () = boolean(false);
    }
#line 830 "pai_parser.cpp"
    break;

  case 9: // expr: OBRAK numbers CBRAK
#line 87 "pai.y"
                        {
        pexit(yystack_[1].value.as < std::vector<i64>* > (), "Found nullptr\n");
        yylhs.value.as < SharedExpr > () = integers(*yystack_[1].value.as < std::vector<i64>* > ());
        delete yystack_[1].value.as < std::vector<i64>* > ();
    }
#line 840 "pai_parser.cpp"
    break;

  case 10: // expr: expr bin_op expr
#line 93 "pai.y"
                     {
        yylhs.value.as < SharedExpr > () = operation(yystack_[2].value.as < SharedExpr > (), yystack_[1].value.as < OperatorType > (), yystack_[0].value.as < SharedExpr > ());
    }
#line 848 "pai_parser.cpp"
    break;

  case 11: // expr: EXCL expr
#line 97 "pai.y"
              {
        yylhs.value.as < SharedExpr > () = operation(yystack_[0].value.as < SharedExpr > (), OT_neg, nullptr);
    }
#line 856 "pai_parser.cpp"
    break;

  case 12: // expr: OPAREN expr CPAREN
#line 101 "pai.y"
                       {
        yylhs.value.as < SharedExpr > () = yystack_[1].value.as < SharedExpr > ();
    }
#line 864 "pai_parser.cpp"
    break;

  case 13: // numbers: NUMBER
#line 106 "pai.y"
           {
        i64 value;
        pexit(std::from_chars(yystack_[0].value.as < std::string > ().data(), yystack_[0].value.as < std::string > ().data() + yystack_[0].value.as < std::string > ().size(), value).ec == std::errc{},
              "std::from_chars failed\n");

        yylhs.value.as < std::vector<i64>* > () = new std::vector<i64>();
        yylhs.value.as < std::vector<i64>* > ()->push_back(value);
    }
#line 877 "pai_parser.cpp"
    break;

  case 14: // numbers: numbers COMMA NUMBER
#line 115 "pai.y"
                         {
        i64 value;
        pexit(std::from_chars(yystack_[0].value.as < std::string > ().data(), yystack_[0].value.as < std::string > ().data() + yystack_[0].value.as < std::string > ().size(), value).ec == std::errc{},
              "std::from_chars failed\n");

        yystack_[2].value.as < std::vector<i64>* > ()->push_back(value);
        yylhs.value.as < std::vector<i64>* > () = yystack_[2].value.as < std::vector<i64>* > ();
    }
#line 890 "pai_parser.cpp"
    break;

  case 15: // bin_op: PLUS
#line 125 "pai.y"
                 { yylhs.value.as < OperatorType > () = OT_plus;    }
#line 896 "pai_parser.cpp"
    break;

  case 16: // bin_op: MINUS
#line 126 "pai.y"
                 { yylhs.value.as < OperatorType > () = OT_minus;   }
#line 902 "pai_parser.cpp"
    break;

  case 17: // bin_op: STAR
#line 127 "pai.y"
                 { yylhs.value.as < OperatorType > () = OT_mul;     }
#line 908 "pai_parser.cpp"
    break;

  case 18: // bin_op: SLASH
#line 128 "pai.y"
                 { yylhs.value.as < OperatorType > () = OT_div;     }
#line 914 "pai_parser.cpp"
    break;

  case 19: // bin_op: DOUBLE_AMP
#line 129 "pai.y"
                 { yylhs.value.as < OperatorType > () = OT_and;     }
#line 920 "pai_parser.cpp"
    break;

  case 20: // bin_op: DOUBLE_PIPE
#line 130 "pai.y"
                 { yylhs.value.as < OperatorType > () = OT_or;      }
#line 926 "pai_parser.cpp"
    break;

  case 21: // bin_op: LESS
#line 131 "pai.y"
                 { yylhs.value.as < OperatorType > () = OT_less;    }
#line 932 "pai_parser.cpp"
    break;

  case 22: // bin_op: GREATER
#line 132 "pai.y"
                 { yylhs.value.as < OperatorType > () = OT_greater; }
#line 938 "pai_parser.cpp"
    break;

  case 23: // bin_op: DOUBLE_EQUAL
#line 133 "pai.y"
                 { yylhs.value.as < OperatorType > () = OT_equal;   }
#line 944 "pai_parser.cpp"
    break;


#line 948 "pai_parser.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        std::string msg = YY_("syntax error");
        error (yyla.location, YY_MOVE (msg));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yyerror_range[1].location = yystack_[0].location;
        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

#if YYDEBUG || 0
  const char *
  parser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytname_[yysymbol];
  }
#endif // #if YYDEBUG || 0





  const signed char parser::yypact_ninf_ = -9;

  const signed char parser::yytable_ninf_ = -1;

  const signed char
  parser::yypact_[] =
  {
      -9,     2,    -4,    -9,    -9,    -9,    -4,    -8,    -9,    -9,
      -4,    15,     6,    -9,    -1,    -9,    -9,    -9,    -9,    -9,
      -9,    -9,    -9,    -9,    -9,    -4,    -9,    -7,    -9,    15,
      -9
  };

  const signed char
  parser::yydefact_[] =
  {
       3,     0,     2,     1,     7,     8,     0,     0,     6,     5,
       0,     4,     0,    13,     0,    11,    20,    19,    23,    21,
      22,    15,    16,    17,    18,     0,    12,     0,     9,    10,
      14
  };

  const signed char
  parser::yypgoto_[] =
  {
      -9,    -9,    -9,    -3,    -9,    -9
  };

  const signed char
  parser::yydefgoto_[] =
  {
       0,     1,     2,    11,    14,    25
  };

  const signed char
  parser::yytable_[] =
  {
       4,     5,     3,    12,     6,    27,     7,    15,    13,    30,
      28,     0,     8,     9,     0,    26,     0,     0,     0,     0,
       0,     0,    29,    10,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    16,    17,    18,    19,    20,    21,    22,
      23,    24
  };

  const signed char
  parser::yycheck_[] =
  {
       4,     5,     0,     6,     8,     6,    10,    10,    16,    16,
      11,    -1,    16,    17,    -1,     9,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    27,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    18,    19,    20,    21,    22,    23,    24,
      25,    26
  };

  const signed char
  parser::yystos_[] =
  {
       0,    29,    30,     0,     4,     5,     8,    10,    16,    17,
      27,    31,    31,    16,    32,    31,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    33,     9,     6,    11,    31,
      16
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    28,    29,    30,    30,    31,    31,    31,    31,    31,
      31,    31,    31,    32,    32,    33,    33,    33,    33,    33,
      33,    33,    33,    33
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     1,     0,     2,     1,     1,     1,     1,     3,
       3,     2,     3,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "IF", "TRUE", "FALSE",
  "COMMA", "BREAK", "OPAREN", "CPAREN", "OBRAK", "CBRAK", "OBRACE",
  "CBRACE", "WHILE", "ASSIGN", "NUMBER", "IDENTIFIER", "DOUBLE_PIPE",
  "DOUBLE_AMP", "DOUBLE_EQUAL", "LESS", "GREATER", "PLUS", "MINUS", "STAR",
  "SLASH", "EXCL", "$accept", "program", "exprs", "expr", "numbers",
  "bin_op", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const unsigned char
  parser::yyrline_[] =
  {
       0,    50,    50,    57,    62,    68,    72,    79,    83,    87,
      93,    97,   101,   106,   115,   125,   126,   127,   128,   129,
     130,   131,   132,   133
  };

  void
  parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  parser::symbol_kind_type
  parser::yytranslate_ (int t)
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const signed char
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
      25,    26,    27
    };
    // Last valid token kind.
    const int code_max = 282;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return YY_CAST (symbol_kind_type, translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

} // yy
#line 1320 "pai_parser.cpp"

#line 135 "pai.y"

