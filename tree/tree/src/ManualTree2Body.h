#include "TEmulatedCollectionProxy.h"
#include "TROOT.h"
#include "TSelectorCint.h"

TBranch *TTreeBranchImpRefCint(TTree *tree, const char* branchname, TClass* ptrClass, EDataType datatype, void* addobj, Int_t bufsize, Int_t splitlevel)
{
   return tree->BranchImpRef(branchname,ptrClass,datatype,addobj,bufsize,splitlevel);
}

static int G__ManualTree2_126_0_132(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   // We need to emulate TTree::Process and properly capture the fact that we go an intepreted TSelector object.

   G__ClassInfo ti( libp->para[0] );
   TClass *ptrClass = TClass::GetClass(ti.Name());
   TSelector *sel = (TSelector*) G__int(libp->para[0]);

   //Interpreted selector: cannot be used as such
   //create a fake selector
   TSelectorCint *selectcint = 0;
   if (ptrClass && !ptrClass->IsLoaded()) {
      selectcint = new TSelectorCint();
      selectcint->Build(sel, &ti, kFALSE);
      sel = selectcint;
   }

   switch (libp->paran) {
   case 4:
      G__letLonglong(result7, 110, (G__int64) ((TTree*) G__getstructoffset())->Process(sel, (Option_t*) G__int(libp->para[1])
, (Long64_t) G__Longlong(libp->para[2]), (Long64_t) G__Longlong(libp->para[3])));
      break;
   case 3:
      G__letLonglong(result7, 110, (G__int64) ((TTree*) G__getstructoffset())->Process(sel, (Option_t*) G__int(libp->para[1])
, (Long64_t) G__Longlong(libp->para[2])));
      break;
   case 2:
      G__letLonglong(result7, 110, (G__int64) ((TTree*) G__getstructoffset())->Process(sel, (Option_t*) G__int(libp->para[1])));
      break;
   case 1:
      G__letLonglong(result7, 110, (G__int64) ((TTree*) G__getstructoffset())->Process(sel));
      break;
   }

   delete selectcint;
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__ManualTree2_126_0_187(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
  // We need to emulate
  // return BranchImp(name,classname,TBuffer::GetClass(typeid(T)),addobj,bufsize,splitlevel);

   // Here find the class name 
   G__ClassInfo ti( libp->para[2] );
   TClass *ptrClass = TClass::GetClass(ti.Name());
   const char* classname = (const char*)G__int(libp->para[1]);
   TClass *claim = TClass::GetClass(classname);
   void **addr = (void**)G__int(libp->para[2]);
   
   const char *branchname = (const char*)G__int(libp->para[0]);
   Bool_t error = kFALSE;

   if (ptrClass && claim) {
      if (!(claim->InheritsFrom(ptrClass)||ptrClass->InheritsFrom(claim)) ) {
         // Note we currently do not warning in case of splicing or over-expectation).
         Error("TTree::Branch","The class requested (%s) for the branch \"%s\" is different from the type of the pointer passed (%s)",
               claim->GetName(),branchname,ptrClass->GetName());
         error = kTRUE;
      } else if (addr && *addr) {
         TClass *actualClass = ptrClass->GetActualClass(*addr);
         if (!actualClass) {
            Warning("TTree::Branch", "The actual TClass corresponding to the object provided for the definition of the branch \"%s\" is missing."
                    "\n\tThe object will be truncated down to its %s part",
                    branchname,classname);
         } else if (claim!=actualClass && !actualClass->InheritsFrom(claim)) {
            Error("TTree::Branch", "The actual class (%s) of the object provided for the definition of the branch \"%s\" does not inherit from %s",
                  actualClass->GetName(),branchname,claim->GetName());
            error = kTRUE;
         } else if (claim->GetCollectionProxy() && dynamic_cast<TEmulatedCollectionProxy*>(claim->GetCollectionProxy())) {
            Error("TTree::Branch", "The class requested (%s) for the branch \"%s\" refer to an stl collection and do not have a compiled CollectionProxy.  "
                  "Please generate the dictionary for this class (%s)", 
                  claim->GetName(), branchname, claim->GetName());

            error = kTRUE;
         }
      }
   }
   if (error) {
      G__letint(result7,85,0);   
   } else {
   
      //if (ptrClass) classname = ptrClass->GetName();
      TTree *t = (TTree*)(G__getstructoffset());
      switch(libp->paran) {
      case 5:
         G__letint(result7,85,(long)t->Branch(branchname,
                                              classname,
                                              (void*)G__int(libp->para[2]),
                                              (Int_t)G__int(libp->para[3]),
                                              (Int_t)G__int(libp->para[4])));
         break;
      case 4:
         G__letint(result7,85,(long)t->Branch(branchname,
                                              classname,
                                              (void*)G__int(libp->para[2]),
                                              (Int_t)G__int(libp->para[3])));
         break;
      case 3:
         G__letint(result7,85,(long)t->Branch(branchname,
                                              classname,
                                              (void*)G__int(libp->para[2])));
         break;
      }
   }
   return(1 || funcname || hash || result7 || libp) ;
}

#include "TDataType.h"

static int G__ManualTree2_126_0_188(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   // We need to emulate 
   // return BranchImp(name,TBuffer::GetClass(typeid(T)),addobj,bufsize,splitlevel);

   G__TypeInfo ti( libp->para[1] );
   string type( TClassEdit::ShortType(ti.Name(),TClassEdit::kDropTrailStar) );

   TClass *ptrClass = TClass::GetClass(type.c_str());
   TDataType *data = gROOT->GetType(type.c_str());
   EDataType datatype = data ? (EDataType)data->GetType() : kOther_t;

   const char *branchname = (const char*)G__int(libp->para[0]);

   if (ti.Reftype()==0) {
      G__letint(result7,85,(long)TTreeBranchImpRefCint(((TTree*) G__getstructoffset()),branchname,ptrClass,datatype,
                                                       (void*)G__int(libp->para[1]),
                                                       (Int_t)G__int(libp->para[2]),
                                                       (Int_t)G__int(libp->para[3])));

   } else {

      TClass *actualClass = 0;
      void **addr = (void**)G__int(libp->para[1]);
      if (ptrClass && addr) actualClass = ptrClass->GetActualClass(*addr);
      
      if (ptrClass && ptrClass->GetCollectionProxy() && dynamic_cast<TEmulatedCollectionProxy*>(ptrClass->GetCollectionProxy())) {
         Error("TTree::Branch", "The class requested (%s) for the branch \"%s\" refer to an stl collection and do not have a compiled CollectionProxy.  "
               "Please generate the dictionary for this class (%s)",
               ptrClass->GetName(), branchname, ptrClass->GetName());   
         G__letint(result7,85,0);
      } else if (actualClass && actualClass->GetCollectionProxy() && dynamic_cast<TEmulatedCollectionProxy*>(actualClass->GetCollectionProxy())) {
         Error("TTree::Branch", "The class requested (%s) for the branch \"%s\" refer to an stl collection and do not have a compiled CollectionProxy.  "
               "Please generate the dictionary for this class (%s)",
               actualClass->GetName(), branchname, actualClass->GetName());   
         G__letint(result7,85,0);
      }
      
      if (ptrClass == 0) {
         Error("TTree::Branch","The pointer specified for %s not of a class known to ROOT",
               branchname);
         G__letint(result7,85,0);
      } else {
         const char* classname = ptrClass->GetName();
         if (actualClass==0) {
            Warning("TTree::Branch", "The actual TClass corresponding to the object provided for the definition of the branch \"%s\" is missing."
                    "\n\tThe object will be truncated down to its %s part",
                    branchname,classname);
         } else {
            classname = actualClass->GetName();
         }
         TTree *t = (TTree*)(G__getstructoffset());
         switch(libp->paran) {
         case 4:
            G__letint(result7,85,(long)t->Branch(branchname,
                                                 classname,
                                                 (void*)G__int(libp->para[1]),
                                                 (Int_t)G__int(libp->para[2]),
                                                 (Int_t)G__int(libp->para[3])));
            break;
         case 3:
            G__letint(result7,85,(long)t->Branch(branchname,
                                                 classname,
                                                 (void*)G__int(libp->para[1]),
                                                 (Int_t)G__int(libp->para[2])));
            break;
         case 2:
            G__letint(result7,85,(long)t->Branch(branchname,
                                                 classname,
                                                 (void*)G__int(libp->para[1])));
            break;
         }         
      }
   }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__ManualTree2_126_0_190(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   // Emulate:
   // template <class T> void SetBranchAddress(const char *bname, T **add, TBranch **ptr);

   G__setnull(result7);

   G__TypeInfo ti( libp->para[1] );
   string type( TClassEdit::ShortType(ti.Name(),TClassEdit::kDropTrailStar) );
   TClass *ptrClass = TClass::GetClass(type.c_str());
   TDataType *data = gROOT->GetType(type.c_str());
   EDataType datatype = data ? (EDataType)data->GetType() : kOther_t;
   TBranch **branchPtr = 0;

   switch (libp->paran) {
   case 3:
      branchPtr = (TBranch**) G__int(libp->para[2]);
      break;
   case 2:
      break;
   }
   ((TTree*)(G__getstructoffset()))->SetBranchAddress((const char*)G__int(libp->para[0]),(void*)G__int(libp->para[1]),branchPtr,ptrClass,datatype,ti.Reftype()==G__PARAP2P);

   return(1 || funcname || hash || result7 || libp) ;
}
