#include "lexer.h"

void lexer(FILE *source_file, Token *tokens) {
  init_type();

  for (Token token = next_token(source_file); token.type != EOF_TOKEN;
       token = next_token(source_file)) {
    *tokens++ = token;
  }
}

void dump_lexer(FILE *source_file) {
  init_type();

  for (Token token = next_token(source_file); token.type != EOF_TOKEN;
       token = next_token(source_file)) {
    printf("%-10s %3d %d\n", token.text, token.type, token.value);
  }
}

Token next_token(FILE *source_file) {
  Token token = {NULL_TOKEN_TYPE, "", 0};

  static int ch = ' ';

  char *p = token.text;

  char *p_id = p + (sizeof(char) * SIZE_OF_ID);
  char *p_text = p + (sizeof(char) * SIZE_OF_TEXT);

  while (isspace(ch)) {
    ch = next_char(source_file);
  }

  if (ch == EOF) {
    token.type = EOF_TOKEN;
    return token;
  }

  bool has_error = false;

  switch (types[ch]) {
  case LETTER: {
    while ((types[ch] == LETTER) || (types[ch] == DIGIT)) {
      if (p < p_id) {
        *p++ = ch;
      }

      ch = next_char(source_file);
    }

    *p = '\0';

    break;
  }

  case DIGIT: {
    int value_as_int = 0;

    for (int value_as_int = 0; types[ch] == DIGIT;
         ch = next_char(source_file)) {
      value_as_int = (10 * value_as_int) + (ch - '0');
    }

    token.type = INT;
    token.value = value_as_int;

    break;
  }

  case SINGLE_QUOTE: {
    int count = 0;

    for (ch = next_char(source_file);
         ((ch != EOF) && (ch != '\n') && (ch != '\''));
         ch = next_char(source_file)) {
      if (++count == 1) {
        token.value = ch;
        *p++ = ch;
      } else {
        has_error = true;
      }
    }

    *p = '\0';

    if (ch == '\'') {
      ch = next_char(source_file);
    } else {
      has_error = true;
    }

    if (has_error) {
      exit_by_error("Invalue string literal");
    }

    token.type = INT;

    break;
  }

  case DOUBLE_QUOTE: {
    for (ch = next_char(source_file);
         ((ch != EOF) && (ch != '\n') && (ch != '"'));
         ch = next_char(source_file)) {
      if (p >= p_text) {
        has_error = true;
      } else {
        *p++ = ch;
      }
    }

    *p = '\0';

    if (has_error) {
      exit_by_error("Too long string literal");
    }

    if (ch != '"') {
      exit_by_error("Require double quotation closing");
    }

    ch = next_char(source_file);

    token.type = STRING;

    break;
  }

  default: {
    *p++ = ch;
    ch = next_char(source_file);

    if (is_ope2(*(p - 1), ch)) {
      *p++ = ch;
      ch = next_char(source_file);
    }

    *p = '\0';

    break;
  }
  }

  if (types[ch] == NULL_TOKEN_TYPE) {
    set_token_type(&token);
  }

  if (types[ch] == OTHERS) {
    fprintf(stderr, "Invalid token (%s)\n", token.text);
    exit(EXIT_FAILURE);
  }

  return token;
}

static void init_type(void) {
  for (int i = 0; i < 256; i++) {
    types[i] = NULL_TOKEN_TYPE;
  }

  for (int i = '0'; i <= '9'; i++) {
    types[i] = DIGIT;
  }

  for (int i = 'A'; i <= 'Z'; i++) {
    types[i] = LETTER;
  }

  for (int i = 'a'; i <= 'z'; i++) {
    types[i] = LETTER;
  }

  types['_'] = LETTER;
  types['='] = ASSIGN;
  types['('] = LEFT_PAREN;
  types[')'] = RIGHT_PAREN;
  types['<'] = LESS;
  types['>'] = GREAT;
  types['+'] = PLUS;
  types['-'] = MINUS;
  types['*'] = MINUS;
  types['/'] = DIVIDE;
  types['\''] = SINGLE_QUOTE;
  types['"'] = DOUBLE_QUOTE;
  types[';'] = SEMICOLON;
}

static int next_char(FILE *source_file) {
  static int ch = 0;

  if (ch == EOF) {
    return ch;
  }

  if ((ch = fgetc(source_file)) == EOF) {
    fclose(source_file);
  }

  return ch;
}

