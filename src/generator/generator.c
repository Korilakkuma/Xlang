/**************************************************************/
/*    filename:cci_code.c R[h¶¬ÆÇÆðßÀs    */
/**************************************************************/
#define CCI_CODE_C
#include "cci.h"
#include "cci_prot.h"

#define CODE_SIZ 20000       /* R[hi[ÌæTCY              */
Inst code[CODE_SIZ+1];       /* R[hi[                        */
int codeCt = -1;             /* code[0]`code[codeCt]ªLø       */

#define STACK_SIZ 100        /* X^bNÌTCY                  */
#define STACK_BTM 0          /* X^bNÌê                      */
int stack[STACK_SIZ+1];      /* IyhX^bN                */
int stp = STACK_BTM;         /* stack[]ÌX^bN|C^         */

#define MEM_MAX 0xFFFF       /* TCY                      */
char memory[MEM_MAX+1];      /* memory[0]`[MEM_MAX]ªÌæ  */
int globalAdrs = 1*INTSIZE;  /* æªÉÍÃIÌæTCYðüêé    */

int Pc;                      /* Pc:vOJE^ */
int baseReg;                 /* x[XWX^        */

int gencode3(OpCode op, int fg, int dt) /* R[h¶¬ */
{                                      /* memo:gencode1,gencode2Í#defineè` */
  if (const_fold(op)) return codeCt;                              /* èô */
  if (++codeCt > CODE_SIZ) {
    err_fi("R[hª%dXebvð´¦½", CODE_SIZ); exit(1);
  }
  code[codeCt].opcode = op;                                           /* ½ß */
  code[codeCt].flag   = fg;                                         /* tO */
  code[codeCt].opdata = dt;                                           /* l */

  return codeCt;                                      /* i[µ½½ßsðÔ· */
}

void gencode_Unary(TknKind kd) /* P½ßì¬ */
{
  OpCode op = NOP;

  switch (kd) {
  case Plus: return;                                   /* +ÌÆ«Í½àµÈ¢ */
  case Minus: op = NEG; break;
  case Not:   op = NOT; break;
  case Incre: op = INC; break;
  case Decre: op = DEC; break;
  }
  gencode1(op);
}

void gencode_Binary(TknKind kd) /* ñ½ßì¬ */
{
  OpCode op = NOP;

  switch (kd) {
  case Plus:  op = ADD;  break;     case Minus:   op = SUB;  break;
  case Multi: op = MUL;  break;     case Divi:    op = DIV;  break;
  case Less:  op = LESS; break;     case LessEq:  op = LSEQ; break;
  case Great: op = GRT;  break;     case GreatEq: op = GTEQ; break;
  case Equal: op = EQU;  break;     case NotEq:   op = NTEQ; break;
  case And:   op = AND;  break;     case Or:      op = OR;   break;
  case Mod:   op = MOD;  break;
  }
  gencode1(op);
}

void to_leftVal(void) /* ¶ÓlÉ·é */
{
  switch (code[codeCt].opcode) {
  case VAL: --codeCt; break;                                       /* VALæÁ */
  case LOD: code[codeCt].opcode = LDA; break;                   /* LOD ¨ LDA */
  default:  err_s("s³È¶Ól");                  /* a+b=100; â ++10; ÈÇ */
  }
}

/* [®Ìl]ðc·ASSV½ßðc³È¢ASS½ßÉÏXBÜ½ÍDEL½ßÇÁ */
void remove_val(void) /* ®ÌlÌí */
{
  if (code[codeCt].opcode == ASSV) code[codeCt].opcode = ASS;        /* ASSÉ */
  else gencode1(DEL);                         /*  é¢ÍDELÅí(var++ÈÇ) */
}

int mallocS(char *s) /* ¶ñmÛ */
{
  int adrs;
  if (s == NULL) s = "";                                          /* ³ïÈl */
  adrs = mallocG(strlen(s) + 1);                                  /* ÌæmÛ */
  strcpy(mem_adrs(adrs), s);                                  /* ¶ñRs[ */
  return adrs;
}

