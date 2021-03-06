#include <stdlib.h>
#include <stdio.h>
#include <dbAccess.h>
#include <devSup.h>
#include <recSup.h>
#include <recGbl.h>
#include <errlog.h>
#include <callback.h>

#include <aiRecord.h>

#include <epicsExport.h>

static long init_record(aiRecord *pao);
static long read_ai(aiRecord *pao);

struct randState {
  unsigned int seed;
  CALLBACK cb; /* New */
};

struct {
  long num;
  DEVSUPFUN  report;
  DEVSUPFUN  init;
  DEVSUPFUN  init_record;
  DEVSUPFUN  get_ioint_info;
  DEVSUPFUN  read_ai;
  DEVSUPFUN  special_linconv;
} devAiRandAsync = {
  6, /* space for 6 functions */
  NULL,
  NULL,
  init_record,
  NULL,
  read_ai,
  NULL
};
epicsExportAddress(dset,devAiRandAsync); 

void rand_cb(CALLBACK* cb);

static long init_record(aiRecord *pao)
{
  struct randState* priv;
  unsigned long start;

  priv=malloc(sizeof(struct randState));
  if(!priv){
    recGblRecordError(S_db_noMemory, (void*)pao,
      "devAoTimebase failed to allocate private struct");
    return S_db_noMemory;
  }

  /* New */
  callbackSetCallback(rand_cb,&priv->cb);
  callbackSetPriority(priorityLow,&priv->cb);
  callbackSetUser(pao,&priv->cb);
  priv->cb.timer=NULL;

  recGblInitConstantLink(&pao->inp,DBF_ULONG,&start);

  priv->seed=start;
  pao->dpvt=priv;
  srand(priv->seed);

  return 0;
}

static long read_ai(aiRecord *pao)
{
  struct randState* priv=pao->dpvt;

  if( ! pao->pact ){
    /* start async operation */
    pao->pact=TRUE;
    callbackSetUser(pao,&priv->cb);
    callbackRequestDelayed(&priv->cb,1.1);
    return 0;
  }else{
    /* complete operation */
    pao->pact=FALSE;
    return 0;
  }
}

void rand_cb(CALLBACK* cb)
{
  aiRecord* prec;
  struct randState* priv;
  struct rset* prset;
  epicsInt32 raw;

  callbackGetUser(prec,cb);
  prset=(struct rset*)prec->rset;
  priv=prec->dpvt;

  raw=rand() % 100;

  dbScanLock((dbCommon*)prec);
  prec->rval=raw;
  (*prset->process)(prec);
  dbScanUnlock((dbCommon*)prec);
}