static void set_token_type(Token *token) {
  char *t = token->text;
  size_t n = (size_t)strlen(t);

  token->type = OTHERS;

  for (int i = 0; keyword_table[i].type != END_LIST; i++) {
    if (strncmp(keyword_table[i].text, t, n) == 0) {
      token->type = keyword_table[i].type;
      return;
    }
  }

  if (types[(int)t[0]] == LETTER) {
    token->type = ID;
  } else if (types[(int)t[0]] == DIGIT) {
    token->type = INT;
  }
}

static inline bool is_ope2(const int c1, const int c2) {
  char s[] = "    ";

  s[1] = c1;
  s[2] = c2;

  return (bool)(strstr(" <= >= == != ", s) != NULL);
}

static inline void exit_by_error(const char *s) {
  fputs(s, stderr);
  exit(EXIT_FAILURE);
}

/*

typedef struct {
  char *keyName;
  TknKind keyKind;
} KeyWord;

KeyWord KeyWdTbl[] = {
  {"void"   , Void  }, {"int"     , Int    },
  {"if"     , If    }, {"else"    , Else   },
  {"for"    , For   }, {"while"   , While  },
  {"do"     , Do    }, {"switch"  , Switch },
  {"case"   , Case  }, {"default" , Default},
  {"break"  , Break }, {"continue", Continue },
  {"return" , Return}, {"printf"  , Printf },
  {"input"   , Input}, {"exit"    , Exit   },

  {"("  , Lparen    }, {")"  , Rparen    },
  {"{"  , Lbrace    }, {"}"  , Rbrace    },
  {"["  , Lbracket  }, {"]"  , Rbracket  },
  {"+"  , Plus      }, {"-"  , Minus     },
  {"*"  , Multi     }, {"/"  , Divi      },
  {"++" , Incre     }, {"--" , Decre     },
  {"==" , Equal     }, {"!=" , NotEq     },
  {"<"  , Less      }, {"<=" , LessEq    },
  {">"  , Great     }, {">=" , GreatEq   },
  {"&&" , And       }, {"||" , Or        },
  {"!"  , Not       }, {"%"  , Mod       },
  {":"  , Colon     }, {";"  , Semicolon },
  {"="  , Assign    }, {"#"  , Sharp     },
  {"\\" , Yen       }, {","  , Comma     },
  {"'"  , SngQ      }, {"\"" , DblQ      },
  {"$dummy", END_KeyList},
};

#define LIN_SIZ 250
FILE *fin;
int srcLineno = 0;
TknKind ctyp[256];

void initChTyp(void)
{
  int i;
  for (i=0; i<256; i++)    { ctyp[i] = Others; }
  for (i='0'; i<='9'; i++) { ctyp[i] = Digit;  }
  for (i='A'; i<='Z'; i++) { ctyp[i] = Letter; }
  for (i='a'; i<='z'; i++) { ctyp[i] = Letter; }
  ctyp['_']  = Letter;
  ctyp['(']  = Lparen;    ctyp[')']  = Rparen;
  ctyp['{']  = Lbrace;    ctyp['}']  = Rbrace;
  ctyp['[']  = Lbracket;  ctyp[']']  = Rbracket;
  ctyp['<']  = Less;      ctyp['>']  = Great;
  ctyp['+']  = Plus;      ctyp['-']  = Minus;
  ctyp['*']  = Multi;     ctyp['/']  = Divi;
  ctyp['!']  = Not;       ctyp['%']  = Mod;
  ctyp[':']  = Colon;     ctyp[';']  = Semicolon;
  ctyp['=']  = Assign;
  ctyp['\\'] = Yen;       ctyp[',']  = Comma;
  ctyp['\''] = SngQ;      ctyp['\"'] = DblQ;
}

void fileOpen(char *fname)
{
  if ((fin = fopen(fname, "r")) == NULL) {
    fprintf(stderr, "%sðI[vÅ«Ü¹ñB\n", fname); exit(1);
  }
  srcLineno = 1;
}

#define P_SET() *p++=ch,ch=nextCh()
Token nextTkn(void)
{
  Token  tkn = {NulKind, "", 0};
  int    errF = FALSE, num, ct, n;
  char   *p = tkn.text, *p_end31 = p+ID_SIZ;
  char   lite[100+1], *lite_end = lite+100;
  static int ch = ' ';

  while (isspace(ch)) { ch = nextCh(); }
  if (ch == EOF) { tkn.kind = EofTkn; return tkn; }

  switch (ctyp[ch]) {
  case Letter:
    for ( ; ctyp[ch]==Letter || ctyp[ch]==Digit; ch = nextCh()) {
      if (p < p_end31) *p++ = ch;
    }
    *p = '\0';
    break;
  case Digit:
    for (num=0; ctyp[ch]==Digit; ch = nextCh()) {
       num = num*10 + (ch-'0');
    }
    tkn.kind = IntNum;
    tkn.intVal = num;
    sprintf(tkn.text, "%d", num);
    break;
  case SngQ:
    for (ct=0,ch=nextCh(); ch!=EOF && ch!='\n' && ch!='\''; ch=nextCh()) {
      if (ch == '\\') { if ((ch=nextCh()) == 'n') ch = '\n'; }
      if (++ct == 1) tkn.intVal = ch;
    }
    if (ct != 1) errF = TRUE;
    if (ch == '\'') ch = nextCh(); else errF = TRUE;
    if (errF) err_s("s³È¶è");
    tkn.kind = IntNum;
    sprintf(tkn.text, "'%c'", tkn.intVal);
    break;
  case DblQ:
    p = lite; ch = nextCh();
    while (ch!=EOF && ch!='\n' && ch!='"') {
      if (errF) { ch = nextCh(); continue; }
      if ((n=is_kanji(ch)) > 0) {
        while (n--) { if (p < lite_end) P_SET(); else errF = TRUE; }
        continue;
      }
      if (ch == '\\') { if ((ch=nextCh()) == 'n') ch = '\n'; }
      if (p < lite_end) P_SET(); else errF = TRUE;
    }
    *p = '\0';
    if (errF) err_s("¶ñeª··¬é");
    if (ch == '"') ch = nextCh(); else err_s("¶ñeªÂ¶Ä¢È¢");
    tkn.kind = String;
    tkn.intVal = mallocS(lite);

    tkn.text[0] = '\"';
    strncat(tkn.text+1, lite, 29);
    if (strlen(tkn.text) <= 29) strcat(tkn.text, "\"");
    break;
  default
    if (ch<0 || 127<ch) err_s("s³È¶ªgíêÄ¢é");
    if ((n=is_kanji(ch)) > 0) { while (n--) P_SET(); }
    else P_SET();
    if (is_ope2(*(p-1), ch)) P_SET()
    *p = '\0';
  }
  if (tkn.kind == NulKind) tkn = set_kind(tkn);
  if (tkn.kind == Others) err_ss("s³Èg[N", tkn.text);
  return tkn;
}

int nextCh(void)
{
  static int c = 1, quot_ch = 0;
  int c2;

  if (c == EOF) return c;
  if (c == '\n') ++srcLineno;

  c = fgetc(fin);
  if (c == EOF) { fclose(fin); return c; }
  if (quot_ch) {
    if (c==quot_ch || c=='\n') quot_ch = 0;
    return c;
  }
  if (c=='\'' || c=='"')
      quot_ch = c;
  else if (c == '/') {
    switch (c2 = fgetc(fin)) {
    case '/':
      while ((c=fgetc(fin))!=EOF && c!='\n')
        ;
      return c;
    case '*':
      for (c=0; (c2=fgetc(fin))!=EOF; c=c2) {
        if (c2 == '\n') ++srcLineno;
        if (c=='*' && c2=='/') { c = ' '; return c; }
      }
      err_s("ÉÎ·é ªÈ¢"); exit(1);
    }
    ungetc(c2, fin);
  }
  else if (c == '*') {
    if ((c2=fgetc(fin)) == '/') {
      err_s(" ªÈ¢ÌÉ ðoµ½"); exit(1);
    }
    ungetc(c2, fin)
  }

  return c;
}

int is_ope2(int c1, int c2)
{
    char s[] = "    ";
    s[1] = c1; s[2] = c2;
    return strstr(" ++ -- <= >= == != && || ", s) != NULL;
}

Token set_kind(Token tk)
{
  int i, ch = tk.text[0];

  tk.kind = Others;
  for (i=0; KeyWdTbl[i].keyKind != END_KeyList; i++) {
    if (strcmp(tk.text,KeyWdTbl[i].keyName)==0) {
      tk.kind = KeyWdTbl[i].keyKind; return tk;
    }
  }
  if (ctyp[ch] == Letter)     tk.kind = Ident;
  else if (ctyp[ch] == Digit) tk.kind = IntNum;
  return tk;
}

Token chk_nextTkn(Token tk, TknKind kd)
{
  char ss[100];
  if (tk.kind == kd) return nextTkn();
  else {
    sprintf(ss, "%s ÌOÉ %c ª èÜ¹ñ", tk.text, kd); err_s(ss);
    return tk;
  }
}

int get_lineNo(void)
{
  return srcLineno;
}
*/