/* åæÌæÉnoCgÌðmÛµæªÊu(memory[]ÌY)ðÔ· */
int mallocG(int n) /* åæt */
{
  if (n <= 0) return globalAdrs;                  /* »ÝÌó«ÌææªðÔ· */
  globalAdrs = (globalAdrs+3)/4*4;               /* «Eí¹Ì½ß4Ì{É */
  if (globalAdrs+n > MEM_MAX) {
    fprintf(stderr, "s«Å·B\n"); exit(1);
  }
  memset(memory+globalAdrs, 0, n);                              /* ÌæNA */
  globalAdrs += n;                                                /* ÌæmÛ */
  return globalAdrs - n;                                    /* mÛÌæÌæª */
}

char *mem_adrs(int indx) /* memory[]ÌAhX */
{
  return memory + indx;
}

/* èæ¾BR[hª1Â¾¯¦A»êªLDI½ßÈçèÅ é */
/* varªNULLÈçÔnðÛ¶·éiè»èÌOõj          */
/* èÈç^ðÔµAèlð*varÉüêAR[hÍí·é     */
int get_const(int *var) /* èæ¾ */
{
  static int start_codeCt = -99;

  if (var == NULL) { start_codeCt = nextCodeCt(); return TRUE; }  /* ÔnÛ¶ */

  if (codeCt==start_codeCt && code[codeCt].opcode==LDI) {
    *var = code[codeCt].opdata;                                 /* èli[ */
    --codeCt;                                                 /* R[hÍsv */
    return TRUE;                                                /* è¾Á½ */
  }
  return FALSE;                                             /* èÅÈ©Á½ */
}

int const_fold(OpCode op) /* èô */
{
  int n = codeCt, dt = code[n].opdata;

  if (n<=1 || code[n].opcode != LDI) return FALSE;                /* ³kÈµ */
  if (op == NOT) { code[n].opdata = !dt; return TRUE; }              /* !Z */
  if (op == NEG) { code[n].opdata = -dt; return TRUE; }              /* -Z */
  if (code[n-1].opcode==LDI && is_binaryOP(op)) {                 /* ñZ */
    code[n-1].opdata = binaryExpr(op, code[n-1].opdata, dt);
    --codeCt; return TRUE;                                       /* 1½ßí */
  }
  return FALSE;                                                   /* ³kÈµ */
}

int is_binaryOP(OpCode op) /* ñZ½ßÈç^ */
{
  return op==ADD  || op==SUB  || op==MUL || op==DIV  || op==MOD ||
         op==LESS || op==LSEQ || op==GRT || op==GTEQ || op==EQU ||
         op==NTEQ || op==AND  || op==OR;
}

int binaryExpr(OpCode op, int d1, int d2) /* èZ */
{
  if ((op==DIV || op==MOD) && d2==0) { d2 = 1; err_s("[ZÅ·B"); }
  switch(op) {
  case ADD:  d1 = d1 +  d2; break;
  case SUB:  d1 = d1 -  d2; break;
  case MUL:  d1 = d1 *  d2; break;
  case DIV:  d1 = d1 /  d2; break;
  case MOD:  d1 = d1 %  d2; break;
  case LESS: d1 = d1 <  d2; break;
  case LSEQ: d1 = d1 <= d2; break;
  case GRT:  d1 = d1 >  d2; break;
  case GTEQ: d1 = d1 >= d2; break;
  case EQU:  d1 = d1 == d2; break;
  case NTEQ: d1 = d1 != d2; break;
  case AND:  d1 = d1 && d2; break;
  case OR:   d1 = d1 || d2; break;
  }
  return d1;
}

int is_code(int n, OpCode op, int dt) /* R[hªêvÈç^ */
{
  return code[n].opcode==op && code[n].opdata==dt;
}

int nextCodeCt(void) /* Ì½ßi[Êu */
{
  return codeCt+1;
}

void backPatch(int n, int adrs) /* R[hÌ¢èf[^ðã */
{
  code[n].opdata = adrs;
}

