#include "cci.h"
#include "cci_prot.h"

/* ----------------------------e[u */
#define TBL_MAX    1000      /* L\ÌTCY   */
#define start_GTBL 1         /* åæpÌJnÊu */
#define LTBL_EMPTY 9999      /* ÇL\ó«   */
SymTbl table[TBL_MAX+1];     /* L\           */
int start_LTBL = LTBL_EMPTY; /* ÇpÌJnÊu */
int tableCt = 0;             /* gpÌÅIÊu */

SymTbl *enter(SymTbl tb, SymKind kind) /* L\o^ */
{
  int tbNo;

  if ((kind==varId || kind==paraId) && tb.dtTyp==VOID_T) {
    err_s("Ï^ëè(void)");
    tb.dtTyp = INT_T;                                            /* intÉ·é */
  }
  tb.nmKind = kind;
  nameChk(tb);                                       /* øÆÏÌd¡chk */

  if (tableCt >= TBL_MAX) { err_s("L\ over"); exit(1); }
  tbNo = ++tableCt;                                             /* L\Ô */
  table[tbNo] = tb;                                           /* L\Éo^ */

  if (kind == paraId) ++table[tbNo].level;  /* øðÖubNÉüêé²® */
  if (kind == varId) set_adrs(&table[tbNo]);          /* ÏÈçAhXÝè */
  if (kind == fncId) table[tbNo].adrs = -tbNo;    /* L\ÔÌlðÖÌ */
                                          /* ÔnÆµÄ¼Ýè(Ôn¢èÌÓ) */
  return &table[tbNo];                                /* L\|C^ðÔ· */
}

void localTBL_open(void) /* ÇL\Jn */
{
  start_LTBL = tableCt + 1;                     /* ÇpL\JnÊuÝè */
}

void localTBL_close(SymTbl *fp) /* ÇL\I¹ */
{                         /* «øo^ðåæÌæÉc·½ßøÂª¾¯²® */
  tableCt = start_LTBL - 1 + fp->args;                  /* åæpÅIÊuñ */
  start_LTBL = LTBL_EMPTY;                                /* ÇL\NA */
}

SymTbl *search(char *s) /* L\õ(T[rX è) */
{
  SymTbl *p;
  static SymTbl *dmy_p=NULL, wkTb = {"tmp$name2",varId,INT_T,0,0,0,0};

  p = search_name(s);
  if (p == NULL) {                                    /* È¯êÎ¼ÏðÔ· */
    err_ss("¢è`Ì¯Êq", s);
    if (dmy_p == NULL) dmy_p = enter(wkTb, varId);              /* ¼Ïo^ */
    p = dmy_p;
  }
  return p;
}

SymTbl *search_name(char *s) /* L\õ */
{
  int i;                         /* :ÇL\ªÈ¢Æ«Ístart_LTBLÍ9999 */
  for (i=tableCt; i>=start_LTBL; i--) {                 /* æÉÇÌæðT· */
    if (strcmp(table[i].name,s)==0) return table + i;
  }
  for (; i>=start_GTBL; i--) {                          /* ÉåæÌæðT· */
    if (table[i].nmKind!=paraId && strcmp(table[i].name,s)==0)
      return table + i;
  }
  return NULL;                                                    /* È©Á½ */
}

void nameChk(SymTbl tb) /* øÆÏÌ¼Od¡`FbN */
{
  SymTbl *p;
  extern int blkNest;                                         /* (cci_pars.c) */
  int nest = blkNest;

  if (tb.nmKind!=paraId && tb.nmKind!=varId) return;
  if ((p = search_name(tb.name)) == NULL) return;
  if (tb.nmKind == paraId) ++nest;                  /* ø¸pÉL¯æ²® */
  if (p->level >= nest) err_ss("¯Êqªd¡µÄ¢é", tb.name);
}

void del_fncTable(SymTbl *f1, SymTbl *f2) /* d¡o^ÌðÁ */
{
  int i;
  if (f1 == NULL) return;
  if (f1->dtTyp!=f2->dtTyp || f1->args!=f2->args)  return;
  if (f1->nmKind==protId && f2->nmKind==fncId) {  /* f1ªvgAf2ª{ÌÈç */
    for (i=0; i<=f2->args; i++) { *(f1+i) = *(f2+i); }      /* f2ðf1ÉRs[ */
  }
  tableCt -= (f2->args + 1);                              /* Vµ¢o^ðæÁ */
  return;
}

int b_flg(SymTbl *tp) /* ÇÏÈç1ðÔ· */
{
  if (tp->level == 0) return 0; else return 1;                   /* åæÈç0 */
}

SymTbl *tb_ptr(int n) /* L\AhXðÔ· */
{
  return &table[n];
}