void backPatch_callAdrs(void) /* CALLÌ¢èÔnðmè */
{
  int i, n;

  if (code[0].opdata < 0) err_s("mainÖªÈ¢");       /* code[0]ÍCALL½ß */
  for (i=2; i<=codeCt; i++) {
    n = code[i].opdata;
    if (code[i].opcode==CALL && n<0) {          /* CALLÌÔnnª(=¢è)Èç */
      code[i].opdata = tb_ptr(-n)->adrs;    /* Î·éÖL\ÌÔnðÝè */
      if (code[i].opdata < 0)                       /* »êÅàlÈçG[ */
        err_ss("¢è`ÌÖ", tb_ptr(-n)->name);
    }
  }
}

void backPatch_RET(int fncAdrs) /* RETÖAJMPÌ¢èÔn */
{
  int i;
  for(i=codeCt; i>=fncAdrs; i--)        /* ¼OªRETÖWJMPÈçsvÈÌÅí */
    if (is_code(i, JMP, NO_FIX_RET_ADRS)) --codeCt; else break;
  for(i=codeCt; i>=fncAdrs; i--)                    /* ¢èÔnÈçÔnÝè */
    if (is_code(i, JMP, NO_FIX_RET_ADRS)) code[i].opdata = codeCt+1;
}

void backPatch_BREAK(int looptop) /* breakpJMPÌòæðã */
{
  int i;
  for(i=codeCt; i>=looptop; i--)        /* ¼OªbreakpJMPÈçsvÈÌÅí */
    if (is_code(i, JMP, NO_FIX_BREAK_ADRS)) --codeCt; else break;
  for(i=codeCt; i>=looptop; i--)                    /* ¢èÔnÈçÔnÝè */
    if (is_code(i, JMP, NO_FIX_BREAK_ADRS)) code[i].opdata = codeCt+1;
}

#define IntMem(n)      (*(int *)(memory+n))
#define ZERO_CHK()     if(stack[stp]==0)exe_err("[ZÅ·B")
#define UNI_OP(op)     stack[stp] = op stack[stp]
#define BIN_OP(op)     stack[stp-1] = stack[stp-1] op stack[stp],--stp
#define INCDEC(dt)     IntMem(stack[stp])+=(dt),stack[stp]=IntMem(stack[stp])
#define ASSIGN(adr,dt) IntMem(adr)=dt
#define PUSH(dt)       stack[++stp]=dt
#define POP()          stack[stp--]

int execute(void) /* vOÀs */
{
  int opCode, opData, adrs;
  int start_localMEM = IntMem(0);        /* 0ÔnàeÍX^bNÌæÌJnÊu */

  Pc = 0;
  baseReg = MEM_MAX;
  stp = STACK_BTM;                               /* stack[]p|C^Ìú» */

  for (;;) {
    if (Pc<0 || codeCt<Pc) {
        fprintf(stderr, "s³ÈI¹(Pc=%d)\n", Pc); exit(1);      /* OÌ½ß */
    }

    opCode = code[Pc].opcode;
    opData = code[Pc].opdata;
    adrs = (code[Pc].flag & 0x01) ? baseReg+opData : opData;      /* âÎÔn */
    ++Pc;                                   /* PcÍÉÀs·é½ßÊuð¦· */

    if (stp > STACK_SIZ) exe_err("stack overflow");
    if (stp < STACK_BTM) exe_err("stack underflow");

    switch(opCode) {
    case DEL:  --stp; break;                              /* X^bN©çí */
    case STOP: if (stp > 0) return POP(); else return 0;            /* OSßl */
    case JMP:  Pc = opData; break;                                /* Wv */
    case JPT:  if (stack[stp--])  Pc = opData; break;         /* TrueWv */
    case JPF:  if (!stack[stp--]) Pc = opData; break;        /* FalseWv */
    case LIB:  if (opData == EXIT_F) return POP();           /* exit(n)Ì */
               library(opData); break;            /* CuÖ(ÖíÊ) */
    case LOD:  PUSH(IntMem(adrs)); break;
    case LDA:  PUSH(adrs); break;
    case LDI:  PUSH(opData); break;
    case STO:  ASSIGN(adrs, stack[stp]); --stp; break;
    case ADBR: baseReg += opData;                             /* t[mÛ */
               if (baseReg < start_localMEM) {
                 fprintf(stderr, "X^bNI[o[\n"); exit(1);
               }
               break;
    case NOP:  ++Pc; break;
    case ASS:  ASSIGN(stack[stp-1], stack[stp]); stp -= 2; break;
    case ASSV: ASSIGN(stack[stp-1], stack[stp]);
               stack[stp-1] = stack[stp]; --stp; break;
    case VAL:  stack[stp] = IntMem(stack[stp]);  break;           /* lÉÏ· */
    case EQCMP: if (opData == stack[stp]) stack[stp] = 1; else PUSH(0);
               break;                                         /* switchpär */
    case CALL: PUSH(Pc); Pc = opData; break;      /* ßÔnðÛ¶µÄWv */
    case RET:  Pc = POP(); break;                             /* ßÔnÌ */
    case INC:  INCDEC(+1); break;
    case DEC:  INCDEC(-1); break;
    case NOT:  UNI_OP(!);  break;
    case NEG:  UNI_OP(-);  break;
    case DIV:  ZERO_CHK(); BIN_OP(/); break;
    case MOD:  ZERO_CHK(); BIN_OP(%); break;
    case ADD:  BIN_OP(+);  break;
    case SUB:  BIN_OP(-);  break;
    case MUL:  BIN_OP(*);  break;
    case LESS: BIN_OP(<);  break;
    case LSEQ: BIN_OP(<=); break;
    case GRT:  BIN_OP(>);  break;
    case GTEQ: BIN_OP(>=); break;
    case EQU:  BIN_OP(==); break;
    case NTEQ: BIN_OP(!=); break;
    case AND:  BIN_OP(&&); break;
    case OR:   BIN_OP(||); break;
    default:   fprintf(stderr, "s³È½ß(Pc=%d)\n", Pc-1); exit(1);
    }
  }
}

void library(int fnc_typ) /* gÖÀs */
{
  int dt = 0;
  char ss[40] = "";

  if (fnc_typ != INPUT_F) dt = stack[stp--];
  switch (fnc_typ) {
  case INPUT_F:   fgets(ss, 40, stdin); stack[++stp] = atoi(ss); break;
  case PRINTF1_F: printf("%s", mem_adrs(dt)); break;
  case PRINTF2_F: printf(mem_adrs(stack[stp--]), dt); break;
  }                      /* :exit(n)Íexecute()àÅ */
}

void code_dump(void) /* R[hð\¦ */
{
  int n;
  OpCode op;
  char nbrss[30] = "";

  for (n=0; n<=codeCt; n++) {
    op = code[n].opcode;
    printf("%04d: ", n);                                /* ¶¬µ½½ßÌÔ */

    sprintf(nbrss, "%d", code[n].opdata);
    if (code[n].flag && (op==LOD || op==LDA || op==STO)) strcat(nbrss, "[b]");

    switch (op) {
    case RET: case ASS:  case ASSV: case NOT:  case INC: case DEC:  case NEG:
    case ADD: case SUB:  case MUL:  case DIV:  case MOD: case LESS: case LSEQ:
    case GRT: case GTEQ: case EQU:  case NTEQ: case AND: case OR:   case VAL:
    case DEL: case NOP:  case STOP:
         printf("%-5s\n", ssOpCode[op]);
         if (op==RET || op==STOP) printf("\n");       /* ÇÝâ·³Ì½ßüs */
         break;
    case LOD:  case LDA: case LDI:  case STO:  case CALL: case ADBR: case LIB:
    case JMP:  case JPT: case JPF: case EQCMP:
         printf("%-5s %-11s\n", ssOpCode[op], nbrss);
         break;
    default:
         printf("Illegal code(#%d code=%d)\n", Pc-1, op);
         exit(1);
    }
  }
}

void exe_err(char *msg) /* ÀsG[ */
{
  fprintf(stderr, "Run-time error(#%d) %s\n", Pc-1, msg); exit(1);
}

